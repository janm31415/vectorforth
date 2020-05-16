
#include "shader_program.h"

#include "logging.h"

#include <sstream>

#include <tbb/parallel_for.h>
#include <tbb/enumerable_thread_specific.h>

#include <vectorforth/context.h>
#include <vectorforth/compiler.h>
#include <vectorforth/compile_data.h>
#include <vectorforth/dictionary.h>
#include <vectorforth/tokenize.h>
#include <vectorforth/stdlib.h>

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
  }

shader_program::shader_program(int w, int h) : _w(w), _h(h), _fun_size(0), _fun(nullptr)
  {
  }

shader_program::~shader_program()
  {  
  for (auto& ctxt : local_context)
    {
    VF::destroy_context(ctxt);
    }
  if (_fun)
    ASM::free_assembled_function((void*)_fun, _fun_size);
  }

bool shader_program::compile(const std::string& script)
  {
  if (_fun)
    ASM::free_assembled_function((void*)_fun, _fun_size);

  std::string main = R"(
: x st@ #32- @ ;
: y st@ #64- @ ;
: rx st@ #96- @ ;
: ry st@ #128- @ ;
: u st@ #160- @ ;
: v st@ #192- @ ;
: t st@ #224- @ ;
: dt st@ #256- @ ;
: frame st@ #288 #- @ ;
: mx st@ #320 #- @ ;
: my st@ #352 #- @ ;
: mz st@ #384 #- @ ;
: mw st@ #416 #- @ ;
)";

  int main_lines = (int)std::count(main.begin(), main.end(), '\n');

  std::string shader = main + script;

  try
    {
    VF::dictionary dict;
    add_stdlib_to_dictionary(dict);
    VF::compile_data cd = VF::create_compile_data();
    ASM::asmcode code;
    auto words = VF::tokenize(shader);
    VF::compile(code, dict, cd, words);
    ASM::first_pass_data d;

    _fun = (fun_ptr)assemble(_fun_size, d, code);
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
  assert(im.width() % 8 == 0);

  const int stack_top_offset = 416;

  assert(stack_top_offset % 32 == 0);

  if (!_fun)
    return;

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

  tbb::parallel_for((int)0, _h, [&](int y)
    {

    uint32_t* p_im = im.data() + y * _w;

    VF::context& ctxt = local_context.local();

    if (ctxt.memory_allocated == nullptr)
      ctxt = VF::create_context(1024 * 1024, 1024, 1024 * 1024);

    const float vrel = (float)y / _input.resolution_y;
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

    for (int x = 0; x < _w; x += 8)
      {
      __m256 x_val = _mm256_add_ps(_mm256_set1_ps((float)x), offset);
      __m256 u_val = _mm256_div_ps(x_val, rx_val);
      _mm256_store_ps((float*)(ctxt.aligned_stack_top - 32), x_val);
      _mm256_store_ps((float*)(ctxt.aligned_stack_top - 160), u_val);
      ctxt.stack_pointer = ctxt.aligned_stack_top - stack_top_offset;

      // reset "here" pointer
      char* data_space_pointer = ctxt.here_pointer + 32;
      *((uint64_t*)ctxt.here_pointer) = (uint64_t)((void*)data_space_pointer);

      _fun(&ctxt);

      const float* b = (const float*)(ctxt.stack_pointer);
      const float* g = (const float*)(ctxt.stack_pointer) + 8;
      const float* r = (const float*)(ctxt.stack_pointer) + 16;

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

      for (int i = 0; i < 8; ++i)
        {
        uint32_t red = (uint64_t)(clamp(r[i], 0.f, 1.f) * 255.f);
        uint32_t green = (uint64_t)(clamp(g[i], 0.f, 1.f) * 255.f);
        uint32_t blue = (uint64_t)(clamp(b[i], 0.f, 1.f) * 255.f);
        *p_im++ = 0xff000000 | (blue << 16) | (green << 8) | red;
        }
      }
    });
  }

void shader_program::set_shader_input(const shader_input& inp)
  {
  _input = inp;
  }