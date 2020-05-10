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
  windows: r12, r13, r14, r15, rbx, rsp, rbp, rdi, rsi
  */
  //code.add(asmcode::MOV, RBX_STORE, asmcode::RBX);
  //code.add(asmcode::MOV, RDI_STORE, asmcode::RDI);
  //code.add(asmcode::MOV, RSI_STORE, asmcode::RSI);
  code.add(asmcode::MOV, RSP_STORE, asmcode::RSP);
  code.add(asmcode::MOV, RBP_STORE, asmcode::RBP);
  //code.add(asmcode::MOV, R12_STORE, asmcode::R12);
  //code.add(asmcode::MOV, R13_STORE, asmcode::R13);
  //code.add(asmcode::MOV, R14_STORE, asmcode::R14);
  code.add(asmcode::MOV, R15_STORE, asmcode::R15);
  }

void load_registers(asmcode& code)
  {
  /*
  linux: r12, r13, r14, r15, rbx, rsp, rbp should be preserved
  windows: r12, r13, r14, r15, rbx, rsp, rbp, rdi, rsi
  */
  //code.add(asmcode::MOV, asmcode::RBX, RBX_STORE);
  //code.add(asmcode::MOV, asmcode::RDI, RDI_STORE);
  //code.add(asmcode::MOV, asmcode::RSI, RSI_STORE);
  code.add(asmcode::MOV, asmcode::RSP, RSP_STORE);
  code.add(asmcode::MOV, asmcode::RBP, RBP_STORE);
  //code.add(asmcode::MOV, asmcode::R12, R12_STORE);
  //code.add(asmcode::MOV, asmcode::R13, R13_STORE);
  //code.add(asmcode::MOV, asmcode::R14, R14_STORE);
  code.add(asmcode::MOV, asmcode::R15, R15_STORE);
  }

void pop_ymm0(ASM::asmcode& code)
  {
  code.add(asmcode::VMOVAPS, asmcode::YMM0, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
  }

void pop_ymm1(ASM::asmcode& code)
  {
  code.add(asmcode::VMOVAPS, asmcode::YMM1, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
  }

void pop_ymm2(ASM::asmcode& code)
  {
  code.add(asmcode::VMOVAPS, asmcode::YMM2, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
  }

void pop_ymm3(ASM::asmcode& code)
  {
  code.add(asmcode::VMOVAPS, asmcode::YMM3, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
  }

void push_ymm0(ASM::asmcode& code)
  {
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -CELLS(4), asmcode::YMM0);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
  }

void push_ymm1(ASM::asmcode& code)
  {
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -CELLS(4), asmcode::YMM1);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
  }

void push_ymm2(ASM::asmcode& code)
  {
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -CELLS(4), asmcode::YMM2);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
  }

void push_ymm3(ASM::asmcode& code)
  {
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -CELLS(4), asmcode::YMM3);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
  }

void pop_rax(ASM::asmcode& code)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
  }

void pop_r11(ASM::asmcode& code)
  {
  code.add(asmcode::MOV, asmcode::R11, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
  }

void push_rax(ASM::asmcode& code)
  {
  code.add(asmcode::MOV, MEM_STACK_REGISTER, -CELLS(4), asmcode::RAX);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, CELLS(4));
  }

/*
void save_before_foreign_call(asmcode& code)
  {
  code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, CELLS(8));
  code.add(asmcode::MOV, asmcode::MEM_RSP, CELLS(0), asmcode::RBX);
  code.add(asmcode::MOV, asmcode::MEM_RSP, CELLS(1), asmcode::RCX);
  code.add(asmcode::MOV, asmcode::MEM_RSP, CELLS(2), asmcode::RDX);
  code.add(asmcode::MOV, asmcode::MEM_RSP, CELLS(3), asmcode::RSI);
  code.add(asmcode::MOV, asmcode::MEM_RSP, CELLS(4), asmcode::RDI);
  code.add(asmcode::MOV, asmcode::MEM_RSP, CELLS(5), asmcode::R8);
  code.add(asmcode::MOV, asmcode::MEM_RSP, CELLS(6), asmcode::R9);
  code.add(asmcode::MOV, asmcode::MEM_RSP, CELLS(7), asmcode::R10);
  }

void restore_after_foreign_call(asmcode& code)
  {
  code.add(asmcode::MOV, asmcode::R10, asmcode::MEM_RSP, CELLS(7));
  code.add(asmcode::MOV, asmcode::R9, asmcode::MEM_RSP, CELLS(6));
  code.add(asmcode::MOV, asmcode::R8, asmcode::MEM_RSP, CELLS(5));
  code.add(asmcode::MOV, asmcode::RDI, asmcode::MEM_RSP, CELLS(4));
  code.add(asmcode::MOV, asmcode::RSI, asmcode::MEM_RSP, CELLS(3));
  code.add(asmcode::MOV, asmcode::RDX, asmcode::MEM_RSP, CELLS(2));
  code.add(asmcode::MOV, asmcode::RCX, asmcode::MEM_RSP, CELLS(1));
  code.add(asmcode::MOV, asmcode::RBX, asmcode::MEM_RSP, CELLS(0));
  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, CELLS(8));
  }
  */
void align_stack(asmcode& code)
  {
  /*
  mov qword [rsp_save], rsp
  and rsp, [rsp_alignment_mask]
  */
  code.add(asmcode::MOV, RSP_SAVE, asmcode::RSP);
  code.add(asmcode::AND, asmcode::RSP, asmcode::NUMBER, 0xFFFFFFFFFFFFFFF0);
  }

void restore_stack(asmcode& code)
  {
  //mov rsp, qword [rsp_save]
  code.add(asmcode::MOV, asmcode::RSP, RSP_SAVE);
  }

VF_END