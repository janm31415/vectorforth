#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string>
#include <sstream>
#include <immintrin.h>
#include <cassert>
#include <chrono>

#include <vectorforth/context.h>
#include <vectorforth/compiler.h>
#include <vectorforth/compile_data.h>
#include <vectorforth/debug.h>
#include <vectorforth/dictionary.h>
#include <vectorforth/tokenize.h>
#include <vectorforth/stdlib.h>

#include <tbb/parallel_for.h>
#include <tbb/enumerable_thread_specific.h>

#include "window.h"

#ifdef _WIN32
#include <windows.h>
#endif

struct listener : public IWindowListener
  {
  bool quit;

  listener() : quit(false) {}

  virtual void OnClose() 
    {
    quit = true;
    };

  virtual void OnKeyDown(int k)
    {
#ifdef _WIN32
    if (k == 27)
      quit = true;
#else
    if (k == 9)
      quit = true;
#endif
    };

  virtual void OnKeyUp(int k) 
    {
#ifdef _WIN32
    if (k == 27)
      quit = true;
#else
    if (k == 9)
      quit = true;
#endif
    };
  };

void os_restart_line()
  {
#ifdef _WIN32
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO info;
  GetConsoleScreenBufferInfo(out, &info);
  info.dwCursorPosition.X = 0;
  SetConsoleCursorPosition(out, info.dwCursorPosition);
#else
  printf("\r");
#endif
  }


inline float clamp(float val, float left, float right)
  {
  return val < left ? left : val > right ? right : val;
  }

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

//#define SINGLE

int main(int argc, char** argv)
  {  
  if (argc < 2)
    {
    std::cout << "Usage: sf <shader.4th>\n";
    return 0;
    }

  std::ifstream f(argv[1]);
  if (!f.is_open())
    {
    std::cout << "Cannot open file " << argv[1] << "\n";
    return -1;
    }

  std::string line;
  std::string shader;

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

  shader += main;

  while (std::getline(f, line))
    {
    shader += line;
    shader.push_back('\n');
    }
  f.close();

  uint64_t fun_size;
  typedef void(*fun_ptr)(void*);
  fun_ptr fun;

  try
    {
    VF::dictionary dict;
    add_stdlib_to_dictionary(dict);
    VF::compile_data cd = VF::create_compile_data();
    ASM::asmcode code;
    auto words = VF::tokenize(shader);
    VF::compile(code, dict, cd, words);
    ASM::first_pass_data d;

    fun = (fun_ptr)assemble(fun_size, d, code);
    }
  catch (std::runtime_error e)
    {
    std::cout << "shader program: " << adapt_line_number_in_error_message(e.what(), main_lines) << "\n";
    return -2;
    }

  std::cout << "Size of shader: " << fun_size << " bytes\n";

  int w = 800;
  int h = 450;

  uint32_t* image = (uint32_t*)(_mm_malloc(w * h * sizeof(uint32_t), 32));

  listener l;

  WindowHandle wh = create_window("Shader forth", w, h); // create window for visualization
  
  register_listener(wh, &l);

  assert(w % 8 == 0);
  const int stack_top_offset = 416;
  assert(stack_top_offset % 32 == 0);
  const __m256 offset = _mm256_set_ps(7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f, 0.f);
  const float defaults[8] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
  __m256 rx_val = _mm256_set1_ps((float)w);
  __m256 ry_val = _mm256_set1_ps((float)h);
  __m256 mx_val = _mm256_set1_ps(0.f);
  __m256 my_val = _mm256_set1_ps(0.f);
  __m256 mz_val = _mm256_set1_ps(0.f);
  __m256 mw_val = _mm256_set1_ps(0.f);
  int frame = 0;
  auto last_tic = std::chrono::high_resolution_clock::now();
  float time = 0.f;
  tbb::enumerable_thread_specific< VF::context > local_context;

  while (!l.quit)
    {
    __m256 frame_val = _mm256_set1_ps((float)frame);
    
    auto tic = std::chrono::high_resolution_clock::now();
    float time_delta = (float)(std::chrono::duration_cast<std::chrono::microseconds>(tic - last_tic).count()) / 1000000.f;
    last_tic = tic;
    time += time_delta;
    __m256 time_delta_val = _mm256_set1_ps((float)time_delta);
    __m256 time_val = _mm256_set1_ps((float)time);

#ifdef SINGLE
    for (int y = 0; y < h; ++y)
#else
    tbb::parallel_for((int)0, h, [&](int y)    
#endif
      {

      uint32_t* p_im = image + y * w;

      VF::context& ctxt = local_context.local();

      if (ctxt.memory_allocated == nullptr)
        ctxt = VF::create_context(1024 * 1024, 2048, 1024 * 1024);

      const float vrel = (float)y / (float)h;
      __m256 y_val = _mm256_set1_ps((float)y);
      __m256 v_val = _mm256_set1_ps(vrel);
      _mm256_store_ps((float*)(ctxt.aligned_stack_top - 64), y_val);
      _mm256_store_ps((float*)(ctxt.aligned_stack_top - 192), v_val);
      _mm256_store_ps((float*)(ctxt.aligned_stack_top - 224), time_val);
      _mm256_store_ps((float*)(ctxt.aligned_stack_top - 256), time_delta_val);
      _mm256_store_ps((float*)(ctxt.aligned_stack_top - 96), rx_val);
      _mm256_store_ps((float*)(ctxt.aligned_stack_top - 128), ry_val);

      _mm256_store_ps((float*)(ctxt.aligned_stack_top - 288), frame_val);
      _mm256_store_ps((float*)(ctxt.aligned_stack_top - 320), mx_val);
      _mm256_store_ps((float*)(ctxt.aligned_stack_top - 352), my_val);
      _mm256_store_ps((float*)(ctxt.aligned_stack_top - 384), mz_val);
      _mm256_store_ps((float*)(ctxt.aligned_stack_top - 416), mw_val);

      for (int x = 0; x < w; x += 8)
        {
        __m256 x_val = _mm256_add_ps(_mm256_set1_ps((float)x), offset);
        __m256 u_val = _mm256_div_ps(x_val, rx_val);
        _mm256_store_ps((float*)(ctxt.aligned_stack_top - 32), x_val);
        _mm256_store_ps((float*)(ctxt.aligned_stack_top - 160), u_val);
        ctxt.stack_pointer = ctxt.aligned_stack_top - stack_top_offset;

        // reset "here" pointer
        char* data_space_pointer = ctxt.here_pointer + 32;
        *((uint64_t*)ctxt.here_pointer) = (uint64_t)((void*)data_space_pointer);

        fun(&ctxt);

        //print_stack(std::cout, ctxt);
        //print_data_space(std::cout, ctxt);

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
          *p_im++ = 0xff000000 | (red << 16) | (green << 8) | blue;
          }
        }
      }
#ifndef SINGLE
    );
#endif
    if (!l.quit)
      paint(wh, (const uint8_t*)image, w, -h, 4);

    os_restart_line();
    printf("%.2f FPS     ", 1.f / time_delta);

    ++frame;
    }

  close_window(wh); // close the visualization window

  _mm_free(image);

  for (auto& ctxt : local_context)
    {
    VF::destroy_context(ctxt);
    }
  ASM::free_assembled_function((void*)fun, fun_size);

  printf("\n");
  return 0;
  }