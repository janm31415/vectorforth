#pragma once

#include "namespace.h"
#include "vectorforth_api.h"
#include "avx_mathfun.h"

#include <immintrin.h>

VF_BEGIN

VECTOR_FORTH_API void initialize_lookup();

#ifdef AVX512
VECTOR_FORTH_API __m512 _VECTORCALL cos_avx_ps_joris(__m512 x);
VECTOR_FORTH_API __m512 _VECTORCALL sin_avx_ps_joris(__m512 x);
VECTOR_FORTH_API __m512 _VECTORCALL sin_avx_ps_lookup(__m512 x);
VECTOR_FORTH_API __m512 _VECTORCALL cos_avx_ps_lookup(__m512 x);
#else
VECTOR_FORTH_API __m256 _VECTORCALL sin_avx_ps_lookup(__m256 x);
VECTOR_FORTH_API __m256 _VECTORCALL cos_avx_ps_lookup(__m256 x);
VECTOR_FORTH_API __m256 _VECTORCALL sin_avx_ps_joris(__m256 x);
VECTOR_FORTH_API __m256 _VECTORCALL cos_avx_ps_joris(__m256 x);
#endif

VF_END