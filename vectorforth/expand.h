#pragma once

#include "namespace.h"
#include "dictionary.h"
#include "primitives.h"
#include "tokenize.h"
#include "vectorforth_api.h"

#include <string>
#include <vector>

VF_BEGIN

struct expanded_token
  {
  enum e_type
    {
    ET_PRIMITIVE,
    ET_INTEGER,
    ET_FLOAT,
    ET_VECTOR,
    ET_OVERWRITE_VARIABLE,
    ET_VARIABLE,
    ET_CREATE_VARIABLE,
    ET_UPDATE_VARIABLE
    };


  expanded_token(token i_t)  : value(i_t.value)
    {
    switch (i_t.type)
      {
      case token::T_FLOAT: t = ET_FLOAT; break;
      case token::T_INTEGER: t = ET_INTEGER; break;
      case token::T_VECTOR: t = ET_VECTOR; break;
      default:
        throw std::runtime_error("compiler error");
      }
    }

  expanded_token(e_type type) : t(type) {}

  e_type t;
  float f[16];
  std::string value;

  uint64_t binding_space_offset;
  uint64_t variable_address;

  prim_fun prim;
  };

struct expand_data;

void expand_words(std::vector<expanded_token>& expanded, dictionary& d, expand_data& cd, std::vector<token>& words);

VECTOR_FORTH_API void expand(std::vector<expanded_token>& expanded, dictionary& d, expand_data& cd, std::vector<token> words);

VF_END