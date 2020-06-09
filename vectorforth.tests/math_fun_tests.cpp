#include "math_fun_tests.h"
#include <vectorforth/sincos_table.h>
#include "test_assert.h"

#include <cmath>

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

  __m512 set_float(float f)
    {
    return _mm512_set1_ps(f);
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

  __m256 set_float(float f)
    {
    return _mm256_set1_ps(f);
    }

#endif
  }

void test_sin_lookup(float value, float tol = 1e-3f)
  {
  float res = get_avx2_f32(sin_avx_ps_lookup(set_float(value)), 0);
  float expected = std::sin(value);
  TEST_EQ_CLOSE(expected, res, tol);
  }

void test_sin_joris(float value, float tol = 1e-3f)
  {
  float res = get_avx2_f32(sin_avx_ps_joris(set_float(value)), 0);
  float expected = std::sin(value);
  TEST_EQ_CLOSE(expected, res, tol);
  }

void test_cos_lookup(float value, float tol = 1e-3f)
  {
  float res = get_avx2_f32(cos_avx_ps_lookup(set_float(value)), 0);
  float expected = std::cos(value);
  TEST_EQ_CLOSE(expected, res, tol);
  }

void test_trigonometric_lookup()
  {
  float tol = 1e-3f;
  for (float f = 0.f; f < 3000.f; f += 0.2f)
    {
    test_sin_lookup(f, tol);
    test_sin_lookup(-f, tol);
    test_cos_lookup(f, tol);
    test_cos_lookup(-f, tol);
    test_sin_joris(f, 0.01);
    test_sin_joris(-f, 0.01);
    }
  }

VF_END

void run_all_math_fun_tests()
  {
  using namespace VF;
  initialize_lookup();
  test_trigonometric_lookup();
  }