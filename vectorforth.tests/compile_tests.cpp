#include "compile_tests.h"
#include "test_assert.h"

#include <asm/assembler.h>

#include <vectorforth/context.h>
#include <vectorforth/compiler.h>
#include <vectorforth/dictionary.h>
#include <vectorforth/stdlib.h>
#include <vectorforth/tokenize.h>

#include <immintrin.h>

#include <iostream>

using namespace ASM;

VF_BEGIN

namespace
  {
  struct compile_fixture
    {
    context ctxt;
    typedef void(*fun_ptr)(void*);
    std::vector<std::pair<fun_ptr, uint64_t>> compiled_functions;
    std::string last_error;
    dictionary dict;


    compile_fixture()
      {
      ctxt = create_context(1024 * 1024);
      add_stdlib_to_dictionary(dict);
      }

    ~compile_fixture()
      {
      for (auto& f : compiled_functions)
        free_assembled_function((void*)f.first, f.second);
      destroy_context(ctxt);
      }

    asmcode get_asmcode(const std::string& script)
      {
      asmcode code;
      auto words = tokenize(script);
      compile(code, dict, words);
      return code;
      }

    char* run(const std::string& script)
      {
      asmcode code;
      try
        {
        code = get_asmcode(script);
        }
      catch (std::runtime_error e)
        {
        last_error = e.what();
        return nullptr;
        }
      first_pass_data d;
      uint64_t size;
      fun_ptr f = (fun_ptr)assemble(size, d, code);
      f(&ctxt);
      compiled_functions.emplace_back(f, size);
      return ctxt.stack_pointer;
      }

    __m256 get_last_stack_value() const
      {
      if (ctxt.stack_pointer >= ctxt.aligned_stack_top)
        throw std::runtime_error("error: stack is empty");
      return _mm256_load_ps((const float*)ctxt.stack_pointer);
      }

    __m256 get_stack_value(size_t index) const
      {
      const float* ptr = (const float*)ctxt.stack_pointer + 8 * index;
      if ((char*)ptr >= ctxt.aligned_stack_top)
        {
        std::cout << "error: stack is empty\n";
        return _mm256_set1_ps(std::numeric_limits<float>::signaling_NaN());
        }
      __m256 sv = _mm256_load_ps(ptr);
      return sv;
      }

    __m256i get_last_stack_value_i() const
      {
      if (ctxt.stack_pointer >= ctxt.aligned_stack_top)
        throw std::runtime_error("error: stack is empty");
      __m256 v = _mm256_load_ps((const float*)ctxt.stack_pointer);
      __m256i out = _mm256_castps_si256(v);
      return out;
      }

    float get_stack_valuef(size_t index) const
      {
      const float* ptr = (const float*)ctxt.stack_pointer + 8 * index;
      if ((char*)ptr >= ctxt.aligned_stack_top)
        {
        std::cout << "error: stack is empty\n";
        return std::numeric_limits<float>::signaling_NaN();
        }
      __m256 sv = _mm256_load_ps(ptr);
      return sv.m256_f32[0];
      }

    uint32_t get_stack_valuei(size_t index) const
      {
      const float* ptr = (const float*)ctxt.stack_pointer + 8 * index;
      if ((char*)ptr >= ctxt.aligned_stack_top)
        {
        std::cout << "error: stack is empty\n";
        return 0xffffffff;
        }
      __m256 sv = _mm256_load_ps(ptr);      
      return *reinterpret_cast<uint32_t*>(&sv.m256_f32[0]);
      }

    uint64_t get_stack_valuef_uint64(size_t index) const
      {
      const float* ptr = (const float*)ctxt.stack_pointer + 8 * index;
      if ((char*)ptr >= ctxt.aligned_stack_top)
        {
        std::cout << "error: stack is empty\n";
        return 0xffffffffffffffff;
        }
      __m256 sv = _mm256_load_ps(ptr);
      return *reinterpret_cast<uint64_t*>(&sv.m256_f32[0]);
      }
    };

  }

struct empty_program : public compile_fixture
  {
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top, run(""));
    }
  };

struct floats : public compile_fixture
  {
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 32, run("3.14"));
    auto v = get_last_stack_value();
    TEST_EQ(3.14f, v.m256_f32[0]);
    TEST_EQ(3.14f, v.m256_f32[1]);
    TEST_EQ(3.14f, v.m256_f32[2]);
    TEST_EQ(3.14f, v.m256_f32[3]);
    TEST_EQ(3.14f, v.m256_f32[4]);
    TEST_EQ(3.14f, v.m256_f32[5]);
    TEST_EQ(3.14f, v.m256_f32[6]);
    TEST_EQ(3.14f, v.m256_f32[7]);

    TEST_EQ(ctxt.aligned_stack_top - 64, run("54"));
    v = get_last_stack_value();
    TEST_EQ(54.f, v.m256_f32[0]);
    TEST_EQ(54.f, v.m256_f32[1]);
    TEST_EQ(54.f, v.m256_f32[2]);
    TEST_EQ(54.f, v.m256_f32[3]);
    TEST_EQ(54.f, v.m256_f32[4]);
    TEST_EQ(54.f, v.m256_f32[5]);
    TEST_EQ(54.f, v.m256_f32[6]);
    TEST_EQ(54.f, v.m256_f32[7]);

    TEST_EQ(ctxt.aligned_stack_top - 128, run("100 20.45"));
    v = get_last_stack_value();
    TEST_EQ(20.45f, v.m256_f32[0]);
    TEST_EQ(20.45f, v.m256_f32[1]);
    TEST_EQ(20.45f, v.m256_f32[2]);
    TEST_EQ(20.45f, v.m256_f32[3]);
    TEST_EQ(20.45f, v.m256_f32[4]);
    TEST_EQ(20.45f, v.m256_f32[5]);
    TEST_EQ(20.45f, v.m256_f32[6]);
    TEST_EQ(20.45f, v.m256_f32[7]);
    }
  };


struct definitions : public compile_fixture
  {
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top, run(": pi 3.14;"));
    dictionary_entry e;
    TEST_ASSERT(find(e, dict, "pi"));
    TEST_ASSERT(e.words.size() == 1);
    TEST_ASSERT(e.words.front().type == token::T_FLOAT);
    TEST_ASSERT(e.words.front().value == "3.14");
    TEST_EQ(ctxt.aligned_stack_top - 32, run("pi"));
    auto v = get_last_stack_value();
    TEST_EQ(3.14f, v.m256_f32[0]);
    TEST_EQ(3.14f, v.m256_f32[1]);
    TEST_EQ(3.14f, v.m256_f32[2]);
    TEST_EQ(3.14f, v.m256_f32[3]);
    TEST_EQ(3.14f, v.m256_f32[4]);
    TEST_EQ(3.14f, v.m256_f32[5]);
    TEST_EQ(3.14f, v.m256_f32[6]);
    TEST_EQ(3.14f, v.m256_f32[7]);
    }
  };

struct add : public compile_fixture
  {
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 32, run("3.1 2.4 +"));
    auto v = get_last_stack_value();
    TEST_EQ(5.5f, v.m256_f32[0]);
    TEST_EQ(5.5f, v.m256_f32[1]);
    TEST_EQ(5.5f, v.m256_f32[2]);
    TEST_EQ(5.5f, v.m256_f32[3]);
    TEST_EQ(5.5f, v.m256_f32[4]);
    TEST_EQ(5.5f, v.m256_f32[5]);
    TEST_EQ(5.5f, v.m256_f32[6]);
    TEST_EQ(5.5f, v.m256_f32[7]);
    }
  };

struct sub : public compile_fixture
  {
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 32, run("3.1 2.4 -"));
    auto v = get_last_stack_value();
    TEST_EQ_CLOSE(0.7f, v.m256_f32[0], 1e-6f);
    TEST_EQ_CLOSE(0.7f, v.m256_f32[1], 1e-6f);
    TEST_EQ_CLOSE(0.7f, v.m256_f32[2], 1e-6f);
    TEST_EQ_CLOSE(0.7f, v.m256_f32[3], 1e-6f);
    TEST_EQ_CLOSE(0.7f, v.m256_f32[4], 1e-6f);
    TEST_EQ_CLOSE(0.7f, v.m256_f32[5], 1e-6f);
    TEST_EQ_CLOSE(0.7f, v.m256_f32[6], 1e-6f);
    TEST_EQ_CLOSE(0.7f, v.m256_f32[7], 1e-6f);
    }
  };

struct mul : public compile_fixture
  {
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 32, run("3.1 2.4 *"));
    auto v = get_last_stack_value();
    TEST_EQ_CLOSE(3.1f*2.4f, v.m256_f32[0], 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, v.m256_f32[1], 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, v.m256_f32[2], 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, v.m256_f32[3], 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, v.m256_f32[4], 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, v.m256_f32[5], 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, v.m256_f32[6], 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, v.m256_f32[7], 1e-6f);
    }
  };

struct division : public compile_fixture
  {
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 32, run("3.1 2.4 /"));
    auto v = get_last_stack_value();
    TEST_EQ_CLOSE(3.1f/2.4f, v.m256_f32[0], 1e-6f);
    TEST_EQ_CLOSE(3.1f/2.4f, v.m256_f32[1], 1e-6f);
    TEST_EQ_CLOSE(3.1f/2.4f, v.m256_f32[2], 1e-6f);
    TEST_EQ_CLOSE(3.1f/2.4f, v.m256_f32[3], 1e-6f);
    TEST_EQ_CLOSE(3.1f/2.4f, v.m256_f32[4], 1e-6f);
    TEST_EQ_CLOSE(3.1f/2.4f, v.m256_f32[5], 1e-6f);
    TEST_EQ_CLOSE(3.1f/2.4f, v.m256_f32[6], 1e-6f);
    TEST_EQ_CLOSE(3.1f/2.4f, v.m256_f32[7], 1e-6f);
    }
  };

struct v8 : public compile_fixture
  {
  void test()
    {
    // ymm0 = x7 / x6 / x5 / x4 / x3 / x2 / x1 / x0
    TEST_EQ(ctxt.aligned_stack_top - 32, run("v8 1 2 3 4 5 6 7 8"));
    auto v = get_last_stack_value();
    TEST_EQ(1.f, v.m256_f32[7]);
    TEST_EQ(2.f, v.m256_f32[6]);
    TEST_EQ(3.f, v.m256_f32[5]);
    TEST_EQ(4.f, v.m256_f32[4]);
    TEST_EQ(5.f, v.m256_f32[3]);
    TEST_EQ(6.f, v.m256_f32[2]);
    TEST_EQ(7.f, v.m256_f32[1]);
    TEST_EQ(8.f, v.m256_f32[0]);
    }
  };

struct avx_mathfun : public compile_fixture
  {
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 32, run("3.14159265359 sin"));
    auto v = get_last_stack_value();
    TEST_EQ_CLOSE(0.f, v.m256_f32[7], 1e-6f);
    TEST_EQ_CLOSE(0.f, v.m256_f32[6], 1e-6f);
    TEST_EQ_CLOSE(0.f, v.m256_f32[5], 1e-6f);
    TEST_EQ_CLOSE(0.f, v.m256_f32[4], 1e-6f);
    TEST_EQ_CLOSE(0.f, v.m256_f32[3], 1e-6f);
    TEST_EQ_CLOSE(0.f, v.m256_f32[2], 1e-6f);
    TEST_EQ_CLOSE(0.f, v.m256_f32[1], 1e-6f);
    TEST_EQ_CLOSE(0.f, v.m256_f32[0], 1e-6f);

    run("v8 0 1 2 3 4 5 6 7 sin");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(0.f, v.m256_f32[7], 1e-6f);
    TEST_EQ_CLOSE(0.841471f, v.m256_f32[6], 1e-6f);
    TEST_EQ_CLOSE(0.909297f, v.m256_f32[5], 1e-6f);
    TEST_EQ_CLOSE(0.14112f, v.m256_f32[4], 1e-6f);
    TEST_EQ_CLOSE(-0.756802f, v.m256_f32[3], 1e-6f);
    TEST_EQ_CLOSE(-0.958924f, v.m256_f32[2], 1e-6f);
    TEST_EQ_CLOSE(-0.279415f, v.m256_f32[1], 1e-6f);
    TEST_EQ_CLOSE(0.656987f, v.m256_f32[0], 1e-6f);

    run("v8 0 1 2 3 4 5 6 7 cos");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(1.f, v.m256_f32[7], 1e-6f);
    TEST_EQ_CLOSE(0.540302f, v.m256_f32[6], 1e-6f);
    TEST_EQ_CLOSE(-0.416147f, v.m256_f32[5], 1e-6f);
    TEST_EQ_CLOSE(-0.989992f, v.m256_f32[4], 1e-6f);
    TEST_EQ_CLOSE(-0.653644f, v.m256_f32[3], 1e-6f);
    TEST_EQ_CLOSE(0.283662f, v.m256_f32[2], 1e-6f);
    TEST_EQ_CLOSE(0.96017f, v.m256_f32[1], 1e-6f);
    TEST_EQ_CLOSE(0.753902f, v.m256_f32[0], 1e-6f);

    run("v8 0 1 2 3 4 5 6 7 exp");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(1.f, v.m256_f32[7], 1e-6f);
    TEST_EQ_CLOSE(2.71828f, v.m256_f32[6], 1e-5f);
    TEST_EQ_CLOSE(7.38906f, v.m256_f32[5], 1e-5f);
    TEST_EQ_CLOSE(20.0855f, v.m256_f32[4], 1e-4f);
    TEST_EQ_CLOSE(54.5982f, v.m256_f32[3], 1e-4f);
    TEST_EQ_CLOSE(148.413f, v.m256_f32[2], 1e-3f);
    TEST_EQ_CLOSE(403.429f, v.m256_f32[1], 1e-3f);
    TEST_EQ_CLOSE(1096.63f, v.m256_f32[0], 1e-2f);

    run("v8 0 1 2 3 4 5 6 7 log");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(-std::numeric_limits<float>::infinity(), v.m256_f32[7], 1e-6f);
    TEST_EQ_CLOSE(0.f, v.m256_f32[6], 1e-6f);
    TEST_EQ_CLOSE(0.693147f, v.m256_f32[5], 1e-6f);
    TEST_EQ_CLOSE(1.09861f, v.m256_f32[4], 1e-5f);
    TEST_EQ_CLOSE(1.38629f, v.m256_f32[3], 1e-5f);
    TEST_EQ_CLOSE(1.60944f, v.m256_f32[2], 1e-5f);
    TEST_EQ_CLOSE(1.79176f, v.m256_f32[1], 1e-5f);
    TEST_EQ_CLOSE(1.94591f, v.m256_f32[0], 1e-5f);

    run("v8 1 4 3 2 3 2 2 2 v8 0 1 2 3 4 5 6 7 **");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(1.f, v.m256_f32[7], 1e-6f);
    TEST_EQ_CLOSE(4.f, v.m256_f32[6], 1e-6f);
    TEST_EQ_CLOSE(9.f, v.m256_f32[5], 1e-6f);
    TEST_EQ_CLOSE(8.f, v.m256_f32[4], 1e-6f);
    TEST_EQ_CLOSE(81.f, v.m256_f32[3], 1e-5f);
    TEST_EQ_CLOSE(32.f, v.m256_f32[2], 1e-5f);
    TEST_EQ_CLOSE(64.f, v.m256_f32[1], 1e-5f);
    TEST_EQ_CLOSE(128.f, v.m256_f32[0], 1e-5f);

    run("v8 1 4 9 16 25 36 49 64 sqrt");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(1.f, v.m256_f32[7], 1e-6f);
    TEST_EQ_CLOSE(2.f, v.m256_f32[6], 1e-6f);
    TEST_EQ_CLOSE(3.f, v.m256_f32[5], 1e-6f);
    TEST_EQ_CLOSE(4.f, v.m256_f32[4], 1e-6f);
    TEST_EQ_CLOSE(5.f, v.m256_f32[3], 1e-5f);
    TEST_EQ_CLOSE(6.f, v.m256_f32[2], 1e-5f);
    TEST_EQ_CLOSE(7.f, v.m256_f32[1], 1e-5f);
    TEST_EQ_CLOSE(8.f, v.m256_f32[0], 1e-5f);
    }
  };

struct prim_test : public compile_fixture
  {
  void test()
    {
    run("1 2 3 drop");
    TEST_EQ(2.f, get_stack_valuef(0));
    TEST_EQ(1.f, get_stack_valuef(1));

    run("4 5 6 swap");
    TEST_EQ(5.f, get_stack_valuef(0));
    TEST_EQ(6.f, get_stack_valuef(1));
    TEST_EQ(4.f, get_stack_valuef(2));

    run("2 4 6 dup");
    TEST_EQ(6.f, get_stack_valuef(0));
    TEST_EQ(6.f, get_stack_valuef(1));
    TEST_EQ(4.f, get_stack_valuef(2));
    TEST_EQ(2.f, get_stack_valuef(3));

    run("2 4 6 over");
    TEST_EQ(4.f, get_stack_valuef(0));
    TEST_EQ(6.f, get_stack_valuef(1));
    TEST_EQ(4.f, get_stack_valuef(2));
    TEST_EQ(2.f, get_stack_valuef(3));

    run("1 2 3 4 rot");
    TEST_EQ(2.f, get_stack_valuef(0));
    TEST_EQ(4.f, get_stack_valuef(1));
    TEST_EQ(3.f, get_stack_valuef(2));
    TEST_EQ(1.f, get_stack_valuef(3));

    run("1 2 3 4 -rot");
    TEST_EQ(3.f, get_stack_valuef(0));
    TEST_EQ(2.f, get_stack_valuef(1));
    TEST_EQ(4.f, get_stack_valuef(2));
    TEST_EQ(1.f, get_stack_valuef(3));

    run("9 10 11 12 2drop");
    TEST_EQ(10.f, get_stack_valuef(0));
    TEST_EQ(9.f, get_stack_valuef(1));    

    run("9 10 11 12 2dup");
    TEST_EQ(12.f, get_stack_valuef(0));
    TEST_EQ(11.f, get_stack_valuef(1));
    TEST_EQ(12.f, get_stack_valuef(2));
    TEST_EQ(11.f, get_stack_valuef(3));
    TEST_EQ(10.f, get_stack_valuef(4));
    TEST_EQ(9.f, get_stack_valuef(5));

    run("9 10 11 12 2swap");
    TEST_EQ(10.f, get_stack_valuef(0));
    TEST_EQ(9.f, get_stack_valuef(1));
    TEST_EQ(12.f, get_stack_valuef(2));
    TEST_EQ(11.f, get_stack_valuef(3));   
    }
  };

struct integers : public compile_fixture
  {
  void test()
    {
    run("#123");
    TEST_EQ(123, get_stack_valuei(0));
    TEST_EQ(123, get_stack_valuef_uint64(0));
    run("#xff");
    TEST_EQ(255, get_stack_valuei(0));
    TEST_EQ(255, get_stack_valuef_uint64(0));
    run("#xabc");
    TEST_EQ(2748, get_stack_valuei(0));
    TEST_EQ(2748, get_stack_valuef_uint64(0));

    run("v8 #1 #2 #3 #4 #5 #6 #7 #8");
    auto v = get_last_stack_value_i();
    TEST_EQ(1, v.m256i_u32[7]);
    TEST_EQ(2, v.m256i_u32[6]);
    TEST_EQ(3, v.m256i_u32[5]);
    TEST_EQ(4, v.m256i_u32[4]);
    TEST_EQ(5, v.m256i_u32[3]);
    TEST_EQ(6, v.m256i_u32[2]);
    TEST_EQ(7, v.m256i_u32[1]);
    TEST_EQ(8, v.m256i_u32[0]);

    run("#100 #1+");
    TEST_EQ(101, get_stack_valuei(0));

    run("#100 #1-");
    TEST_EQ(99, get_stack_valuei(0));

    run("#100 #32+");
    TEST_EQ(132, get_stack_valuei(0));

    run("#100 #32-");
    TEST_EQ(68, get_stack_valuei(0));

    run("#100 #64+");
    TEST_EQ(164, get_stack_valuei(0));

    run("#100 #64-");
    TEST_EQ(36, get_stack_valuei(0));

    run("#100 #96+");
    TEST_EQ(196, get_stack_valuei(0));

    run("#100 #96-");
    TEST_EQ(4, get_stack_valuei(0));

    run("#200 #128+");
    TEST_EQ(328, get_stack_valuei(0));

    run("#200 #128-");
    TEST_EQ(72, get_stack_valuei(0));

    run("#100 #12 #+");
    TEST_EQ(112, get_stack_valuei(0));

    run("#100 #12 #-");
    TEST_EQ(88, get_stack_valuei(0));

    run("#100 #12 #*");
    TEST_EQ(1200, get_stack_valuei(0));

    run("#-7 #8 #*");
    TEST_EQ(-56, get_stack_valuei(0));

    run("#-7 #-8 #*");
    TEST_EQ(56, get_stack_valuei(0));

    run("#70 #10 #/");
    TEST_EQ(7, get_stack_valuei(0));

    run("#70 #-10 #/");
    TEST_EQ(-7, get_stack_valuei(0));

    run("#-70 #10 #/");
    TEST_EQ(-7, get_stack_valuei(0));
    }
  };

struct comparisons : public compile_fixture
  {
  void test()
    {
    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 =");
    auto v = get_last_stack_value_i();
    TEST_EQ(-1, v.m256i_i32[7]);
    TEST_EQ(-1, v.m256i_i32[6]);
    TEST_EQ(0, v.m256i_i32[5]);
    TEST_EQ(0, v.m256i_i32[4]);
    TEST_EQ(0, v.m256i_i32[3]);
    TEST_EQ(0, v.m256i_i32[2]);
    TEST_EQ(0, v.m256i_i32[1]);
    TEST_EQ(-1, v.m256i_i32[0]);

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 <>");
    v = get_last_stack_value_i();
    TEST_EQ(0, v.m256i_i32[7]);
    TEST_EQ(0, v.m256i_i32[6]);
    TEST_EQ(-1, v.m256i_i32[5]);
    TEST_EQ(-1, v.m256i_i32[4]);
    TEST_EQ(-1, v.m256i_i32[3]);
    TEST_EQ(-1, v.m256i_i32[2]);
    TEST_EQ(-1, v.m256i_i32[1]);
    TEST_EQ(0, v.m256i_i32[0]);

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 <");
    v = get_last_stack_value_i();
    TEST_EQ(0, v.m256i_i32[7]);
    TEST_EQ(0, v.m256i_i32[6]);
    TEST_EQ(0, v.m256i_i32[5]);
    TEST_EQ(0, v.m256i_i32[4]);
    TEST_EQ(-1, v.m256i_i32[3]);
    TEST_EQ(-1, v.m256i_i32[2]);
    TEST_EQ(-1, v.m256i_i32[1]);
    TEST_EQ(0, v.m256i_i32[0]);

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 <=");
    v = get_last_stack_value_i();
    TEST_EQ(-1, v.m256i_i32[7]);
    TEST_EQ(-1, v.m256i_i32[6]);
    TEST_EQ(0, v.m256i_i32[5]);
    TEST_EQ(0, v.m256i_i32[4]);
    TEST_EQ(-1, v.m256i_i32[3]);
    TEST_EQ(-1, v.m256i_i32[2]);
    TEST_EQ(-1, v.m256i_i32[1]);
    TEST_EQ(-1, v.m256i_i32[0]);

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 >");
    v = get_last_stack_value_i();
    TEST_EQ(0, v.m256i_i32[7]);
    TEST_EQ(0, v.m256i_i32[6]);
    TEST_EQ(-1, v.m256i_i32[5]);
    TEST_EQ(-1, v.m256i_i32[4]);
    TEST_EQ(0, v.m256i_i32[3]);
    TEST_EQ(0, v.m256i_i32[2]);
    TEST_EQ(0, v.m256i_i32[1]);
    TEST_EQ(0, v.m256i_i32[0]);

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 >=");
    v = get_last_stack_value_i();
    TEST_EQ(-1, v.m256i_i32[7]);
    TEST_EQ(-1, v.m256i_i32[6]);
    TEST_EQ(-1, v.m256i_i32[5]);
    TEST_EQ(-1, v.m256i_i32[4]);
    TEST_EQ(0, v.m256i_i32[3]);
    TEST_EQ(0, v.m256i_i32[2]);
    TEST_EQ(0, v.m256i_i32[1]);
    TEST_EQ(-1, v.m256i_i32[0]);


    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 f=");
    auto f = get_last_stack_value();
    TEST_EQ(1.f, f.m256_f32[7]);
    TEST_EQ(1.f, f.m256_f32[6]);
    TEST_EQ(0.f, f.m256_f32[5]);
    TEST_EQ(0.f, f.m256_f32[4]);
    TEST_EQ(0.f, f.m256_f32[3]);
    TEST_EQ(0.f, f.m256_f32[2]);
    TEST_EQ(0.f, f.m256_f32[1]);
    TEST_EQ(1.f, f.m256_f32[0]);

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 f<>");
    f = get_last_stack_value();
    TEST_EQ(0.f, f.m256_f32[7]);
    TEST_EQ(0.f, f.m256_f32[6]);
    TEST_EQ(1.f, f.m256_f32[5]);
    TEST_EQ(1.f, f.m256_f32[4]);
    TEST_EQ(1.f, f.m256_f32[3]);
    TEST_EQ(1.f, f.m256_f32[2]);
    TEST_EQ(1.f, f.m256_f32[1]);
    TEST_EQ(0.f, f.m256_f32[0]);

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 f<");
    f = get_last_stack_value();
    TEST_EQ(0.f, f.m256_f32[7]);
    TEST_EQ(0.f, f.m256_f32[6]);
    TEST_EQ(0.f, f.m256_f32[5]);
    TEST_EQ(0.f, f.m256_f32[4]);
    TEST_EQ(1.f, f.m256_f32[3]);
    TEST_EQ(1.f, f.m256_f32[2]);
    TEST_EQ(1.f, f.m256_f32[1]);
    TEST_EQ(0.f, f.m256_f32[0]);

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 f<=");
    f = get_last_stack_value();
    TEST_EQ(1.f, f.m256_f32[7]);
    TEST_EQ(1.f, f.m256_f32[6]);
    TEST_EQ(0.f, f.m256_f32[5]);
    TEST_EQ(0.f, f.m256_f32[4]);
    TEST_EQ(1.f, f.m256_f32[3]);
    TEST_EQ(1.f, f.m256_f32[2]);
    TEST_EQ(1.f, f.m256_f32[1]);
    TEST_EQ(1.f, f.m256_f32[0]);

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 f>");
    f = get_last_stack_value();
    TEST_EQ(0.f, f.m256_f32[7]);
    TEST_EQ(0.f, f.m256_f32[6]);
    TEST_EQ(1.f, f.m256_f32[5]);
    TEST_EQ(1.f, f.m256_f32[4]);
    TEST_EQ(0.f, f.m256_f32[3]);
    TEST_EQ(0.f, f.m256_f32[2]);
    TEST_EQ(0.f, f.m256_f32[1]);
    TEST_EQ(0.f, f.m256_f32[0]);

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 f>=");
    f = get_last_stack_value();
    TEST_EQ(1.f, f.m256_f32[7]);
    TEST_EQ(1.f, f.m256_f32[6]);
    TEST_EQ(1.f, f.m256_f32[5]);
    TEST_EQ(1.f, f.m256_f32[4]);
    TEST_EQ(0.f, f.m256_f32[3]);
    TEST_EQ(0.f, f.m256_f32[2]);
    TEST_EQ(0.f, f.m256_f32[1]);
    TEST_EQ(1.f, f.m256_f32[0]);
    }
  };

struct address : public compile_fixture
  {
  void test()
    {
    char* ptr = run("12345 2 3 4 5 st@ sp@");
    uint64_t st = get_stack_valuef_uint64(1);
    TEST_EQ((uint64_t)ctxt.aligned_stack_top, st);
    uint64_t sp = get_stack_valuef_uint64(0);
    TEST_EQ((uint64_t)(ptr+32), sp);

    run("st@ #32- @"); // get stack top, then move 32 bytes back, then fetch
    auto v = get_last_stack_value();
    TEST_EQ(12345.f, v.m256_f32[7]);
    TEST_EQ(12345.f, v.m256_f32[6]);
    TEST_EQ(12345.f, v.m256_f32[5]);
    TEST_EQ(12345.f, v.m256_f32[4]);
    TEST_EQ(12345.f, v.m256_f32[3]);
    TEST_EQ(12345.f, v.m256_f32[2]);
    TEST_EQ(12345.f, v.m256_f32[1]);
    TEST_EQ(12345.f, v.m256_f32[0]);

    run("101 st@ #32- !   st@ #32- @ "); // store 101 on stack, get stack top, move 32 bytes back, store 101 at address,  get stack top, move 32 bytes back, fetch
    v = get_last_stack_value();
    TEST_EQ(101.f, v.m256_f32[7]);
    TEST_EQ(101.f, v.m256_f32[6]);
    TEST_EQ(101.f, v.m256_f32[5]);
    TEST_EQ(101.f, v.m256_f32[4]);
    TEST_EQ(101.f, v.m256_f32[3]);
    TEST_EQ(101.f, v.m256_f32[2]);
    TEST_EQ(101.f, v.m256_f32[1]);
    TEST_EQ(101.f, v.m256_f32[0]);
    }
  };

struct prim : public compile_fixture
  {
  void test()
    {
    run("v8 -1 2 -3 4 5 -6 7 -8 abs");
    auto v = get_last_stack_value();
    TEST_EQ(1.f, v.m256_f32[7]);
    TEST_EQ(2.f, v.m256_f32[6]);
    TEST_EQ(3.f, v.m256_f32[5]);
    TEST_EQ(4.f, v.m256_f32[4]);
    TEST_EQ(5.f, v.m256_f32[3]);
    TEST_EQ(6.f, v.m256_f32[2]);
    TEST_EQ(7.f, v.m256_f32[1]);
    TEST_EQ(8.f, v.m256_f32[0]);

    run("v8 -1 2 -3 4 5 -6 7 -8 negate");
    v = get_last_stack_value();
    TEST_EQ(1.f, v.m256_f32[7]);
    TEST_EQ(-2.f, v.m256_f32[6]);
    TEST_EQ(3.f, v.m256_f32[5]);
    TEST_EQ(-4.f, v.m256_f32[4]);
    TEST_EQ(-5.f, v.m256_f32[3]);
    TEST_EQ(6.f, v.m256_f32[2]);
    TEST_EQ(-7.f, v.m256_f32[1]);
    TEST_EQ(8.f, v.m256_f32[0]);

    run("v8 #1 #3 #7 #15 #31 #63 #127 #255  v8 #10 #10 #10 #10 #10 #10 #10 #10  and");
    auto i = get_last_stack_value_i();
    TEST_EQ(0, i.m256i_u32[7]);
    TEST_EQ(2, i.m256i_u32[6]);
    TEST_EQ(2, i.m256i_u32[5]);
    TEST_EQ(10, i.m256i_u32[4]);
    TEST_EQ(10, i.m256i_u32[3]);
    TEST_EQ(10, i.m256i_u32[2]);
    TEST_EQ(10, i.m256i_u32[1]);
    TEST_EQ(10, i.m256i_u32[0]);

    run("v8 #1 #3 #7 #15 #31 #63 #127 #255  v8 #10 #10 #10 #10 #10 #10 #10 #10  or");
    i = get_last_stack_value_i();
    TEST_EQ(11, i.m256i_u32[7]);
    TEST_EQ(11, i.m256i_u32[6]);
    TEST_EQ(15, i.m256i_u32[5]);
    TEST_EQ(15, i.m256i_u32[4]);
    TEST_EQ(31, i.m256i_u32[3]);
    TEST_EQ(63, i.m256i_u32[2]);
    TEST_EQ(127, i.m256i_u32[1]);
    TEST_EQ(255, i.m256i_u32[0]);

    run("v8 #1 #3 #7 #15 #31 #63 #127 #255  v8 #10 #10 #10 #10 #10 #10 #10 #10  xor");
    i = get_last_stack_value_i();
    TEST_EQ(11, i.m256i_u32[7]);
    TEST_EQ(9, i.m256i_u32[6]);
    TEST_EQ(13, i.m256i_u32[5]);
    TEST_EQ(5, i.m256i_u32[4]);
    TEST_EQ(21, i.m256i_u32[3]);
    TEST_EQ(53, i.m256i_u32[2]);
    TEST_EQ(117, i.m256i_u32[1]);
    TEST_EQ(245, i.m256i_u32[0]);

    run("v8 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 floor");
    v = get_last_stack_value();
    TEST_EQ(1.f, v.m256_f32[7]);
    TEST_EQ(-2.f, v.m256_f32[6]);
    TEST_EQ(1.f, v.m256_f32[5]);
    TEST_EQ(-2.f, v.m256_f32[4]);
    TEST_EQ(3.f, v.m256_f32[3]);
    TEST_EQ(-3.f, v.m256_f32[2]);
    TEST_EQ(1.f, v.m256_f32[1]);
    TEST_EQ(-2.f, v.m256_f32[0]);

    run("v8 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 ceil");
    v = get_last_stack_value();
    TEST_EQ(2.f, v.m256_f32[7]);
    TEST_EQ(-1.f, v.m256_f32[6]);
    TEST_EQ(2.f, v.m256_f32[5]);
    TEST_EQ(-1.f, v.m256_f32[4]);
    TEST_EQ(3.f, v.m256_f32[3]);
    TEST_EQ(-3.f, v.m256_f32[2]);
    TEST_EQ(2.f, v.m256_f32[1]);
    TEST_EQ(-1.f, v.m256_f32[0]);

    run("v8 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 round");
    v = get_last_stack_value();
    TEST_EQ(1.f, v.m256_f32[7]);
    TEST_EQ(-1.f, v.m256_f32[6]);
    TEST_EQ(2.f, v.m256_f32[5]);
    TEST_EQ(-2.f, v.m256_f32[4]);
    TEST_EQ(3.f, v.m256_f32[3]);
    TEST_EQ(-3.f, v.m256_f32[2]);
    TEST_EQ(2.f, v.m256_f32[1]);
    TEST_EQ(-2.f, v.m256_f32[0]);

    run("v8 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 trunc");
    v = get_last_stack_value();
    TEST_EQ(1.f, v.m256_f32[7]);
    TEST_EQ(-1.f, v.m256_f32[6]);
    TEST_EQ(1.f, v.m256_f32[5]);
    TEST_EQ(-1.f, v.m256_f32[4]);
    TEST_EQ(3.f, v.m256_f32[3]);
    TEST_EQ(-3.f, v.m256_f32[2]);
    TEST_EQ(1.f, v.m256_f32[1]);
    TEST_EQ(-1.f, v.m256_f32[0]);
    
    run("v8 #-1 #xf0f0f0f0 #x0f0f0f0f #0 #0 #0 #0 #0 not");
    i = get_last_stack_value_i();
    TEST_EQ(0, i.m256i_u32[7]);
    TEST_EQ(0x0f0f0f0f, i.m256i_u32[6]);
    TEST_EQ(0xf0f0f0f0, i.m256i_u32[5]);
    TEST_EQ(-1, i.m256i_u32[4]);

    run("v8 #1 #2 #3 #4 #5 #6 #7 #8 fcast");
    v = get_last_stack_value();
    TEST_EQ(1.f, v.m256_f32[7]);
    TEST_EQ(2.f, v.m256_f32[6]);
    TEST_EQ(3.f, v.m256_f32[5]);
    TEST_EQ(4.f, v.m256_f32[4]);
    TEST_EQ(5.f, v.m256_f32[3]);
    TEST_EQ(6.f, v.m256_f32[2]);
    TEST_EQ(7.f, v.m256_f32[1]);
    TEST_EQ(8.f, v.m256_f32[0]);

    run("v8 1 2 3 4 5 6 7 8 icast");
    i = get_last_stack_value_i();
    TEST_EQ(1, i.m256i_u32[7]);
    TEST_EQ(2, i.m256i_u32[6]);
    TEST_EQ(3, i.m256i_u32[5]);
    TEST_EQ(4, i.m256i_u32[4]);
    TEST_EQ(5, i.m256i_u32[3]);
    TEST_EQ(6, i.m256i_u32[2]);
    TEST_EQ(7, i.m256i_u32[1]);
    TEST_EQ(8, i.m256i_u32[0]);

    run("4.2 2.1 fm/mod");
    TEST_EQ(2.f, get_stack_valuef(0));
    TEST_EQ(0.f, get_stack_valuef(1));

    run("4.2 2 fm/mod");
    TEST_EQ(2.f, get_stack_valuef(0));
    TEST_EQ_CLOSE(0.2f, get_stack_valuef(1), 1e-5f);

    run("10 3 fm/mod");
    TEST_EQ(3.f, get_stack_valuef(0));
    TEST_EQ_CLOSE(1.f, get_stack_valuef(1), 1e-5f);

    run("v8 1 2 3 4 5 6 7 8 v8 8 7 6 5 4 3 2 1 min");
    v = get_last_stack_value();
    TEST_EQ(1.f, v.m256_f32[7]);
    TEST_EQ(2.f, v.m256_f32[6]);
    TEST_EQ(3.f, v.m256_f32[5]);
    TEST_EQ(4.f, v.m256_f32[4]);
    TEST_EQ(4.f, v.m256_f32[3]);
    TEST_EQ(3.f, v.m256_f32[2]);
    TEST_EQ(2.f, v.m256_f32[1]);
    TEST_EQ(1.f, v.m256_f32[0]);

    run("v8 1 2 3 4 5 6 7 8 v8 8 7 6 5 4 3 2 1 max");
    v = get_last_stack_value();
    TEST_EQ(8.f, v.m256_f32[7]);
    TEST_EQ(7.f, v.m256_f32[6]);
    TEST_EQ(6.f, v.m256_f32[5]);
    TEST_EQ(5.f, v.m256_f32[4]);
    TEST_EQ(5.f, v.m256_f32[3]);
    TEST_EQ(6.f, v.m256_f32[2]);
    TEST_EQ(7.f, v.m256_f32[1]);
    TEST_EQ(8.f, v.m256_f32[0]);
    }
  };

struct complex : public compile_fixture
  {
  void test()
    {
    run("3 4 -1 2 z*");
    TEST_EQ(-11.f, get_stack_valuef(1));
    TEST_EQ(2.f, get_stack_valuef(0));

    run("3 4 -1 2 z+");
    TEST_EQ(2.f, get_stack_valuef(1));
    TEST_EQ(6.f, get_stack_valuef(0));

    run("3 4 -1 2 z-");
    TEST_EQ(4.f, get_stack_valuef(1));
    TEST_EQ(2.f, get_stack_valuef(0));
    }
  };

struct return_stack : public compile_fixture
  {
  void test()
    {
    run("1 2 3 push + pop *");
    TEST_EQ(9.f, get_stack_valuef(0));

    run("1 2 3 >r + r> *");
    TEST_EQ(9.f, get_stack_valuef(0));

    run("1 >r 2 3 r@");
    TEST_EQ(1.f, get_stack_valuef(0));

    run("1 >r 2 3 r@ + + pop +");
    TEST_EQ(7.f, get_stack_valuef(0));

    run("0.5 push 0 sin pop");
    TEST_EQ(0.5f, get_stack_valuef(0));
    }
  };

struct redefine_primitives : public compile_fixture
  {
  void test()
    {
    run(": sin 3.1415926535 2 * * sin ; 0.1 sin");
    TEST_EQ(0.58778525229f, get_stack_valuef(0));

    run(": sin cos ; 5 sin 5 cos");
    TEST_EQ(get_stack_valuef(1), get_stack_valuef(0));
    }
  };

struct if_tests : public compile_fixture
  {
  void test()
    {
    /*
    if <true> else <false> then should be balanced:
    - both branches should add equally amount on the stack
    - any usage of the return stack should be balanced
    - no branch is allowed to pop from the stack that existed before the branch
    */
    run("v8 1 2 3 4 5 6 7 8 4 > if 2 else 3 then");
    auto v = get_stack_value(0);
    TEST_EQ(3.f, v.m256_f32[7]);
    TEST_EQ(3.f, v.m256_f32[6]);
    TEST_EQ(3.f, v.m256_f32[5]);
    TEST_EQ(3.f, v.m256_f32[4]);
    TEST_EQ(2.f, v.m256_f32[3]);
    TEST_EQ(2.f, v.m256_f32[2]);
    TEST_EQ(2.f, v.m256_f32[1]);
    TEST_EQ(2.f, v.m256_f32[0]);

    run("v8 1 2 3 4 5 6 7 8 2 fm/mod drop 0 = if 10 12 else 11 13 then");
    v = get_stack_value(0);
    TEST_EQ(13.f, v.m256_f32[7]);
    TEST_EQ(12.f, v.m256_f32[6]);
    TEST_EQ(13.f, v.m256_f32[5]);
    TEST_EQ(12.f, v.m256_f32[4]);
    TEST_EQ(13.f, v.m256_f32[3]);
    TEST_EQ(12.f, v.m256_f32[2]);
    TEST_EQ(13.f, v.m256_f32[1]);
    TEST_EQ(12.f, v.m256_f32[0]);
    v = get_stack_value(1);
    TEST_EQ(11.f, v.m256_f32[7]);
    TEST_EQ(10.f, v.m256_f32[6]);
    TEST_EQ(11.f, v.m256_f32[5]);
    TEST_EQ(10.f, v.m256_f32[4]);
    TEST_EQ(11.f, v.m256_f32[3]);
    TEST_EQ(10.f, v.m256_f32[2]);
    TEST_EQ(11.f, v.m256_f32[1]);
    TEST_EQ(10.f, v.m256_f32[0]);

    run("1 2 > if 3 else 5 then");
    v = get_stack_value(0);
    TEST_EQ(5.f, v.m256_f32[7]);
    TEST_EQ(5.f, v.m256_f32[6]);
    TEST_EQ(5.f, v.m256_f32[5]);
    TEST_EQ(5.f, v.m256_f32[4]);
    TEST_EQ(5.f, v.m256_f32[3]);
    TEST_EQ(5.f, v.m256_f32[2]);
    TEST_EQ(5.f, v.m256_f32[1]);
    TEST_EQ(5.f, v.m256_f32[0]);

    run("1 2 < if 3 else 5 then");
    v = get_stack_value(0);
    TEST_EQ(3.f, v.m256_f32[7]);
    TEST_EQ(3.f, v.m256_f32[6]);
    TEST_EQ(3.f, v.m256_f32[5]);
    TEST_EQ(3.f, v.m256_f32[4]);
    TEST_EQ(3.f, v.m256_f32[3]);
    TEST_EQ(3.f, v.m256_f32[2]);
    TEST_EQ(3.f, v.m256_f32[1]);
    TEST_EQ(3.f, v.m256_f32[0]);
    
    run("1 2 < if 3 2 1 else 5 6 7 then");    
    for (int i = 0; i < 3; ++i)
      {
      v = get_stack_value(i);
      for (int j = 0; j < 8; ++j)
        TEST_EQ(i == 0 ? 1.f : i == 1 ? 2.f : 3.f, v.m256_f32[j]);
      }
   
    run("1 2 > if 3 2 1 else 5 6 7 then");
    for (int i = 0; i < 3; ++i)
      {
      v = get_stack_value(i);
      for (int j = 0; j < 8; ++j)
        TEST_EQ(i == 0 ? 7.f : i == 1 ? 6.f : 5.f, v.m256_f32[j]);
      }

    run("v8 1 2 3 4 5 6 7 8 dup 2 fm/mod drop 0 = if dup 4 > if 40 else 39 then else  dup 2 < if 11 else 27 then then");
    v = get_stack_value(0);
    TEST_EQ(11.f, v.m256_f32[7]);
    TEST_EQ(39.f, v.m256_f32[6]);
    TEST_EQ(27.f, v.m256_f32[5]);
    TEST_EQ(39.f, v.m256_f32[4]);
    TEST_EQ(27.f, v.m256_f32[3]);
    TEST_EQ(40.f, v.m256_f32[2]);
    TEST_EQ(27.f, v.m256_f32[1]);
    TEST_EQ(40.f, v.m256_f32[0]);

    run("3 4 > if 2 > 1 if 10 else 11 then else 7 8 < if 2 else 20 then then");
    v = get_stack_value(0);
    TEST_EQ(2.f, v.m256_f32[7]);
    TEST_EQ(2.f, v.m256_f32[6]);
    TEST_EQ(2.f, v.m256_f32[5]);
    TEST_EQ(2.f, v.m256_f32[4]);
    TEST_EQ(2.f, v.m256_f32[3]);
    TEST_EQ(2.f, v.m256_f32[2]);
    TEST_EQ(2.f, v.m256_f32[1]);
    TEST_EQ(2.f, v.m256_f32[0]);
    }
  };

struct stdlib_tests : public compile_fixture
  {
  void test()
    {
    run("3 4 5 nip");
    TEST_EQ(5.f, get_stack_valuef(0));
    TEST_EQ(3.f, get_stack_valuef(1));

    run("3 4 5 tuck");
    TEST_EQ(5.f, get_stack_valuef(0));
    TEST_EQ(4.f, get_stack_valuef(1));
    TEST_EQ(5.f, get_stack_valuef(2));
    TEST_EQ(3.f, get_stack_valuef(3));

    run("5 4 3 2 1 0 #3 pick");
    TEST_EQ(3.f, get_stack_valuef(0));
    TEST_EQ(0.f, get_stack_valuef(1));
    TEST_EQ(1.f, get_stack_valuef(2));
    TEST_EQ(2.f, get_stack_valuef(3));
    TEST_EQ(3.f, get_stack_valuef(4));
    TEST_EQ(4.f, get_stack_valuef(5));
    TEST_EQ(5.f, get_stack_valuef(6));

    run("5 4 3 2 1 0 #0 pick");
    TEST_EQ(0.f, get_stack_valuef(0));
    TEST_EQ(0.f, get_stack_valuef(1));
    TEST_EQ(1.f, get_stack_valuef(2));
    TEST_EQ(2.f, get_stack_valuef(3));
    TEST_EQ(3.f, get_stack_valuef(4));
    TEST_EQ(4.f, get_stack_valuef(5));
    TEST_EQ(5.f, get_stack_valuef(6));

    run("5 4 3 2 1 0 #5 pick");
    TEST_EQ(5.f, get_stack_valuef(0));
    TEST_EQ(0.f, get_stack_valuef(1));
    TEST_EQ(1.f, get_stack_valuef(2));
    TEST_EQ(2.f, get_stack_valuef(3));
    TEST_EQ(3.f, get_stack_valuef(4));
    TEST_EQ(4.f, get_stack_valuef(5));
    TEST_EQ(5.f, get_stack_valuef(6));


    run("2 1 3 within");
    auto u = get_last_stack_value_i();
    TEST_EQ(-1, u.m256i_i32[7]);
    TEST_EQ(-1, u.m256i_i32[6]);
    TEST_EQ(-1, u.m256i_i32[5]);
    TEST_EQ(-1, u.m256i_i32[4]);
    TEST_EQ(-1, u.m256i_i32[3]);
    TEST_EQ(-1, u.m256i_i32[2]);
    TEST_EQ(-1, u.m256i_i32[1]);
    TEST_EQ(-1, u.m256i_i32[0]);


    run("v8 0 0.5 1 1.5 2 2.5 3 3.5 1 3 within");
    u = get_last_stack_value_i();
    TEST_EQ(0, u.m256i_i32[7]);
    TEST_EQ(0, u.m256i_i32[6]);
    TEST_EQ(-1, u.m256i_i32[5]);
    TEST_EQ(-1, u.m256i_i32[4]);
    TEST_EQ(-1, u.m256i_i32[3]);
    TEST_EQ(-1, u.m256i_i32[2]);
    TEST_EQ(0, u.m256i_i32[1]);
    TEST_EQ(0, u.m256i_i32[0]);

    ctxt.stack_pointer = ctxt.aligned_stack_top;
    run("100 200 300 depth");
    TEST_EQ(3, get_stack_valuei(0));

    ctxt.stack_pointer = ctxt.aligned_stack_top;
    run("100 200 300 400 500 depth");
    TEST_EQ(5, get_stack_valuei(0));

    ctxt.stack_pointer = ctxt.aligned_stack_top;
    run("100 200 300 400 500 rdepth");
    TEST_EQ(0, get_stack_valuei(0));

    ctxt.stack_pointer = ctxt.aligned_stack_top;
    run("100 200 300 400 500 >r >r rdepth");
    TEST_EQ(2, get_stack_valuei(0));

    run("v8 0 0.5 1 1.5 2 2.5 3 3.5 1 3 clamp");
    auto v = get_last_stack_value();
    TEST_EQ(1.f, v.m256_f32[7]);
    TEST_EQ(1.f, v.m256_f32[6]);
    TEST_EQ(1.f, v.m256_f32[5]);
    TEST_EQ(1.5f, v.m256_f32[4]);
    TEST_EQ(2.f, v.m256_f32[3]);
    TEST_EQ(2.5f, v.m256_f32[2]);
    TEST_EQ(3.f, v.m256_f32[1]);
    TEST_EQ(3.f, v.m256_f32[0]);
    }
  };

VF_END

void run_all_compile_tests()
  {
  using namespace VF;
  empty_program().test();
  floats().test();
  definitions().test();
  add().test();
  sub().test();
  mul().test();
  division().test();
  v8().test();
  avx_mathfun().test();
  prim_test().test();
  integers().test();
  comparisons().test();
  address().test();
  prim().test();
  complex().test();
  return_stack().test();
  redefine_primitives().test();
  if_tests().test();
  stdlib_tests().test();
  }