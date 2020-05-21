#pragma once

#include "namespace.h"

#include <immintrin.h>

#ifdef _WIN32
#define _VECTORCALL __vectorcall
#else
#define _VECTORCALL
#endif

VF_BEGIN

#ifdef AVX512
__m512 _VECTORCALL exp_avx_ps(__m512 x);
__m512 _VECTORCALL log_avx_ps(__m512 x);
__m512 _VECTORCALL sin_avx_ps(__m512 x);
__m512 _VECTORCALL cos_avx_ps(__m512 x);
__m512 _VECTORCALL tan_avx_ps(__m512 x);
__m512 _VECTORCALL pos_avx_ps(__m512 x, __m512 y);
__m512 _VECTORCALL atan2_avx_ps(__m512 y, __m2512 x);
#else
__m256 _VECTORCALL exp_avx_ps(__m256 x);
__m256 _VECTORCALL log_avx_ps(__m256 x);
__m256 _VECTORCALL sin_avx_ps(__m256 x);
__m256 _VECTORCALL cos_avx_ps(__m256 x);
__m256 _VECTORCALL tan_avx_ps(__m256 x);
__m256 _VECTORCALL pos_avx_ps(__m256 x, __m256 y);
__m256 _VECTORCALL atan2_avx_ps(__m256 y, __m256 x);
#endif

VF_END