#include "compile_error.h"

#include <sstream>

VF_BEGIN

std::string compile_error_text(compile_error err, int line_nr, int column_nr, std::string info)
  {
  std::stringstream ss;
  ss << "error: ";
  if (line_nr >= 0)
    ss << "line " << line_nr << ": ";
  if (column_nr >= 0)
    ss << "column " << column_nr << ": ";
  switch (err)
    {
    case VF_ERROR_NO_CORRESPONDING_SEMICOLON: ss << "no matching ; found for definition"; break;
    case VF_ERROR_NO_CORRESPONDING_COLON: ss << "invalid keyword: ; is reserved for signalling the end of a definition"; break;
    case VF_ERROR_EMPTY_DEFINITION: ss << "empty definition"; break;
    case VF_ERROR_INVALID_DEFINITION_NAME: ss << "invalid definition name"; break;
    case VF_ERROR_WORD_UNKNOWN: ss << "word unknown"; break;
#ifdef AVX512
    case VF_ERROR_VECTOR16_INVALID_SYNTAX: ss << "invalid syntax: V16 expects 16 floats or integers to form a simd vector"; break;
#else
    case VF_ERROR_VECTOR8_INVALID_SYNTAX: ss << "invalid syntax: V8 expects 8 floats or integers to form a simd vector"; break;
#endif
    case VF_ERROR_CREATE_WAS_ALREADY_CALLED: ss << "previous create was not yet resolved"; break;
    case VF_ERROR_UNCLEAR_TARGET_FOR_CREATE: ss << "no clear target for the create keyword"; break;
    case VF_ERROR_UNCLEAR_TARGET_FOR_TO: ss << "no clear target for the to keyword"; break;
    default: ss << "unknown error"; break;
    }
  if (!info.empty())
    ss << ": " << info;
  return ss.str();
  }

VF_END