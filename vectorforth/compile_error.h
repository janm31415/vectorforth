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
#ifdef AVX512
  VF_ERROR_VECTOR16_INVALID_SYNTAX, 
#else
  VF_ERROR_VECTOR8_INVALID_SYNTAX, 
#endif
  VF_ERROR_CREATE_WAS_ALREADY_CALLED,
  VF_ERROR_UNCLEAR_TARGET_FOR_TO,
  VF_ERROR_UNCLEAR_TARGET_FOR_CREATE
  };

std::string compile_error_text(compile_error err, int line_nr, int column_nr, std::string info = "");

VF_END