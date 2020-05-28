#pragma once

#include "namespace.h"
#include "dictionary.h"
#include "primitives.h"
#include "superoperators.h"
#include "tokenize.h"
#include "vectorforth_api.h"
#include "context_defs.h"
#include <asm/asmcode.h>

#include <vector>

VF_BEGIN
struct expanded_token;

typedef void(*superoperator_fun)(ASM::asmcode&, const expanded_token&);

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
    ET_UPDATE_VARIABLE,
    // superoperators
    ET_SUPEROPERATOR,
    ET_FLOAT2,
    ET_FLOAT3,
    ET_FLOAT4,
    ET_FLOAT5,
    ET_FLOAT6
    };

  expanded_token(e_type type) : t(type) {}

  e_type t;
  float f[AVX_LENGTH]; // used by ET_FLOAT and ET_VECTOR
  int64_t int_value; // used by ET_INTEGER

  uint64_t binding_space_offset; //used by ET_OVERWRITE_VARIABLE. Contains address with the new value of the variable.
  uint64_t variable_address; // used by all ET_<...>_VARIABLE types.

  prim_fun prim; // used by ET_PRIMITIVE. Contains pointer to primitive operation.
  int64_t reserve1;

  superoperator_fun supop; // used by ET_SUPEROPERATOR.
  };

struct expand_data;

void expand_words(std::vector<expanded_token>& expanded, dictionary& d, expand_data& ed, std::vector<token>& words);

VECTOR_FORTH_API void expand(std::vector<expanded_token>& expanded, dictionary& d, expand_data& ed, std::vector<token> words);

VF_END