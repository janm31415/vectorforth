#include "compile_tests.h"
#include "test_assert.h"

#include <asm/assembler.h>

#include <vectorforth/context.h>
#include <vectorforth/compiler.h>
#include <vectorforth/compile_data.h>
#include <vectorforth/debug.h>
#include <vectorforth/dictionary.h>
#include <vectorforth/stdlib.h>
#include <vectorforth/tokenize.h>

#include <immintrin.h>

#include <iostream>

#include <cmath>

using namespace ASM;

VF_BEGIN

namespace
  {

#ifdef AVX512

  int32_t get_avx2_i32(__m512i v, int i)
    {
#ifdef _WIN32
    return v.m512i_i32[i];
#else
    int32_t* a = (int32_t*)&v;
    return a[i];
#endif
    }

  uint64_t get_avx2_u64(__m512i v, int i)
    {
#ifdef _WIN32
    return v.m512i_u64[i];
#else
    uint64_t* a = (uint64_t*)&v;
    return a[i];
#endif
    }

  float get_avx2_f32(__m512 v, int i)
    {
#ifdef _WIN32
    return v.m512_f32[i];
#else
    return v[i];
#endif
    }

  int32_t get_avx2_i32(__m512 v, int i)
    {
    float f = get_avx2_f32(v, i);
    return *reinterpret_cast<int32_t*>(&f);
    }

  uint64_t get_avx2_u64(__m512 v, int i)
    {
#ifdef _WIN32    
    return *reinterpret_cast<uint64_t*>(&v.m512_f32[i]);
#else
    return *reinterpret_cast<uint64_t*>(&v[i]);
#endif
    }

#else

  int32_t get_avx2_i32(__m256i v, int i)
    {
#ifdef _WIN32
    return v.m256i_i32[i];
#else
    int32_t* a = (int32_t*)&v;
    return a[i];
#endif
    }

  uint64_t get_avx2_u64(__m256i v, int i)
    {
#ifdef _WIN32
    return v.m256i_u64[i];
#else
    uint64_t* a = (uint64_t*)&v;
    return a[i];
#endif
    }

  float get_avx2_f32(__m256 v, int i)
    {
#ifdef _WIN32
    return v.m256_f32[i];
#else
    return v[i];
#endif
    }

  int32_t get_avx2_i32(__m256 v, int i)
    {    
    float f = get_avx2_f32(v, i);
    return *reinterpret_cast<int32_t*>(&f);
    }

  uint64_t get_avx2_u64(__m256 v, int i)
    {
#ifdef _WIN32    
    return *reinterpret_cast<uint64_t*>(&v.m256_f32[i]);
#else
    return *reinterpret_cast<uint64_t*>(&v[i]);
#endif
    }

#endif

  struct compile_fixture
    {
    context ctxt;
    typedef void(*fun_ptr)(void*);
    std::vector<std::pair<fun_ptr, uint64_t>> compiled_functions;
    std::string last_error;
    dictionary dict;
    compile_data cd;


    compile_fixture()
      {
      ctxt = create_context(1024 * 1024, 1024, 1024 * 1024);
      add_stdlib_to_dictionary(dict);
      cd = create_compile_data();
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
      compile(code, dict, cd, words);
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
        std::cout << last_error << std::endl;
        return nullptr;
        }
      first_pass_data d;
      uint64_t size;
      fun_ptr f = (fun_ptr)assemble(size, d, code);
      f(&ctxt);
      compiled_functions.emplace_back(f, size);
      return ctxt.stack_pointer;
      }

#ifdef AVX512
    __m512 get_last_stack_value() const
      {
      if (ctxt.stack_pointer >= ctxt.aligned_stack_top)
        throw std::runtime_error("error: stack is empty");
      return _mm512_load_ps((const float*)ctxt.stack_pointer);
      }

    __m512 get_stack_value(size_t index) const
      {
      const float* ptr = (const float*)ctxt.stack_pointer + 16 * index;
      if ((char*)ptr >= ctxt.aligned_stack_top)
        {
        std::cout << "error: stack is empty\n";
        return _mm512_set1_ps(std::numeric_limits<float>::signaling_NaN());
        }
      __m512 sv = _mm512_load_ps(ptr);
      return sv;
      }

    __m512 get_data_space_value(size_t index) const
      {
      char* ptr = ctxt.here_pointer + 64 + index * 64;
      __m512 value = _mm512_load_ps((float*)ptr);
      return value;
      }

    __m512i get_last_stack_value_i() const
      {
      if (ctxt.stack_pointer >= ctxt.aligned_stack_top)
        throw std::runtime_error("error: stack is empty");
      __m512 v = _mm512_load_ps((const float*)ctxt.stack_pointer);
      __m512i out = _mm512_castps_si512(v);
      return out;
      }

    float get_stack_valuef(size_t index) const
      {
      const float* ptr = (const float*)ctxt.stack_pointer + 16 * index;
      if ((char*)ptr >= ctxt.aligned_stack_top)
        {
        std::cout << "error: stack is empty\n";
        return std::numeric_limits<float>::signaling_NaN();
        }
      __m512 sv = _mm512_load_ps(ptr);
      return get_avx2_f32(sv, 0);
      }

    uint32_t get_stack_valuei(size_t index) const
      {
      const float* ptr = (const float*)ctxt.stack_pointer + 16 * index;
      if ((char*)ptr >= ctxt.aligned_stack_top)
        {
        std::cout << "error: stack is empty\n";
        return 0xffffffff;
        }
      __m512 sv = _mm512_load_ps(ptr);
      //return *reinterpret_cast<uint32_t*>(&sget_avx2_f32(v, 0));
      return (uint32_t)get_avx2_i32(sv, 0);
      }

    uint64_t get_stack_valuef_uint64(size_t index) const
      {
      const float* ptr = (const float*)ctxt.stack_pointer + 16 * index;
      if ((char*)ptr >= ctxt.aligned_stack_top)
        {
        std::cout << "error: stack is empty\n";
        return 0xffffffffffffffff;
        }
      __m512 sv = _mm512_load_ps(ptr);
      return get_avx2_u64(sv, 0);
      }
#else
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

    __m256 get_data_space_value(size_t index) const
      {
      char* ptr = ctxt.here_pointer + 32 + index*32;
      __m256 value = _mm256_load_ps((float*)ptr);
      return value;
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
      return get_avx2_f32(sv, 0);
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
      //return *reinterpret_cast<uint32_t*>(&sget_avx2_f32(v, 0));
      return (uint32_t)get_avx2_i32(sv, 0);
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
      return get_avx2_u64(sv, 0);
      }

#endif
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
#ifdef AVX512
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 64, run("3.14"));
    
    auto v = get_last_stack_value();
    TEST_EQ(3.14f, get_avx2_f32(v, 0));
    TEST_EQ(3.14f, get_avx2_f32(v, 1));
    TEST_EQ(3.14f, get_avx2_f32(v, 2));
    TEST_EQ(3.14f, get_avx2_f32(v, 3));
    TEST_EQ(3.14f, get_avx2_f32(v, 4));
    TEST_EQ(3.14f, get_avx2_f32(v, 5));
    TEST_EQ(3.14f, get_avx2_f32(v, 6));
    TEST_EQ(3.14f, get_avx2_f32(v, 7));
    TEST_EQ(3.14f, get_avx2_f32(v, 8));
    TEST_EQ(3.14f, get_avx2_f32(v, 9));
    TEST_EQ(3.14f, get_avx2_f32(v, 10));
    TEST_EQ(3.14f, get_avx2_f32(v, 11));
    TEST_EQ(3.14f, get_avx2_f32(v, 12));
    TEST_EQ(3.14f, get_avx2_f32(v, 13));
    TEST_EQ(3.14f, get_avx2_f32(v, 14));
    TEST_EQ(3.14f, get_avx2_f32(v, 15));

    TEST_EQ(ctxt.aligned_stack_top - 128, run("54"));
    v = get_last_stack_value();
    TEST_EQ(54.f, get_avx2_f32(v, 0));
    TEST_EQ(54.f, get_avx2_f32(v, 1));
    TEST_EQ(54.f, get_avx2_f32(v, 2));
    TEST_EQ(54.f, get_avx2_f32(v, 3));
    TEST_EQ(54.f, get_avx2_f32(v, 4));
    TEST_EQ(54.f, get_avx2_f32(v, 5));
    TEST_EQ(54.f, get_avx2_f32(v, 6));
    TEST_EQ(54.f, get_avx2_f32(v, 7));
    TEST_EQ(54.f, get_avx2_f32(v, 8));
    TEST_EQ(54.f, get_avx2_f32(v, 9));
    TEST_EQ(54.f, get_avx2_f32(v, 10));
    TEST_EQ(54.f, get_avx2_f32(v, 11));
    TEST_EQ(54.f, get_avx2_f32(v, 12));
    TEST_EQ(54.f, get_avx2_f32(v, 13));
    TEST_EQ(54.f, get_avx2_f32(v, 14));
    TEST_EQ(54.f, get_avx2_f32(v, 15));

    TEST_EQ(ctxt.aligned_stack_top - 256, run("100 20.45"));
    v = get_last_stack_value();
    TEST_EQ(20.45f, get_avx2_f32(v, 0));
    TEST_EQ(20.45f, get_avx2_f32(v, 1));
    TEST_EQ(20.45f, get_avx2_f32(v, 2));
    TEST_EQ(20.45f, get_avx2_f32(v, 3));
    TEST_EQ(20.45f, get_avx2_f32(v, 4));
    TEST_EQ(20.45f, get_avx2_f32(v, 5));
    TEST_EQ(20.45f, get_avx2_f32(v, 6));
    TEST_EQ(20.45f, get_avx2_f32(v, 7));
    TEST_EQ(20.45f, get_avx2_f32(v, 8));
    TEST_EQ(20.45f, get_avx2_f32(v, 9));
    TEST_EQ(20.45f, get_avx2_f32(v, 10));
    TEST_EQ(20.45f, get_avx2_f32(v, 11));
    TEST_EQ(20.45f, get_avx2_f32(v, 12));
    TEST_EQ(20.45f, get_avx2_f32(v, 13));
    TEST_EQ(20.45f, get_avx2_f32(v, 14));
    TEST_EQ(20.45f, get_avx2_f32(v, 15));
    }
#else
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 32, run("3.14"));
    auto v = get_last_stack_value();
    TEST_EQ(3.14f, get_avx2_f32(v, 0));
    TEST_EQ(3.14f, get_avx2_f32(v, 1));
    TEST_EQ(3.14f, get_avx2_f32(v, 2));
    TEST_EQ(3.14f, get_avx2_f32(v, 3));
    TEST_EQ(3.14f, get_avx2_f32(v, 4));
    TEST_EQ(3.14f, get_avx2_f32(v, 5));
    TEST_EQ(3.14f, get_avx2_f32(v, 6));
    TEST_EQ(3.14f, get_avx2_f32(v, 7));

    TEST_EQ(ctxt.aligned_stack_top - 64, run("54"));
    v = get_last_stack_value();
    TEST_EQ(54.f, get_avx2_f32(v, 0));
    TEST_EQ(54.f, get_avx2_f32(v, 1));
    TEST_EQ(54.f, get_avx2_f32(v, 2));
    TEST_EQ(54.f, get_avx2_f32(v, 3));
    TEST_EQ(54.f, get_avx2_f32(v, 4));
    TEST_EQ(54.f, get_avx2_f32(v, 5));
    TEST_EQ(54.f, get_avx2_f32(v, 6));
    TEST_EQ(54.f, get_avx2_f32(v, 7));

    TEST_EQ(ctxt.aligned_stack_top - 128, run("100 20.45"));
    v = get_last_stack_value();
    TEST_EQ(20.45f, get_avx2_f32(v, 0));
    TEST_EQ(20.45f, get_avx2_f32(v, 1));
    TEST_EQ(20.45f, get_avx2_f32(v, 2));
    TEST_EQ(20.45f, get_avx2_f32(v, 3));
    TEST_EQ(20.45f, get_avx2_f32(v, 4));
    TEST_EQ(20.45f, get_avx2_f32(v, 5));
    TEST_EQ(20.45f, get_avx2_f32(v, 6));
    TEST_EQ(20.45f, get_avx2_f32(v, 7));
    }
#endif
  };


struct definitions : public compile_fixture
  {
#ifdef AVX512
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top, run(": pi 3.14;"));
    dictionary_entry e;
    TEST_ASSERT(find(e, dict, "pi"));
    TEST_ASSERT(e.words.size() == 1);
    TEST_ASSERT(e.words.front().type == token::T_FLOAT);
    TEST_ASSERT(e.words.front().value == "3.14");
    TEST_EQ(ctxt.aligned_stack_top - 64, run("pi"));
    auto v = get_last_stack_value();
    TEST_EQ(3.14f, get_avx2_f32(v, 0));
    TEST_EQ(3.14f, get_avx2_f32(v, 1));
    TEST_EQ(3.14f, get_avx2_f32(v, 2));
    TEST_EQ(3.14f, get_avx2_f32(v, 3));
    TEST_EQ(3.14f, get_avx2_f32(v, 4));
    TEST_EQ(3.14f, get_avx2_f32(v, 5));
    TEST_EQ(3.14f, get_avx2_f32(v, 6));
    TEST_EQ(3.14f, get_avx2_f32(v, 7));
    TEST_EQ(3.14f, get_avx2_f32(v, 8));
    TEST_EQ(3.14f, get_avx2_f32(v, 9));
    TEST_EQ(3.14f, get_avx2_f32(v, 10));
    TEST_EQ(3.14f, get_avx2_f32(v, 11));
    TEST_EQ(3.14f, get_avx2_f32(v, 12));
    TEST_EQ(3.14f, get_avx2_f32(v, 13));
    TEST_EQ(3.14f, get_avx2_f32(v, 14));
    TEST_EQ(3.14f, get_avx2_f32(v, 15));
    }
#else
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
    TEST_EQ(3.14f, get_avx2_f32(v, 0));
    TEST_EQ(3.14f, get_avx2_f32(v, 1));
    TEST_EQ(3.14f, get_avx2_f32(v, 2));
    TEST_EQ(3.14f, get_avx2_f32(v, 3));
    TEST_EQ(3.14f, get_avx2_f32(v, 4));
    TEST_EQ(3.14f, get_avx2_f32(v, 5));
    TEST_EQ(3.14f, get_avx2_f32(v, 6));
    TEST_EQ(3.14f, get_avx2_f32(v, 7));
    }
#endif
  };

struct add : public compile_fixture
  {
#ifdef AVX512
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 64, run("3.1 2.4 +"));
    auto v = get_last_stack_value();
    TEST_EQ(5.5f, get_avx2_f32(v, 0));
    TEST_EQ(5.5f, get_avx2_f32(v, 1));
    TEST_EQ(5.5f, get_avx2_f32(v, 2));
    TEST_EQ(5.5f, get_avx2_f32(v, 3));
    TEST_EQ(5.5f, get_avx2_f32(v, 4));
    TEST_EQ(5.5f, get_avx2_f32(v, 5));
    TEST_EQ(5.5f, get_avx2_f32(v, 6));
    TEST_EQ(5.5f, get_avx2_f32(v, 7));
    TEST_EQ(5.5f, get_avx2_f32(v, 8));
    TEST_EQ(5.5f, get_avx2_f32(v, 9));
    TEST_EQ(5.5f, get_avx2_f32(v, 10));
    TEST_EQ(5.5f, get_avx2_f32(v, 11));
    TEST_EQ(5.5f, get_avx2_f32(v, 12));
    TEST_EQ(5.5f, get_avx2_f32(v, 13));
    TEST_EQ(5.5f, get_avx2_f32(v, 14));
    TEST_EQ(5.5f, get_avx2_f32(v, 15));
    }
#else
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 32, run("3.1 2.4 +"));
    auto v = get_last_stack_value();
    TEST_EQ(5.5f, get_avx2_f32(v, 0));
    TEST_EQ(5.5f, get_avx2_f32(v, 1));
    TEST_EQ(5.5f, get_avx2_f32(v, 2));
    TEST_EQ(5.5f, get_avx2_f32(v, 3));
    TEST_EQ(5.5f, get_avx2_f32(v, 4));
    TEST_EQ(5.5f, get_avx2_f32(v, 5));
    TEST_EQ(5.5f, get_avx2_f32(v, 6));
    TEST_EQ(5.5f, get_avx2_f32(v, 7));
    }
#endif
  };

struct sub : public compile_fixture
  {
#ifdef AVX512
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 64, run("3.1 2.4 -"));
    auto v = get_last_stack_value();
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 0), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 1), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 2), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 3), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 4), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 5), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 6), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 7), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 8), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 9), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 10), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 11), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 12), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 13), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 14), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 15), 1e-6f);
    }
#else
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 32, run("3.1 2.4 -"));
    auto v = get_last_stack_value();
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 0), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 1), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 2), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 3), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 4), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 5), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 6), 1e-6f);
    TEST_EQ_CLOSE(0.7f, get_avx2_f32(v, 7), 1e-6f);
    }
#endif
  };

struct mul : public compile_fixture
  {
#ifdef AVX512
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 64, run("3.1 2.4 *"));
    auto v = get_last_stack_value();
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 0), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 1), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 2), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 3), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 4), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 5), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 6), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 7), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 8), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 9), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 10), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 11), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 12), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 13), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 14), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 15), 1e-6f);
    }
#else
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 32, run("3.1 2.4 *"));
    auto v = get_last_stack_value();
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 0), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 1), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 2), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 3), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 4), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 5), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 6), 1e-6f);
    TEST_EQ_CLOSE(3.1f*2.4f, get_avx2_f32(v, 7), 1e-6f);
    }
#endif
  };

struct division : public compile_fixture
  {
#ifdef AVX512
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 64, run("3.1 2.4 /"));
    auto v = get_last_stack_value();
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 0), 1e-6f);
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 1), 1e-6f);
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 2), 1e-6f);
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 3), 1e-6f);
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 4), 1e-6f);
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 5), 1e-6f);
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 6), 1e-6f);
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 7), 1e-6f);
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 8), 1e-6f);
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 9), 1e-6f);
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 10), 1e-6f);
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 11), 1e-6f);
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 12), 1e-6f);
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 13), 1e-6f);
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 14), 1e-6f);
    TEST_EQ_CLOSE(3.1f / 2.4f, get_avx2_f32(v, 15), 1e-6f);
    }
#else
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 32, run("3.1 2.4 /"));
    auto v = get_last_stack_value();
    TEST_EQ_CLOSE(3.1f/2.4f, get_avx2_f32(v, 0), 1e-6f);
    TEST_EQ_CLOSE(3.1f/2.4f, get_avx2_f32(v, 1), 1e-6f);
    TEST_EQ_CLOSE(3.1f/2.4f, get_avx2_f32(v, 2), 1e-6f);
    TEST_EQ_CLOSE(3.1f/2.4f, get_avx2_f32(v, 3), 1e-6f);
    TEST_EQ_CLOSE(3.1f/2.4f, get_avx2_f32(v, 4), 1e-6f);
    TEST_EQ_CLOSE(3.1f/2.4f, get_avx2_f32(v, 5), 1e-6f);
    TEST_EQ_CLOSE(3.1f/2.4f, get_avx2_f32(v, 6), 1e-6f);
    TEST_EQ_CLOSE(3.1f/2.4f, get_avx2_f32(v, 7), 1e-6f);
    }
#endif
  };

struct v8 : public compile_fixture
  {
#ifdef AVX512
  void test()
    {
    // zmm0 = x15 / x14 / x13 / x12 / x11 / x10 / x9 / x8 / x7 / x6 / x5 / x4 / x3 / x2 / x1 / x0
    TEST_EQ(ctxt.aligned_stack_top - 64, run("v16 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16"));
    auto v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 15));
    TEST_EQ(2.f, get_avx2_f32(v, 14));
    TEST_EQ(3.f, get_avx2_f32(v, 13));
    TEST_EQ(4.f, get_avx2_f32(v, 12));
    TEST_EQ(5.f, get_avx2_f32(v, 11));
    TEST_EQ(6.f, get_avx2_f32(v, 10));
    TEST_EQ(7.f, get_avx2_f32(v, 9));
    TEST_EQ(8.f, get_avx2_f32(v, 8));
    TEST_EQ(9.f, get_avx2_f32(v, 7));
    TEST_EQ(10.f, get_avx2_f32(v, 6));
    TEST_EQ(11.f, get_avx2_f32(v, 5));
    TEST_EQ(12.f, get_avx2_f32(v, 4));
    TEST_EQ(13.f, get_avx2_f32(v, 3));
    TEST_EQ(14.f, get_avx2_f32(v, 2));
    TEST_EQ(15.f, get_avx2_f32(v, 1));
    TEST_EQ(16.f, get_avx2_f32(v, 0));
    }
#else
  void test()
    {
    // ymm0 = x7 / x6 / x5 / x4 / x3 / x2 / x1 / x0
    TEST_EQ(ctxt.aligned_stack_top - 32, run("v8 1 2 3 4 5 6 7 8"));
    auto v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(2.f, get_avx2_f32(v, 6));
    TEST_EQ(3.f, get_avx2_f32(v, 5));
    TEST_EQ(4.f, get_avx2_f32(v, 4));
    TEST_EQ(5.f, get_avx2_f32(v, 3));
    TEST_EQ(6.f, get_avx2_f32(v, 2));
    TEST_EQ(7.f, get_avx2_f32(v, 1));
    TEST_EQ(8.f, get_avx2_f32(v, 0));
    }
#endif
  };

struct avx_mathfun : public compile_fixture
  {
#ifdef AVX512
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 64, run("3.14159265359 sin"));
    auto v = get_last_stack_value();
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 15), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 14), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 13), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 12), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 11), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 10), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 9), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 8), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 7), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 6), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 5), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 4), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 3), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 2), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 1), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 0), 1e-6f);

    run("v16 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 sin");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 15), 1e-6f);
    TEST_EQ_CLOSE(0.841471f, get_avx2_f32(v, 14), 1e-6f);
    TEST_EQ_CLOSE(0.909297f, get_avx2_f32(v, 13), 1e-6f);
    TEST_EQ_CLOSE(0.14112f, get_avx2_f32(v, 12), 1e-6f);
    TEST_EQ_CLOSE(-0.756802f, get_avx2_f32(v, 11), 1e-6f);
    TEST_EQ_CLOSE(-0.958924f, get_avx2_f32(v, 10), 1e-6f);
    TEST_EQ_CLOSE(-0.279415f, get_avx2_f32(v, 9), 1e-6f);
    TEST_EQ_CLOSE(0.656987f, get_avx2_f32(v, 8), 1e-6f);
    TEST_EQ_CLOSE(0.989358f, get_avx2_f32(v, 7), 1e-6f);
    TEST_EQ_CLOSE(0.412118f, get_avx2_f32(v, 6), 1e-6f);
    TEST_EQ_CLOSE(-0.544021f, get_avx2_f32(v, 5), 1e-6f);
    TEST_EQ_CLOSE(-0.99999f, get_avx2_f32(v, 4), 1e-6f);
    TEST_EQ_CLOSE(-0.536573f, get_avx2_f32(v, 3), 1e-6f);
    TEST_EQ_CLOSE(0.420167f, get_avx2_f32(v, 2), 1e-6f);
    TEST_EQ_CLOSE(0.990607f, get_avx2_f32(v, 1), 1e-6f);
    TEST_EQ_CLOSE(0.650288f, get_avx2_f32(v, 0), 1e-6f);
    
    run("v16 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 cos");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(1.f, get_avx2_f32(v, 15), 1e-6f);
    TEST_EQ_CLOSE(0.540302f, get_avx2_f32(v, 14), 1e-6f);
    TEST_EQ_CLOSE(-0.416147f, get_avx2_f32(v, 13), 1e-6f);
    TEST_EQ_CLOSE(-0.989992f, get_avx2_f32(v, 12), 1e-6f);
    TEST_EQ_CLOSE(-0.653644f, get_avx2_f32(v, 11), 1e-6f);
    TEST_EQ_CLOSE(0.283662f, get_avx2_f32(v, 10), 1e-6f);
    TEST_EQ_CLOSE(0.96017f, get_avx2_f32(v, 9), 1e-6f);
    TEST_EQ_CLOSE(0.753902f, get_avx2_f32(v, 8), 1e-6f);

    run("v16 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 exp");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(1.f, get_avx2_f32(v, 15), 1e-6f);
    TEST_EQ_CLOSE(2.71828f, get_avx2_f32(v, 14), 1e-5f);
    TEST_EQ_CLOSE(7.38906f, get_avx2_f32(v, 13), 1e-5f);
    TEST_EQ_CLOSE(20.0855f, get_avx2_f32(v, 12), 1e-4f);
    TEST_EQ_CLOSE(54.5982f, get_avx2_f32(v, 11), 1e-4f);
    TEST_EQ_CLOSE(148.413f, get_avx2_f32(v, 10), 1e-3f);
    TEST_EQ_CLOSE(403.429f, get_avx2_f32(v, 9), 1e-3f);
    TEST_EQ_CLOSE(1096.63f, get_avx2_f32(v, 8), 1e-2f);

    run("v16 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 log");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(-std::numeric_limits<float>::infinity(), get_avx2_f32(v, 15), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 14), 1e-6f);
    TEST_EQ_CLOSE(0.693147f, get_avx2_f32(v, 13), 1e-6f);
    TEST_EQ_CLOSE(1.09861f, get_avx2_f32(v, 12), 1e-5f);
    TEST_EQ_CLOSE(1.38629f, get_avx2_f32(v, 11), 1e-5f);
    TEST_EQ_CLOSE(1.60944f, get_avx2_f32(v, 10), 1e-5f);
    TEST_EQ_CLOSE(1.79176f, get_avx2_f32(v, 9), 1e-5f);
    TEST_EQ_CLOSE(1.94591f, get_avx2_f32(v, 8), 1e-5f);

    run("v16 1 4 3 2 3 2 2 2 1 1 1 1 1 1 1 1 v16 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 **");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(1.f, get_avx2_f32(v, 15), 1e-6f);
    TEST_EQ_CLOSE(4.f, get_avx2_f32(v, 14), 1e-6f);
    TEST_EQ_CLOSE(9.f, get_avx2_f32(v, 13), 1e-6f);
    TEST_EQ_CLOSE(8.f, get_avx2_f32(v, 12), 1e-6f);
    TEST_EQ_CLOSE(81.f, get_avx2_f32(v, 11), 1e-5f);
    TEST_EQ_CLOSE(32.f, get_avx2_f32(v, 10), 1e-5f);
    TEST_EQ_CLOSE(64.f, get_avx2_f32(v, 9), 1e-5f);
    TEST_EQ_CLOSE(128.f, get_avx2_f32(v, 8), 1e-5f);

    run("v16 1 4 9 16 25 36 49 64 0 0 0 0 0 0 0 0 sqrt");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(1.f, get_avx2_f32(v, 15), 1e-6f);
    TEST_EQ_CLOSE(2.f, get_avx2_f32(v, 14), 1e-6f);
    TEST_EQ_CLOSE(3.f, get_avx2_f32(v, 13), 1e-6f);
    TEST_EQ_CLOSE(4.f, get_avx2_f32(v, 12), 1e-6f);
    TEST_EQ_CLOSE(5.f, get_avx2_f32(v, 11), 1e-5f);
    TEST_EQ_CLOSE(6.f, get_avx2_f32(v, 10), 1e-5f);
    TEST_EQ_CLOSE(7.f, get_avx2_f32(v, 9), 1e-5f);
    TEST_EQ_CLOSE(8.f, get_avx2_f32(v, 8), 1e-5f);
    }
#else
  void test()
    {
    TEST_EQ(ctxt.aligned_stack_top - 32, run("3.14159265359 sin"));
    auto v = get_last_stack_value();
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 7), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 6), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 5), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 4), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 3), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 2), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 1), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 0), 1e-6f);

    run("v8 0 1 2 3 4 5 6 7 sin");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 7), 1e-6f);
    TEST_EQ_CLOSE(0.841471f, get_avx2_f32(v, 6), 1e-6f);
    TEST_EQ_CLOSE(0.909297f, get_avx2_f32(v, 5), 1e-6f);
    TEST_EQ_CLOSE(0.14112f, get_avx2_f32(v, 4), 1e-6f);
    TEST_EQ_CLOSE(-0.756802f, get_avx2_f32(v, 3), 1e-6f);
    TEST_EQ_CLOSE(-0.958924f, get_avx2_f32(v, 2), 1e-6f);
    TEST_EQ_CLOSE(-0.279415f, get_avx2_f32(v, 1), 1e-6f);
    TEST_EQ_CLOSE(0.656987f, get_avx2_f32(v, 0), 1e-6f);

    run("v8 0 1 2 3 4 5 6 7 cos");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(1.f, get_avx2_f32(v, 7), 1e-6f);
    TEST_EQ_CLOSE(0.540302f, get_avx2_f32(v, 6), 1e-6f);
    TEST_EQ_CLOSE(-0.416147f, get_avx2_f32(v, 5), 1e-6f);
    TEST_EQ_CLOSE(-0.989992f, get_avx2_f32(v, 4), 1e-6f);
    TEST_EQ_CLOSE(-0.653644f, get_avx2_f32(v, 3), 1e-6f);
    TEST_EQ_CLOSE(0.283662f, get_avx2_f32(v, 2), 1e-6f);
    TEST_EQ_CLOSE(0.96017f, get_avx2_f32(v, 1), 1e-6f);
    TEST_EQ_CLOSE(0.753902f, get_avx2_f32(v, 0), 1e-6f);

    run("v8 0 1 2 3 4 5 6 7 exp");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(1.f, get_avx2_f32(v, 7), 1e-6f);
    TEST_EQ_CLOSE(2.71828f, get_avx2_f32(v, 6), 1e-5f);
    TEST_EQ_CLOSE(7.38906f, get_avx2_f32(v, 5), 1e-5f);
    TEST_EQ_CLOSE(20.0855f, get_avx2_f32(v, 4), 1e-4f);
    TEST_EQ_CLOSE(54.5982f, get_avx2_f32(v, 3), 1e-4f);
    TEST_EQ_CLOSE(148.413f, get_avx2_f32(v, 2), 1e-3f);
    TEST_EQ_CLOSE(403.429f, get_avx2_f32(v, 1), 1e-3f);
    TEST_EQ_CLOSE(1096.63f, get_avx2_f32(v, 0), 1e-2f);

    run("v8 0 1 2 3 4 5 6 7 log");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(-std::numeric_limits<float>::infinity(), get_avx2_f32(v, 7), 1e-6f);
    TEST_EQ_CLOSE(0.f, get_avx2_f32(v, 6), 1e-6f);
    TEST_EQ_CLOSE(0.693147f, get_avx2_f32(v, 5), 1e-6f);
    TEST_EQ_CLOSE(1.09861f, get_avx2_f32(v, 4), 1e-5f);
    TEST_EQ_CLOSE(1.38629f, get_avx2_f32(v, 3), 1e-5f);
    TEST_EQ_CLOSE(1.60944f, get_avx2_f32(v, 2), 1e-5f);
    TEST_EQ_CLOSE(1.79176f, get_avx2_f32(v, 1), 1e-5f);
    TEST_EQ_CLOSE(1.94591f, get_avx2_f32(v, 0), 1e-5f);

    run("v8 1 4 3 2 3 2 2 2 v8 0 1 2 3 4 5 6 7 **");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(1.f, get_avx2_f32(v, 7), 1e-6f);
    TEST_EQ_CLOSE(4.f, get_avx2_f32(v, 6), 1e-6f);
    TEST_EQ_CLOSE(9.f, get_avx2_f32(v, 5), 1e-6f);
    TEST_EQ_CLOSE(8.f, get_avx2_f32(v, 4), 1e-6f);
    TEST_EQ_CLOSE(81.f, get_avx2_f32(v, 3), 1e-5f);
    TEST_EQ_CLOSE(32.f, get_avx2_f32(v, 2), 1e-5f);
    TEST_EQ_CLOSE(64.f, get_avx2_f32(v, 1), 1e-5f);
    TEST_EQ_CLOSE(128.f, get_avx2_f32(v, 0), 1e-5f);

    run("v8 1 4 9 16 25 36 49 64 sqrt");
    v = get_last_stack_value();
    TEST_EQ_CLOSE(1.f, get_avx2_f32(v, 7), 1e-6f);
    TEST_EQ_CLOSE(2.f, get_avx2_f32(v, 6), 1e-6f);
    TEST_EQ_CLOSE(3.f, get_avx2_f32(v, 5), 1e-6f);
    TEST_EQ_CLOSE(4.f, get_avx2_f32(v, 4), 1e-6f);
    TEST_EQ_CLOSE(5.f, get_avx2_f32(v, 3), 1e-5f);
    TEST_EQ_CLOSE(6.f, get_avx2_f32(v, 2), 1e-5f);
    TEST_EQ_CLOSE(7.f, get_avx2_f32(v, 1), 1e-5f);
    TEST_EQ_CLOSE(8.f, get_avx2_f32(v, 0), 1e-5f);    
    }
#endif
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

#ifdef AVX512
    run("v16 #1 #2 #3 #4 #5 #6 #7 #8 #1 #2 #3 #4 #5 #6 #7 #8");
#else
    run("v8 #1 #2 #3 #4 #5 #6 #7 #8");
#endif
    auto v = get_last_stack_value_i();
    TEST_EQ(1, get_avx2_i32(v, 7));
    TEST_EQ(2, get_avx2_i32(v, 6));
    TEST_EQ(3, get_avx2_i32(v, 5));
    TEST_EQ(4, get_avx2_i32(v, 4));
    TEST_EQ(5, get_avx2_i32(v, 3));
    TEST_EQ(6, get_avx2_i32(v, 2));
    TEST_EQ(7, get_avx2_i32(v, 1));
    TEST_EQ(8, get_avx2_i32(v, 0));

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
#ifdef AVX512
  void test()
    {
    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 v16 1 2 0 0 8 8 8 8 1 2 0 0 8 8 8 8 =");
    auto v = get_last_stack_value_i();
    TEST_EQ(-1, get_avx2_i32(v, 7));
    TEST_EQ(-1, get_avx2_i32(v, 6));
    TEST_EQ(0, get_avx2_i32(v, 5));
    TEST_EQ(0, get_avx2_i32(v, 4));
    TEST_EQ(0, get_avx2_i32(v, 3));
    TEST_EQ(0, get_avx2_i32(v, 2));
    TEST_EQ(0, get_avx2_i32(v, 1));
    TEST_EQ(-1, get_avx2_i32(v, 0));
    
    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 v16 1 2 0 0 8 8 8 8 1 2 0 0 8 8 8 8 <>");
    v = get_last_stack_value_i();
    TEST_EQ(0, get_avx2_i32(v, 7));
    TEST_EQ(0, get_avx2_i32(v, 6));
    TEST_EQ(-1, get_avx2_i32(v, 5));
    TEST_EQ(-1, get_avx2_i32(v, 4));
    TEST_EQ(-1, get_avx2_i32(v, 3));
    TEST_EQ(-1, get_avx2_i32(v, 2));
    TEST_EQ(-1, get_avx2_i32(v, 1));
    TEST_EQ(0, get_avx2_i32(v, 0));

    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 v16 1 2 0 0 8 8 8 8 1 2 0 0 8 8 8 8 <");
    v = get_last_stack_value_i();
    TEST_EQ(0, get_avx2_i32(v, 7));
    TEST_EQ(0, get_avx2_i32(v, 6));
    TEST_EQ(0, get_avx2_i32(v, 5));
    TEST_EQ(0, get_avx2_i32(v, 4));
    TEST_EQ(-1, get_avx2_i32(v, 3));
    TEST_EQ(-1, get_avx2_i32(v, 2));
    TEST_EQ(-1, get_avx2_i32(v, 1));
    TEST_EQ(0, get_avx2_i32(v, 0));

    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 v16 1 2 0 0 8 8 8 8 1 2 0 0 8 8 8 8 <=");
    v = get_last_stack_value_i();
    TEST_EQ(-1, get_avx2_i32(v, 7));
    TEST_EQ(-1, get_avx2_i32(v, 6));
    TEST_EQ(0, get_avx2_i32(v, 5));
    TEST_EQ(0, get_avx2_i32(v, 4));
    TEST_EQ(-1, get_avx2_i32(v, 3));
    TEST_EQ(-1, get_avx2_i32(v, 2));
    TEST_EQ(-1, get_avx2_i32(v, 1));
    TEST_EQ(-1, get_avx2_i32(v, 0));

    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 v16 1 2 0 0 8 8 8 8 1 2 0 0 8 8 8 8 >");
    v = get_last_stack_value_i();
    TEST_EQ(0, get_avx2_i32(v, 7));
    TEST_EQ(0, get_avx2_i32(v, 6));
    TEST_EQ(-1, get_avx2_i32(v, 5));
    TEST_EQ(-1, get_avx2_i32(v, 4));
    TEST_EQ(0, get_avx2_i32(v, 3));
    TEST_EQ(0, get_avx2_i32(v, 2));
    TEST_EQ(0, get_avx2_i32(v, 1));
    TEST_EQ(0, get_avx2_i32(v, 0));

    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 v16 1 2 0 0 8 8 8 8 1 2 0 0 8 8 8 8 >=");
    v = get_last_stack_value_i();
    TEST_EQ(-1, get_avx2_i32(v, 7));
    TEST_EQ(-1, get_avx2_i32(v, 6));
    TEST_EQ(-1, get_avx2_i32(v, 5));
    TEST_EQ(-1, get_avx2_i32(v, 4));
    TEST_EQ(0, get_avx2_i32(v, 3));
    TEST_EQ(0, get_avx2_i32(v, 2));
    TEST_EQ(0, get_avx2_i32(v, 1));
    TEST_EQ(-1, get_avx2_i32(v, 0));

    
    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 v16 1 2 0 0 8 8 8 8 1 2 0 0 8 8 8 8 f=");
    auto f = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(f, 7));
    TEST_EQ(1.f, get_avx2_f32(f, 6));
    TEST_EQ(0.f, get_avx2_f32(f, 5));
    TEST_EQ(0.f, get_avx2_f32(f, 4));
    TEST_EQ(0.f, get_avx2_f32(f, 3));
    TEST_EQ(0.f, get_avx2_f32(f, 2));
    TEST_EQ(0.f, get_avx2_f32(f, 1));
    TEST_EQ(1.f, get_avx2_f32(f, 0));
    
    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 v16 1 2 0 0 8 8 8 8 1 2 0 0 8 8 8 8 f<>");
    f = get_last_stack_value();
    TEST_EQ(0.f, get_avx2_f32(f, 7));
    TEST_EQ(0.f, get_avx2_f32(f, 6));
    TEST_EQ(1.f, get_avx2_f32(f, 5));
    TEST_EQ(1.f, get_avx2_f32(f, 4));
    TEST_EQ(1.f, get_avx2_f32(f, 3));
    TEST_EQ(1.f, get_avx2_f32(f, 2));
    TEST_EQ(1.f, get_avx2_f32(f, 1));
    TEST_EQ(0.f, get_avx2_f32(f, 0));

    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 v16 1 2 0 0 8 8 8 8 1 2 0 0 8 8 8 8 f<");
    f = get_last_stack_value();
    TEST_EQ(0.f, get_avx2_f32(f, 7));
    TEST_EQ(0.f, get_avx2_f32(f, 6));
    TEST_EQ(0.f, get_avx2_f32(f, 5));
    TEST_EQ(0.f, get_avx2_f32(f, 4));
    TEST_EQ(1.f, get_avx2_f32(f, 3));
    TEST_EQ(1.f, get_avx2_f32(f, 2));
    TEST_EQ(1.f, get_avx2_f32(f, 1));
    TEST_EQ(0.f, get_avx2_f32(f, 0));

    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 v16 1 2 0 0 8 8 8 8 1 2 0 0 8 8 8 8 f<=");
    f = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(f, 7));
    TEST_EQ(1.f, get_avx2_f32(f, 6));
    TEST_EQ(0.f, get_avx2_f32(f, 5));
    TEST_EQ(0.f, get_avx2_f32(f, 4));
    TEST_EQ(1.f, get_avx2_f32(f, 3));
    TEST_EQ(1.f, get_avx2_f32(f, 2));
    TEST_EQ(1.f, get_avx2_f32(f, 1));
    TEST_EQ(1.f, get_avx2_f32(f, 0));

    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 v16 1 2 0 0 8 8 8 8 1 2 0 0 8 8 8 8 f>");
    f = get_last_stack_value();
    TEST_EQ(0.f, get_avx2_f32(f, 7));
    TEST_EQ(0.f, get_avx2_f32(f, 6));
    TEST_EQ(1.f, get_avx2_f32(f, 5));
    TEST_EQ(1.f, get_avx2_f32(f, 4));
    TEST_EQ(0.f, get_avx2_f32(f, 3));
    TEST_EQ(0.f, get_avx2_f32(f, 2));
    TEST_EQ(0.f, get_avx2_f32(f, 1));
    TEST_EQ(0.f, get_avx2_f32(f, 0));

    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 v16 1 2 0 0 8 8 8 8 1 2 0 0 8 8 8 8 f>=");
    f = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(f, 7));
    TEST_EQ(1.f, get_avx2_f32(f, 6));
    TEST_EQ(1.f, get_avx2_f32(f, 5));
    TEST_EQ(1.f, get_avx2_f32(f, 4));
    TEST_EQ(0.f, get_avx2_f32(f, 3));
    TEST_EQ(0.f, get_avx2_f32(f, 2));
    TEST_EQ(0.f, get_avx2_f32(f, 1));
    TEST_EQ(1.f, get_avx2_f32(f, 0));
    }
    
#else
  void test()
    {
    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 =");
    auto v = get_last_stack_value_i();
    TEST_EQ(-1, get_avx2_i32(v, 7));
    TEST_EQ(-1, get_avx2_i32(v, 6));
    TEST_EQ(0, get_avx2_i32(v, 5));
    TEST_EQ(0, get_avx2_i32(v, 4));
    TEST_EQ(0, get_avx2_i32(v, 3));
    TEST_EQ(0, get_avx2_i32(v, 2));
    TEST_EQ(0, get_avx2_i32(v, 1));
    TEST_EQ(-1, get_avx2_i32(v, 0));

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 <>");
    v = get_last_stack_value_i();
    TEST_EQ(0, get_avx2_i32(v, 7));
    TEST_EQ(0, get_avx2_i32(v, 6));
    TEST_EQ(-1, get_avx2_i32(v, 5));
    TEST_EQ(-1, get_avx2_i32(v, 4));
    TEST_EQ(-1, get_avx2_i32(v, 3));
    TEST_EQ(-1, get_avx2_i32(v, 2));
    TEST_EQ(-1, get_avx2_i32(v, 1));
    TEST_EQ(0, get_avx2_i32(v, 0));

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 <");
    v = get_last_stack_value_i();
    TEST_EQ(0, get_avx2_i32(v, 7));
    TEST_EQ(0, get_avx2_i32(v, 6));
    TEST_EQ(0, get_avx2_i32(v, 5));
    TEST_EQ(0, get_avx2_i32(v, 4));
    TEST_EQ(-1, get_avx2_i32(v, 3));
    TEST_EQ(-1, get_avx2_i32(v, 2));
    TEST_EQ(-1, get_avx2_i32(v, 1));
    TEST_EQ(0, get_avx2_i32(v, 0));

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 <=");
    v = get_last_stack_value_i();
    TEST_EQ(-1, get_avx2_i32(v, 7));
    TEST_EQ(-1, get_avx2_i32(v, 6));
    TEST_EQ(0, get_avx2_i32(v, 5));
    TEST_EQ(0, get_avx2_i32(v, 4));
    TEST_EQ(-1, get_avx2_i32(v, 3));
    TEST_EQ(-1, get_avx2_i32(v, 2));
    TEST_EQ(-1, get_avx2_i32(v, 1));
    TEST_EQ(-1, get_avx2_i32(v, 0));

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 >");
    v = get_last_stack_value_i();
    TEST_EQ(0, get_avx2_i32(v, 7));
    TEST_EQ(0, get_avx2_i32(v, 6));
    TEST_EQ(-1, get_avx2_i32(v, 5));
    TEST_EQ(-1, get_avx2_i32(v, 4));
    TEST_EQ(0, get_avx2_i32(v, 3));
    TEST_EQ(0, get_avx2_i32(v, 2));
    TEST_EQ(0, get_avx2_i32(v, 1));
    TEST_EQ(0, get_avx2_i32(v, 0));

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 >=");
    v = get_last_stack_value_i();
    TEST_EQ(-1, get_avx2_i32(v, 7));
    TEST_EQ(-1, get_avx2_i32(v, 6));
    TEST_EQ(-1, get_avx2_i32(v, 5));
    TEST_EQ(-1, get_avx2_i32(v, 4));
    TEST_EQ(0, get_avx2_i32(v, 3));
    TEST_EQ(0, get_avx2_i32(v, 2));
    TEST_EQ(0, get_avx2_i32(v, 1));
    TEST_EQ(-1, get_avx2_i32(v, 0));


    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 f=");
    auto f = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(f, 7));
    TEST_EQ(1.f, get_avx2_f32(f, 6));
    TEST_EQ(0.f, get_avx2_f32(f, 5));
    TEST_EQ(0.f, get_avx2_f32(f, 4));
    TEST_EQ(0.f, get_avx2_f32(f, 3));
    TEST_EQ(0.f, get_avx2_f32(f, 2));
    TEST_EQ(0.f, get_avx2_f32(f, 1));
    TEST_EQ(1.f, get_avx2_f32(f, 0));

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 f<>");
    f = get_last_stack_value();
    TEST_EQ(0.f, get_avx2_f32(f, 7));
    TEST_EQ(0.f, get_avx2_f32(f, 6));
    TEST_EQ(1.f, get_avx2_f32(f, 5));
    TEST_EQ(1.f, get_avx2_f32(f, 4));
    TEST_EQ(1.f, get_avx2_f32(f, 3));
    TEST_EQ(1.f, get_avx2_f32(f, 2));
    TEST_EQ(1.f, get_avx2_f32(f, 1));
    TEST_EQ(0.f, get_avx2_f32(f, 0));

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 f<");
    f = get_last_stack_value();
    TEST_EQ(0.f, get_avx2_f32(f, 7));
    TEST_EQ(0.f, get_avx2_f32(f, 6));
    TEST_EQ(0.f, get_avx2_f32(f, 5));
    TEST_EQ(0.f, get_avx2_f32(f, 4));
    TEST_EQ(1.f, get_avx2_f32(f, 3));
    TEST_EQ(1.f, get_avx2_f32(f, 2));
    TEST_EQ(1.f, get_avx2_f32(f, 1));
    TEST_EQ(0.f, get_avx2_f32(f, 0));

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 f<=");
    f = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(f, 7));
    TEST_EQ(1.f, get_avx2_f32(f, 6));
    TEST_EQ(0.f, get_avx2_f32(f, 5));
    TEST_EQ(0.f, get_avx2_f32(f, 4));
    TEST_EQ(1.f, get_avx2_f32(f, 3));
    TEST_EQ(1.f, get_avx2_f32(f, 2));
    TEST_EQ(1.f, get_avx2_f32(f, 1));
    TEST_EQ(1.f, get_avx2_f32(f, 0));

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 f>");
    f = get_last_stack_value();
    TEST_EQ(0.f, get_avx2_f32(f, 7));
    TEST_EQ(0.f, get_avx2_f32(f, 6));
    TEST_EQ(1.f, get_avx2_f32(f, 5));
    TEST_EQ(1.f, get_avx2_f32(f, 4));
    TEST_EQ(0.f, get_avx2_f32(f, 3));
    TEST_EQ(0.f, get_avx2_f32(f, 2));
    TEST_EQ(0.f, get_avx2_f32(f, 1));
    TEST_EQ(0.f, get_avx2_f32(f, 0));

    run("v8 1 2 3 4 5 6 7 8 v8 1 2 0 0 8 8 8 8 f>=");
    f = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(f, 7));
    TEST_EQ(1.f, get_avx2_f32(f, 6));
    TEST_EQ(1.f, get_avx2_f32(f, 5));
    TEST_EQ(1.f, get_avx2_f32(f, 4));
    TEST_EQ(0.f, get_avx2_f32(f, 3));
    TEST_EQ(0.f, get_avx2_f32(f, 2));
    TEST_EQ(0.f, get_avx2_f32(f, 1));
    TEST_EQ(1.f, get_avx2_f32(f, 0));
    }
#endif
  };

struct address : public compile_fixture
  {
#ifdef AVX512
  void test()
    {
    char* ptr = run("12345 2 3 4 5 st@ sp@");
    uint64_t st = get_stack_valuef_uint64(1);
    TEST_EQ((uint64_t)ctxt.aligned_stack_top, st);
    uint64_t sp = get_stack_valuef_uint64(0);
    TEST_EQ((uint64_t)(ptr + 64), sp);

    run("st@ #64- @"); // get stack top, then move 64 bytes back, then fetch
    auto v = get_last_stack_value();
    TEST_EQ(12345.f, get_avx2_f32(v, 15));
    TEST_EQ(12345.f, get_avx2_f32(v, 14));
    TEST_EQ(12345.f, get_avx2_f32(v, 13));
    TEST_EQ(12345.f, get_avx2_f32(v, 12));
    TEST_EQ(12345.f, get_avx2_f32(v, 11));
    TEST_EQ(12345.f, get_avx2_f32(v, 10));
    TEST_EQ(12345.f, get_avx2_f32(v, 9));
    TEST_EQ(12345.f, get_avx2_f32(v, 8));
    TEST_EQ(12345.f, get_avx2_f32(v, 7));
    TEST_EQ(12345.f, get_avx2_f32(v, 6));
    TEST_EQ(12345.f, get_avx2_f32(v, 5));
    TEST_EQ(12345.f, get_avx2_f32(v, 4));
    TEST_EQ(12345.f, get_avx2_f32(v, 3));
    TEST_EQ(12345.f, get_avx2_f32(v, 2));
    TEST_EQ(12345.f, get_avx2_f32(v, 1));
    TEST_EQ(12345.f, get_avx2_f32(v, 0));

    run("101 st@ #64- !   st@ #64- @ "); // store 101 on stack, get stack top, move 32 bytes back, store 101 at address,  get stack top, move 32 bytes back, fetch
    v = get_last_stack_value();
    TEST_EQ(101.f, get_avx2_f32(v, 15));
    TEST_EQ(101.f, get_avx2_f32(v, 14));
    TEST_EQ(101.f, get_avx2_f32(v, 13));
    TEST_EQ(101.f, get_avx2_f32(v, 12));
    TEST_EQ(101.f, get_avx2_f32(v, 11));
    TEST_EQ(101.f, get_avx2_f32(v, 10));
    TEST_EQ(101.f, get_avx2_f32(v, 9));
    TEST_EQ(101.f, get_avx2_f32(v, 8));
    TEST_EQ(101.f, get_avx2_f32(v, 7));
    TEST_EQ(101.f, get_avx2_f32(v, 6));
    TEST_EQ(101.f, get_avx2_f32(v, 5));
    TEST_EQ(101.f, get_avx2_f32(v, 4));
    TEST_EQ(101.f, get_avx2_f32(v, 3));
    TEST_EQ(101.f, get_avx2_f32(v, 2));
    TEST_EQ(101.f, get_avx2_f32(v, 1));
    TEST_EQ(101.f, get_avx2_f32(v, 0));
    }
#else
  void test()
    {
    char* ptr = run("12345 2 3 4 5 st@ sp@");
    uint64_t st = get_stack_valuef_uint64(1);
    TEST_EQ((uint64_t)ctxt.aligned_stack_top, st);
    uint64_t sp = get_stack_valuef_uint64(0);
    TEST_EQ((uint64_t)(ptr+32), sp);

    run("st@ #32- @"); // get stack top, then move 32 bytes back, then fetch
    auto v = get_last_stack_value();
    TEST_EQ(12345.f, get_avx2_f32(v, 7));
    TEST_EQ(12345.f, get_avx2_f32(v, 6));
    TEST_EQ(12345.f, get_avx2_f32(v, 5));
    TEST_EQ(12345.f, get_avx2_f32(v, 4));
    TEST_EQ(12345.f, get_avx2_f32(v, 3));
    TEST_EQ(12345.f, get_avx2_f32(v, 2));
    TEST_EQ(12345.f, get_avx2_f32(v, 1));
    TEST_EQ(12345.f, get_avx2_f32(v, 0));

    run("101 st@ #32- !   st@ #32- @ "); // store 101 on stack, get stack top, move 32 bytes back, store 101 at address,  get stack top, move 32 bytes back, fetch
    v = get_last_stack_value();
    TEST_EQ(101.f, get_avx2_f32(v, 7));
    TEST_EQ(101.f, get_avx2_f32(v, 6));
    TEST_EQ(101.f, get_avx2_f32(v, 5));
    TEST_EQ(101.f, get_avx2_f32(v, 4));
    TEST_EQ(101.f, get_avx2_f32(v, 3));
    TEST_EQ(101.f, get_avx2_f32(v, 2));
    TEST_EQ(101.f, get_avx2_f32(v, 1));
    TEST_EQ(101.f, get_avx2_f32(v, 0));
    }
#endif
  };

struct prim : public compile_fixture
  {
#ifdef AVX512
  void test()
    {
    run("v16 -1 2 -3 4 5 -6 7 -8 9 -10 11 -12 13 -14 15 -16 abs");
    auto v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 15));
    TEST_EQ(2.f, get_avx2_f32(v, 14));
    TEST_EQ(3.f, get_avx2_f32(v, 13));
    TEST_EQ(4.f, get_avx2_f32(v, 12));
    TEST_EQ(5.f, get_avx2_f32(v, 11));
    TEST_EQ(6.f, get_avx2_f32(v, 10));
    TEST_EQ(7.f, get_avx2_f32(v, 9));
    TEST_EQ(8.f, get_avx2_f32(v, 8));
    TEST_EQ(9.f, get_avx2_f32(v, 7));
    TEST_EQ(10.f, get_avx2_f32(v, 6));
    TEST_EQ(11.f, get_avx2_f32(v, 5));
    TEST_EQ(12.f, get_avx2_f32(v, 4));
    TEST_EQ(13.f, get_avx2_f32(v, 3));
    TEST_EQ(14.f, get_avx2_f32(v, 2));
    TEST_EQ(15.f, get_avx2_f32(v, 1));
    TEST_EQ(16.f, get_avx2_f32(v, 0));
    
    run("v16 -1 2 -3 4 5 -6 7 -8 9 -10 11 -12 13 -14 15 -16 negate");
    v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 15));
    TEST_EQ(-2.f, get_avx2_f32(v, 14));
    TEST_EQ(3.f, get_avx2_f32(v, 13));
    TEST_EQ(-4.f, get_avx2_f32(v, 12));
    TEST_EQ(-5.f, get_avx2_f32(v, 11));
    TEST_EQ(6.f, get_avx2_f32(v, 10));
    TEST_EQ(-7.f, get_avx2_f32(v, 9));
    TEST_EQ(8.f, get_avx2_f32(v, 8));
    TEST_EQ(-9.f, get_avx2_f32(v, 7));
    TEST_EQ(10.f, get_avx2_f32(v, 6));
    TEST_EQ(-11.f, get_avx2_f32(v, 5));
    TEST_EQ(12.f, get_avx2_f32(v, 4));
    TEST_EQ(-13.f, get_avx2_f32(v, 3));
    TEST_EQ(14.f, get_avx2_f32(v, 2));
    TEST_EQ(-15.f, get_avx2_f32(v, 1));
    TEST_EQ(16.f, get_avx2_f32(v, 0));
    
    run("v16 #1 #3 #7 #15 #31 #63 #127 #255 #1 #3 #7 #15 #31 #63 #127 #255 v16 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 and");
    auto i = get_last_stack_value_i();
    TEST_EQ(0, get_avx2_i32(i, 15));
    TEST_EQ(2, get_avx2_i32(i, 14));
    TEST_EQ(2, get_avx2_i32(i, 13));
    TEST_EQ(10, get_avx2_i32(i, 12));
    TEST_EQ(10, get_avx2_i32(i, 11));
    TEST_EQ(10, get_avx2_i32(i, 10));
    TEST_EQ(10, get_avx2_i32(i, 9));
    TEST_EQ(10, get_avx2_i32(i, 8));
    TEST_EQ(0, get_avx2_i32(i, 7));
    TEST_EQ(2, get_avx2_i32(i, 6));
    TEST_EQ(2, get_avx2_i32(i, 5));
    TEST_EQ(10, get_avx2_i32(i, 4));
    TEST_EQ(10, get_avx2_i32(i, 3));
    TEST_EQ(10, get_avx2_i32(i, 2));
    TEST_EQ(10, get_avx2_i32(i, 1));
    TEST_EQ(10, get_avx2_i32(i, 0));
    
    run("v16 #1 #3 #7 #15 #31 #63 #127 #255 #1 #3 #7 #15 #31 #63 #127 #255 v16 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 or");
    i = get_last_stack_value_i();
    TEST_EQ(11, get_avx2_i32(i, 7));
    TEST_EQ(11, get_avx2_i32(i, 6));
    TEST_EQ(15, get_avx2_i32(i, 5));
    TEST_EQ(15, get_avx2_i32(i, 4));
    TEST_EQ(31, get_avx2_i32(i, 3));
    TEST_EQ(63, get_avx2_i32(i, 2));
    TEST_EQ(127, get_avx2_i32(i, 1));
    TEST_EQ(255, get_avx2_i32(i, 0));
    
    run("v16 #1 #3 #7 #15 #31 #63 #127 #255 #1 #3 #7 #15 #31 #63 #127 #255 v16 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10 #10  xor");
    i = get_last_stack_value_i();
    TEST_EQ(11, get_avx2_i32(i, 7));
    TEST_EQ(9, get_avx2_i32(i, 6));
    TEST_EQ(13, get_avx2_i32(i, 5));
    TEST_EQ(5, get_avx2_i32(i, 4));
    TEST_EQ(21, get_avx2_i32(i, 3));
    TEST_EQ(53, get_avx2_i32(i, 2));
    TEST_EQ(117, get_avx2_i32(i, 1));
    TEST_EQ(245, get_avx2_i32(i, 0));
    
    run("v16 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 floor");
    v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(-2.f, get_avx2_f32(v, 6));
    TEST_EQ(1.f, get_avx2_f32(v, 5));
    TEST_EQ(-2.f, get_avx2_f32(v, 4));
    TEST_EQ(3.f, get_avx2_f32(v, 3));
    TEST_EQ(-3.f, get_avx2_f32(v, 2));
    TEST_EQ(1.f, get_avx2_f32(v, 1));
    TEST_EQ(-2.f, get_avx2_f32(v, 0));
    
    run("v16 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 ceil");
    v = get_last_stack_value();
    TEST_EQ(2.f, get_avx2_f32(v, 7));
    TEST_EQ(-1.f, get_avx2_f32(v, 6));
    TEST_EQ(2.f, get_avx2_f32(v, 5));
    TEST_EQ(-1.f, get_avx2_f32(v, 4));
    TEST_EQ(3.f, get_avx2_f32(v, 3));
    TEST_EQ(-3.f, get_avx2_f32(v, 2));
    TEST_EQ(2.f, get_avx2_f32(v, 1));
    TEST_EQ(-1.f, get_avx2_f32(v, 0));

    run("v16 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 round");
    v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(-1.f, get_avx2_f32(v, 6));
    TEST_EQ(2.f, get_avx2_f32(v, 5));
    TEST_EQ(-2.f, get_avx2_f32(v, 4));
    TEST_EQ(3.f, get_avx2_f32(v, 3));
    TEST_EQ(-3.f, get_avx2_f32(v, 2));
    TEST_EQ(2.f, get_avx2_f32(v, 1));
    TEST_EQ(-2.f, get_avx2_f32(v, 0));

    run("v16 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 trunc");
    v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(-1.f, get_avx2_f32(v, 6));
    TEST_EQ(1.f, get_avx2_f32(v, 5));
    TEST_EQ(-1.f, get_avx2_f32(v, 4));
    TEST_EQ(3.f, get_avx2_f32(v, 3));
    TEST_EQ(-3.f, get_avx2_f32(v, 2));
    TEST_EQ(1.f, get_avx2_f32(v, 1));
    TEST_EQ(-1.f, get_avx2_f32(v, 0));
    
    run("v16 #-1 #xf0f0f0f0 #x0f0f0f0f #0 #0 #0 #0 #0  #-1 #xf0f0f0f0 #x0f0f0f0f #0 #0 #0 #0 #0 not");
    i = get_last_stack_value_i();
    TEST_EQ(0, get_avx2_i32(i, 7));
    TEST_EQ(0x0f0f0f0f, get_avx2_i32(i, 6));
    TEST_EQ(0xf0f0f0f0, get_avx2_i32(i, 5));
    TEST_EQ(-1, get_avx2_i32(i, 4));
    
    run("v16 #1 #2 #3 #4 #5 #6 #7 #8 #9 #10 #11 #12 #13 #14 #15 #16 fcast");
    v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 15));
    TEST_EQ(2.f, get_avx2_f32(v, 14));
    TEST_EQ(3.f, get_avx2_f32(v, 13));
    TEST_EQ(4.f, get_avx2_f32(v, 12));
    TEST_EQ(5.f, get_avx2_f32(v, 11));
    TEST_EQ(6.f, get_avx2_f32(v, 10));
    TEST_EQ(7.f, get_avx2_f32(v, 9));
    TEST_EQ(8.f, get_avx2_f32(v, 8));
    TEST_EQ(9.f, get_avx2_f32(v, 7));
    TEST_EQ(10.f, get_avx2_f32(v, 6));
    TEST_EQ(11.f, get_avx2_f32(v, 5));
    TEST_EQ(12.f, get_avx2_f32(v, 4));
    TEST_EQ(13.f, get_avx2_f32(v, 3));
    TEST_EQ(14.f, get_avx2_f32(v, 2));
    TEST_EQ(15.f, get_avx2_f32(v, 1));
    TEST_EQ(16.f, get_avx2_f32(v, 0));
    
    run("v16 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 icast");
    i = get_last_stack_value_i();
    TEST_EQ(1, get_avx2_f32(v, 15));
    TEST_EQ(2, get_avx2_f32(v, 14));
    TEST_EQ(3, get_avx2_f32(v, 13));
    TEST_EQ(4, get_avx2_f32(v, 12));
    TEST_EQ(5, get_avx2_f32(v, 11));
    TEST_EQ(6, get_avx2_f32(v, 10));
    TEST_EQ(7, get_avx2_f32(v, 9));
    TEST_EQ(8, get_avx2_f32(v, 8));
    TEST_EQ(9, get_avx2_f32(v, 7));
    TEST_EQ(10, get_avx2_f32(v, 6));
    TEST_EQ(11, get_avx2_f32(v, 5));
    TEST_EQ(12, get_avx2_f32(v, 4));
    TEST_EQ(13, get_avx2_f32(v, 3));
    TEST_EQ(14, get_avx2_f32(v, 2));
    TEST_EQ(15, get_avx2_f32(v, 1));
    TEST_EQ(16, get_avx2_f32(v, 0));
    
    run("4.2 2.1 fm/mod");
    TEST_EQ(2.f, get_stack_valuef(0));
    TEST_EQ(0.f, get_stack_valuef(1));

    run("4.2 2 fm/mod");
    TEST_EQ(2.f, get_stack_valuef(0));
    TEST_EQ_CLOSE(0.2f, get_stack_valuef(1), 1e-5f);

    run("10 3 fm/mod");
    TEST_EQ(3.f, get_stack_valuef(0));
    TEST_EQ_CLOSE(1.f, get_stack_valuef(1), 1e-5f);
    
    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 v16 8 7 6 5 4 3 2 1 8 7 6 5 4 3 2 1 min");
    v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(2.f, get_avx2_f32(v, 6));
    TEST_EQ(3.f, get_avx2_f32(v, 5));
    TEST_EQ(4.f, get_avx2_f32(v, 4));
    TEST_EQ(4.f, get_avx2_f32(v, 3));
    TEST_EQ(3.f, get_avx2_f32(v, 2));
    TEST_EQ(2.f, get_avx2_f32(v, 1));
    TEST_EQ(1.f, get_avx2_f32(v, 0));

    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 v16 8 7 6 5 4 3 2 1 8 7 6 5 4 3 2 1 max");
    v = get_last_stack_value();
    TEST_EQ(8.f, get_avx2_f32(v, 7));
    TEST_EQ(7.f, get_avx2_f32(v, 6));
    TEST_EQ(6.f, get_avx2_f32(v, 5));
    TEST_EQ(5.f, get_avx2_f32(v, 4));
    TEST_EQ(5.f, get_avx2_f32(v, 3));
    TEST_EQ(6.f, get_avx2_f32(v, 2));
    TEST_EQ(7.f, get_avx2_f32(v, 1));
    TEST_EQ(8.f, get_avx2_f32(v, 0));
    
    }
#else
  void test()
    {
    run("v8 -1 2 -3 4 5 -6 7 -8 abs");
    auto v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(2.f, get_avx2_f32(v, 6));
    TEST_EQ(3.f, get_avx2_f32(v, 5));
    TEST_EQ(4.f, get_avx2_f32(v, 4));
    TEST_EQ(5.f, get_avx2_f32(v, 3));
    TEST_EQ(6.f, get_avx2_f32(v, 2));
    TEST_EQ(7.f, get_avx2_f32(v, 1));
    TEST_EQ(8.f, get_avx2_f32(v, 0));

    run("v8 -1 2 -3 4 5 -6 7 -8 negate");
    v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(-2.f, get_avx2_f32(v, 6));
    TEST_EQ(3.f, get_avx2_f32(v, 5));
    TEST_EQ(-4.f, get_avx2_f32(v, 4));
    TEST_EQ(-5.f, get_avx2_f32(v, 3));
    TEST_EQ(6.f, get_avx2_f32(v, 2));
    TEST_EQ(-7.f, get_avx2_f32(v, 1));
    TEST_EQ(8.f, get_avx2_f32(v, 0));

    run("v8 #1 #3 #7 #15 #31 #63 #127 #255  v8 #10 #10 #10 #10 #10 #10 #10 #10  and");
    auto i = get_last_stack_value_i();
    TEST_EQ(0, get_avx2_i32(i, 7));
    TEST_EQ(2, get_avx2_i32(i, 6));
    TEST_EQ(2, get_avx2_i32(i, 5));
    TEST_EQ(10, get_avx2_i32(i, 4));
    TEST_EQ(10, get_avx2_i32(i, 3));
    TEST_EQ(10, get_avx2_i32(i, 2));
    TEST_EQ(10, get_avx2_i32(i, 1));
    TEST_EQ(10, get_avx2_i32(i, 0));

    run("v8 #1 #3 #7 #15 #31 #63 #127 #255  v8 #10 #10 #10 #10 #10 #10 #10 #10  or");
    i = get_last_stack_value_i();
    TEST_EQ(11, get_avx2_i32(i, 7));
    TEST_EQ(11, get_avx2_i32(i, 6));
    TEST_EQ(15, get_avx2_i32(i, 5));
    TEST_EQ(15, get_avx2_i32(i, 4));
    TEST_EQ(31, get_avx2_i32(i, 3));
    TEST_EQ(63, get_avx2_i32(i, 2));
    TEST_EQ(127, get_avx2_i32(i, 1));
    TEST_EQ(255, get_avx2_i32(i, 0));

    run("v8 #1 #3 #7 #15 #31 #63 #127 #255  v8 #10 #10 #10 #10 #10 #10 #10 #10  xor");
    i = get_last_stack_value_i();
    TEST_EQ(11, get_avx2_i32(i, 7));
    TEST_EQ(9, get_avx2_i32(i, 6));
    TEST_EQ(13, get_avx2_i32(i, 5));
    TEST_EQ(5, get_avx2_i32(i, 4));
    TEST_EQ(21, get_avx2_i32(i, 3));
    TEST_EQ(53, get_avx2_i32(i, 2));
    TEST_EQ(117, get_avx2_i32(i, 1));
    TEST_EQ(245, get_avx2_i32(i, 0));

    run("v8 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 floor");
    v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(-2.f, get_avx2_f32(v, 6));
    TEST_EQ(1.f, get_avx2_f32(v, 5));
    TEST_EQ(-2.f, get_avx2_f32(v, 4));
    TEST_EQ(3.f, get_avx2_f32(v, 3));
    TEST_EQ(-3.f, get_avx2_f32(v, 2));
    TEST_EQ(1.f, get_avx2_f32(v, 1));
    TEST_EQ(-2.f, get_avx2_f32(v, 0));

    run("v8 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 ceil");
    v = get_last_stack_value();
    TEST_EQ(2.f, get_avx2_f32(v, 7));
    TEST_EQ(-1.f, get_avx2_f32(v, 6));
    TEST_EQ(2.f, get_avx2_f32(v, 5));
    TEST_EQ(-1.f, get_avx2_f32(v, 4));
    TEST_EQ(3.f, get_avx2_f32(v, 3));
    TEST_EQ(-3.f, get_avx2_f32(v, 2));
    TEST_EQ(2.f, get_avx2_f32(v, 1));
    TEST_EQ(-1.f, get_avx2_f32(v, 0));

    run("v8 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 round");
    v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(-1.f, get_avx2_f32(v, 6));
    TEST_EQ(2.f, get_avx2_f32(v, 5));
    TEST_EQ(-2.f, get_avx2_f32(v, 4));
    TEST_EQ(3.f, get_avx2_f32(v, 3));
    TEST_EQ(-3.f, get_avx2_f32(v, 2));
    TEST_EQ(2.f, get_avx2_f32(v, 1));
    TEST_EQ(-2.f, get_avx2_f32(v, 0));

    run("v8 1.2 -1.2 1.7 -1.7 3 -3 1.5 -1.5 trunc");
    v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(-1.f, get_avx2_f32(v, 6));
    TEST_EQ(1.f, get_avx2_f32(v, 5));
    TEST_EQ(-1.f, get_avx2_f32(v, 4));
    TEST_EQ(3.f, get_avx2_f32(v, 3));
    TEST_EQ(-3.f, get_avx2_f32(v, 2));
    TEST_EQ(1.f, get_avx2_f32(v, 1));
    TEST_EQ(-1.f, get_avx2_f32(v, 0));
    
    run("v8 #-1 #xf0f0f0f0 #x0f0f0f0f #0 #0 #0 #0 #0 not");
    i = get_last_stack_value_i();
    TEST_EQ(0, get_avx2_i32(i, 7));
    TEST_EQ(0x0f0f0f0f, get_avx2_i32(i, 6));
    TEST_EQ(0xf0f0f0f0, get_avx2_i32(i, 5));
    TEST_EQ(-1, get_avx2_i32(i, 4));

    run("v8 #1 #2 #3 #4 #5 #6 #7 #8 fcast");
    v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(2.f, get_avx2_f32(v, 6));
    TEST_EQ(3.f, get_avx2_f32(v, 5));
    TEST_EQ(4.f, get_avx2_f32(v, 4));
    TEST_EQ(5.f, get_avx2_f32(v, 3));
    TEST_EQ(6.f, get_avx2_f32(v, 2));
    TEST_EQ(7.f, get_avx2_f32(v, 1));
    TEST_EQ(8.f, get_avx2_f32(v, 0));

    run("v8 1 2 3 4 5 6 7 8 icast");
    i = get_last_stack_value_i();
    TEST_EQ(1, get_avx2_i32(i, 7));
    TEST_EQ(2, get_avx2_i32(i, 6));
    TEST_EQ(3, get_avx2_i32(i, 5));
    TEST_EQ(4, get_avx2_i32(i, 4));
    TEST_EQ(5, get_avx2_i32(i, 3));
    TEST_EQ(6, get_avx2_i32(i, 2));
    TEST_EQ(7, get_avx2_i32(i, 1));
    TEST_EQ(8, get_avx2_i32(i, 0));

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
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(2.f, get_avx2_f32(v, 6));
    TEST_EQ(3.f, get_avx2_f32(v, 5));
    TEST_EQ(4.f, get_avx2_f32(v, 4));
    TEST_EQ(4.f, get_avx2_f32(v, 3));
    TEST_EQ(3.f, get_avx2_f32(v, 2));
    TEST_EQ(2.f, get_avx2_f32(v, 1));
    TEST_EQ(1.f, get_avx2_f32(v, 0));

    run("v8 1 2 3 4 5 6 7 8 v8 8 7 6 5 4 3 2 1 max");
    v = get_last_stack_value();
    TEST_EQ(8.f, get_avx2_f32(v, 7));
    TEST_EQ(7.f, get_avx2_f32(v, 6));
    TEST_EQ(6.f, get_avx2_f32(v, 5));
    TEST_EQ(5.f, get_avx2_f32(v, 4));
    TEST_EQ(5.f, get_avx2_f32(v, 3));
    TEST_EQ(6.f, get_avx2_f32(v, 2));
    TEST_EQ(7.f, get_avx2_f32(v, 1));
    TEST_EQ(8.f, get_avx2_f32(v, 0));
    }
#endif
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
#ifdef AVX512
  void test()
    {
    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 4 > if 2 else 3 then");
    auto v = get_stack_value(0);
    TEST_EQ(3.f, get_avx2_f32(v, 7));
    TEST_EQ(3.f, get_avx2_f32(v, 6));
    TEST_EQ(3.f, get_avx2_f32(v, 5));
    TEST_EQ(3.f, get_avx2_f32(v, 4));
    TEST_EQ(2.f, get_avx2_f32(v, 3));
    TEST_EQ(2.f, get_avx2_f32(v, 2));
    TEST_EQ(2.f, get_avx2_f32(v, 1));
    TEST_EQ(2.f, get_avx2_f32(v, 0));
    
    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 2 fm/mod drop 0 = if 10 12 else 11 13 then");
    v = get_stack_value(0);
    TEST_EQ(13.f, get_avx2_f32(v, 7));
    TEST_EQ(12.f, get_avx2_f32(v, 6));
    TEST_EQ(13.f, get_avx2_f32(v, 5));
    TEST_EQ(12.f, get_avx2_f32(v, 4));
    TEST_EQ(13.f, get_avx2_f32(v, 3));
    TEST_EQ(12.f, get_avx2_f32(v, 2));
    TEST_EQ(13.f, get_avx2_f32(v, 1));
    TEST_EQ(12.f, get_avx2_f32(v, 0));
    v = get_stack_value(1);
    TEST_EQ(11.f, get_avx2_f32(v, 7));
    TEST_EQ(10.f, get_avx2_f32(v, 6));
    TEST_EQ(11.f, get_avx2_f32(v, 5));
    TEST_EQ(10.f, get_avx2_f32(v, 4));
    TEST_EQ(11.f, get_avx2_f32(v, 3));
    TEST_EQ(10.f, get_avx2_f32(v, 2));
    TEST_EQ(11.f, get_avx2_f32(v, 1));
    TEST_EQ(10.f, get_avx2_f32(v, 0));
    
    run("1 2 > if 3 else 5 then");
    v = get_stack_value(0);
    TEST_EQ(5.f, get_avx2_f32(v, 7));
    TEST_EQ(5.f, get_avx2_f32(v, 6));
    TEST_EQ(5.f, get_avx2_f32(v, 5));
    TEST_EQ(5.f, get_avx2_f32(v, 4));
    TEST_EQ(5.f, get_avx2_f32(v, 3));
    TEST_EQ(5.f, get_avx2_f32(v, 2));
    TEST_EQ(5.f, get_avx2_f32(v, 1));
    TEST_EQ(5.f, get_avx2_f32(v, 0));
    
    run("1 2 < if 3 else 5 then");
    v = get_stack_value(0);
    TEST_EQ(3.f, get_avx2_f32(v, 7));
    TEST_EQ(3.f, get_avx2_f32(v, 6));
    TEST_EQ(3.f, get_avx2_f32(v, 5));
    TEST_EQ(3.f, get_avx2_f32(v, 4));
    TEST_EQ(3.f, get_avx2_f32(v, 3));
    TEST_EQ(3.f, get_avx2_f32(v, 2));
    TEST_EQ(3.f, get_avx2_f32(v, 1));
    TEST_EQ(3.f, get_avx2_f32(v, 0));
    
    run("1 2 < if 3 2 1 else 5 6 7 then");
    for (int i = 0; i < 3; ++i)
      {
      v = get_stack_value(i);
      for (int j = 0; j < 16; ++j)
        TEST_EQ(i == 0 ? 1.f : i == 1 ? 2.f : 3.f, get_avx2_f32(v, j));
      }

    run("1 2 > if 3 2 1 else 5 6 7 then");
    for (int i = 0; i < 3; ++i)
      {
      v = get_stack_value(i);
      for (int j = 0; j < 16; ++j)
        TEST_EQ(i == 0 ? 7.f : i == 1 ? 6.f : 5.f, get_avx2_f32(v, j));
      }
    
    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 dup 2 fm/mod drop 0 = if dup 4 > if 40 else 39 then else  dup 2 < if 11 else 27 then then");
    v = get_stack_value(0);
    TEST_EQ(11.f, get_avx2_f32(v, 7));
    TEST_EQ(39.f, get_avx2_f32(v, 6));
    TEST_EQ(27.f, get_avx2_f32(v, 5));
    TEST_EQ(39.f, get_avx2_f32(v, 4));
    TEST_EQ(27.f, get_avx2_f32(v, 3));
    TEST_EQ(40.f, get_avx2_f32(v, 2));
    TEST_EQ(27.f, get_avx2_f32(v, 1));
    TEST_EQ(40.f, get_avx2_f32(v, 0));

    run("3 4 > if 2 > 1 if 10 else 11 then else 7 8 < if 2 else 20 then then");
    v = get_stack_value(0);
    TEST_EQ(2.f, get_avx2_f32(v, 7));
    TEST_EQ(2.f, get_avx2_f32(v, 6));
    TEST_EQ(2.f, get_avx2_f32(v, 5));
    TEST_EQ(2.f, get_avx2_f32(v, 4));
    TEST_EQ(2.f, get_avx2_f32(v, 3));
    TEST_EQ(2.f, get_avx2_f32(v, 2));
    TEST_EQ(2.f, get_avx2_f32(v, 1));
    TEST_EQ(2.f, get_avx2_f32(v, 0));
    }
#else
  void test()
    {
    
    //if <true> else <false> then should be balanced:
    //- both branches should add equally amount on the stack
    //- any usage of the return stack should be balanced
    //- no branch is allowed to pop from the stack that existed before the branch
    
    run("v8 1 2 3 4 5 6 7 8 4 > if 2 else 3 then");
    auto v = get_stack_value(0);
    TEST_EQ(3.f, get_avx2_f32(v, 7));
    TEST_EQ(3.f, get_avx2_f32(v, 6));
    TEST_EQ(3.f, get_avx2_f32(v, 5));
    TEST_EQ(3.f, get_avx2_f32(v, 4));
    TEST_EQ(2.f, get_avx2_f32(v, 3));
    TEST_EQ(2.f, get_avx2_f32(v, 2));
    TEST_EQ(2.f, get_avx2_f32(v, 1));
    TEST_EQ(2.f, get_avx2_f32(v, 0));

    run("v8 1 2 3 4 5 6 7 8 2 fm/mod drop 0 = if 10 12 else 11 13 then");
    v = get_stack_value(0);
    TEST_EQ(13.f, get_avx2_f32(v, 7));
    TEST_EQ(12.f, get_avx2_f32(v, 6));
    TEST_EQ(13.f, get_avx2_f32(v, 5));
    TEST_EQ(12.f, get_avx2_f32(v, 4));
    TEST_EQ(13.f, get_avx2_f32(v, 3));
    TEST_EQ(12.f, get_avx2_f32(v, 2));
    TEST_EQ(13.f, get_avx2_f32(v, 1));
    TEST_EQ(12.f, get_avx2_f32(v, 0));
    v = get_stack_value(1);
    TEST_EQ(11.f, get_avx2_f32(v, 7));
    TEST_EQ(10.f, get_avx2_f32(v, 6));
    TEST_EQ(11.f, get_avx2_f32(v, 5));
    TEST_EQ(10.f, get_avx2_f32(v, 4));
    TEST_EQ(11.f, get_avx2_f32(v, 3));
    TEST_EQ(10.f, get_avx2_f32(v, 2));
    TEST_EQ(11.f, get_avx2_f32(v, 1));
    TEST_EQ(10.f, get_avx2_f32(v, 0));

    run("1 2 > if 3 else 5 then");
    v = get_stack_value(0);
    TEST_EQ(5.f, get_avx2_f32(v, 7));
    TEST_EQ(5.f, get_avx2_f32(v, 6));
    TEST_EQ(5.f, get_avx2_f32(v, 5));
    TEST_EQ(5.f, get_avx2_f32(v, 4));
    TEST_EQ(5.f, get_avx2_f32(v, 3));
    TEST_EQ(5.f, get_avx2_f32(v, 2));
    TEST_EQ(5.f, get_avx2_f32(v, 1));
    TEST_EQ(5.f, get_avx2_f32(v, 0));

    run("1 2 < if 3 else 5 then");
    v = get_stack_value(0);
    TEST_EQ(3.f, get_avx2_f32(v, 7));
    TEST_EQ(3.f, get_avx2_f32(v, 6));
    TEST_EQ(3.f, get_avx2_f32(v, 5));
    TEST_EQ(3.f, get_avx2_f32(v, 4));
    TEST_EQ(3.f, get_avx2_f32(v, 3));
    TEST_EQ(3.f, get_avx2_f32(v, 2));
    TEST_EQ(3.f, get_avx2_f32(v, 1));
    TEST_EQ(3.f, get_avx2_f32(v, 0));
    
    run("1 2 < if 3 2 1 else 5 6 7 then");    
    for (int i = 0; i < 3; ++i)
      {
      v = get_stack_value(i);
      for (int j = 0; j < 8; ++j)
        TEST_EQ(i == 0 ? 1.f : i == 1 ? 2.f : 3.f, get_avx2_f32(v, j));
      }
   
    run("1 2 > if 3 2 1 else 5 6 7 then");
    for (int i = 0; i < 3; ++i)
      {
      v = get_stack_value(i);
      for (int j = 0; j < 8; ++j)
        TEST_EQ(i == 0 ? 7.f : i == 1 ? 6.f : 5.f, get_avx2_f32(v, j));
      }

    run("v8 1 2 3 4 5 6 7 8 dup 2 fm/mod drop 0 = if dup 4 > if 40 else 39 then else  dup 2 < if 11 else 27 then then");
    v = get_stack_value(0);
    TEST_EQ(11.f, get_avx2_f32(v, 7));
    TEST_EQ(39.f, get_avx2_f32(v, 6));
    TEST_EQ(27.f, get_avx2_f32(v, 5));
    TEST_EQ(39.f, get_avx2_f32(v, 4));
    TEST_EQ(27.f, get_avx2_f32(v, 3));
    TEST_EQ(40.f, get_avx2_f32(v, 2));
    TEST_EQ(27.f, get_avx2_f32(v, 1));
    TEST_EQ(40.f, get_avx2_f32(v, 0));

    run("3 4 > if 2 > 1 if 10 else 11 then else 7 8 < if 2 else 20 then then");
    v = get_stack_value(0);
    TEST_EQ(2.f, get_avx2_f32(v, 7));
    TEST_EQ(2.f, get_avx2_f32(v, 6));
    TEST_EQ(2.f, get_avx2_f32(v, 5));
    TEST_EQ(2.f, get_avx2_f32(v, 4));
    TEST_EQ(2.f, get_avx2_f32(v, 3));
    TEST_EQ(2.f, get_avx2_f32(v, 2));
    TEST_EQ(2.f, get_avx2_f32(v, 1));
    TEST_EQ(2.f, get_avx2_f32(v, 0));
    }
#endif
  };

struct stdlib_tests : public compile_fixture
  {
#ifdef AVX512
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
    TEST_EQ(-1, get_avx2_i32(u, 7));
    TEST_EQ(-1, get_avx2_i32(u, 6));
    TEST_EQ(-1, get_avx2_i32(u, 5));
    TEST_EQ(-1, get_avx2_i32(u, 4));
    TEST_EQ(-1, get_avx2_i32(u, 3));
    TEST_EQ(-1, get_avx2_i32(u, 2));
    TEST_EQ(-1, get_avx2_i32(u, 1));
    TEST_EQ(-1, get_avx2_i32(u, 0));

    
    run("v16 0 0.5 1 1.5 2 2.5 3 3.5 0 0.5 1 1.5 2 2.5 3 3.5 1 3 within");
    u = get_last_stack_value_i();
    TEST_EQ(0, get_avx2_i32(u, 7));
    TEST_EQ(0, get_avx2_i32(u, 6));
    TEST_EQ(-1, get_avx2_i32(u, 5));
    TEST_EQ(-1, get_avx2_i32(u, 4));
    TEST_EQ(-1, get_avx2_i32(u, 3));
    TEST_EQ(-1, get_avx2_i32(u, 2));
    TEST_EQ(0, get_avx2_i32(u, 1));
    TEST_EQ(0, get_avx2_i32(u, 0));

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
    
    run("v16 0 0.5 1 1.5 2 2.5 3 3.5 0 0.5 1 1.5 2 2.5 3 3.5 1 3 clamp");
    auto v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(1.f, get_avx2_f32(v, 6));
    TEST_EQ(1.f, get_avx2_f32(v, 5));
    TEST_EQ(1.5f, get_avx2_f32(v, 4));
    TEST_EQ(2.f, get_avx2_f32(v, 3));
    TEST_EQ(2.5f, get_avx2_f32(v, 2));
    TEST_EQ(3.f, get_avx2_f32(v, 1));
    TEST_EQ(3.f, get_avx2_f32(v, 0));
    
    }
#else
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
    TEST_EQ(-1, get_avx2_i32(u, 7));
    TEST_EQ(-1, get_avx2_i32(u, 6));
    TEST_EQ(-1, get_avx2_i32(u, 5));
    TEST_EQ(-1, get_avx2_i32(u, 4));
    TEST_EQ(-1, get_avx2_i32(u, 3));
    TEST_EQ(-1, get_avx2_i32(u, 2));
    TEST_EQ(-1, get_avx2_i32(u, 1));
    TEST_EQ(-1, get_avx2_i32(u, 0));


    run("v8 0 0.5 1 1.5 2 2.5 3 3.5 1 3 within");
    u = get_last_stack_value_i();
    TEST_EQ(0, get_avx2_i32(u, 7));
    TEST_EQ(0, get_avx2_i32(u, 6));
    TEST_EQ(-1, get_avx2_i32(u, 5));
    TEST_EQ(-1, get_avx2_i32(u, 4));
    TEST_EQ(-1, get_avx2_i32(u, 3));
    TEST_EQ(-1, get_avx2_i32(u, 2));
    TEST_EQ(0, get_avx2_i32(u, 1));
    TEST_EQ(0, get_avx2_i32(u, 0));

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
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(1.f, get_avx2_f32(v, 6));
    TEST_EQ(1.f, get_avx2_f32(v, 5));
    TEST_EQ(1.5f, get_avx2_f32(v, 4));
    TEST_EQ(2.f, get_avx2_f32(v, 3));
    TEST_EQ(2.5f, get_avx2_f32(v, 2));
    TEST_EQ(3.f, get_avx2_f32(v, 1));
    TEST_EQ(3.f, get_avx2_f32(v, 0));
    }
#endif
  };

struct begin_while_repeat_tests : public compile_fixture
  {
#ifdef AVX512
  void test()
    {
    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 begin dup 10 > while 1 + repeat");
    auto v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(2.f, get_avx2_f32(v, 6));
    TEST_EQ(3.f, get_avx2_f32(v, 5));
    TEST_EQ(4.f, get_avx2_f32(v, 4));
    TEST_EQ(5.f, get_avx2_f32(v, 3));
    TEST_EQ(6.f, get_avx2_f32(v, 2));
    TEST_EQ(7.f, get_avx2_f32(v, 1));
    TEST_EQ(8.f, get_avx2_f32(v, 0));
    
    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 begin dup 10 < while 1 + repeat");
    v = get_last_stack_value();
    TEST_EQ(10.f, get_avx2_f32(v, 7));
    TEST_EQ(11.f, get_avx2_f32(v, 6));
    TEST_EQ(12.f, get_avx2_f32(v, 5));
    TEST_EQ(13.f, get_avx2_f32(v, 4));
    TEST_EQ(14.f, get_avx2_f32(v, 3));
    TEST_EQ(15.f, get_avx2_f32(v, 2));
    TEST_EQ(16.f, get_avx2_f32(v, 1));
    TEST_EQ(17.f, get_avx2_f32(v, 0));

    run("v16 1 2 3 4 5 6 7 8 1 2 3 4 5 6 7 8 begin dup 10 < while 2 begin dup 4 < while 1 + repeat + repeat");
    v = get_last_stack_value();
    TEST_EQ(13.f, get_avx2_f32(v, 7));
    TEST_EQ(14.f, get_avx2_f32(v, 6));
    TEST_EQ(15.f, get_avx2_f32(v, 5));
    TEST_EQ(16.f, get_avx2_f32(v, 4));
    TEST_EQ(17.f, get_avx2_f32(v, 3));
    TEST_EQ(18.f, get_avx2_f32(v, 2));
    TEST_EQ(19.f, get_avx2_f32(v, 1));
    TEST_EQ(20.f, get_avx2_f32(v, 0));
    
    }
#else
  void test()
    {
    run("v8 1 2 3 4 5 6 7 8 begin dup 10 > while 1 + repeat");
    auto v = get_last_stack_value();
    TEST_EQ(1.f, get_avx2_f32(v, 7));
    TEST_EQ(2.f, get_avx2_f32(v, 6));
    TEST_EQ(3.f, get_avx2_f32(v, 5));
    TEST_EQ(4.f, get_avx2_f32(v, 4));
    TEST_EQ(5.f, get_avx2_f32(v, 3));
    TEST_EQ(6.f, get_avx2_f32(v, 2));
    TEST_EQ(7.f, get_avx2_f32(v, 1));
    TEST_EQ(8.f, get_avx2_f32(v, 0));

    run("v8 1 2 3 4 5 6 7 8 begin dup 10 < while 1 + repeat");
    v = get_last_stack_value();
    TEST_EQ(10.f, get_avx2_f32(v, 7));
    TEST_EQ(11.f, get_avx2_f32(v, 6));
    TEST_EQ(12.f, get_avx2_f32(v, 5));
    TEST_EQ(13.f, get_avx2_f32(v, 4));
    TEST_EQ(14.f, get_avx2_f32(v, 3));
    TEST_EQ(15.f, get_avx2_f32(v, 2));
    TEST_EQ(16.f, get_avx2_f32(v, 1));
    TEST_EQ(17.f, get_avx2_f32(v, 0));

    run("v8 1 2 3 4 5 6 7 8 begin dup 10 < while 2 begin dup 4 < while 1 + repeat + repeat");
    v = get_last_stack_value();
    TEST_EQ(13.f, get_avx2_f32(v, 7));
    TEST_EQ(14.f, get_avx2_f32(v, 6));
    TEST_EQ(15.f, get_avx2_f32(v, 5));
    TEST_EQ(16.f, get_avx2_f32(v, 4));
    TEST_EQ(17.f, get_avx2_f32(v, 3));
    TEST_EQ(18.f, get_avx2_f32(v, 2));
    TEST_EQ(19.f, get_avx2_f32(v, 1));
    TEST_EQ(20.f, get_avx2_f32(v, 0));
    }
#endif
  };

struct data_space_tests : public compile_fixture
  {
#ifdef AVX512
  void test()
    {
    }
#else
  void test()
    {
    run("here");
    auto v = get_last_stack_value_i();
    uint64_t here_pointer = get_avx2_u64(v, 0);
    uint64_t ctxt_here_pointer = (uint64_t)(void*)ctxt.here_pointer;
    TEST_EQ(here_pointer, ctxt_here_pointer);
    uint64_t ctxt_data_space_pointer = (uint64_t)(void*)ctxt.here_pointer+32;
    uint64_t here_pointer_content = *(uint64_t*)ctxt.here_pointer;
    TEST_EQ(here_pointer_content, ctxt_data_space_pointer);

    run("#32 here #+!");
    here_pointer_content = *(uint64_t*)ctxt.here_pointer;
    TEST_EQ(here_pointer_content, ctxt_data_space_pointer+32);

    run("#32 here #-!");
    here_pointer_content = *(uint64_t*)ctxt.here_pointer;
    TEST_EQ(here_pointer_content, ctxt_data_space_pointer);

    run("#96 allot");
    v = get_last_stack_value_i();
    uint64_t allot_address = get_avx2_u64(v, 0);
    TEST_EQ(allot_address, ctxt_data_space_pointer);
    here_pointer_content = *(uint64_t*)ctxt.here_pointer;
    TEST_EQ(here_pointer_content, ctxt_data_space_pointer + 96);

    run("#2 cells allot");
    v = get_last_stack_value_i();
    allot_address = get_avx2_u64(v, 0);
    TEST_EQ(allot_address, ctxt_data_space_pointer + 96);
    here_pointer_content = *(uint64_t*)ctxt.here_pointer;
    TEST_EQ(here_pointer_content, ctxt_data_space_pointer + 96 + 64);

    run("101 ,");
    here_pointer_content = *(uint64_t*)ctxt.here_pointer;
    TEST_EQ(here_pointer_content, ctxt_data_space_pointer + 96 + 96);
    void* ptr = (void*)here_pointer_content; // get the address in the here pointer
    ptr = ((float*)ptr) - 8; // go back one cell
    __m256 value = _mm256_load_ps((float*)ptr); // load the value
    TEST_EQ(101.f, get_avx2_f32(value, 0)); // it should be 101
    TEST_EQ(101.f, get_avx2_f32(value, 1));
    TEST_EQ(101.f, get_avx2_f32(value, 2));
    TEST_EQ(101.f, get_avx2_f32(value, 3));
    TEST_EQ(101.f, get_avx2_f32(value, 4));
    TEST_EQ(101.f, get_avx2_f32(value, 5));
    TEST_EQ(101.f, get_avx2_f32(value, 6));
    TEST_EQ(101.f, get_avx2_f32(value, 7));

    run("here @ create a");
    TEST_EQ(cd.binding_space_offset, 32);
    uint64_t a_val = *(uint64_t*)ctxt.binding_space_pointer;
    here_pointer_content = *(uint64_t*)(ctxt.here_pointer);
    TEST_EQ(a_val, here_pointer_content);

    run("here @ create a");
    TEST_EQ(cd.binding_space_offset, 32);
    uint64_t a_val_2 = *(uint64_t*)ctxt.binding_space_pointer;
    here_pointer_content = *(uint64_t*)(ctxt.here_pointer);
    TEST_EQ(a_val_2, here_pointer_content);

    run("1000 a !");

    run("1 2 3 a @");
    auto f = get_last_stack_value();
    TEST_EQ(1000.f, get_avx2_f32(f, 0));

    
    run("variable x 999 x ! variable y -999 y ! x @ y @");
    f = get_stack_value(0);
    TEST_EQ(-999.f, get_avx2_f32(f, 0));
    f = get_stack_value(1);
    TEST_EQ(999.f, get_avx2_f32(f, 0));

    run("20 , 30 , 40 , x @ y @");    
    f = get_stack_value(1);
    TEST_EQ(999.f, get_avx2_f32(f, 0));
    f = get_stack_value(0);
    TEST_EQ(-999.f, get_avx2_f32(f, 0));
    
    for (int i = 0; i < 64; ++i) // only 32 variables possible, since constant space equals 1024 in this test, but reusing a variable name will also reuse its memory
      {
      run("20 value val");
      run("val");
      f = get_stack_value(0);
      TEST_EQ(20.f, get_avx2_f32(f, 0));
      }

    run("x @");
    f = get_stack_value(0);
    TEST_EQ(999.f, get_avx2_f32(f, 0));

    run("30 to val");
    run("val");
    f = get_stack_value(0);
    TEST_EQ(30.f, get_avx2_f32(f, 0));
    }
#endif
  };

struct vec3_tests : public compile_fixture
  {
#ifdef AVX512
  void test()
    {
    }
#else
  void test()
    {
    run("vec3 v 1 2 3 v vec3!");    
    auto dsv0 = get_data_space_value(0);
    auto dsv1 = get_data_space_value(1);
    auto dsv2 = get_data_space_value(2);
    TEST_EQ(1.f, get_avx2_f32(dsv0, 0));
    TEST_EQ(2.f, get_avx2_f32(dsv1, 0));
    TEST_EQ(3.f, get_avx2_f32(dsv2, 0));

    run("vec3 w 7 8 9 w vec3!");
    auto dsv3 = get_data_space_value(3);
    auto dsv4 = get_data_space_value(4);
    auto dsv5 = get_data_space_value(5);
    TEST_EQ(7.f, get_avx2_f32(dsv3, 0));
    TEST_EQ(8.f, get_avx2_f32(dsv4, 0));
    TEST_EQ(9.f, get_avx2_f32(dsv5, 0));

    run("v w dot3");
    auto f = get_stack_value(0);
    TEST_EQ(50.f, get_avx2_f32(f, 0));

    run("vec3 result");

    run("v w result cross3");
    auto dsv6 = get_data_space_value(6);
    auto dsv7 = get_data_space_value(7);
    auto dsv8 = get_data_space_value(8);
    TEST_EQ(-6.f, get_avx2_f32(dsv6, 0));
    TEST_EQ(12.f, get_avx2_f32(dsv7, 0));
    TEST_EQ(-6.f, get_avx2_f32(dsv8, 0));

    run("v w result add3");
    dsv6 = get_data_space_value(6);
    dsv7 = get_data_space_value(7);
    dsv8 = get_data_space_value(8);
    TEST_EQ(8.f, get_avx2_f32(dsv6, 0));
    TEST_EQ(10.f, get_avx2_f32(dsv7, 0));
    TEST_EQ(12.f, get_avx2_f32(dsv8, 0));

    run("3 11 41 v vec3! v w result sub3");
    dsv6 = get_data_space_value(6);
    dsv7 = get_data_space_value(7);
    dsv8 = get_data_space_value(8);
    TEST_EQ(-4.f, get_avx2_f32(dsv6, 0));
    TEST_EQ(3.f, get_avx2_f32(dsv7, 0));
    TEST_EQ(32.f, get_avx2_f32(dsv8, 0));

    run("100 w result scalarmul3");
    dsv6 = get_data_space_value(6);
    dsv7 = get_data_space_value(7);
    dsv8 = get_data_space_value(8);
    TEST_EQ(700.f, get_avx2_f32(dsv6, 0));
    TEST_EQ(800.f, get_avx2_f32(dsv7, 0));
    TEST_EQ(900.f, get_avx2_f32(dsv8, 0));

    run("w length3");
    f = get_stack_value(0);
    TEST_EQ(std::sqrt(49.f+64.f+81.f), get_avx2_f32(f, 0));

    run("w result normalize3");
    dsv6 = get_data_space_value(6);
    dsv7 = get_data_space_value(7);
    dsv8 = get_data_space_value(8);
    TEST_EQ(7.f/ std::sqrt(49.f + 64.f + 81.f), get_avx2_f32(dsv6, 0));
    TEST_EQ(8.f/ std::sqrt(49.f + 64.f + 81.f), get_avx2_f32(dsv7, 0));
    TEST_EQ(9.f/ std::sqrt(49.f + 64.f + 81.f), get_avx2_f32(dsv8, 0));

    //print_stack(std::cout, ctxt);
    //print_data_space(std::cout, ctxt);
    }
#endif
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
  begin_while_repeat_tests().test();
  data_space_tests().test();
  vec3_tests().test();
  
  }