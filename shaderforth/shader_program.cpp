
#include "shader_program.h"

#include "logging.h"

#include <sstream>

#include <jtk/concurrency.h>

#include <vectorforth/context.h>
#include <vectorforth/compiler.h>
#include <vectorforth/expand_data.h>
#include <vectorforth/dictionary.h>
#include <vectorforth/tokenize.h>
#include <vectorforth/stdlib.h>
#include <vectorforth/sincos_table.h>

#define USE_THREAD_POOL

namespace
  {

  std::string adapt_line_number_in_error_message(const std::string& msg, int line_change)
    {
    std::stringstream ss;
    ss << msg;
    std::string out;
    bool read_line = false;
    while (!ss.eof())
      {
      std::string txt;
      if (!read_line)
        {
        ss >> txt;
        if (txt == "line")
          read_line = true;
        }
      else
        {
        int ln;
        ss >> ln;
        ln -= line_change;
        std::stringstream ss2;
        ss2 << ln;
        ss2 >> txt;
        read_line = false;
        }
      out += txt;
      out += " ";
      }
    return out;
    }

  std::string get_header()
    {
#ifdef AVX512
    std::string main = R"(
: x st@ #64 #- @ ;
: y st@ #128 #- @ ;
: rx st@ #192 #- @ ;
: ry st@ #256 #- @ ;
: u st@ #320 #- @ ;
: v st@ #384 #- @ ;
: t st@ #448 #- @ ;
: dt st@ #512 #- @ ;
: frame st@ #576 #- @ ;
: mx st@ #640 #- @ ;
: my st@ #704 #- @ ;
: mz st@ #768 #- @ ;
: mw st@ #832 #- @ ;
: globaltime st@ #896 #- @ ;
)";
#else
    std::string main = R"(
: x st@ #32 #- @ ;
: y st@ #64 #- @ ;
: rx st@ #96 #- @ ;
: ry st@ #128 #- @ ;
: u st@ #160 #- @ ;
: v st@ #192 #- @ ;
: t st@ #224 #- @ ;
: dt st@ #256 #- @ ;
: frame st@ #288 #- @ ;
: mx st@ #320 #- @ ;
: my st@ #352 #- @ ;
: mz st@ #384 #- @ ;
: mw st@ #416 #- @ ;
: globaltime st@ #448 #- @ ;
)";
#endif
    return main;
    }
  }

shader_program::shader_program(int w, int h) : _w(w), _h(h), _fun_size(0), _fun(nullptr)
  {
#if defined(USE_THREAD_POOL)
  _tp.init();
#endif
  VF::initialize_lookup();
  }

shader_program::~shader_program()
  {
#if defined(USE_THREAD_POOL)
  _tp.stop();
#endif
  local_context.combine_each([](VF::context& ctxt)
    {
    VF::destroy_context(ctxt);
    });
  if (_fun)
    ASM::free_assembled_function((void*)_fun, _fun_size);
  }

void shader_program::log_assembly(const std::string& script, bool optimize)
  {
  std::string main = get_header();
  int main_lines = (int)std::count(main.begin(), main.end(), '\n');

  VF::dictionary dict;
  add_stdlib_to_dictionary(dict);
  VF::expand_data ed = VF::create_expand_data();
  ASM::asmcode code;

  std::string shader = main + script;
  try
    {
    auto words = VF::tokenize(shader);
    if (optimize)
      VF::compile(code, dict, ed, words);
    else
      VF::compile_single_pass(code, dict, ed, words);
    }
  catch (std::runtime_error e)
    {
    Logging::Error() << "shader program: " << adapt_line_number_in_error_message(e.what(), main_lines) << "\n";    
    return;
    }
  std::stringstream ss;
  code.stream(ss);
  Logging::GetInstance() << ss.str() << "\n";
  }

bool shader_program::compile(const std::string& script, bool optimize)
  {  
  if (_fun)
    ASM::free_assembled_function((void*)_fun, _fun_size);

  std::string main = get_header();

  int main_lines = (int)std::count(main.begin(), main.end(), '\n');

  std::string shader = main + script;

  try
    {
    VF::dictionary dict;
    add_stdlib_to_dictionary(dict);
    VF::expand_data ed = VF::create_expand_data();
    ASM::asmcode code;
    auto words = VF::tokenize(shader);
    if (optimize)
      VF::compile(code, dict, ed, words);
    else
      VF::compile_single_pass(code, dict, ed, words);
    ASM::first_pass_data d;

    _fun = (fun_ptr)assemble(_fun_size, d, code);
    Logging::Info() << "shader program size: " << _fun_size << " bytes\n";
    }
  catch (std::runtime_error e)
    {
    Logging::Error() << "shader program: " << adapt_line_number_in_error_message(e.what(), main_lines) << "\n";
    _fun = nullptr;
    return false;
    }
  return true;
  }

namespace
  {
  inline float clamp(float val, float left, float right)
    {
    return val < left ? left : val > right ? right : val;
    }
  }

void shader_program::run(image<uint32_t>& im)
  {
  if (!_fun)
    return;

#ifdef AVX512
  assert(im.width() % 16 == 0);

  const int stack_top_offset = 896;

  assert(stack_top_offset % 64 == 0);

  const __m512 offset = _mm512_set_ps(15.f, 14.f, 13.f, 12.f, 11.f, 10.f, 9.f, 8.f, 7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f);

  const float defaults[16] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

  __m512 t_val = _mm512_set1_ps(_input.time);
  __m512 rx_val = _mm512_set1_ps(_input.resolution_x);
  __m512 ry_val = _mm512_set1_ps(_input.resolution_y);
  __m512 dt_val = _mm512_set1_ps(_input.time_delta);

  __m512 frame_val = _mm512_set1_ps((float)_input.frame);
  __m512 mx_val = _mm512_set1_ps(_input.mx);
  __m512 my_val = _mm512_set1_ps(_input.my);
  __m512 mz_val = _mm512_set1_ps(_input.mz);
  __m512 mw_val = _mm512_set1_ps(_input.mw);

  __m512 global_time_val = _mm512_set1_ps(_input.global_time);
#else

  assert(im.width() % 8 == 0);

  const int stack_top_offset = 448;

  assert(stack_top_offset % 32 == 0);

  const __m256 offset = _mm256_set_ps(7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f);

  const float defaults[8] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

  __m256 t_val = _mm256_set1_ps(_input.time);
  __m256 rx_val = _mm256_set1_ps(_input.resolution_x);
  __m256 ry_val = _mm256_set1_ps(_input.resolution_y);
  __m256 dt_val = _mm256_set1_ps(_input.time_delta);

  __m256 frame_val = _mm256_set1_ps((float)_input.frame);
  __m256 mx_val = _mm256_set1_ps(_input.mx);
  __m256 my_val = _mm256_set1_ps(_input.my);
  __m256 mz_val = _mm256_set1_ps(_input.mz);
  __m256 mw_val = _mm256_set1_ps(_input.mw);

  __m256 global_time_val = _mm256_set1_ps(_input.global_time);
#endif

#if defined(USE_THREAD_POOL)
  jtk::pooled_parallel_for((int)0, _h, [&](int y)
#else
  jtk::parallel_for((int)0, _h, [&](int y)
#endif
    {

    uint32_t* p_im = im.data() + y * _w;

    VF::context& ctxt = local_context.local();

    if (ctxt.memory_allocated == nullptr)
#ifdef AVX512
      ctxt = VF::create_context(2048 * 1024, 4096*2, 2048 * 1024);
#else
      ctxt = VF::create_context(1024 * 1024, 2048*2, 1024 * 1024);
#endif

    const float vrel = (float)y / _input.resolution_y;
#ifdef AVX512
    __m512 y_val = _mm512_set1_ps((float)y);
    __m512 v_val = _mm512_set1_ps(vrel);
    _mm512_store_ps((float*)(ctxt.aligned_stack_top - 2 * 64), y_val);
    _mm512_store_ps((float*)(ctxt.aligned_stack_top - 2 * 192), v_val);
    _mm512_store_ps((float*)(ctxt.aligned_stack_top - 2 * 224), t_val);
    _mm512_store_ps((float*)(ctxt.aligned_stack_top - 2 * 256), dt_val);
    _mm512_store_ps((float*)(ctxt.aligned_stack_top - 2 * 96), rx_val);
    _mm512_store_ps((float*)(ctxt.aligned_stack_top - 2 * 128), ry_val);

    _mm512_store_ps((float*)(ctxt.aligned_stack_top - 2 * 288), frame_val);
    _mm512_store_ps((float*)(ctxt.aligned_stack_top - 2 * 320), mx_val);
    _mm512_store_ps((float*)(ctxt.aligned_stack_top - 2 * 352), my_val);
    _mm512_store_ps((float*)(ctxt.aligned_stack_top - 2 * 384), mz_val);
    _mm512_store_ps((float*)(ctxt.aligned_stack_top - 2 * 416), mw_val);
    _mm512_store_ps((float*)(ctxt.aligned_stack_top - 2 * 448), global_time_val);
#else
    __m256 y_val = _mm256_set1_ps((float)y);
    __m256 v_val = _mm256_set1_ps(vrel);
    _mm256_store_ps((float*)(ctxt.aligned_stack_top - 64), y_val);
    _mm256_store_ps((float*)(ctxt.aligned_stack_top - 192), v_val);
    _mm256_store_ps((float*)(ctxt.aligned_stack_top - 224), t_val);
    _mm256_store_ps((float*)(ctxt.aligned_stack_top - 256), dt_val);
    _mm256_store_ps((float*)(ctxt.aligned_stack_top - 96), rx_val);
    _mm256_store_ps((float*)(ctxt.aligned_stack_top - 128), ry_val);

    _mm256_store_ps((float*)(ctxt.aligned_stack_top - 288), frame_val);
    _mm256_store_ps((float*)(ctxt.aligned_stack_top - 320), mx_val);
    _mm256_store_ps((float*)(ctxt.aligned_stack_top - 352), my_val);
    _mm256_store_ps((float*)(ctxt.aligned_stack_top - 384), mz_val);
    _mm256_store_ps((float*)(ctxt.aligned_stack_top - 416), mw_val);

    _mm256_store_ps((float*)(ctxt.aligned_stack_top - 448), global_time_val);
#endif

#ifdef AVX512
    for (int x = 0; x < _w; x += 16)
#else
    for (int x = 0; x < _w; x += 8)
#endif
      {
#ifdef AVX512
      __m512 x_val = _mm512_add_ps(_mm512_set1_ps((float)x), offset);
      __m512 u_val = _mm512_div_ps(x_val, rx_val);
      _mm512_store_ps((float*)(ctxt.aligned_stack_top - 2 * 32), x_val);
      _mm512_store_ps((float*)(ctxt.aligned_stack_top - 2 * 160), u_val);
#else
      __m256 x_val = _mm256_add_ps(_mm256_set1_ps((float)x), offset);
      __m256 u_val = _mm256_div_ps(x_val, rx_val);
      _mm256_store_ps((float*)(ctxt.aligned_stack_top - 32), x_val);
      _mm256_store_ps((float*)(ctxt.aligned_stack_top - 160), u_val);
#endif
      ctxt.stack_pointer = ctxt.aligned_stack_top - stack_top_offset;

      // reset "here" pointer
#ifdef AVX512
      char* data_space_pointer = ctxt.here_pointer + 64;
#else
      char* data_space_pointer = ctxt.here_pointer + 32;
#endif
      *((uint64_t*)ctxt.here_pointer) = (uint64_t)((void*)data_space_pointer);

      _fun(&ctxt);

#ifdef AVX512
      const float* b = (const float*)(ctxt.stack_pointer);
      const float* g = (const float*)(ctxt.stack_pointer) + 16;
      const float* r = (const float*)(ctxt.stack_pointer) + 32;
#else
      const float* b = (const float*)(ctxt.stack_pointer);
      const float* g = (const float*)(ctxt.stack_pointer) + 8;
      const float* r = (const float*)(ctxt.stack_pointer) + 16;
#endif

      if (b >= (const float*)(ctxt.aligned_stack_top - stack_top_offset))
        {
        b = &defaults[0];
        g = &defaults[0];
        r = &defaults[0];
        }
      else if (g >= (const float*)(ctxt.aligned_stack_top - stack_top_offset))
        {
        r = b;
        b = &defaults[0];
        g = &defaults[0];
        }
      else if (r >= (const float*)(ctxt.aligned_stack_top - stack_top_offset))
        {
        r = g;
        g = b;        
        b = &defaults[0];
        }

#ifdef AVX512
      for (int i = 0; i < 16; ++i)
#else
      for (int i = 0; i < 8; ++i)
#endif
        {
        uint32_t red = (uint32_t)(clamp(r[i], 0.f, 1.f) * 255.f);
        uint32_t green = (uint32_t)(clamp(g[i], 0.f, 1.f) * 255.f);
        uint32_t blue = (uint32_t)(clamp(b[i], 0.f, 1.f) * 255.f);
        *p_im++ = 0xff000000 | (blue << 16) | (green << 8) | red;
        }
      }
#if defined(USE_THREAD_POOL)
    }, _tp);
#else
    });
#endif
  }

void shader_program::set_shader_input(const shader_input& inp)
  {
  _input = inp;
  }
