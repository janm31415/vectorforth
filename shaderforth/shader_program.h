#pragma once

#include "shader_input.h"

#include "image.h"
#include <string>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <jtk/concurrency.h>

#include <vectorforth/context.h>

class shader_program
  {
  public:
    shader_program(int w, int h);
    ~shader_program();

    void set_shader_input(const shader_input& inp);

   
    bool compile(const std::string& script, bool optimize);

    void log_assembly(const std::string& script, bool optimize);

    void run(image<uint32_t>& im);

  private:
    shader_input _input;
    int _w, _h;

    uint64_t _fun_size;
    typedef void(*fun_ptr)(void*);
    fun_ptr _fun;

    jtk::combinable< VF::context > local_context;
    jtk::thread_pool _tp;
  };
