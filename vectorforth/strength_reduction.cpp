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
      // replace 3 pow by dup dup * *
      if (it1->t == expanded_token::ET_FLOAT && it1->f[0] == 3.f)
        {
        it1->t = expanded_token::ET_PRIMITIVE;
        it1->prim = &primitive_dup;
        it = words.insert(it, *it1);
        ++it;
        it->prim = &primitive_mul;
        it = words.insert(it, *it);
        ++it;
        return it;
        }
      // replace 4 pow by dup dup dup * * *
      if (it1->t == expanded_token::ET_FLOAT && it1->f[0] == 4.f)
        {
        it1->t = expanded_token::ET_PRIMITIVE;
        it1->prim = &primitive_dup;
        it = words.insert(it, *it1);
        ++it;
        it->prim = &primitive_dup;
        it = words.insert(it, *it);
        ++it;
        it->prim = &primitive_mul;
        it = words.insert(it, *it);
        ++it;
        it = words.insert(it, *it);
        ++it;
        return it;
        }
      }
    return it;
    }

  std::vector<expanded_token>::iterator reduce_if(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(words.begin(), it);
    if (sz >= 1)
      {
      auto it1 = it - 1;
      if (it1->t == expanded_token::ET_FLOAT || it1->t == expanded_token::ET_VECTOR)
        {
        if (*reinterpret_cast<uint32_t*>(&(it1->f[0])) == 0)
          {
          if (it1->t == expanded_token::ET_VECTOR)
            {
            for (int j = 1; j < AVX_LENGTH; ++j)
              if (*reinterpret_cast<uint32_t*>(&(it1->f[j])) != 0)
                return it;
            auto it_else = it;
            int nr_of_ifs = 1;
            while (nr_of_ifs > 0 && it_else != words.end())
              {
              while (it_else != words.end() && (it_else->t != expanded_token::ET_PRIMITIVE || it_else->prim != &primitive_else))
                {
                ++it_else;
                if (it_else != words.end() && it_else->t == expanded_token::ET_PRIMITIVE && it_else->prim == &primitive_if)
                  ++nr_of_ifs;
                }
              --nr_of_ifs;
              if (nr_of_ifs > 0 && it_else != words.end())
                ++it_else;
              }
            if (it_else == words.end())
              return it;
            it = words.erase(it1, it_else + 1);
            auto it_then = it;
            nr_of_ifs = 1;
            while (nr_of_ifs > 0 && it_then != words.end())
              {
              while (it_then != words.end() && (it_then->t != expanded_token::ET_PRIMITIVE || it_then->prim != &primitive_then))
                {
                ++it_then;
                if (it_then != words.end() && it_then->t == expanded_token::ET_PRIMITIVE && it_then->prim == &primitive_if)
                  ++nr_of_ifs;
                }
              --nr_of_ifs;
              if (nr_of_ifs > 0 && it_then != words.end())
                ++it_then;
              }
            if (it_then == words.end())
              return it;
            words.erase(it_then);
            return it;
            }

          }
        else if (*reinterpret_cast<uint32_t*>(&(it1->f[0])) == 0xffffffff)
          {
          if (it1->t == expanded_token::ET_VECTOR)
            {
            for (int j = 1; j < AVX_LENGTH; ++j)
              if (*reinterpret_cast<uint32_t*>(&(it1->f[j])) != 0xffffffff)
                return it;
            auto it_else = it;
            int nr_of_ifs = 1;
            while (nr_of_ifs > 0 && it_else != words.end())
              {
              while (it_else != words.end() && (it_else->t != expanded_token::ET_PRIMITIVE || it_else->prim != &primitive_else))
                {
                ++it_else;
                if (it_else != words.end() && it_else->t == expanded_token::ET_PRIMITIVE && it_else->prim == &primitive_if)
                  ++nr_of_ifs;
                }
              --nr_of_ifs;
              if (nr_of_ifs > 0 && it_else != words.end())
                ++it_else;
              }
            if (it_else == words.end())
              return it;            
            auto it_then = it_else;
            nr_of_ifs = 1;
            while (nr_of_ifs > 0 && it_then != words.end())
              {
              while (it_then != words.end() && (it_then->t != expanded_token::ET_PRIMITIVE || it_then->prim != &primitive_then))
                {
                ++it_then;
                if (it_then != words.end() && it_then->t == expanded_token::ET_PRIMITIVE && it_then->prim == &primitive_if)
                  ++nr_of_ifs;
                }
              --nr_of_ifs;
              if (nr_of_ifs > 0 && it_then != words.end())
                ++it_then;
              }
            if (it_then == words.end())
              return it;
            words.erase(it_else, it_then+1);
            it = words.erase(it1, it + 1);
            return it;
            }
          }

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
      else if (it->prim == &primitive_if)
        it = reduce_if(words, it);
      }
    ++it;
    }
  }

VF_END