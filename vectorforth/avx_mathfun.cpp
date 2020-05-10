#include "avx_mathfun.h"

#define INSTRSET 8

#include <vectorclass.h>
#include <vectormath_exp.h>
#include <vectormath_trig.h>

VF_BEGIN

__m256 exp256_ps(__m256 x)
  {
  Vec8f vx(x);
  Vec8f p = exp(vx);
  return p.operator __m256();
  }

__m256 log256_ps(__m256 x)
  {
  Vec8f vx(x);
  Vec8f p = log(vx);
  return p.operator __m256();
  }

__m256 pow256_ps(__m256 x, __m256 y)
  {
  Vec8f vx(x), vy(y);
  Vec8f p = pow(vx, vy);
  return p.operator __m256();
  }

__m256 tan256_ps(__m256 x)
  {
  Vec8f vx(x);
  Vec8f p = tan(vx);
  return p.operator __m256();
  }

__m256 atan2_256_ps(__m256 y, __m256 x)
  {
  Vec8f vx(x), vy(y);
  Vec8f p = atan2(vy, vx);
  return p.operator __m256();
  }

__m256 sin256_ps(__m256 x)
  {
  Vec8f vx(x);
  Vec8f p = sin(vx);
  return p.operator __m256();
  }

__m256 cos256_ps(__m256 x)
  {
  Vec8f vx(x);
  Vec8f p = cos(vx);
  return p.operator __m256();
  }

VF_END