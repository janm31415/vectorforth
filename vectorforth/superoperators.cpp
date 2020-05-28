#include "superoperators.h"
#include "primitives.h"

VF_BEGIN

using namespace ASM;

void superoperator_address_subi(asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR #addr #-");

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, et.int_value);
  code.add(asmcode::SUB, asmcode::RAX, asmcode::R11);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR #addr #-");
  }

void superoperator_address_addi(asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR #addr #+");

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, et.int_value);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::R11);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR #addr #+");
  }

namespace
  {
  bool is_float(std::vector<expanded_token>::iterator it)
    {
    return it->t == expanded_token::ET_FLOAT;
    }

  bool is_primitive(std::vector<expanded_token>::iterator it)
    {
    return it->t == expanded_token::ET_PRIMITIVE;
    }

  bool is_integer(std::vector<expanded_token>::iterator it)
    {
    return it->t == expanded_token::ET_INTEGER;
    }

  bool is_subi(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_subi);
    }

  bool is_addi(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_addi);
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


  std::vector<expanded_token>::iterator combine_primitives(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    return it;
    }

  std::vector<expanded_token>::iterator combine_address_ops(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(it, words.end());
    if (sz >= 2)
      {
      auto it1 = it + 1;
      if (is_subi(it1))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_address_subi;
        *it1 = *it;
        it = words.erase(it, it1);
        return it;
        }
      if (is_addi(it1))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_address_addi;
        *it1 = *it;
        it = words.erase(it, it1);
        return it;
        }
      }
    return it;
    }

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
    if (is_primitive(it))
      {
      it = combine_primitives(words, it);
      }
    if (is_integer(it))
      {
      it = combine_address_ops(words, it);
      }
    ++it;
    }
  }

VF_END