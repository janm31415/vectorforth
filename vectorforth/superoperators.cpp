#include "superoperators.h"

VF_BEGIN

namespace
  {
  bool is_float(std::vector<expanded_token>::iterator it)
    {
    return it->t == expanded_token::ET_FLOAT;
    }

  std::vector<expanded_token>::iterator combine_floats(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(it, words.end());    
    if (sz >= 6)
      {
      auto it1 = it + 1;
      auto it2 = it + 2;
      auto it3 = it + 3;
      auto it4 = it + 4;
      auto it5 = it + 5;
      if (is_float(it5) && is_float(it4) && is_float(it3) && is_float(it2) && is_float(it1))
        {
        it->f[1] = it1->f[0];
        it->f[2] = it2->f[0];
        it->f[3] = it3->f[0];
        it->f[4] = it4->f[0];
        it->f[5] = it5->f[0];
        it->t = expanded_token::ET_FLOAT6;
        *it5 = *it;
        it = words.erase(it, it5);
        return it;
        }
      }
    if (sz >= 5)
      {
      auto it1 = it + 1;
      auto it2 = it + 2;
      auto it3 = it + 3;
      auto it4 = it + 4;
      if (is_float(it4) && is_float(it3) && is_float(it2) && is_float(it1))
        {
        it->f[1] = it1->f[0];
        it->f[2] = it2->f[0];
        it->f[3] = it3->f[0];
        it->f[4] = it4->f[0];
        it->t = expanded_token::ET_FLOAT5;
        *it4 = *it;
        it = words.erase(it, it4);
        return it;
        }
      }
   if (sz >= 4)
     {
     auto it1 = it + 1;
     auto it2 = it + 2;
     auto it3 = it + 3;
     if (is_float(it3) && is_float(it2) && is_float(it1))
       {
       it->f[1] = it1->f[0];
       it->f[2] = it2->f[0];
       it->f[3] = it3->f[0];
       it->t = expanded_token::ET_FLOAT4;
       *it3 = *it;
       it = words.erase(it, it3);
       return it;
       }
     }     
    if (sz >= 3)
      {
      auto it1 = it + 1;
      auto it2 = it + 2;
      if (is_float(it2) && is_float(it1))
        {
        it->f[1] = it1->f[0];
        it->f[2] = it2->f[0];
        it->t = expanded_token::ET_FLOAT3;
        *it2 = *it;
        it = words.erase(it, it2);
        return it;
        }
      }    
   if (sz >= 2)
     {
     auto it1 = it + 1;
     if (is_float(it1))
       {
       it->f[1] = it1->f[0];
       it->t = expanded_token::ET_FLOAT2;
       *it1 = *it;
       it = words.erase(it, it1);
       return it;
       }
     }
    return it;
    } // combine_floats
  } // anonymous namespace

void superoperators(std::vector<expanded_token>& words)
  {
  auto it = words.begin();
  while (it != words.end())
    {
    if (is_float(it))
      {
      it = combine_floats(words, it);
      }
    ++it;
    }
  }

VF_END