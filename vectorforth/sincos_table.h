#pragma once

#include "namespace.h"
#include "vectorforth_api.h"
#include "avx_mathfun.h"

#include <immintrin.h>

VF_BEGIN

VECTOR_FORTH_API void initialize_lookup();

VECTOR_FORTH_API __m256 _VECTORCALL sin_avx_ps_lookup(__m256 x);
VECTOR_FORTH_API __m256 _VECTORCALL cos_avx_ps_lookup(__m256 x);
VECTOR_FORTH_API __m256 _VECTORCALL tan_avx_ps_lookup(__m256 x);

VF_END