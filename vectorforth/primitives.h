#pragma once

#include "namespace.h"
#include "compile_data.h"

#include <asm/asmcode.h>

#include <map>
#include <string>

VF_BEGIN

void primitive_stack_top_fetch(ASM::asmcode& code, compile_data& cd);
void primitive_stack_pointer_fetch(ASM::asmcode& code, compile_data& cd);

void primitive_fetch(ASM::asmcode& code, compile_data& cd);
void primitive_store(ASM::asmcode& code, compile_data& cd);
void primitive_addstorei(ASM::asmcode& code, compile_data& cd);
void primitive_substorei(ASM::asmcode& code, compile_data& cd);

void primitive_here(ASM::asmcode& code, compile_data& cd);
void primitive_cells(ASM::asmcode& code, compile_data& cd);

void primitive_add(ASM::asmcode& code, compile_data& cd);
void primitive_sub(ASM::asmcode& code, compile_data&);
void primitive_mul(ASM::asmcode& code, compile_data& cd);
void primitive_complex_mul(ASM::asmcode& code, compile_data& cd);
void primitive_complex_add(ASM::asmcode& code, compile_data& cd);
void primitive_complex_sub(ASM::asmcode& code, compile_data& cd);
void primitive_div(ASM::asmcode& code, compile_data&);
void primitive_sin(ASM::asmcode& code, compile_data&);
void primitive_cos(ASM::asmcode& code, compile_data&);
void primitive_tan(ASM::asmcode& code, compile_data&);
void primitive_exp(ASM::asmcode& code, compile_data&);
void primitive_log(ASM::asmcode& code, compile_data&);
void primitive_pow(ASM::asmcode& code, compile_data&);
void primitive_atan2(ASM::asmcode& code, compile_data&);
void primitive_sqrt(ASM::asmcode& code, compile_data&);
void primitive_abs(ASM::asmcode& code, compile_data&);
void primitive_negate(ASM::asmcode& code, compile_data&);
void primitive_and(ASM::asmcode& code, compile_data&);
void primitive_or(ASM::asmcode& code, compile_data&);
void primitive_xor(ASM::asmcode& code, compile_data&);
void primitive_not(ASM::asmcode& code, compile_data&);

void primitive_fmod(ASM::asmcode& code, compile_data&);

void primitive_min(ASM::asmcode& code, compile_data&);
void primitive_max(ASM::asmcode& code, compile_data&);

void primitive_drop(ASM::asmcode& code, compile_data& cd);
void primitive_swap(ASM::asmcode& code, compile_data& cd);
void primitive_dup(ASM::asmcode& code, compile_data& cd);
void primitive_over(ASM::asmcode& code, compile_data& cd);
void primitive_rot(ASM::asmcode& code, compile_data& cd);
void primitive_minrot(ASM::asmcode& code, compile_data& cd);
void primitive_twodrop(ASM::asmcode& code, compile_data& cd);
void primitive_twodup(ASM::asmcode& code, compile_data& cd);
void primitive_twoswap(ASM::asmcode& code, compile_data& cd);

void primitive_addi(ASM::asmcode& code, compile_data& cd);
void primitive_subi(ASM::asmcode& code, compile_data&);
void primitive_muli(ASM::asmcode& code, compile_data&);
void primitive_divi(ASM::asmcode& code, compile_data&);
void primitive_incri(ASM::asmcode& code, compile_data& cd);
void primitive_decri(ASM::asmcode& code, compile_data& cd);
void primitive_incr32i(ASM::asmcode& code, compile_data& cd);
void primitive_decr32i(ASM::asmcode& code, compile_data& cd);
void primitive_incr64i(ASM::asmcode& code, compile_data& cd);
void primitive_decr64i(ASM::asmcode& code, compile_data& cd);
void primitive_incr96i(ASM::asmcode& code, compile_data& cd);
void primitive_decr96i(ASM::asmcode& code, compile_data& cd);
void primitive_incr128i(ASM::asmcode& code, compile_data& cd);
void primitive_decr128i(ASM::asmcode& code, compile_data& cd);
void primitive_incr160i(ASM::asmcode& code, compile_data& cd);
void primitive_decr160i(ASM::asmcode& code, compile_data& cd);
void primitive_incr192i(ASM::asmcode& code, compile_data& cd);
void primitive_decr192i(ASM::asmcode& code, compile_data& cd);
void primitive_incr224i(ASM::asmcode& code, compile_data& cd);
void primitive_decr224i(ASM::asmcode& code, compile_data& cd);
void primitive_incr256i(ASM::asmcode& code, compile_data& cd);
void primitive_decr256i(ASM::asmcode& code, compile_data& cd);



void primitive_equ(ASM::asmcode& code, compile_data& cd);
void primitive_nequ(ASM::asmcode& code, compile_data& cd);
void primitive_lt(ASM::asmcode& code, compile_data& cd);
void primitive_gt(ASM::asmcode& code, compile_data& cd);
void primitive_le(ASM::asmcode& code, compile_data& cd);
void primitive_ge(ASM::asmcode& code, compile_data& cd);
void primitive_fequ(ASM::asmcode& code, compile_data& cd);
void primitive_fnequ(ASM::asmcode& code, compile_data& cd);
void primitive_flt(ASM::asmcode& code, compile_data& cd);
void primitive_fgt(ASM::asmcode& code, compile_data& cd);
void primitive_fle(ASM::asmcode& code, compile_data& cd);
void primitive_fge(ASM::asmcode& code, compile_data& cd);

void primitive_floor(ASM::asmcode& code, compile_data& cd);
void primitive_ceil(ASM::asmcode& code, compile_data& cd);
void primitive_round(ASM::asmcode& code, compile_data& cd);
void primitive_truncate(ASM::asmcode& code, compile_data& cd);

void primitive_fcast(ASM::asmcode& code, compile_data& cd);
void primitive_icast(ASM::asmcode& code, compile_data& cd);

void primitive_return_stack_push(ASM::asmcode& code, compile_data& cd);
void primitive_return_stack_pop(ASM::asmcode& code, compile_data& cd);
void primitive_return_stack_top(ASM::asmcode& code, compile_data& cd);
void primitive_return_stack_top_fetch(ASM::asmcode& code, compile_data& cd);
void primitive_return_stack_pointer_fetch(ASM::asmcode& code, compile_data& cd);

void primitive_if(ASM::asmcode& code, compile_data& cd);
void primitive_then(ASM::asmcode& code, compile_data& cd);
void primitive_else(ASM::asmcode& code, compile_data& cd);

void primitive_begin(ASM::asmcode& code, compile_data& cd);
void primitive_while(ASM::asmcode& code, compile_data& cd);
void primitive_repeat(ASM::asmcode& code, compile_data& cd);

typedef void(*prim_fun)(ASM::asmcode&, compile_data&);
typedef std::map<std::string, prim_fun> prim_map;

prim_map generate_primitives_map();

VF_END