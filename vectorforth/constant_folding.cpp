#include "constant_folding.h"
#include "primitives.h"
#include "context_defs.h"

VF_BEGIN

namespace
  {

  struct add_op
    {
    float operator () (float a, float b)
      {
      return a + b;
      }
    };

  struct sub_op
    {
    float operator () (float a, float b)
      {
      return a - b;
      }
    };

  struct mul_op
    {
    float operator () (float a, float b)
      {
      return a * b;
      }
    };

  struct div_op
    {
    float operator () (float a, float b)
      {
      return a / b;
      }
    };

  struct atan2_op
    {
    float operator () (float a, float b)
      {
      return std::atan2(a, b);
      }
    };

  struct pow_op
    {
    float operator () (float a, float b)
      {
      return std::pow(a, b);
      }
    };

  struct min_op
    {
    float operator () (float a, float b)
      {
      return a < b ? a : b;
      }
    };

  struct max_op
    {
    float operator () (float a, float b)
      {
      return a > b ? a : b;
      }
    };

  struct and_op
    {
    float operator () (float a, float b)
      {
      uint32_t aa = *reinterpret_cast<uint32_t*>(&a);
      uint32_t bb = *reinterpret_cast<uint32_t*>(&b);
      uint32_t res = aa & bb;
      return *reinterpret_cast<float*>(&res);
      }
    };

  struct or_op
    {
    float operator () (float a, float b)
      {
      uint32_t aa = *reinterpret_cast<uint32_t*>(&a);
      uint32_t bb = *reinterpret_cast<uint32_t*>(&b);
      uint32_t res = aa | bb;
      return *reinterpret_cast<float*>(&res);
      }
    };

  struct xor_op
    {
    float operator () (float a, float b)
      {
      uint32_t aa = *reinterpret_cast<uint32_t*>(&a);
      uint32_t bb = *reinterpret_cast<uint32_t*>(&b);
      uint32_t res = aa ^ bb;
      return *reinterpret_cast<float*>(&res);
      }
    };

  struct not_op
    {
    float operator () (float a)
      {
      uint32_t aa = *reinterpret_cast<uint32_t*>(&a);      
      uint32_t res = ~aa;
      return *reinterpret_cast<float*>(&res);
      }
    };

  struct sin_op
    {
    float operator () (float v)
      {
      return std::sin(v);
      }
    };

  struct cos_op
    {
    float operator () (float v)
      {
      return std::cos(v);
      }
    };

  struct tan_op
    {
    float operator () (float v)
      {
      return std::tan(v);
      }
    };

  struct log_op
    {
    float operator () (float v)
      {
      return std::log(v);
      }
    };

  struct exp_op
    {
    float operator () (float v)
      {
      return std::exp(v);
      }
    };

  struct sqrt_op
    {
    float operator () (float v)
      {
      return std::sqrt(v);
      }
    };

  struct abs_op
    {
    float operator () (float v)
      {
      return std::abs(v);
      }
    };

  struct negate_op
    {
    float operator () (float v)
      {
      return -v;
      }
    };

  struct floor_op
    {
    float operator () (float v)
      {
      return std::floor(v);
      }
    };

  struct ceil_op
    {
    float operator () (float v)
      {
      return std::ceil(v);
      }
    };

  struct round_op
    {
    float operator () (float v)
      {
      return std::round(v);
      }
    };

  struct trunc_op
    {
    float operator () (float v)
      {
      return std::trunc(v);
      }
    };

  template <class oper>
  std::vector<expanded_token>::iterator apply_2_floats(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    oper op;
    auto sz = std::distance(words.begin(), it);
    if (sz >= 2)
      {
      auto it1 = it - 1;
      auto it2 = it - 2;
      if (it1->t == expanded_token::ET_FLOAT && it2->t == expanded_token::ET_FLOAT)
        {
        it2->f[0] = op(it2->f[0], it1->f[0]);
        words.erase(it1, it + 1);
        return it2;
        }
      if (it1->t == expanded_token::ET_FLOAT && it2->t == expanded_token::ET_VECTOR)
        {
        for (int i = 0; i < AVX_LENGTH; ++i)
          it2->f[i] = op(it2->f[i], it1->f[0]);
        words.erase(it1, it + 1);
        return it2;
        }
      if (it1->t == expanded_token::ET_VECTOR && it2->t == expanded_token::ET_FLOAT)
        {
        for (int i = 0; i < AVX_LENGTH; ++i)
          it2->f[i] = op(it2->f[0], it1->f[i]);
        it2->t = expanded_token::ET_VECTOR;
        words.erase(it1, it + 1);
        return it2;
        }
      if (it1->t == expanded_token::ET_VECTOR && it2->t == expanded_token::ET_VECTOR)
        {
        for (int i = 0; i < AVX_LENGTH; ++i)
          it2->f[i] = op(it2->f[i], it1->f[i]);
        words.erase(it1, it + 1);
        return it2;
        }
      }
    return it;
    }

  template <class oper>
  std::vector<expanded_token>::iterator apply_1_float(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    oper op;
    auto sz = std::distance(words.begin(), it);
    if (sz >= 1)
      {
      auto it1 = it - 1;
      if (it1->t == expanded_token::ET_FLOAT)
        {
        it1->f[0] = op(it1->f[0]);
        words.erase(it);
        return it1;
        }
      if (it1->t == expanded_token::ET_VECTOR)
        {
        for (int i = 0; i < AVX_LENGTH; ++i)
          it1->f[i] = op(it1->f[i]);
        words.erase(it);
        return it1;
        }      
      }
    return it;
    }

  }

void constant_folding(std::vector<expanded_token>& words)
  {
  auto it = words.begin();
  while (it != words.end())
    {
    if (it->t == expanded_token::ET_PRIMITIVE)
      {
      if (it->prim == &primitive_add)
        it = apply_2_floats<add_op>(words, it);
      else if (it->prim == &primitive_sub)
        it = apply_2_floats<sub_op>(words, it);
      else if (it->prim == &primitive_mul)
        it = apply_2_floats<mul_op>(words, it);
      else if (it->prim == &primitive_div)
        it = apply_2_floats<div_op>(words, it);
      else if (it->prim == &primitive_atan2)
        it = apply_2_floats<atan2_op>(words, it);
      else if (it->prim == &primitive_pow)
        it = apply_2_floats<pow_op>(words, it);
      else if (it->prim == &primitive_min)
        it = apply_2_floats<min_op>(words, it);
      else if (it->prim == &primitive_max)
        it = apply_2_floats<max_op>(words, it);
      else if (it->prim == &primitive_and)
        it = apply_2_floats<and_op>(words, it);
      else if (it->prim == &primitive_or)
        it = apply_2_floats<or_op>(words, it);
      else if (it->prim == &primitive_xor)
        it = apply_2_floats<xor_op>(words, it);
      else if (it->prim == &primitive_sin)
        it = apply_1_float<sin_op>(words, it);
      else if (it->prim == &primitive_cos)
        it = apply_1_float<cos_op>(words, it);
      else if (it->prim == &primitive_tan)
        it = apply_1_float<tan_op>(words, it);
      else if (it->prim == &primitive_log)
        it = apply_1_float<log_op>(words, it);
      else if (it->prim == &primitive_exp)
        it = apply_1_float<exp_op>(words, it);
      else if (it->prim == &primitive_sqrt)
        it = apply_1_float<sqrt_op>(words, it);
      else if (it->prim == &primitive_abs)
        it = apply_1_float<abs_op>(words, it);
      else if (it->prim == &primitive_negate)
        it = apply_1_float<negate_op>(words, it);
      else if (it->prim == &primitive_floor)
        it = apply_1_float<floor_op>(words, it);
      else if (it->prim == &primitive_ceil)
        it = apply_1_float<ceil_op>(words, it);
      else if (it->prim == &primitive_round)
        it = apply_1_float<round_op>(words, it);
      else if (it->prim == &primitive_truncate)
        it = apply_1_float<trunc_op>(words, it);
      else if (it->prim == &primitive_not)
        it = apply_1_float<not_op>(words, it);
      }
    ++it;
    }
  }

VF_END