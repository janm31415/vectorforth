#include "avx_mathfun.h"

#ifdef AVX512
#define INSTRSET 9
#else
#define INSTRSET 8
#endif

#include <vectorclass.h>
#include <vectormath_exp.h>
#include <vectormath_trig.h>

#include <iostream>

#include "context_defs.h"

//#define TEST_FOR_NAN

VF_BEGIN

#ifdef AVX512

__m512 _VECTORCALL exp_avx_ps(__m512 x)
  {
  Vec16f vx(x);
  Vec16f p = exp(vx);
#ifdef TEST_FOR_NAN
  for (int i = 0; i < AVX_LENGTH; ++i)
    {
    if (p[i] != p[i])
      printf("exp_avx_ps: nan from %f\n", vx[i]);
    }
#endif
  return p.operator __m512();
  }

__m512 _VECTORCALL log_avx_ps(__m512 x)
  {
  Vec16f vx(x);
  Vec16f p = log(vx);
#ifdef TEST_FOR_NAN
  for (int i = 0; i < AVX_LENGTH; ++i)
    {
    if (p[i] != p[i])
      printf("log_avx_ps: nan from %f\n", vx[i]);
    }
#endif
  return p.operator __m512();
  }

__m512 _VECTORCALL pow_avx_ps(__m512 x, __m512 y)
  {
  Vec16f vx(x), vy(y);
  Vec16f p = pow(vx, vy);
#ifdef TEST_FOR_NAN
  for (int i = 0; i < AVX_LENGTH; ++i)
    {
    if (p[i] != p[i])
      printf("pow_avx_ps: nan from %f\n", vx[i]);
    }
#endif
  return p.operator __m512();
  }

__m512 _VECTORCALL tan_avx_ps(__m512 x)
  {
  Vec16f vx(x);
  Vec16f p = tan(vx);
#ifdef TEST_FOR_NAN
  for (int i = 0; i < AVX_LENGTH; ++i)
    {
    if (p[i] != p[i])
      printf("tan_avx_ps: nan from %f\n", vx[i]);
    }
#endif
  return p.operator __m512();
  }

__m512 _VECTORCALL atan2_avx_ps(__m512 y, __m512 x)
  {
  Vec16f vx(x), vy(y);
  Vec16f p = atan2_janm(vy, vx); // Trick the VS2017 compiler, see atan2 implementation and look for comment [JanM]. I suspect there is an optimization bug in VS2017 related to AVX512.
#ifdef TEST_FOR_NAN
  for (int i = 0; i < AVX_LENGTH; ++i)
    {
    if (p[i] != p[i])
      printf("atan2_avx_ps: nan from %f\n", vx[i]);
    }
#endif
  return p.operator __m512();  
  }

__m512 _VECTORCALL sin_avx_ps(__m512 x)
  {
  Vec16f vx(x);
  Vec16f p = sin(vx);
#ifdef TEST_FOR_NAN
  for (int i = 0; i < AVX_LENGTH; ++i)
    {
    if (p[i] != p[i])
      printf("sin_avx_ps: nan from %f\n", vx[i]);
    }
#endif
  return p.operator __m512();
  }

__m512 _VECTORCALL cos_avx_ps(__m512 x)
  {
  Vec16f vx(x);
  Vec16f p = cos(vx);
#ifdef TEST_FOR_NAN
  for (int i = 0; i < AVX_LENGTH; ++i)
    {
    if (p[i] != p[i])
      printf("cos_avx_ps: nan from %f\n", vx[i]);
    }
#endif
  return p.operator __m512();
  }

#else

__m256 _VECTORCALL exp_avx_ps(__m256 x)
  {
  Vec8f vx(x);
  Vec8f p = exp(vx);
#ifdef TEST_FOR_NAN
  for (int i = 0; i < AVX_LENGTH; ++i)
    {
    if (p[i] != p[i])
      printf("exp_avx_ps: nan from %f\n", vx[i]);
    }
#endif
  return p.operator __m256();
  }

__m256 _VECTORCALL log_avx_ps(__m256 x)
  {
  Vec8f vx(x);
  Vec8f p = log(vx);
#ifdef TEST_FOR_NAN
  for (int i = 0; i < AVX_LENGTH; ++i)
    {
    if (p[i] != p[i])
      printf("log_avx_ps: nan from %f\n", vx[i]);
    }
#endif
  return p.operator __m256();
  }

__m256 _VECTORCALL pow_avx_ps(__m256 x, __m256 y)
  {
  Vec8f vx(x), vy(y);
  Vec8f p = pow(vx, vy);
#ifdef TEST_FOR_NAN
  for (int i = 0; i < AVX_LENGTH; ++i)
    {
    if (p[i] != p[i])
      printf("pow_avx_ps: nan from %f\n", vx[i]);
    }
#endif
  return p.operator __m256();
  }

__m256 _VECTORCALL tan_avx_ps(__m256 x)
  {
  Vec8f vx(x);
  Vec8f p = tan(vx);
#ifdef TEST_FOR_NAN
  for (int i = 0; i < AVX_LENGTH; ++i)
    {
    if (p[i] != p[i])
      printf("tan_avx_ps: nan from %f\n", vx[i]);
    }
#endif
  return p.operator __m256();
  }

__m256 _VECTORCALL atan2_avx_ps(__m256 y, __m256 x)
  {
  Vec8f vx(x), vy(y);
  Vec8f p = atan2(vy, vx);
#ifdef TEST_FOR_NAN
  for (int i = 0; i < AVX_LENGTH; ++i)
    {
    if (p[i] != p[i])
      printf("atan2_avx_ps: nan from %f\n", vx[i]);
    }
#endif
  return p.operator __m256();
  }

__m256 _VECTORCALL sin_avx_ps(__m256 x)
  {
  Vec8f vx(x);
  Vec8f p = sin(vx);
#ifdef TEST_FOR_NAN
  for (int i = 0; i < AVX_LENGTH; ++i)
    {
    if (p[i] != p[i])
      printf("sin_avx_ps: nan from %f\n", vx[i]);
    }
#endif
  return p.operator __m256();
  }

__m256 _VECTORCALL cos_avx_ps(__m256 x)
  {
  Vec8f vx(x);
  Vec8f p = cos(vx);
#ifdef TEST_FOR_NAN
  for (int i = 0; i < AVX_LENGTH; ++i)
    {
    if (p[i] != p[i])
      printf("cos_avx_ps: nan from %f\n", vx[i]);
    }
#endif
  return p.operator __m256();
  }

#endif

VF_END