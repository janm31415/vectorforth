#pragma once

#include "namespace.h"

VF_BEGIN

#define CONTEXT ASM::asmcode::R10
#define RBX_STORE ASM::asmcode::MEM_R10, 0
#define RDI_STORE ASM::asmcode::MEM_R10, 8
#define RSI_STORE ASM::asmcode::MEM_R10, 16
#define RSP_STORE ASM::asmcode::MEM_R10, 24
#define RBP_STORE ASM::asmcode::MEM_R10, 32
#define R12_STORE ASM::asmcode::MEM_R10, 40
#define R13_STORE ASM::asmcode::MEM_R10, 48
#define R14_STORE ASM::asmcode::MEM_R10, 56
#define R15_STORE ASM::asmcode::MEM_R10, 64

#define STACK_TOP ASM::asmcode::MEM_R10, 72
#define STACK_POINTER ASM::asmcode::MEM_R10, 88

#define HERE_POINTER ASM::asmcode::MEM_R10, 120

#define STACK_REGISTER ASM::asmcode::RBP
#define MEM_STACK_REGISTER ASM::asmcode::MEM_RBP

#define CONSTANT_SPACE_POINTER ASM::asmcode::MEM_R10, 112

#define HERE ASM::asmcode::RBX
#define MEM_HERE ASM::asmcode::MEM_RBX

#define RSP_SAVE ASM::asmcode::MEM_R10, 96

#define RSP_TOP ASM::asmcode::MEM_R10, 104

#define SIGN_BIT ASM::asmcode::MEM_R10, 128
#define NOT_SIGN_BIT ASM::asmcode::MEM_R10, 160
#define ALL_BITS ASM::asmcode::MEM_R10, 192

#define CELLS(n) (n)*8

VF_END
