#pragma once

#include "shader_input.h"

#include "image.h"
#include <string>
#include <stdint.h>

#include <tbb/enumerable_thread_specific.h>

#include <vectorforth/context.h>

class shader_program
  {
  public:
    shader_program(int w, int h);
    ~shader_program();

    void set_shader_input(const shader_input& inp);

   
    bool compile(const std::string& script);

    void run(image<uint32_t>& im);

  private:
    shader_input _input;
    int _w, _h;

    uint64_t _fun_size;
    typedef void(*fun_ptr)(void*);
    fun_ptr _fun;

    tbb::enumerable_thread_specific< VF::context > local_context;
  };
