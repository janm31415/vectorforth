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
    ET_CREATE_VARIABLE
    };


  expanded_token(token i_t)  : value(i_t.value), line_nr(i_t.line_nr), column_nr(i_t.column_nr)
    {
    switch (i_t.type)
      {
      case token::T_FLOAT: t = ET_FLOAT; break;
      case token::T_INTEGER: t = ET_INTEGER; break;
      case token::T_VECTOR: t = ET_VECTOR; break;
      default:
        throw std::runtime_error("not implemented");
      }
    }

  expanded_token(e_type type, int i_line_nr, int i_column_nr) : t(type), line_nr(i_line_nr), column_nr(i_column_nr) {}

  e_type t;
  float f[16];
  std::string value;
  int line_nr;
  int column_nr;

  uint64_t binding_space_offset;
  uint64_t variable_address;
  bool variable_to_called;

  prim_fun prim;
  };

struct expand_data;

void expand_words(std::vector<expanded_token>& expanded, dictionary& d, expand_data& cd, std::vector<token>& words);

VECTOR_FORTH_API void expand(std::vector<expanded_token>& expanded, dictionary& d, expand_data& cd, std::vector<token> words);

VF_END