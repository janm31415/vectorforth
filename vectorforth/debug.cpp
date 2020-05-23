#include "debug.h"

#include <immintrin.h>

VF_BEGIN

namespace
  {
#ifdef AVX512
  inline float get_avx2_f32(__m512 v, int i)
    {
#ifdef _WIN32
    return v.m512_f32[i];
#else
    return v[i];
#endif
    }

#else
  inline float get_avx2_f32(__m256 v, int i)
    {
#ifdef _WIN32
    return v.m256_f32[i];
#else
    return v[i];
#endif
    }
#endif
  }

void print_stack(std::ostream& os, const context& ctxt)
  {
  os << "stack dump\n";
  os << "----------\n";
  if (ctxt.stack_pointer == ctxt.aligned_stack_top)
    {
    os << "empty stack\n";
    }
#ifdef AVX512
  char* ptr = ctxt.aligned_stack_top - 64;
#else
  char* ptr = ctxt.aligned_stack_top - 32;
#endif
  int cnt = 0;
  while (ptr >= ctxt.stack_pointer)
    {
#ifdef AVX512
    __m512 value = _mm512_load_ps((float*)ptr);
#else
    __m256 value = _mm256_load_ps((float*)ptr);
#endif

    os << "[" << cnt++ << "] ";
#ifdef AVX512
    for (int i = 0; i < 16; ++i)
#else
    for (int i = 0; i < 8; ++i)
#endif
      {
      os << get_avx2_f32(value, i) << " ";
      }
    os << "\n";
#ifdef AVX512
    ptr -= 64;
#else
    ptr -= 32;
#endif
    }
  os << "\n";
  }

void print_data_space(std::ostream& os, const context& ctxt)
  {
  os << "data space dump\n";
  os << "---------------\n";
#ifdef AVX512
  char* data_space_pointer_start = ctxt.here_pointer + 64;
#else
  char* data_space_pointer_start = ctxt.here_pointer + 32;
#endif
  uint64_t data_space_pointer_end_address = *((uint64_t*)ctxt.here_pointer);
  char* data_space_pointer_end = (char*)((void*)data_space_pointer_end_address);
  char* ptr = data_space_pointer_start;
  if (ptr >= data_space_pointer_end)
    {
    os << "empty data space\n";
    }
  int cnt = 0;
  while (ptr < data_space_pointer_end)
    {
#ifdef AVX512
    __m512 value = _mm512_load_ps((float*)ptr);
#else
    __m256 value = _mm256_load_ps((float*)ptr);
#endif

    os << "[" << cnt++ << "] ";
#ifdef AVX512
    for (int i = 0; i < 16; ++i)
#else
    for (int i = 0; i < 8; ++i)
#endif
      {
      os << get_avx2_f32(value, i) << " ";
      }
    os << "\n";

#ifdef AVX512
    ptr += 64;
#else
    ptr += 32;
#endif
    }
  os << "\n";
  }

VF_END