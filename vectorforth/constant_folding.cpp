#include "constant_folding.h"
#include "primitives.h"
#include "context_defs.h"

VF_BEGIN

namespace
  {

  struct addi_op
    {
    int64_t operator () (int64_t a, int64_t b)
      {
      return a + b;
      }
    };

  struct subi_op
    {
    int64_t operator () (int64_t a, int64_t b)
      {
      return a - b;
      }
    };

  struct muli_op
    {
    int64_t operator () (int64_t a, int64_t b)
      {
      return a * b;
      }
    };

  struct divi_op
    {
    int64_t operator () (int64_t a, int64_t b)
      {
      return a / b;
      }
    };

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

  struct fequ_op
    {
    float operator () (float a, float b)
      {
      return a == b ? 1.f : 0.f;
      }
    };

  struct fnequ_op
    {
    float operator () (float a, float b)
      {
      return a != b ? 1.f : 0.f;
      }
    };

  struct flt_op
    {
    float operator () (float a, float b)
      {
      return a < b ? 1.f : 0.f;
      }
    };

  struct fgt_op
    {
    float operator () (float a, float b)
      {
      return a > b ? 1.f : 0.f;
      }
    };

  struct fle_op
    {
    float operator () (float a, float b)
      {
      return a <= b ? 1.f : 0.f;
      }
    };

  struct fge_op
    {
    float operator () (float a, float b)
      {
      return a >= b ? 1.f : 0.f;
      }
    };

  struct equ_op
    {
    float operator () (float a, float b)
      {
      uint32_t fa = 0xffffffff;
      float nan = *reinterpret_cast<float*>(&fa);
      return a == b ? nan : 0.f;
      }
    };

  struct nequ_op
    {
    float operator () (float a, float b)
      {
      uint32_t fa = 0xffffffff;
      float nan = *reinterpret_cast<float*>(&fa);
      return a != b ? nan : 0.f;
      }
    };

  struct lt_op
    {
    float operator () (float a, float b)
      {
      uint32_t fa = 0xffffffff;
      float nan = *reinterpret_cast<float*>(&fa);
      return a < b ? nan : 0.f;
      }
    };

  struct gt_op
    {
    float operator () (float a, float b)
      {
      uint32_t fa = 0xffffffff;
      float nan = *reinterpret_cast<float*>(&fa);
      return a > b ? nan : 0.f;
      }
    };

  struct le_op
    {
    float operator () (float a, float b)
      {
      uint32_t fa = 0xffffffff;
      float nan = *reinterpret_cast<float*>(&fa);
      return a <= b ? nan : 0.f;
      }
    };

  struct ge_op
    {
    float operator () (float a, float b)
      {
      uint32_t fa = 0xffffffff;
      float nan = *reinterpret_cast<float*>(&fa);
      return a >= b ? nan : 0.f;
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

  void convert_to_vector(std::vector<expanded_token>::iterator it)
    {
    if (it->t != expanded_token::ET_VECTOR)
      {
      for (int i = 1; i < AVX_LENGTH; ++i)
        it->f[i] = it->f[0];
      it->t = expanded_token::ET_VECTOR;
      }
    }

  template <class oper>
  std::vector<expanded_token>::iterator apply_2_floats(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it, bool vectorize = false)
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
        if (vectorize)
          convert_to_vector(it2);
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
        for (int i = 1; i < AVX_LENGTH; ++i)
          it2->f[i] = it2->f[0];
        it2->t = expanded_token::ET_VECTOR;
        for (int i = 0; i < AVX_LENGTH; ++i)
          it2->f[i] = op(it2->f[i], it1->f[i]);        
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
  std::vector<expanded_token>::iterator apply_2_integers(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    oper op;
    auto sz = std::distance(words.begin(), it);
    if (sz >= 2)
      {
      auto it1 = it - 1;
      auto it2 = it - 2;
      if (it1->t == expanded_token::ET_INTEGER && it2->t == expanded_token::ET_INTEGER)
        {
        it2->int_value = op(it2->int_value, it1->int_value);
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

  std::vector<expanded_token>::iterator apply_fcast(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(words.begin(), it);
    if (sz >= 1)
      {
      auto it1 = it - 1;
      if (it1->t == expanded_token::ET_VECTOR)
        {
        for (int i = 0; i < AVX_LENGTH; ++i)
          {
          int32_t v = *reinterpret_cast<int32_t*>(&it1->f[i]);
          it1->f[i] = static_cast<float>(v);
          }
        words.erase(it);
        return it1;
        }
      }
    return it;
    }

  std::vector<expanded_token>::iterator apply_icast(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(words.begin(), it);
    if (sz >= 1)
      {
      auto it1 = it - 1;
      if (it1->t == expanded_token::ET_VECTOR)
        {
        for (int i = 0; i < AVX_LENGTH; ++i)
          {
          int32_t v = static_cast<int32_t>(it1->f[i]);
          it1->f[i] = *reinterpret_cast<float*>(&v);
          }
        words.erase(it);
        return it1;
        }
      else if (it1->t == expanded_token::ET_FLOAT)
        {
        for (int i = 1; i < AVX_LENGTH; ++i)
          it1->f[i] = it1->f[0];
        it1->t = expanded_token::ET_VECTOR;
        for (int i = 0; i < AVX_LENGTH; ++i)
          {
          int32_t v = static_cast<int32_t>(it1->f[i]);
          it1->f[i] = *reinterpret_cast<float*>(&v);
          }
        words.erase(it);
        return it1;
        }
      }
    return it;
    }


  std::vector<expanded_token>::iterator apply_dup(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(words.begin(), it);
    if (sz >= 1)
      {
      auto it1 = it - 1;
      if (it1->t == expanded_token::ET_VECTOR || it1->t == expanded_token::ET_FLOAT || it1->t == expanded_token::ET_INTEGER)
        {
        *it = *it1;
        return it;
        }
      }
    return it;
    }

  std::vector<expanded_token>::iterator apply_twodup(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(words.begin(), it);
    if (sz >= 2)
      {
      auto it1 = it - 1;
      auto it2 = it - 2;
      if ((it1->t == expanded_token::ET_VECTOR || it1->t == expanded_token::ET_FLOAT || it1->t == expanded_token::ET_INTEGER) &&
        (it2->t == expanded_token::ET_VECTOR || it2->t == expanded_token::ET_FLOAT || it2->t == expanded_token::ET_INTEGER))
        {
        *it = *it1;
        it = words.insert(it, *it2);
        return ++it;
        }
      }
    return it;
    }

  std::vector<expanded_token>::iterator apply_swap(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(words.begin(), it);
    if (sz >= 2)
      {
      auto it1 = it - 1;
      auto it2 = it - 2;
      if ((it1->t == expanded_token::ET_VECTOR || it1->t == expanded_token::ET_FLOAT || it1->t == expanded_token::ET_INTEGER) &&
        (it2->t == expanded_token::ET_VECTOR || it2->t == expanded_token::ET_FLOAT || it2->t == expanded_token::ET_INTEGER))
        {
        std::swap(*it1, *it2);
        words.erase(it);
        return it1;
        }
      }
    return it;
    }

  std::vector<expanded_token>::iterator apply_twoswap(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(words.begin(), it);
    if (sz >= 4)
      {
      auto it1 = it - 1;
      auto it2 = it - 2;
      auto it3 = it - 3;
      auto it4 = it - 4;
      if ((it1->t == expanded_token::ET_VECTOR || it1->t == expanded_token::ET_FLOAT || it1->t == expanded_token::ET_INTEGER) &&
        (it2->t == expanded_token::ET_VECTOR || it2->t == expanded_token::ET_FLOAT || it2->t == expanded_token::ET_INTEGER) &&
        (it3->t == expanded_token::ET_VECTOR || it3->t == expanded_token::ET_FLOAT || it3->t == expanded_token::ET_INTEGER) &&
        (it4->t == expanded_token::ET_VECTOR || it4->t == expanded_token::ET_FLOAT || it4->t == expanded_token::ET_INTEGER))
        {
        std::swap(*it1, *it3);
        std::swap(*it2, *it4);
        words.erase(it);
        return it1;
        }
      }
    return it;
    }

  std::vector<expanded_token>::iterator apply_over(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(words.begin(), it);
    if (sz >= 2)
      {
      auto it1 = it - 1;
      auto it2 = it - 2;
      if ((it1->t == expanded_token::ET_VECTOR || it1->t == expanded_token::ET_FLOAT || it1->t == expanded_token::ET_INTEGER) &&
        (it2->t == expanded_token::ET_VECTOR || it2->t == expanded_token::ET_FLOAT || it2->t == expanded_token::ET_INTEGER))
        {
        *it = *it2;
        return it;
        }
      }
    return it;
    }

  std::vector<expanded_token>::iterator apply_rot(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it, bool positive)
    {
    auto sz = std::distance(words.begin(), it);
    if (sz >= 3)
      {
      auto it1 = it - 1;
      auto it2 = it - 2;
      auto it3 = it - 3;
      if ((it1->t == expanded_token::ET_VECTOR || it1->t == expanded_token::ET_FLOAT || it1->t == expanded_token::ET_INTEGER) &&
        (it2->t == expanded_token::ET_VECTOR || it2->t == expanded_token::ET_FLOAT || it2->t == expanded_token::ET_INTEGER) &&
        (it3->t == expanded_token::ET_VECTOR || it3->t == expanded_token::ET_FLOAT || it3->t == expanded_token::ET_INTEGER))
        {
        if (positive)
          {
          auto tmp = *it3;
          *it3 = *it2;
          *it2 = *it1;
          *it1 = tmp;
          }
        else
          {
          auto tmp = *it1;
          *it1 = *it2;
          *it2 = *it3;
          *it3 = tmp;
          }
        words.erase(it);
        return it1;
        }
      }
    return it;
    }

  std::vector<expanded_token>::iterator apply_fmod(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(words.begin(), it);
    if (sz >= 2)
      {
      auto it1 = it - 1;
      auto it2 = it - 2;
      if (it1->t == expanded_token::ET_FLOAT && it2->t == expanded_token::ET_FLOAT)
        {
        float it2divit1 = std::floor(it2->f[0] / it1->f[0]);
        float remainder = it2->f[0] - it2divit1 * it1->f[0];
        it2->f[0] = remainder;
        it1->f[0] = it2divit1;
        words.erase(it);
        return it1;
        }      
      if (it1->t == expanded_token::ET_FLOAT && it2->t == expanded_token::ET_VECTOR)
        {
        for (int i = 1; i < AVX_LENGTH; ++i)
          it1->f[i] = it1->f[0];
        it1->t = expanded_token::ET_VECTOR;
        for (int i = 0; i < AVX_LENGTH; ++i)
          {
          float it2divit1 = std::floor(it2->f[i] / it1->f[i]);
          float remainder = it2->f[i] - it2divit1 * it1->f[i];
          it2->f[i] = remainder;
          it1->f[i] = it2divit1;
          }
        words.erase(it);
        return it1;
        }
      if (it1->t == expanded_token::ET_VECTOR && it2->t == expanded_token::ET_FLOAT)
        {
        for (int i = 1; i < AVX_LENGTH; ++i)
          it2->f[i] = it2->f[0];
        it2->t = expanded_token::ET_VECTOR;
        for (int i = 0; i < AVX_LENGTH; ++i)
          {
          float it2divit1 = std::floor(it2->f[i] / it1->f[i]);
          float remainder = it2->f[i] - it2divit1 * it1->f[i];
          it2->f[i] = remainder;
          it1->f[i] = it2divit1;
          }
        it2->t = expanded_token::ET_VECTOR;
        words.erase(it);
        return it1;
        }
      if (it1->t == expanded_token::ET_VECTOR && it2->t == expanded_token::ET_VECTOR)
        {
        for (int i = 0; i < AVX_LENGTH; ++i)
          {
          float it2divit1 = std::floor(it2->f[i] / it1->f[i]);
          float remainder = it2->f[i] - it2divit1 * it1->f[i];
          it2->f[i] = remainder;
          it1->f[i] = it2divit1;
          }
        words.erase(it);
        return it1;
        }
      }
    return it;
    }

  std::vector<expanded_token>::iterator apply_drop(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(words.begin(), it);
    if (sz >= 1)
      {
      auto it1 = it - 1;
      if (it1->t == expanded_token::ET_VECTOR || it1->t == expanded_token::ET_FLOAT || it1->t == expanded_token::ET_INTEGER)        
        {
        it = words.erase(it1, it+1);   
        if (it != words.begin())
          --it;
        return it;
        }
      }
    return it;
    }

  std::vector<expanded_token>::iterator apply_twodrop(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(words.begin(), it);
    if (sz >= 2)
      {
      auto it1 = it - 1;
      auto it2 = it - 2;
      if ((it1->t == expanded_token::ET_VECTOR || it1->t == expanded_token::ET_FLOAT || it1->t == expanded_token::ET_INTEGER) &&
        (it2->t == expanded_token::ET_VECTOR || it2->t == expanded_token::ET_FLOAT || it2->t == expanded_token::ET_INTEGER))
        {
        it = words.erase(it2, it + 1);
        if (it != words.begin())
          --it;
        return it;
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
      else if (it->prim == &primitive_fequ)
        it = apply_2_floats<fequ_op>(words, it);
      else if (it->prim == &primitive_fnequ)
        it = apply_2_floats<fnequ_op>(words, it);
      else if (it->prim == &primitive_flt)
        it = apply_2_floats<flt_op>(words, it);
      else if (it->prim == &primitive_fgt)
        it = apply_2_floats<fgt_op>(words, it);
      else if (it->prim == &primitive_fle)
        it = apply_2_floats<fle_op>(words, it);
      else if (it->prim == &primitive_fge)
        it = apply_2_floats<fge_op>(words, it);
      else if (it->prim == &primitive_equ)        
        it = apply_2_floats<equ_op>(words, it, true);        
      else if (it->prim == &primitive_nequ)        
        it = apply_2_floats<nequ_op>(words, it, true);
      else if (it->prim == &primitive_lt)        
        it = apply_2_floats<lt_op>(words, it, true);
      else if (it->prim == &primitive_gt)        
        it = apply_2_floats<gt_op>(words, it, true);
      else if (it->prim == &primitive_le)        
        it = apply_2_floats<le_op>(words, it, true);
      else if (it->prim == &primitive_ge)        
        it = apply_2_floats<ge_op>(words, it, true);
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
      else if (it->prim == &primitive_addi)
        it = apply_2_integers<addi_op>(words, it);
      else if (it->prim == &primitive_subi)
        it = apply_2_integers<subi_op>(words, it);
      else if (it->prim == &primitive_muli)
        it = apply_2_integers<muli_op>(words, it);
      else if (it->prim == &primitive_divi)
        it = apply_2_integers<divi_op>(words, it);
      else if (it->prim == &primitive_fcast)
        it = apply_fcast(words, it);
      else if (it->prim == &primitive_icast)
        it = apply_icast(words, it);
      else if (it->prim == &primitive_dup)
        it = apply_dup(words, it);
      else if (it->prim == &primitive_twodup)
        it = apply_twodup(words, it);
      else if (it->prim == &primitive_swap)
        it = apply_swap(words, it);
      else if (it->prim == &primitive_twoswap)
        it = apply_twoswap(words, it);
      else if (it->prim == &primitive_over)
        it = apply_over(words, it);
      else if (it->prim == &primitive_rot)
        it = apply_rot(words, it, true);
      else if (it->prim == &primitive_minrot)
        it = apply_rot(words, it, false);
      else if (it->prim == &primitive_fmod)
        it = apply_fmod(words, it);
      else if (it->prim == &primitive_drop)
        {
        it = apply_drop(words, it);
        if (it == words.end())
          break;
        }
      else if (it->prim == &primitive_twodrop)
        {
        it = apply_twodrop(words, it);
        if (it == words.end())
          break;
        }
      }    
    ++it;
    }
  }

VF_END