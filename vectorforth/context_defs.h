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
#define DWORD_MEM_STACK_REGISTER ASM::asmcode::DWORD_MEM_RBP

#define CONSTANT_SPACE_POINTER ASM::asmcode::MEM_R10, 112

#define HERE ASM::asmcode::RBX
#define MEM_HERE ASM::asmcode::MEM_RBX

#define RSP_SAVE ASM::asmcode::MEM_R10, 96

#define RSP_TOP ASM::asmcode::MEM_R10, 104

#define SIGN_BIT ASM::asmcode::MEM_R10, 128
#define NOT_SIGN_BIT ASM::asmcode::MEM_R10, 192
#define ALL_BITS ASM::asmcode::MEM_R10, 256
#define NO_BITS ASM::asmcode::MEM_R10, 320
#define ONEF_BITS ASM::asmcode::MEM_R10, 384

#define CELLS(n) (n)*8

#ifdef AVX512
#define AVX_REG0 ASM::asmcode::ZMM0
#define AVX_REG1 ASM::asmcode::ZMM1
#define AVX_REG2 ASM::asmcode::ZMM2
#define AVX_REG3 ASM::asmcode::ZMM3
#define AVX_REG4 ASM::asmcode::ZMM4
#define AVX_REG5 ASM::asmcode::ZMM5
#define AVX_CELLS(n) (n)*64
#define AVX_ALIGNMENT 64
#define AVX_LENGTH 16
#else
#define AVX_REG0 ASM::asmcode::YMM0
#define AVX_REG1 ASM::asmcode::YMM1
#define AVX_REG2 ASM::asmcode::YMM2
#define AVX_REG3 ASM::asmcode::YMM3
#define AVX_REG4 ASM::asmcode::YMM4
#define AVX_REG5 ASM::asmcode::YMM5
#define AVX_CELLS(n) (n)*32
#define AVX_ALIGNMENT 32
#define AVX_LENGTH 8
#endif
VF_END
