#include "asm_aux.h"
#include "context_defs.h"
#include <sstream>

using namespace ASM;

VF_BEGIN

std::string label_to_string(uint64_t lab)
  {
  std::stringstream str;
  str << "L_" << lab;
  return str.str();
  }

void store_registers(asmcode& code)
  {
  /*
  linux: r12, r13, r14, r15, rbx, rsp, rbp should be preserved
  windows: r12, r13, r14, r15, rbx, rsp, rbp, rdi, rsi, x/y/zmm6, x/y/zmm7
  */
  code.add(asmcode::MOV, RBX_STORE, asmcode::RBX);
  //code.add(asmcode::MOV, RDI_STORE, asmcode::RDI);
  //code.add(asmcode::MOV, RSI_STORE, asmcode::RSI);
  code.add(asmcode::MOV, RSP_STORE, asmcode::RSP);
  code.add(asmcode::MOV, RBP_STORE, asmcode::RBP);
  //code.add(asmcode::MOV, R12_STORE, asmcode::R12);
  //code.add(asmcode::MOV, R13_STORE, asmcode::R13);
  //code.add(asmcode::MOV, R14_STORE, asmcode::R14);
  code.add(asmcode::MOV, R15_STORE, asmcode::R15);

#ifdef _WIN32
  code.add(asmcode::VMOVAPS, ZMM6_STORE, AVX_REG6);
  code.add(asmcode::VMOVAPS, ZMM7_STORE, AVX_REG7);
#endif
  }

void load_registers(asmcode& code)
  {
  /*
  linux: r12, r13, r14, r15, rbx, rsp, rbp should be preserved
  windows: r12, r13, r14, r15, rbx, rsp, rbp, rdi, rsi
  */
  code.add(asmcode::MOV, asmcode::RBX, RBX_STORE);
  //code.add(asmcode::MOV, asmcode::RDI, RDI_STORE);
  //code.add(asmcode::MOV, asmcode::RSI, RSI_STORE);
  code.add(asmcode::MOV, asmcode::RSP, RSP_STORE);
  code.add(asmcode::MOV, asmcode::RBP, RBP_STORE);
  //code.add(asmcode::MOV, asmcode::R12, R12_STORE);
  //code.add(asmcode::MOV, asmcode::R13, R13_STORE);
  //code.add(asmcode::MOV, asmcode::R14, R14_STORE);
  code.add(asmcode::MOV, asmcode::R15, R15_STORE);

#ifdef _WIN32
  code.add(asmcode::VMOVAPS, AVX_REG6, ZMM6_STORE);
  code.add(asmcode::VMOVAPS, AVX_REG7, ZMM7_STORE);
#endif
  }

VF_END