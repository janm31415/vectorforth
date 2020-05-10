#pragma once

#include "namespace.h"

#include <immintrin.h>

#ifdef _WIN32
#define USE_VECTORCALL
#endif

#ifdef USE_VECTORCALL

#define _VECTORCALL __vectorcall

#else

#define _VECTORCALL

#endif


VF_BEGIN

__m256 _VECTORCALL exp256_ps(__m256 x);
__m256 _VECTORCALL log256_ps(__m256 x);
__m256 _VECTORCALL sin256_ps(__m256 x);
__m256 _VECTORCALL cos256_ps(__m256 x);
__m256 _VECTORCALL tan256_ps(__m256 x);
__m256 _VECTORCALL pow256_ps(__m256 x, __m256 y);
__m256 _VECTORCALL atan2_256_ps(__m256 y, __m256 x);

VF_END