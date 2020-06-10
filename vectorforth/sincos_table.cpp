#include "sincos_table.h"

#include <cmath>
#include <stdint.h>

#define PI    3.1415926535897f
#define TWO_PI (2.0 * 3.1415926535897)
#define HLF_PI (3.1415926535897f/2.f)

VF_BEGIN

// Integer representations of PI
#define TWO_PI_INT        8192//16384
#define PI_INT            TWO_PI_INT / 2
#define HLF_PI_INT        TWO_PI_INT / 4

// Constants for sine/cosine lookup table
#define NR_SAMPLES        TWO_PI_INT

// Lookup table
#ifdef _WIN32
__declspec(align(64)) float lookup[NR_SAMPLES];
#else
float lookup[NR_SAMPLES] __attribute__((aligned(64)));
#endif

void initialize_lookup()
  {
  for (unsigned i = 0; i < NR_SAMPLES; i++) {
    lookup[i] = (float)std::sin((double)i * (TWO_PI / (double)TWO_PI_INT));
    }
  }

#ifdef AVX512

namespace
  {
  __m512  f1 = _mm512_set1_ps((float)((double)TWO_PI_INT / (double)TWO_PI));
  __m512i u0 = _mm512_set1_epi32(HLF_PI_INT);      // constant 0.5 * pi
  __m512i u1 = _mm512_set1_epi32(TWO_PI_INT - 1);  // mask 2 * pi
  }

__m512 _VECTORCALL sin_avx_ps_lookup(__m512 f0)
  {  
  __m512  f2 = _mm512_mul_ps(f0, f1);              // apply scale
  __m512i u2 = _mm512_cvtps_epi32(f2);             // round float to int
  __m512i u5 = _mm512_and_si512(u1, u2);           // apply mask of 2 * pi, first index  
  __m512  f4 = _mm512_i32gather_ps(u5, lookup, 4); // perform lookup of imag (sin)
  return f4;
  }

__m512 _VECTORCALL cos_avx_ps_lookup(__m512 f0)
  {  
  __m512  f2 = _mm512_mul_ps(f0, f1);              // apply scale
  __m512i u2 = _mm512_cvtps_epi32(f2);             // round float to int
  __m512i u3 = _mm512_add_epi32(u2, u0);           // add 0.5 * pi
  __m512i u4 = _mm512_and_si512(u1, u3);           // apply mask of 2 * pi, second index  
  __m512  f3 = _mm512_i32gather_ps(u4, lookup, 4); // perform lookup of real (cos)  
  return f3;
  }

__m512 _VECTORCALL sin_avx_ps_joris(__m512 x)
  {
  __m512 one_over_twopi = _mm512_set1_ps(1.f/(2.f*PI));

  __m512 s = _mm512_set1_ps(2.f);

  __m512 y = _mm512_mul_ps(x, one_over_twopi);
  __m512 k = _mm512_roundscale_ps(y, _MM_FROUND_TO_NEAREST_INT);
  __m512 f = _mm512_mul_ps(_mm512_sub_ps(y, k), s);
  

  //sin(pi*x) = x * (a2 * x * x + a0) / (b2 * x * x + b0)
  __m512 a0 = _mm512_set1_ps(3.21670175718844e+000);
  __m512 a2 = _mm512_set1_ps(-3.23395890594424e+000);
  __m512 b0 = _mm512_set1_ps(1.00000000000000e+000);
  __m512 b2 = _mm512_set1_ps(8.53391292646496e-001);

  __m512 ff = _mm512_mul_ps(f, f);
  __m512 top = _mm512_mul_ps(f, _mm512_add_ps(_mm512_mul_ps(a2, ff), a0));
  __m512 denom = _mm512_add_ps(_mm512_mul_ps(b2, ff), b0);
  return _mm512_div_ps(top, denom);
  }

__m512 _VECTORCALL cos_avx_ps_joris(__m512 x)
  {
  __m512 xx = _mm512_sub_ps(_mm512_set1_ps(PI / 2.f), x);
  __m512 one_over_twopi = _mm512_set1_ps(1.f / (2.f*PI));

  __m512 s = _mm512_set1_ps(2.f);

  __m512 y = _mm512_mul_ps(xx, one_over_twopi);
  __m512 k = _mm512_roundscale_ps(y, _MM_FROUND_TO_NEAREST_INT);
  __m512 f = _mm512_mul_ps(_mm512_sub_ps(y, k), s);


  //sin(pi*x) = x * (a2 * x * x + a0) / (b2 * x * x + b0)
  __m512 a0 = _mm512_set1_ps(3.21670175718844e+000);
  __m512 a2 = _mm512_set1_ps(-3.23395890594424e+000);
  __m512 b0 = _mm512_set1_ps(1.00000000000000e+000);
  __m512 b2 = _mm512_set1_ps(8.53391292646496e-001);

  __m512 ff = _mm512_mul_ps(f, f);
  __m512 top = _mm512_mul_ps(f, _mm512_add_ps(_mm512_mul_ps(a2, ff), a0));
  __m512 denom = _mm512_add_ps(_mm512_mul_ps(b2, ff), b0);
  return _mm512_div_ps(top, denom);
  }

#else

namespace
  {
  __m256  f1 = _mm256_set1_ps((float)((double)TWO_PI_INT / (double)TWO_PI));
  __m256i u0 = _mm256_set1_epi32(HLF_PI_INT);      // constant 0.5 * pi
  __m256i u1 = _mm256_set1_epi32(TWO_PI_INT - 1);  // mask 2 * pi
  }

__m256 _VECTORCALL sin_avx_ps_lookup(__m256 f0)
  {  
  __m256  f2 = _mm256_mul_ps(f0, f1);              // apply scale  
  __m256i u2 = _mm256_cvtps_epi32(f2);             // round float to int 
  __m256i u5 = _mm256_and_si256(u1, u2);           // apply mask of 2 * pi, first index  
  __m256  f4 = _mm256_i32gather_ps(lookup, u5, 4); // perform lookup of imag (sin)
  return f4;
  }

__m256 _VECTORCALL cos_avx_ps_lookup(__m256 f0)
  {  
  __m256  f2 = _mm256_mul_ps(f0, f1);              // apply scale
  __m256i u2 = _mm256_cvtps_epi32(f2);             // round float to int
  __m256i u3 = _mm256_add_epi32(u2, u0);           // add 0.5 * pi
  __m256i u4 = _mm256_and_si256(u1, u3);           // apply mask of 2 * pi, second index  
  __m256  f3 = _mm256_i32gather_ps(lookup, u4, 4); // perform lookup of real (cos)  
  return f3;
  }


__m256 _VECTORCALL sin_avx_ps_joris(__m256 x)
  {
  __m256 one_over_twopi = _mm256_set1_ps(1.f / (2.f*PI));

  __m256 s = _mm256_set1_ps(2.f);

  __m256 y = _mm256_mul_ps(x, one_over_twopi);
  __m256 k = _mm256_round_ps(y, _MM_FROUND_TO_NEAREST_INT);
  __m256 f = _mm256_mul_ps(_mm256_sub_ps(y, k), s);


  //sin(pi*x) = x * (a2 * x * x + a0) / (b2 * x * x + b0)
  __m256 a0 = _mm256_set1_ps(3.21670175718844e+000);
  __m256 a2 = _mm256_set1_ps(-3.23395890594424e+000);
  __m256 b0 = _mm256_set1_ps(1.00000000000000e+000);
  __m256 b2 = _mm256_set1_ps(8.53391292646496e-001);

  __m256 ff = _mm256_mul_ps(f, f);
  __m256 top = _mm256_mul_ps(f, _mm256_add_ps(_mm256_mul_ps(a2, ff), a0));
  __m256 denom = _mm256_add_ps(_mm256_mul_ps(b2, ff), b0);
  return _mm256_div_ps(top, denom);
  }

__m256 _VECTORCALL cos_avx_ps_joris(__m256 x)
  {
  __m256 xx = _mm256_sub_ps(_mm256_set1_ps(PI / 2.f), x);
  __m256 one_over_twopi = _mm256_set1_ps(1.f / (2.f*PI));

  __m256 s = _mm256_set1_ps(2.f);

  __m256 y = _mm256_mul_ps(xx, one_over_twopi);
  __m256 k = _mm256_round_ps(y, _MM_FROUND_TO_NEAREST_INT);
  __m256 f = _mm256_mul_ps(_mm256_sub_ps(y, k), s);


  //sin(pi*x) = x * (a2 * x * x + a0) / (b2 * x * x + b0)
  __m256 a0 = _mm256_set1_ps(3.21670175718844e+000);
  __m256 a2 = _mm256_set1_ps(-3.23395890594424e+000);
  __m256 b0 = _mm256_set1_ps(1.00000000000000e+000);
  __m256 b2 = _mm256_set1_ps(8.53391292646496e-001);

  __m256 ff = _mm256_mul_ps(f, f);
  __m256 top = _mm256_mul_ps(f, _mm256_add_ps(_mm256_mul_ps(a2, ff), a0));
  __m256 denom = _mm256_add_ps(_mm256_mul_ps(b2, ff), b0);
  return _mm256_div_ps(top, denom);
  }
  
#endif

VF_END