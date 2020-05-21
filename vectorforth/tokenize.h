#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <ostream>
#include "namespace.h"
#include "vectorforth_api.h"

VF_BEGIN

VECTOR_FORTH_API float to_float(const char* value);
int is_number(int* is_real, int* is_scientific, const char* value);

struct token
  {
  enum e_type
    {
    T_WORD,
    T_PRIMITIVE,
    T_INTEGER,
    T_FLOAT,
    T_COLON,
    T_SEMICOLON,
#ifdef AVX512
    T_VECTOR16
#else
    T_VECTOR8
#endif
    };

  e_type type;
  std::string value;
  int line_nr;
  int column_nr;

  token(e_type i_type, const std::string& v, int i_line_nr, int i_column_nr) : type(i_type), value(v), line_nr(i_line_nr), column_nr(i_column_nr) {}
  };

VECTOR_FORTH_API std::vector<token> tokenize(const std::string& str);

VF_END
