#pragma once

#include "namespace.h"
#include "dictionary.h"
#include "tokenize.h"
#include "vectorforth_api.h"

#include <asm/asmcode.h>
#include <asm/assembler.h>

VF_BEGIN

struct compile_data;

void compile_words(ASM::asmcode& code, dictionary& d, compile_data& cd, std::vector<token>& words);

VECTOR_FORTH_API void compile(ASM::asmcode& code, dictionary& d, std::vector<token> words);

VF_END