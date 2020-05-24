#include "strength_reduction.h"
#include "primitives.h"
VF_BEGIN

namespace
  {
  std::vector<expanded_token>::iterator reduce_pow(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(words.begin(), it);
    if (sz >= 1)
      {
      auto it1 = it - 1;
      // replace 2 pow by dup *
      if (it1->t == expanded_token::ET_FLOAT && it1->f[0] == 2.f)
        {
        it1->t = expanded_token::ET_PRIMITIVE;
        it1->prim = &primitive_dup;
        it->prim = &primitive_mul;
        return it;
        }
      }
    return it;
    }
  }

void strength_reduction(std::vector<expanded_token>& words)
  {
  auto it = words.begin();
  while (it != words.end())
    {
    if (it->t == expanded_token::ET_PRIMITIVE)
      {
      if (it->prim == &primitive_pow)
        it = reduce_pow(words, it);
      }
    ++it;
    }
  }

VF_END