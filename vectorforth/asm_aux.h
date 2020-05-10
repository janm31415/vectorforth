#pragma once

#include "namespace.h"
#include <asm/asmcode.h>
#include <string>

VF_BEGIN

std::string label_to_string(uint64_t lab);
void store_registers(ASM::asmcode& code);
void load_registers(ASM::asmcode& code);

void pop_ymm0(ASM::asmcode& code);
void pop_ymm1(ASM::asmcode& code);
void pop_ymm2(ASM::asmcode& code);
void pop_ymm3(ASM::asmcode& code);

void push_ymm0(ASM::asmcode& code);
void push_ymm1(ASM::asmcode& code);
void push_ymm2(ASM::asmcode& code);
void push_ymm3(ASM::asmcode& code);

void pop_rax(ASM::asmcode& code);
void pop_r11(ASM::asmcode& code);
void push_rax(ASM::asmcode& code);

void align_stack(ASM::asmcode& code);
void restore_stack(ASM::asmcode& code);

VF_END