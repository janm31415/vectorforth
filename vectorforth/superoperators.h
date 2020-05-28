#pragma once

#include "expand.h"
#include "vectorforth_api.h"
#include <vector>

VF_BEGIN

/*
This step introduces superoperators: an optimization technique for bytecoded interpreters.
Superoperators are virtual machine operations automatically synthesized from smaller
operations to avoid costly per-operation overhead.

For instance: three floats pushed on the stack as three seperate operations can be replaced
by one operator that pushes the 3 values on the stack at once.

Sources: https://www.complang.tuwien.ac.at/forth/peep/

@InProceedings{proebsting95,
  author = 	 "Todd A. Proebsting",
  title = 	 "Optimizing an ANSI C Interpreter with Superoperators",
  crossref =	 "popl95",
  pages =	 "322--332"
}
*/

struct compile_data;
struct expanded_token;

void superoperator_address_subi(ASM::asmcode& code, const expanded_token& et);
void superoperator_address_addi(ASM::asmcode& code, const expanded_token& et);
void superoperator_address_subi_fetch(ASM::asmcode& code, const expanded_token& et);
void superoperator_address_addi_fetch(ASM::asmcode& code, const expanded_token& et);
void superoperator_stacktopfetch_address_subi_fetch(ASM::asmcode& code, const expanded_token& et);
void superoperator_stacktopfetch_address_addi_fetch(ASM::asmcode& code, const expanded_token& et);


VECTOR_FORTH_API void superoperators(std::vector<expanded_token>& words);

VF_END
