#pragma once

#include "namespace.h"
#include "dictionary.h"
#include "tokenize.h"
#include "vectorforth_api.h"

#include <asm/asmcode.h>
#include <asm/assembler.h>

VF_BEGIN

struct expand_data;
struct compile_data;

/*
Compiles with a single pass: is fastest but no optimization possible.
*/
VECTOR_FORTH_API void compile_single_pass(ASM::asmcode& code, dictionary& d, expand_data& ed, std::vector<token> words);


/*
Compiles with multiple passes so that compiler optimizations can be performed.
*/
VECTOR_FORTH_API void compile(ASM::asmcode& code, dictionary& d, expand_data& ed, const std::vector<token>& words);

VF_END