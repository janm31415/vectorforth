#pragma once

#include "namespace.h"
#include <string>

VF_BEGIN

enum compile_error
  {
  VF_ERROR_NO_CORRESPONDING_SEMICOLON,
  VF_ERROR_NO_CORRESPONDING_COLON,
  VF_ERROR_EMPTY_DEFINITION, 
  VF_ERROR_INVALID_DEFINITION_NAME,
  VF_ERROR_WORD_UNKNOWN,
  VF_ERROR_VECTOR8_INVALID_SYNTAX
  };

std::string compile_error_text(compile_error err, int line_nr, int column_nr, std::string info = "");

VF_END