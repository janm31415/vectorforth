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
void superoperator_address_muli(ASM::asmcode& code, const expanded_token& et);
void superoperator_address_shiftlefti(ASM::asmcode& code, const expanded_token& et);
void superoperator_address_shiftrighti(ASM::asmcode& code, const expanded_token& et);
void superoperator_address_subi_fetch(ASM::asmcode& code, const expanded_token& et);
void superoperator_address_addi_fetch(ASM::asmcode& code, const expanded_token& et);
void superoperator_stacktopfetch_address_subi_fetch(ASM::asmcode& code, const expanded_token& et);
void superoperator_stacktopfetch_address_subi_fetch_twice(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_add(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_sub(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_mul(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_div(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_add_float_add(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_sub_float_add(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_mul_float_add(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_div_float_add(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_add_float_sub(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_sub_float_sub(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_mul_float_sub(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_div_float_sub(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_add_float_mul(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_sub_float_mul(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_mul_float_mul(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_div_float_mul(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_add_float_div(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_sub_float_div(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_mul_float_div(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_div_float_div(ASM::asmcode& code, const expanded_token& et);

void superoperator_float_add_add(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_add_sub(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_add_mul(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_add_div(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_sub_add(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_sub_sub(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_sub_mul(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_sub_div(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_mul_add(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_mul_sub(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_mul_mul(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_mul_div(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_div_add(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_div_sub(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_div_mul(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_div_div(ASM::asmcode& code, const expanded_token& et);

void superoperator_swap_drop(ASM::asmcode& code, const expanded_token& et);
void superoperator_swap_over(ASM::asmcode& code, const expanded_token& et);
void superoperator_swap_over_address_addi_store(ASM::asmcode& code, const expanded_token& et);
void superoperator_address_here_fetch_swap_here_addstorei(ASM::asmcode& code, const expanded_token& et);
void superoperator_here_fetch_swap_here_addstorei(ASM::asmcode& code, const expanded_token& et);
void superoperator_dup_floor_sub(ASM::asmcode& code, const expanded_token& et);
void superoperator_variable_fetch(ASM::asmcode& code, const expanded_token& et);
void superoperator_variable_integer_addi_fetch(ASM::asmcode& code, const expanded_token& et);
void superoperator_rtpopop(ASM::asmcode& code, const expanded_token& et);
void superoperator_rtpopopop(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_fmmod_drop(ASM::asmcode& code, const expanded_token& et);


void superoperator_mulmul(ASM::asmcode& code, const expanded_token& et);
void superoperator_addadd(ASM::asmcode& code, const expanded_token& et);
void superoperator_subsub(ASM::asmcode& code, const expanded_token& et);
void superoperator_divdiv(ASM::asmcode& code, const expanded_token& et);

void superoperator_muladd(ASM::asmcode& code, const expanded_token& et);
void superoperator_mulsub(ASM::asmcode& code, const expanded_token& et);
void superoperator_muldiv(ASM::asmcode& code, const expanded_token& et);
void superoperator_addmul(ASM::asmcode& code, const expanded_token& et);
void superoperator_addsub(ASM::asmcode& code, const expanded_token& et);
void superoperator_adddiv(ASM::asmcode& code, const expanded_token& et);
void superoperator_subadd(ASM::asmcode& code, const expanded_token& et);
void superoperator_submul(ASM::asmcode& code, const expanded_token& et);
void superoperator_subdiv(ASM::asmcode& code, const expanded_token& et);
void superoperator_divadd(ASM::asmcode& code, const expanded_token& et);
void superoperator_divmul(ASM::asmcode& code, const expanded_token& et);
void superoperator_divsub(ASM::asmcode& code, const expanded_token& et);


void superoperator_mulmulmul(ASM::asmcode& code, const expanded_token& et);
void superoperator_addaddadd(ASM::asmcode& code, const expanded_token& et);
void superoperator_subsubsub(ASM::asmcode& code, const expanded_token& et);
void superoperator_divdivdiv(ASM::asmcode& code, const expanded_token& et);

void superoperator_addmuladd(ASM::asmcode& code, const expanded_token& et);

void superoperator_dupup(ASM::asmcode& code, const expanded_token& et);
void superoperator_dupupup(ASM::asmcode& code, const expanded_token& et);
void superoperator_dupupupup(ASM::asmcode& code, const expanded_token& et);

void superoperator_rtpush_2dup_addr_addi_fetch(ASM::asmcode& code, const expanded_token& et);
void superoperator_rtpush_addr_addi_fetch(ASM::asmcode& code, const expanded_token& et);

void superoperator_var_float_equ(ASM::asmcode& code, const expanded_token& et);
void superoperator_var_float_nequ(ASM::asmcode& code, const expanded_token& et);
void superoperator_var_float_lt(ASM::asmcode& code, const expanded_token& et);
void superoperator_var_float_le(ASM::asmcode& code, const expanded_token& et);
void superoperator_var_float_gt(ASM::asmcode& code, const expanded_token& et);
void superoperator_var_float_ge(ASM::asmcode& code, const expanded_token& et);

void superoperator_var_float_fequ(ASM::asmcode& code, const expanded_token& et);
void superoperator_var_float_fnequ(ASM::asmcode& code, const expanded_token& et);
void superoperator_var_float_flt(ASM::asmcode& code, const expanded_token& et);
void superoperator_var_float_fle(ASM::asmcode& code, const expanded_token& et);
void superoperator_var_float_fgt(ASM::asmcode& code, const expanded_token& et);
void superoperator_var_float_fge(ASM::asmcode& code, const expanded_token& et);

void superoperator_dup_float_equ(ASM::asmcode& code, const expanded_token& et);
void superoperator_dup_float_nequ(ASM::asmcode& code, const expanded_token& et);
void superoperator_dup_float_lt(ASM::asmcode& code, const expanded_token& et);
void superoperator_dup_float_le(ASM::asmcode& code, const expanded_token& et);
void superoperator_dup_float_gt(ASM::asmcode& code, const expanded_token& et);
void superoperator_dup_float_ge(ASM::asmcode& code, const expanded_token& et);

void superoperator_dup_float_fequ(ASM::asmcode& code, const expanded_token& et);
void superoperator_dup_float_fnequ(ASM::asmcode& code, const expanded_token& et);
void superoperator_dup_float_flt(ASM::asmcode& code, const expanded_token& et);
void superoperator_dup_float_fle(ASM::asmcode& code, const expanded_token& et);
void superoperator_dup_float_fgt(ASM::asmcode& code, const expanded_token& et);
void superoperator_dup_float_fge(ASM::asmcode& code, const expanded_token& et);

void superoperator_here_fetch(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_comma(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_comma_float_comma(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_comma_float_comma_float_comma(ASM::asmcode& code, const expanded_token& et);

void superoperator_float_float_float_var_vec3store(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_float_var_vec2store(ASM::asmcode& code, const expanded_token& et);
void superoperator_float_float_float_float_var_mat2x2store(ASM::asmcode& code, const expanded_token& et);

VECTOR_FORTH_API void superoperators(std::vector<expanded_token>& words);

VF_END
