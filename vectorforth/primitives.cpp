#include "primitives.h"
#include "context_defs.h"
#include "asm_aux.h"
#include "avx_mathfun.h"

#include <cassert>

using namespace ASM;

VF_BEGIN



void primitive_addi(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE #+");
  code.add(asmcode::MOV, asmcode::R11, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::ADD, asmcode::RAX, asmcode::R11);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  code.add(asmcode::COMMENT, "END PRIMITIVE #+");
  }

void primitive_subi(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE #-");
  code.add(asmcode::MOV, asmcode::R11, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::SUB, asmcode::RAX, asmcode::R11);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  code.add(asmcode::COMMENT, "END PRIMITIVE #-");
  }

void primitive_muli(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE #*");
  code.add(asmcode::MOV, asmcode::R11, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::IMUL, asmcode::R11);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  code.add(asmcode::COMMENT, "END PRIMITIVE #*");
  }

void primitive_divi(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE #/");
  code.add(asmcode::MOV, asmcode::R11, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::XOR, asmcode::RDX, asmcode::RDX);
  code.add(asmcode::CQO);
  code.add(asmcode::IDIV, asmcode::R11);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  code.add(asmcode::COMMENT, "END PRIMITIVE #/");
  }

void primitive_shiftlefti(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE #<<");
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::SHL, asmcode::RAX, asmcode::CL);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  code.add(asmcode::COMMENT, "END PRIMITIVE #<<");
  }

void primitive_shiftrighti(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE #>>");
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::SHR, asmcode::RAX, asmcode::CL);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  code.add(asmcode::COMMENT, "END PRIMITIVE #>>");
  }

void primitive_add(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE +");
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE +");
  }

void primitive_sub(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE -");
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE -");
  }

void primitive_mul(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE *");
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE *");
  }

void primitive_complex_add(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE z+");
  //y0 y1 y2 y3

  // result y0 + y2  y1 + y3

  code.add(asmcode::VMOVAPS, AVX_REG3, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(3));

  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG1, AVX_REG1, AVX_REG3);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG1);
  code.add(asmcode::COMMENT, "END PRIMITIVE z+");
  }


void primitive_complex_sub(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE z-");
  //y0 y1 y2 y3

  // result y0 - y2  y1 - y3

  code.add(asmcode::VMOVAPS, AVX_REG3, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(3));

  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG3);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG1);
  code.add(asmcode::COMMENT, "END PRIMITIVE z-");
  }

void primitive_complex_mul(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE z*");
  //y0 y1 y2 y3

  // result y0y2 - y1y3  y0y3 + y1y2

  code.add(asmcode::VMOVAPS, AVX_REG3, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(3));

  code.add(asmcode::VMULPS, AVX_REG4, AVX_REG0, AVX_REG2); //y0y2
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG3); //y0y3
  code.add(asmcode::VMULPS, AVX_REG5, AVX_REG1, AVX_REG3); //y1y3
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG2); //y1y2

  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1); //y0y3 + y1y2
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG4, AVX_REG5); //y0y2 - y1y3

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE z*");
  }

void primitive_div(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE /");

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE /");
  }

void primitive_drop(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE drop");
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::COMMENT, "END PRIMITIVE drop");
  }

void primitive_swap(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE swap");
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG1);

  code.add(asmcode::COMMENT, "END PRIMITIVE swap");
  }

void primitive_dup(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE dup");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE dup");
  }

void primitive_over(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE over");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE over");
  }

void primitive_rot(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE rot");
  // a b c >> b c a

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(2));

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(2), AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG2);

  code.add(asmcode::COMMENT, "END PRIMITIVE rot");
  }

void primitive_minrot(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE -rot");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(2));

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(2), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG2);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG1);
  code.add(asmcode::COMMENT, "END PRIMITIVE -rot");
  }

void primitive_twodrop(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE 2drop");
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::COMMENT, "END PRIMITIVE 2drop");
  }

void primitive_twodup(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE 2dup");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(2), AVX_REG0);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::COMMENT, "END PRIMITIVE 2dup");
  }

void primitive_twoswap(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE 2swap");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG3, MEM_STACK_REGISTER, AVX_CELLS(3));

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG2);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG3);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(2), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(3), AVX_REG1);

  code.add(asmcode::COMMENT, "END PRIMITIVE 2swap");
  }

void primitive_sin(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE sin");
  code.add(asmcode::MOV, asmcode::R15, CONTEXT); // r15 should be saved by the callee but r10 not, so we save the context in r15

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

#ifdef _WIN32
  code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, 32);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, (uint64_t)&sin_avx_ps);
#else
  code.add(asmcode::XOR, asmcode::RAX, asmcode::RAX);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, (uint64_t)&sin_avx_ps);
#endif  
  code.add(asmcode::CALL, asmcode::R11);

  code.add(asmcode::MOV, CONTEXT, asmcode::R15); // restore context

#ifdef _WIN32
  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, 32);
#endif

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE sin");
  }

void primitive_cos(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE cos");
  code.add(asmcode::MOV, asmcode::R15, CONTEXT); // r15 should be saved by the callee but r10 not, so we save the context in r15

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

#ifdef _WIN32
  code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, 32);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, (uint64_t)&cos_avx_ps);
#else
  code.add(asmcode::XOR, asmcode::RAX, asmcode::RAX);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, (uint64_t)&cos_avx_ps);
#endif  
  code.add(asmcode::CALL, asmcode::R11);

  code.add(asmcode::MOV, CONTEXT, asmcode::R15); // restore context
#ifdef _WIN32
  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, 32);
#endif

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE cos");
  }

void primitive_tan(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE tan");
  code.add(asmcode::MOV, asmcode::R15, CONTEXT); // r15 should be saved by the callee but r10 not, so we save the context in r15

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

#ifdef _WIN32
  code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, 32);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, (uint64_t)&tan_avx_ps);
#else
  code.add(asmcode::XOR, asmcode::RAX, asmcode::RAX);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, (uint64_t)&tan_avx_ps);
#endif  
  code.add(asmcode::CALL, asmcode::R11);

  code.add(asmcode::MOV, CONTEXT, asmcode::R15); // restore context
#ifdef _WIN32
  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, 32);
#endif

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE tan");
  }

void primitive_exp(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE exp");
  code.add(asmcode::MOV, asmcode::R15, CONTEXT); // r15 should be saved by the callee but r10 not, so we save the context in r15

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

#ifdef _WIN32
  code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, 32);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, (uint64_t)&exp_avx_ps);
#else
  code.add(asmcode::XOR, asmcode::RAX, asmcode::RAX);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, (uint64_t)&exp_avx_ps);
#endif  
  code.add(asmcode::CALL, asmcode::R11);

  code.add(asmcode::MOV, CONTEXT, asmcode::R15); // restore context
#ifdef _WIN32
  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, 32);
#endif

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE exp");
  }

void primitive_log(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE log");
  code.add(asmcode::MOV, asmcode::R15, CONTEXT); // r15 should be saved by the callee but r10 not, so we save the context in r15

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

#ifdef _WIN32
  code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, 32);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, (uint64_t)&log_avx_ps);
#else
  code.add(asmcode::XOR, asmcode::RAX, asmcode::RAX);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, (uint64_t)&log_avx_ps);
#endif  
  code.add(asmcode::CALL, asmcode::R11);

  code.add(asmcode::MOV, CONTEXT, asmcode::R15); // restore context
#ifdef _WIN32
  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, 32);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE log");
  }

void primitive_pow(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE **");
  code.add(asmcode::MOV, asmcode::R15, CONTEXT); // r15 should be saved by the callee but r10 not, so we save the context in r15

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));

#ifdef _WIN32
  code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, 32);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, (uint64_t)&pos_avx_ps);
#else
  code.add(asmcode::XOR, asmcode::RAX, asmcode::RAX);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, (uint64_t)&pos_avx_ps);
#endif  
  code.add(asmcode::CALL, asmcode::R11);

  code.add(asmcode::MOV, CONTEXT, asmcode::R15); // restore context
#ifdef _WIN32
  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, 32);
#endif

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE **");
  }

void primitive_atan2(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE atan2");
  code.add(asmcode::MOV, asmcode::R15, CONTEXT); // r15 should be saved by the callee but r10 not, so we save the context in r15

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));

#ifdef _WIN32
  code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, 32);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, (uint64_t)&atan2_avx_ps);
#else
  code.add(asmcode::XOR, asmcode::RAX, asmcode::RAX);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, (uint64_t)&atan2_avx_ps);
#endif  
  code.add(asmcode::CALL, asmcode::R11);

  code.add(asmcode::MOV, CONTEXT, asmcode::R15); // restore context
#ifdef _WIN32
  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, 32);
#endif

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE atan2");
  }

void primitive_sqrt(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE sqrt");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VSQRTPS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE sqrt");
  }

void primitive_abs(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE abs");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  //code.add(asmcode::MOV, asmcode::EAX, asmcode::NUMBER, 0x7fffffff);
  //code.add(asmcode::MOVD, asmcode::XMM1, asmcode::EAX);
  //code.add(asmcode::VBROADCASTSS, AVX_REG1, asmcode::XMM1);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE abs");
  }

void primitive_negate(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE NEGATE");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, SIGN_BIT);
  //code.add(asmcode::MOV, asmcode::EAX, asmcode::NUMBER, 0x80000000);
  //code.add(asmcode::MOVD, asmcode::XMM1, asmcode::EAX);
  //code.add(asmcode::VBROADCASTSS, AVX_REG1, asmcode::XMM1);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE NEGATE");
  }

void primitive_and(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE and");
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE and");
  }

void primitive_or(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE or");
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VORPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE or");
  }

void primitive_xor(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE xor");
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE xor");
  }

void primitive_not(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE not");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  //code.add(asmcode::MOV, asmcode::EAX, asmcode::NUMBER, 0xffffffff);
  //code.add(asmcode::MOVD, asmcode::XMM1, asmcode::EAX);
  //code.add(asmcode::VBROADCASTSS, AVX_REG1, asmcode::XMM1);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE not");
  }

void primitive_equ(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE =");
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
#endif
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE =");
  }

void primitive_nequ(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE <>");

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 4);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 4);
#endif
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE <>");
  }

void primitive_lt(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE <");
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1);
#endif
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE <");
  }

void primitive_gt(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE >");

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1);
#endif
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE >");
  }

void primitive_le(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE <=");
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 2);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 2);
#endif
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE <=");
  }

void primitive_ge(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE >=");

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2);
#endif
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE >=");
  }

void primitive_fequ(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE f=");
  //1.f if a is equal to b, else 0.f
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  //code.add(asmcode::MOV, asmcode::EAX, asmcode::NUMBER, 0x7fffffff);
  //code.add(asmcode::MOVD, asmcode::XMM1, asmcode::EAX);
  //code.add(asmcode::VBROADCASTSS, AVX_REG1, asmcode::XMM1);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE f=");
  }

void primitive_fnequ(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE f<>");
  //1.f if a is not equal to b, else 0.f
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 4);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 4);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  //code.add(asmcode::MOV, asmcode::EAX, asmcode::NUMBER, 0x7fffffff);
  //code.add(asmcode::MOVD, asmcode::XMM1, asmcode::EAX);
  //code.add(asmcode::VBROADCASTSS, AVX_REG1, asmcode::XMM1);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE f<>");
  }

void primitive_flt(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE f<");
  //1.f if a is less than b, else 0.f
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  //code.add(asmcode::MOV, asmcode::EAX, asmcode::NUMBER, 0x7fffffff);
  //code.add(asmcode::MOVD, asmcode::XMM1, asmcode::EAX);
  //code.add(asmcode::VBROADCASTSS, AVX_REG1, asmcode::XMM1);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE f<");
  }

void primitive_fgt(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE f>");
  //1.f if a is greater than b, else 0.f
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  //code.add(asmcode::MOV, asmcode::EAX, asmcode::NUMBER, 0x7fffffff);
  //code.add(asmcode::MOVD, asmcode::XMM1, asmcode::EAX);
  //code.add(asmcode::VBROADCASTSS, AVX_REG1, asmcode::XMM1);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE f>");
  }

void primitive_fle(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE f<=");
  //1.f if a is less than or equal to b, else 0.f
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 2);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 2);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  //code.add(asmcode::MOV, asmcode::EAX, asmcode::NUMBER, 0x7fffffff);
  //code.add(asmcode::MOVD, asmcode::XMM1, asmcode::EAX);
  //code.add(asmcode::VBROADCASTSS, AVX_REG1, asmcode::XMM1);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE f<=");
  }

void primitive_fge(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE f>=");
  //1.f if a is greater than or equal to b, else 0.f
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  //code.add(asmcode::MOV, asmcode::EAX, asmcode::NUMBER, 0x7fffffff);
  //code.add(asmcode::MOVD, asmcode::XMM1, asmcode::EAX);
  //code.add(asmcode::VBROADCASTSS, AVX_REG1, asmcode::XMM1);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE f>=");
  }

void primitive_stack_top_fetch(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE st@");
  code.add(asmcode::MOV, asmcode::RAX, STACK_TOP);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  code.add(asmcode::COMMENT, "END PRIMITIVE st@");
  }

void primitive_stack_pointer_fetch(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE sp@");
  code.add(asmcode::MOV, asmcode::RAX, STACK_REGISTER);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  code.add(asmcode::COMMENT, "END PRIMITIVE sp@");
  }

void primitive_fetch(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE @");
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE @");
  }

void primitive_store(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE !");
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE !");
  }

void primitive_addstorei(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE #+!");
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::ADD, asmcode::MEM_RAX, asmcode::RCX);
  code.add(asmcode::COMMENT, "END PRIMITIVE #+!");
  }

void primitive_substorei(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE #-!");
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::SUB, asmcode::MEM_RAX, asmcode::RCX);
  code.add(asmcode::COMMENT, "END PRIMITIVE #-!");
  }

void primitive_here(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE here");
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, HERE);
  code.add(asmcode::COMMENT, "END PRIMITIVE here");
  }

void primitive_comma(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE ,");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, asmcode::RAX, MEM_HERE);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
#ifdef AVX512
  code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, 64);
#else
  code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, 32);
#endif
  code.add(asmcode::MOV, MEM_HERE, asmcode::RAX);
  code.add(asmcode::COMMENT, "END PRIMITIVE ,");
  }

void primitive_floor(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE floor");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
#ifdef AVX512
  code.add(asmcode::VRNDSCALEPS, AVX_REG0, AVX_REG0, asmcode::NUMBER, 1 + 8);
#else
  code.add(asmcode::VROUNDPS, AVX_REG0, AVX_REG0, asmcode::NUMBER, 1);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE floor");
  }

void primitive_ceil(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE ceil");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
#ifdef AVX512
  code.add(asmcode::VRNDSCALEPS, AVX_REG0, AVX_REG0, asmcode::NUMBER, 2 + 8);
#else
  code.add(asmcode::VROUNDPS, AVX_REG0, AVX_REG0, asmcode::NUMBER, 2);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE ceil");
  }

void primitive_round(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE round");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
#ifdef AVX512
  code.add(asmcode::VRNDSCALEPS, AVX_REG0, AVX_REG0, asmcode::NUMBER, 0 + 8);
#else
  code.add(asmcode::VROUNDPS, AVX_REG0, AVX_REG0, asmcode::NUMBER, 0);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE round");
  }

void primitive_truncate(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE trunc");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
#ifdef AVX512
  code.add(asmcode::VRNDSCALEPS, AVX_REG0, AVX_REG0, asmcode::NUMBER, 3 + 8);
#else
  code.add(asmcode::VROUNDPS, AVX_REG0, AVX_REG0, asmcode::NUMBER, 3);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE trunc");
  }

void primitive_fcast(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE fcast");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE fcast");
  }

void primitive_icast(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE icast");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VCVTPS2DQ, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE ADD");
  }

void primitive_fmod(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE fm/mod");
  /*
  (d1 n1 -- n2 n3)
  Divide d1 by n1, giving the floored quotient n3 and the remainder n2. Input and output stack arguments are signed.
  An ambiguous condition exists if n1 is zero or if the quotient lies outside the range of a single-cell signed integer.
  */

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER); // n1
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1)); // d1
  code.add(asmcode::VDIVPS, AVX_REG2, AVX_REG0, AVX_REG1); // quotient
#ifdef AVX512
  code.add(asmcode::VRNDSCALEPS, AVX_REG2, AVX_REG2, asmcode::NUMBER, 8 + 1); // floored quotient n3
#else
  code.add(asmcode::VROUNDPS, AVX_REG2, AVX_REG2, asmcode::NUMBER, 1); // floored quotient n3
#endif
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG2, AVX_REG1); // floored quotient n3 times n1
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1); // remainder n2
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG2);
  code.add(asmcode::COMMENT, "END PRIMITIVE fm/mod");
  }

void primitive_min(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE min");
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMINPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE min");
  }

void primitive_max(asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE max");
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMAXPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE max");
  }

void primitive_return_stack_push(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE >r");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, asmcode::MEM_RSP, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE >r");
  }

void primitive_return_stack_pop(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE r>");
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RSP);
  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE r>");
  }

void primitive_return_stack_top(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE r@");
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RSP);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE r@");
  }

void primitive_return_stack_top_fetch(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE rt@");
  code.add(asmcode::MOV, asmcode::RAX, RSP_TOP);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  code.add(asmcode::COMMENT, "END PRIMITIVE rt@");
  }

void primitive_return_stack_pointer_fetch(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE rp@");
  code.add(asmcode::MOV, asmcode::RAX, asmcode::RSP);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  code.add(asmcode::COMMENT, "END PRIMITIVE rp@");
  }

void primitive_if(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE if");
  cd.else_label.push_back(label_to_string(cd.label++));
  cd.then_label.push_back(label_to_string(cd.label++));

  //code.add(asmcode::VMOVMSKPS, asmcode::RAX, AVX_REG1); // if rax == 0, then all conditions are false, if rax == 255, all conditions are true
  //code.add(asmcode::TEST, asmcode::RAX, asmcode::RAX);
  //code.add(asmcode::JE, loop_end); // if all false, the true loop is not necessary

  // push test condition to return stack
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, asmcode::MEM_RSP, AVX_CELLS(1), AVX_REG0);
  // push stack register on return stack
  code.add(asmcode::MOV, asmcode::MEM_RSP, STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VXORPS, AVX_REG1, AVX_REG1, AVX_REG1);
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
  code.add(asmcode::KMOVW, asmcode::EAX, asmcode::K1);
  //code.add(asmcode::TEST, asmcode::EAX, asmcode::EAX);
  code.add(asmcode::CMP, asmcode::EAX, asmcode::NUMBER, 0xffff);
#else
  code.add(asmcode::VMOVMSKPS, asmcode::RAX, AVX_REG0); // if rax == 0, then all conditions are false, if rax == 255, all conditions are true
  code.add(asmcode::TEST, asmcode::RAX, asmcode::RAX);
#endif
  code.add(asmcode::JE, cd.else_label.back()); // if rax == 0, skip to the end of the else code  
  code.add(asmcode::COMMENT, "END PRIMITIVE if");
  }

void primitive_then(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE then");
  /*
  return stack looks like this:
  true branch value a, true branch value b, ..., test condition , address of stack register before branch
  */

  auto loop = label_to_string(cd.label++);
  auto loop_end = label_to_string(cd.label++);

  code.add(asmcode::MOV, asmcode::RCX, asmcode::MEM_RSP); // address of stack register before branch
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RSP, AVX_CELLS(1)); // test condition
  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(2));

#ifdef AVX512
  code.add(asmcode::VXORPS, AVX_REG2, AVX_REG2, AVX_REG2);
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG2, asmcode::NUMBER, 0);
  code.add(asmcode::KMOVW, asmcode::EAX, asmcode::K1);
  //code.add(asmcode::TEST, asmcode::EAX, asmcode::EAX);
  code.add(asmcode::CMP, asmcode::EAX, asmcode::NUMBER, 0xffff);
#else
  code.add(asmcode::VMOVMSKPS, asmcode::RAX, AVX_REG1); // if rax == 0, then all conditions are false, if rax == 255, all conditions are true
  code.add(asmcode::TEST, asmcode::RAX, asmcode::RAX);
#endif
  code.add(asmcode::JE, loop_end);

  code.add(asmcode::MOV, asmcode::RAX, STACK_REGISTER); // address of current stack register in rax
  code.add(asmcode::SUB, asmcode::RCX, asmcode::RAX);
#ifdef AVX512
  code.add(asmcode::SHR, asmcode::RCX, asmcode::NUMBER, 6); // divide rcx by 64: now rcx contains number of items added on the stack
#else
  code.add(asmcode::SHR, asmcode::RCX, asmcode::NUMBER, 5); // divide rcx by 32: now rcx contains number of items added on the stack
#endif

  code.add(asmcode::MOV, asmcode::RDX, STACK_REGISTER);

  code.add(asmcode::LABEL, loop);
  code.add(asmcode::TEST, asmcode::RCX, asmcode::RCX);
  code.add(asmcode::JES, loop_end);
  code.add(asmcode::DEC, asmcode::RCX);
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RDX);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RSP);

  //_mm_or_ps(_mm_and_ps(mask.m128, updated_values.m128), _mm_andnot_ps(mask.m128, original.m128));
  code.add(asmcode::VANDPS, AVX_REG3, AVX_REG1, AVX_REG0);
  code.add(asmcode::VANDNPS, AVX_REG4, AVX_REG1, AVX_REG2);
  code.add(asmcode::VORPS, AVX_REG0, AVX_REG3, AVX_REG4);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RDX, AVX_REG0);

  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::ADD, asmcode::RDX, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::JMPS, loop);
  code.add(asmcode::LABEL, loop_end);

  code.add(asmcode::LABEL, cd.then_label.back());
  cd.then_label.pop_back();
  code.add(asmcode::COMMENT, "END PRIMITIVE then");
  }

void primitive_else(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE else");
  /*
  return stack looks like this:
  test condition , address of stack register before branch
  */

  auto loop = label_to_string(cd.label++);
  auto loop_end = label_to_string(cd.label++);

  code.add(asmcode::MOV, asmcode::RCX, asmcode::MEM_RSP); // address of stack register before branch
  code.add(asmcode::MOV, asmcode::RDX, asmcode::RCX); // save in rdx for further reference
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RSP, AVX_CELLS(1)); // test condition
  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(2));

#ifdef AVX512
  code.add(asmcode::VXORPS, AVX_REG2, AVX_REG2, AVX_REG2);
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG2, asmcode::NUMBER, 0);
  code.add(asmcode::KMOVW, asmcode::EAX, asmcode::K1);
  code.add(asmcode::CMP, asmcode::EAX, asmcode::NUMBER, 0xffff);
#else
  code.add(asmcode::VMOVMSKPS, asmcode::RAX, AVX_REG1); // if rax == 0, then all conditions are false, if rax == 255, all conditions are true
  code.add(asmcode::CMP, asmcode::RAX, asmcode::NUMBER, 255);
#endif
  code.add(asmcode::JE, cd.then_label.back());

  code.add(asmcode::MOV, asmcode::RAX, STACK_REGISTER); // address of current stack register in rax
  code.add(asmcode::SUB, asmcode::RCX, asmcode::RAX);
#ifdef AVX512
  code.add(asmcode::SHR, asmcode::RCX, asmcode::NUMBER, 6); // divide rcx by 64: now rcx contains number of items added on the stack  
#else
  code.add(asmcode::SHR, asmcode::RCX, asmcode::NUMBER, 5); // divide rcx by 32: now rcx contains number of items added on the stack  
#endif

  code.add(asmcode::MOV, STACK_REGISTER, asmcode::RDX); // the stack should point to the same location as before we did the if branch
  code.add(asmcode::SUB, asmcode::RDX, asmcode::NUMBER, AVX_CELLS(1)); // rdx now points to first item added to the stack by the if branch

  code.add(asmcode::LABEL, loop);
  code.add(asmcode::TEST, asmcode::RCX, asmcode::RCX);
  code.add(asmcode::JES, loop_end);
  code.add(asmcode::DEC, asmcode::RCX);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RDX); // save stack item pointed to by rdx on the return stack 
  code.add(asmcode::SUB, asmcode::RDX, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, asmcode::MEM_RSP, AVX_REG0);
  code.add(asmcode::JMPS, loop);
  code.add(asmcode::LABEL, loop_end);


  // stack is now as before the if branch. Data is temporarily stored on the return stack

  code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, asmcode::MEM_RSP, AVX_CELLS(1), AVX_REG1); // test condition
  // push stack register on return stack
  code.add(asmcode::MOV, asmcode::MEM_RSP, STACK_REGISTER); // stack address 

  code.add(asmcode::LABEL, cd.else_label.back());
  cd.else_label.pop_back();

  code.add(asmcode::COMMENT, "END PRIMITIVE else");
  }

void primitive_begin(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE begin");
  cd.begin_label.push_back(label_to_string(cd.label++));
  cd.repeat_label.push_back(label_to_string(cd.label++));
  code.add(asmcode::LABEL, cd.begin_label.back());
  code.add(asmcode::COMMENT, "END PRIMITIVE begin");
  }

void primitive_while(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE while");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER); // pop ymm0 from stack
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));

  /*
  The begin while repeat test only fails if it fails for all values in the simd vector
  */

#ifdef AVX512
  code.add(asmcode::VXORPS, AVX_REG1, AVX_REG1, AVX_REG1);
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
  code.add(asmcode::KMOVW, asmcode::EAX, asmcode::K1);
  //code.add(asmcode::TEST, asmcode::EAX, asmcode::EAX);
  code.add(asmcode::CMP, asmcode::EAX, asmcode::NUMBER, 0xffff);
#else
  code.add(asmcode::VMOVMSKPS, asmcode::RAX, AVX_REG0); // if rax == 0, then all conditions are false
  code.add(asmcode::TEST, asmcode::RAX, asmcode::RAX);
#endif
  code.add(asmcode::JE, cd.repeat_label.back()); // if rax == 0, skip to the end of the begin while repeat code, otherwise continue
  code.add(asmcode::COMMENT, "END PRIMITIVE while");
  }

void primitive_repeat(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE repeat");
  code.add(asmcode::JMP, cd.begin_label.back());
  code.add(asmcode::LABEL, cd.repeat_label.back());
  cd.begin_label.pop_back();
  cd.repeat_label.pop_back();
  code.add(asmcode::COMMENT, "END PRIMITIVE repeat");
  }

void primitive_clamp(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE clamp");
  // c a b clamp returns c if a < c and c < b, or a if c < a, or b if c > b
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER); // b
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1)); // a
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2)); // c

#ifdef AVX512
  code.add(asmcode::VXORPS, AVX_REG3, AVX_REG3, AVX_REG3);
  code.add(asmcode::VXORPS, AVX_REG4, AVX_REG4, AVX_REG4);
  code.add(asmcode::VXORPS, AVX_REG5, AVX_REG5, AVX_REG5);
  code.add(asmcode::VXORPS, AVX_REG6, AVX_REG6, AVX_REG6);
  code.add(asmcode::VMOVAPS, AVX_REG7, ONEF_BITS);
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2); // a <= c
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::k1, AVX_REG7);

  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG2, asmcode::NUMBER, 2); // c <= b
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::k1, AVX_REG7);

  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1); // c < a
  code.add(asmcode::VMOVAPS, AVX_REG5, asmcode::k1, AVX_REG7);

  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG2, AVX_REG0, asmcode::NUMBER, 1); // b < c
  code.add(asmcode::VMOVAPS, AVX_REG6, asmcode::k1, AVX_REG7);

#else
  code.add(asmcode::VCMPPS, AVX_REG3, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2); // a <= c
  code.add(asmcode::VCMPPS, AVX_REG4, AVX_REG0, AVX_REG2, asmcode::NUMBER, 2); // c <= b
  code.add(asmcode::VCMPPS, AVX_REG5, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1); // c < a
  code.add(asmcode::VCMPPS, AVX_REG6, AVX_REG2, AVX_REG0, asmcode::NUMBER, 1); // b < c

  code.add(asmcode::VCVTDQ2PS, AVX_REG3, AVX_REG3);
  code.add(asmcode::VCVTDQ2PS, AVX_REG4, AVX_REG4);
  code.add(asmcode::VCVTDQ2PS, AVX_REG5, AVX_REG5);
  code.add(asmcode::VCVTDQ2PS, AVX_REG6, AVX_REG6);

  code.add(asmcode::VMOVAPS, AVX_REG7, NOT_SIGN_BIT);
  //code.add(asmcode::MOV, asmcode::EAX, asmcode::NUMBER, 0x7fffffff);
  //code.add(asmcode::MOVD, asmcode::XMM7, asmcode::EAX);
  //code.add(asmcode::VBROADCASTSS, AVX_REG7, asmcode::XMM7);  
  code.add(asmcode::VANDPS, AVX_REG3, AVX_REG3, AVX_REG7);
  code.add(asmcode::VANDPS, AVX_REG4, AVX_REG4, AVX_REG7);
  code.add(asmcode::VANDPS, AVX_REG5, AVX_REG5, AVX_REG7);
  code.add(asmcode::VANDPS, AVX_REG6, AVX_REG6, AVX_REG7);
#endif

  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG4); // a <= c && c <= b

  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG5, AVX_REG5, AVX_REG1);
  code.add(asmcode::VMULPS, AVX_REG6, AVX_REG6, AVX_REG2);

  code.add(asmcode::VORPS, AVX_REG0, AVX_REG3, AVX_REG5);
  code.add(asmcode::VORPS, AVX_REG0, AVX_REG0, AVX_REG6);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END PRIMITIVE clamp");
  }

void primitive_within(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE within");
  //(c a b within returns true if a <= c and c < b)
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER); // b
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1)); // a
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2)); // c
#ifdef AVX512
  code.add(asmcode::VMOVAPS, AVX_REG5, ALL_BITS);
  code.add(asmcode::VXORPS, AVX_REG3, AVX_REG3, AVX_REG3);
  code.add(asmcode::VXORPS, AVX_REG4, AVX_REG4, AVX_REG4);

  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2); // a <= c    
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::k1, AVX_REG5);
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG2, asmcode::NUMBER, 1); // c < b
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::k1, AVX_REG5);

#else
  code.add(asmcode::VCMPPS, AVX_REG3, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2); // a <= c
  code.add(asmcode::VCMPPS, AVX_REG4, AVX_REG0, AVX_REG2, asmcode::NUMBER, 1); // c < b
#endif
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG3, AVX_REG4);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE within");
  }

void primitive_mat2x2_store(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE mat2x2!");
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER); // address
  code.add(asmcode::VMOVAPS, AVX_REG3, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(4));
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(2), AVX_REG2);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(3), AVX_REG3);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(5));
  code.add(asmcode::COMMENT, "END PRIMITIVE mat2x2!");
  }

void primitive_mat3x3_store(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE mat3x3!");
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER); // address
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(9));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(8));  
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(7));
  code.add(asmcode::VMOVAPS, AVX_REG3, MEM_STACK_REGISTER, AVX_CELLS(6));  
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(2), AVX_REG2);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(3), AVX_REG3);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(5));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(4));
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG3, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG4, MEM_STACK_REGISTER, AVX_CELLS(1));        
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(4), AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(5), AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(6), AVX_REG2);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(7), AVX_REG3);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(8), AVX_REG4);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(10));
  code.add(asmcode::COMMENT, "END PRIMITIVE mat3x3!");
  }

void primitive_mul2x2(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE mul2x2");
  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER); // address target vec2
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address vec2
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(2)); // address mat2x2
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX, AVX_CELLS(1));

  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG3);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_REG0);

  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(3));
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG3);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(1), AVX_REG0);

  code.add(asmcode::COMMENT, "END PRIMITIVE mul2x2");
  }

void primitive_mul3x3(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE mul3x3");

  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER); // address target vec3
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address vec3
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(2)); // address mat3x3
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(6));
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG5, asmcode::MEM_RCX, AVX_CELLS(2));

  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG3);
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG4);
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG2, AVX_REG5);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_REG0);

  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(4));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(7));

  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG3);
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG4);
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG2, AVX_REG5);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(1), AVX_REG0);

  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(5));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(8));

  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG3);
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG4);
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG2, AVX_REG5);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(2), AVX_REG0);

  code.add(asmcode::COMMENT, "END PRIMITIVE mul3x3");
  }

void primitive_vec2_store(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE vec2!");
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER); // address
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::COMMENT, "END PRIMITIVE vec2!");
  }

void primitive_dot2(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE dot2");
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER); // address first vec2
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address second vec2
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RCX);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE dot2");
  }

void primitive_add2(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE add2");
  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER); // address target vec3
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address second vec3
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(2)); // address first vec3
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG1, AVX_REG1, AVX_REG3);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::COMMENT, "END PRIMITIVE add2");
  }

void primitive_sub2(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE sub2");
  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER); // address target vec3
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address second vec3
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(2)); // address first vec3
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG3);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::COMMENT, "END PRIMITIVE sub2");
  }

void primitive_mul2(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE mul2");
  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER); // address target vec3
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address second vec3
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(2)); // address first vec3
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG3);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::COMMENT, "END PRIMITIVE mul2");
  }

void primitive_div2(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE div2");
  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER); // address target vec3
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address second vec3
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(2)); // address first vec3
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VDIVPS, AVX_REG1, AVX_REG1, AVX_REG3);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::COMMENT, "END PRIMITIVE div2");
  }

void primitive_scalarmul2(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE scalarmul2");
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER); // address target vec3
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address input vec3
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2)); // scalar
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG2, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_CELLS(1), AVX_REG2);
  code.add(asmcode::COMMENT, "END PRIMITIVE scalarmul2");
  }

void primitive_length2(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE length2");
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER); // address vec3  
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG1);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VSQRTPS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE length2");
  }

void primitive_normalize2(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE normalize2");
  //(normalize a vec2 (&v &result => -), where result contains the normalized vec2 v)
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER); // address target vec3
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address input vec3
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG1, AVX_REG1);
  code.add(asmcode::VADDPS, AVX_REG2, AVX_REG2, AVX_REG3);
  code.add(asmcode::VSQRTPS, AVX_REG2, AVX_REG2);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VDIVPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::COMMENT, "END PRIMITIVE normalize2");
  }

void primitive_vec3_store(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE vec3!");
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER); // address
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(2), AVX_REG2);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(4));
  code.add(asmcode::COMMENT, "END PRIMITIVE vec3!");
  }

void primitive_dot3(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE dot3");
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER); // address first vec3
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address second vec3
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RCX);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RCX, AVX_CELLS(2));
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE dot3");
  }

void primitive_cross3(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE cross3");
  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER); // address target vec3
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address second vec3
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(2)); // address first vec3
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG5, asmcode::MEM_RCX, AVX_CELLS(2));
  code.add(asmcode::VMULPS, AVX_REG6, AVX_REG1, AVX_REG5); //a_y*b_z
  code.add(asmcode::VMULPS, AVX_REG7, AVX_REG2, AVX_REG4); //a_z*b_y
  code.add(asmcode::VSUBPS, AVX_REG6, AVX_REG6, AVX_REG7); //a_y*b_z-a_z*b_y
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_REG6);
  code.add(asmcode::VMULPS, AVX_REG6, AVX_REG2, AVX_REG3); //a_z*b_x
  code.add(asmcode::VMULPS, AVX_REG7, AVX_REG0, AVX_REG5); //a_x*b_z
  code.add(asmcode::VSUBPS, AVX_REG6, AVX_REG6, AVX_REG7); //a_z*b_x-a_x*b_z
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(1), AVX_REG6);
  code.add(asmcode::VMULPS, AVX_REG6, AVX_REG0, AVX_REG4); //a_x*b_y
  code.add(asmcode::VMULPS, AVX_REG7, AVX_REG1, AVX_REG3); //a_y*b_x
  code.add(asmcode::VSUBPS, AVX_REG6, AVX_REG6, AVX_REG7); //a_x*b_y - a_y*b_x
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(2), AVX_REG6);
  code.add(asmcode::COMMENT, "END PRIMITIVE cross3");
  }

void primitive_add3(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE add3");
  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER); // address target vec3
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address second vec3
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(2)); // address first vec3
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG5, asmcode::MEM_RCX, AVX_CELLS(2));
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG3);
  code.add(asmcode::VADDPS, AVX_REG1, AVX_REG1, AVX_REG4);
  code.add(asmcode::VADDPS, AVX_REG2, AVX_REG2, AVX_REG5);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(2), AVX_REG2);
  code.add(asmcode::COMMENT, "END PRIMITIVE add3");
  }

void primitive_sub3(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE sub3");
  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER); // address target vec3
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address second vec3
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(2)); // address first vec3
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG5, asmcode::MEM_RCX, AVX_CELLS(2));
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG3);
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG4);
  code.add(asmcode::VSUBPS, AVX_REG2, AVX_REG2, AVX_REG5);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(2), AVX_REG2);
  code.add(asmcode::COMMENT, "END PRIMITIVE sub3");
  }

void primitive_mul3(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE mul3");
  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER); // address target vec3
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address second vec3
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(2)); // address first vec3
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG5, asmcode::MEM_RCX, AVX_CELLS(2));
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG3);
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG4);
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG2, AVX_REG5);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(2), AVX_REG2);
  code.add(asmcode::COMMENT, "END PRIMITIVE mul3");
  }

void primitive_div3(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE div3");
  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER); // address target vec3
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address second vec3
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(2)); // address first vec3
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG5, asmcode::MEM_RCX, AVX_CELLS(2));
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG3);
  code.add(asmcode::VDIVPS, AVX_REG1, AVX_REG1, AVX_REG4);
  code.add(asmcode::VDIVPS, AVX_REG2, AVX_REG2, AVX_REG5);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(2), AVX_REG2);
  code.add(asmcode::COMMENT, "END PRIMITIVE div3");
  }

void primitive_scalarmul3(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE scalarmul3");
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER); // address target vec3
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address input vec3
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2)); // scalar
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG2, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_CELLS(1), AVX_REG2);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_CELLS(2), AVX_REG3);
  code.add(asmcode::COMMENT, "END PRIMITIVE scalarmul3");
  }

void primitive_length3(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE length3");
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER); // address vec3  
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG1);
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG2, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VSQRTPS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE length3");
  }

void primitive_normalize3(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE normalize3");
  //(normalize a vec3 (&v &result => -), where result contains the normalized vec3 v)
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER); // address target vec3
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address input vec3
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG4, AVX_REG1, AVX_REG1);
  code.add(asmcode::VMULPS, AVX_REG5, AVX_REG2, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG3, AVX_REG3, AVX_REG4);
  code.add(asmcode::VADDPS, AVX_REG3, AVX_REG3, AVX_REG5);
  code.add(asmcode::VSQRTPS, AVX_REG3, AVX_REG3);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG3);
  code.add(asmcode::VDIVPS, AVX_REG1, AVX_REG1, AVX_REG3);
  code.add(asmcode::VDIVPS, AVX_REG2, AVX_REG2, AVX_REG3);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_CELLS(2), AVX_REG2);
  code.add(asmcode::COMMENT, "END PRIMITIVE normalize3");
  }

void primitive_reflect3(ASM::asmcode& code, compile_data& cd)
  {
  /*
  reflect (&i &n &result => - )
  result = i - 2.0 * dot(n, i) * n
  */

  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE reflect3");
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1)); // address n
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(2)); // i
  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER); // result
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RCX);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RCX, AVX_CELLS(2));
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1); // dot(n, i) in avx_reg0

  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG0); // 2dot(n,i) in avx_reg0

  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX); // nx
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(1)); // ny
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RAX, AVX_CELLS(2)); // nz
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG0); // 2dot(n, i) * nx
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG2, AVX_REG0); // 2dot(n, i) * ny
  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG0); // 2dot(n, i) * nz

  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RCX); // ix
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1); // ix - 2dot(n, i) * nx
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_REG0);

  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RCX, AVX_CELLS(1)); // iy
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG2); // iy - 2dot(n, i) * ny
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(1), AVX_REG0);

  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RCX, AVX_CELLS(2)); // iz
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG3); // iz - 2dot(n, i) * nz
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(2), AVX_REG0);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));

  code.add(asmcode::COMMENT, "END PRIMITIVE reflect3");
  }

void primitive_mix(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE mix");
  /*
float mix(float x, float y, float a)
              {
              return x * (1 - a) + y * a;
              }
*/
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER); // a
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1)); // y
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(2)); // x
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG3, ONEF_BITS);
  code.add(asmcode::VSUBPS, AVX_REG3, AVX_REG3, AVX_REG0); // 1-a
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG2, AVX_REG3); // x*(1-a)
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG0); // y*a
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG1, AVX_REG2);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE mix");
  }

void primitive_mix3(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE mix3");
  /*
float mix3(vec3 x, vec3 y, float a)
              {
              return x * (1 - a) + y * a;
              }
*/

  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER); // result
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1)); // a
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(2)); // y
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(3)); // x
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(4));


  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG0); // 1-a

  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX); // x_x
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX); // y_x
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG1, AVX_REG2); // x_x (1-a)
  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG0, AVX_REG3); // y_x a
  code.add(asmcode::VADDPS, AVX_REG2, AVX_REG2, AVX_REG3);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_REG2);

  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(1)); // x_y
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX, AVX_CELLS(1)); // y_y
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG1, AVX_REG2); // x_y (1-a)
  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG0, AVX_REG3); // y_y a
  code.add(asmcode::VADDPS, AVX_REG2, AVX_REG2, AVX_REG3);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(1), AVX_REG2);

  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(2)); // x_z
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX, AVX_CELLS(2)); // y_z
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG1, AVX_REG2); // x_z (1-a)
  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG0, AVX_REG3); // y_z a
  code.add(asmcode::VADDPS, AVX_REG2, AVX_REG2, AVX_REG3);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(2), AVX_REG2);

  code.add(asmcode::COMMENT, "END PRIMITIVE mix3");
  }

void primitive_mix2(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE mix2");
  /*
float mix2(vec2 x, vec2 y, float a)
              {
              return x * (1 - a) + y * a;
              }
*/

  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER); // result
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1)); // a
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(2)); // y
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(3)); // x
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(4));


  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG0); // 1-a

  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX); // x_x
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX); // y_x
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG1, AVX_REG2); // x_x (1-a)
  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG0, AVX_REG3); // y_x a
  code.add(asmcode::VADDPS, AVX_REG2, AVX_REG2, AVX_REG3);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_REG2);

  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(1)); // x_y
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX, AVX_CELLS(1)); // y_y
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG1, AVX_REG2); // x_y (1-a)
  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG0, AVX_REG3); // y_y a
  code.add(asmcode::VADDPS, AVX_REG2, AVX_REG2, AVX_REG3);
  code.add(asmcode::VMOVAPS, asmcode::MEM_R8, AVX_CELLS(1), AVX_REG2);

  code.add(asmcode::COMMENT, "END PRIMITIVE mix2");
  }

void primitive_smoothstep(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE smoothstep");
  /*
  float smoothstep(float edge0, float edge1, float x)
    {
    float t = clamp((x - edge0) / (edge1 - edge0), 0.f, 1.f);
    return t * t * (3.f - 2.f * t);
    }
  */
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER); // x
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1)); // edge1
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(2)); // edge0

  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1); // (x - edge0) / (edge1 - edge0)

  code.add(asmcode::VXORPS, AVX_REG1, AVX_REG1, AVX_REG1); // 0
  code.add(asmcode::VMOVAPS, AVX_REG2, ONEF_BITS); // 1

  // c a b clamp returns c if a < c and c < b, or a if c < a, or b if c > b

#ifdef AVX512
  code.add(asmcode::VXORPS, AVX_REG3, AVX_REG3, AVX_REG3);
  code.add(asmcode::VXORPS, AVX_REG4, AVX_REG4, AVX_REG4);
  code.add(asmcode::VXORPS, AVX_REG5, AVX_REG5, AVX_REG5);
  code.add(asmcode::VXORPS, AVX_REG6, AVX_REG6, AVX_REG6);
  code.add(asmcode::VMOVAPS, AVX_REG7, ONEF_BITS);
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2); // a <= c
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::k1, AVX_REG7);

  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG2, asmcode::NUMBER, 2); // c <= b
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::k1, AVX_REG7);

  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1); // c < a
  code.add(asmcode::VMOVAPS, AVX_REG5, asmcode::k1, AVX_REG7);

  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG2, AVX_REG0, asmcode::NUMBER, 1); // b < c
  code.add(asmcode::VMOVAPS, AVX_REG6, asmcode::k1, AVX_REG7);

#else
  code.add(asmcode::VCMPPS, AVX_REG3, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2); // a <= c
  code.add(asmcode::VCMPPS, AVX_REG4, AVX_REG0, AVX_REG2, asmcode::NUMBER, 2); // c <= b
  code.add(asmcode::VCMPPS, AVX_REG5, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1); // c < a
  code.add(asmcode::VCMPPS, AVX_REG6, AVX_REG2, AVX_REG0, asmcode::NUMBER, 1); // b < c

  code.add(asmcode::VCVTDQ2PS, AVX_REG3, AVX_REG3);
  code.add(asmcode::VCVTDQ2PS, AVX_REG4, AVX_REG4);
  code.add(asmcode::VCVTDQ2PS, AVX_REG5, AVX_REG5);
  code.add(asmcode::VCVTDQ2PS, AVX_REG6, AVX_REG6);

  code.add(asmcode::VMOVAPS, AVX_REG7, NOT_SIGN_BIT);
  //code.add(asmcode::MOV, asmcode::EAX, asmcode::NUMBER, 0x7fffffff);
  //code.add(asmcode::MOVD, asmcode::XMM7, asmcode::EAX);
  //code.add(asmcode::VBROADCASTSS, AVX_REG7, asmcode::XMM7);  
  code.add(asmcode::VANDPS, AVX_REG3, AVX_REG3, AVX_REG7);
  code.add(asmcode::VANDPS, AVX_REG4, AVX_REG4, AVX_REG7);
  code.add(asmcode::VANDPS, AVX_REG5, AVX_REG5, AVX_REG7);
  code.add(asmcode::VANDPS, AVX_REG6, AVX_REG6, AVX_REG7);
#endif

  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG4); // a <= c && c <= b

  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG5, AVX_REG5, AVX_REG1);
  code.add(asmcode::VMULPS, AVX_REG6, AVX_REG6, AVX_REG2);

  code.add(asmcode::VORPS, AVX_REG0, AVX_REG3, AVX_REG5);
  code.add(asmcode::VORPS, AVX_REG0, AVX_REG0, AVX_REG6);

  //t * t * (3.f - 2.f * t);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, 0x40000000); // 2
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, 0x40400000); // 3
  code.add(asmcode::VBROADCASTSS, AVX_REG2, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG0); // 2*t
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG2, AVX_REG1); // 3-2*t
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG0); // t*t
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1); // t*t*(3-2*t)  
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE smoothstep");
  }

void primitive_smin(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE smin");
  /*
  float smin( float a, float b, float k )
{
    float h = max(k-abs(a-b),0.0);
    return min(a, b) - h*h*0.25/k;
}
  */

  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER); // k
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1)); // b
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2)); // a
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VSUBPS, AVX_REG3, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, AVX_REG4, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG3, AVX_REG3, AVX_REG4); // abs(a-b)
  code.add(asmcode::VSUBPS, AVX_REG4, AVX_REG2, AVX_REG3); // k - abs(a-b)
  code.add(asmcode::VXORPS, AVX_REG5, AVX_REG5, AVX_REG5); // 0
  code.add(asmcode::VMAXPS, AVX_REG4, AVX_REG4, AVX_REG5); // h = max(k-abs(a-b), 0)
  code.add(asmcode::VMULPS, AVX_REG4, AVX_REG4, AVX_REG4); // h*h
  float f = 0.25f;
  uint32_t v = *(reinterpret_cast<uint32_t*>(&f));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG5, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG4, AVX_REG4, AVX_REG5); // h*h*0.25
  code.add(asmcode::VDIVPS, AVX_REG2, AVX_REG4, AVX_REG2); // h*h*0.25/k
  code.add(asmcode::VMINPS, AVX_REG0, AVX_REG0, AVX_REG1); // min(a,b)
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG2); // min(a, b) - h*h*0.25/k;
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END PRIMITIVE smin");
  }

void primitive_smax(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE smax");
  /*

float smax( float a, float b, float k )
{
    float h = max(k-abs(a-b),0.0);
    return max(a, b) + h*h*0.25/k;
}

  */

  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER); // k
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1)); // b
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2)); // a
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VSUBPS, AVX_REG3, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, AVX_REG4, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG3, AVX_REG3, AVX_REG4); // abs(a-b)
  code.add(asmcode::VSUBPS, AVX_REG4, AVX_REG2, AVX_REG3); // k - abs(a-b)
  code.add(asmcode::VXORPS, AVX_REG5, AVX_REG5, AVX_REG5); // 0
  code.add(asmcode::VMAXPS, AVX_REG4, AVX_REG4, AVX_REG5); // h = max(k-abs(a-b), 0)
  code.add(asmcode::VMULPS, AVX_REG4, AVX_REG4, AVX_REG4); // h*h
  float f = 0.25f;
  uint32_t v = *(reinterpret_cast<uint32_t*>(&f));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG5, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG4, AVX_REG4, AVX_REG5); // h*h*0.25
  code.add(asmcode::VDIVPS, AVX_REG2, AVX_REG4, AVX_REG2); // h*h*0.25/k
  code.add(asmcode::VMAXPS, AVX_REG0, AVX_REG0, AVX_REG1); // max(a,b)
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG2); // max(a, b) + h*h*0.25/k;
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END PRIMITIVE smax");
  }

void primitive_sdsphere(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE sdsphere");
  /*
  : sdSphere (in vec3 p, in float s)
    swap length3 swap -
    ;
  */
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1)); // ptr to vec3
  code.add(asmcode::VMOVAPS, AVX_REG4, MEM_STACK_REGISTER); // radius


  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG1);
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG2, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VSQRTPS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG4);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END PRIMITIVE sdsphere");
  }

void primitive_sdellipsoid(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE sdellipsoid");
  /*
  : sdEllipsoid (in vec3 pos, in vec3 rad)
  tuck (rad pos rad)
  tmp4 div3  (rad)
  tmp4 length3 (rad k0)
  swap tmp4 swap (k0 tmp4 rad)
  tmp4 div3 tmp4 length3 (k0 k1)
  swap dup (k1 k0 k0)
  1 - * swap /
;

float sdEllipsoid( vec3 p, vec3 r )
{
  float k0 = length(p/r);
  float k1 = length(p/(r*r));
  return k0*(k0-1.0)/k1;
}
  */
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1)); // ptr to vec3 p
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER); // ptr to vec3 rad
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG5, asmcode::MEM_RCX, AVX_CELLS(2));
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG3);
  code.add(asmcode::VDIVPS, AVX_REG1, AVX_REG1, AVX_REG4);
  code.add(asmcode::VDIVPS, AVX_REG2, AVX_REG2, AVX_REG5);
  code.add(asmcode::VDIVPS, AVX_REG3, AVX_REG0, AVX_REG3);
  code.add(asmcode::VDIVPS, AVX_REG4, AVX_REG1, AVX_REG4);
  code.add(asmcode::VDIVPS, AVX_REG5, AVX_REG2, AVX_REG5);

  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG1);
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG2, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG2);
  code.add(asmcode::VSQRTPS, AVX_REG0, AVX_REG0);

  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG3);
  code.add(asmcode::VMULPS, AVX_REG4, AVX_REG4, AVX_REG4);
  code.add(asmcode::VMULPS, AVX_REG5, AVX_REG5, AVX_REG5);
  code.add(asmcode::VADDPS, AVX_REG3, AVX_REG3, AVX_REG4);
  code.add(asmcode::VADDPS, AVX_REG3, AVX_REG3, AVX_REG5);
  code.add(asmcode::VSQRTPS, AVX_REG3, AVX_REG3);

  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG3);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END PRIMITIVE sdellipsoid");
  }

void primitive_sdstick(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE sdstick");
  /*
  vec2 sdStick(vec3 p, vec3 a, vec3 b, float r1, float r2)
{
    vec3 pa = p-a, ba = b-a;
  float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
  return length( pa - ba*h ) - mix(r1,r2,h*h*(3.0-2.0*h));
}
  */

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(4)); // ptr to vec3 p
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(3)); // ptr to vec3 a
  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER, AVX_CELLS(2)); // ptr to vec3 b

  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG5, asmcode::MEM_RCX, AVX_CELLS(2));
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG3); // p-a
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG4);
  code.add(asmcode::VSUBPS, AVX_REG2, AVX_REG2, AVX_REG5);
  code.add(asmcode::VMOVAPS, AVX_REG6, asmcode::MEM_R8);
  code.add(asmcode::VSUBPS, AVX_REG3, AVX_REG6, AVX_REG3); // b - a
  code.add(asmcode::VMOVAPS, AVX_REG6, asmcode::MEM_R8, AVX_CELLS(1));
  code.add(asmcode::VSUBPS, AVX_REG4, AVX_REG6, AVX_REG4); // b - a
  code.add(asmcode::VMOVAPS, AVX_REG6, asmcode::MEM_R8, AVX_CELLS(2));
  code.add(asmcode::VSUBPS, AVX_REG5, AVX_REG6, AVX_REG5); // b - a

  code.add(asmcode::VMULPS, AVX_REG6, AVX_REG0, AVX_REG3);
  code.add(asmcode::VMULPS, AVX_REG7, AVX_REG1, AVX_REG4);
  code.add(asmcode::VADDPS, AVX_REG6, AVX_REG6, AVX_REG7);
  code.add(asmcode::VMULPS, AVX_REG7, AVX_REG2, AVX_REG5);
  code.add(asmcode::VADDPS, AVX_REG6, AVX_REG6, AVX_REG7); // dot(pa, ba)

  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG3);
  code.add(asmcode::VMULPS, AVX_REG4, AVX_REG4, AVX_REG4);
  code.add(asmcode::VMULPS, AVX_REG5, AVX_REG5, AVX_REG5);
  code.add(asmcode::VADDPS, AVX_REG3, AVX_REG3, AVX_REG4);
  code.add(asmcode::VADDPS, AVX_REG3, AVX_REG3, AVX_REG5); // dot(ba, ba)

  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG6, AVX_REG3);

  // clamp
  code.add(asmcode::VXORPS, AVX_REG1, AVX_REG1, AVX_REG1); // 0
  code.add(asmcode::VMOVAPS, AVX_REG2, ONEF_BITS); // 1

  // c a b clamp returns c if a < c and c < b, or a if c < a, or b if c > b

#ifdef AVX512
  code.add(asmcode::VXORPS, AVX_REG3, AVX_REG3, AVX_REG3);
  code.add(asmcode::VXORPS, AVX_REG4, AVX_REG4, AVX_REG4);
  code.add(asmcode::VXORPS, AVX_REG5, AVX_REG5, AVX_REG5);
  code.add(asmcode::VXORPS, AVX_REG6, AVX_REG6, AVX_REG6);
  code.add(asmcode::VMOVAPS, AVX_REG7, ONEF_BITS);
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2); // a <= c
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::k1, AVX_REG7);

  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG2, asmcode::NUMBER, 2); // c <= b
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::k1, AVX_REG7);

  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1); // c < a
  code.add(asmcode::VMOVAPS, AVX_REG5, asmcode::k1, AVX_REG7);

  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG2, AVX_REG0, asmcode::NUMBER, 1); // b < c
  code.add(asmcode::VMOVAPS, AVX_REG6, asmcode::k1, AVX_REG7);

#else
  code.add(asmcode::VCMPPS, AVX_REG3, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2); // a <= c
  code.add(asmcode::VCMPPS, AVX_REG4, AVX_REG0, AVX_REG2, asmcode::NUMBER, 2); // c <= b
  code.add(asmcode::VCMPPS, AVX_REG5, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1); // c < a
  code.add(asmcode::VCMPPS, AVX_REG6, AVX_REG2, AVX_REG0, asmcode::NUMBER, 1); // b < c

  code.add(asmcode::VCVTDQ2PS, AVX_REG3, AVX_REG3);
  code.add(asmcode::VCVTDQ2PS, AVX_REG4, AVX_REG4);
  code.add(asmcode::VCVTDQ2PS, AVX_REG5, AVX_REG5);
  code.add(asmcode::VCVTDQ2PS, AVX_REG6, AVX_REG6);

  code.add(asmcode::VMOVAPS, AVX_REG7, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG3, AVX_REG3, AVX_REG7);
  code.add(asmcode::VANDPS, AVX_REG4, AVX_REG4, AVX_REG7);
  code.add(asmcode::VANDPS, AVX_REG5, AVX_REG5, AVX_REG7);
  code.add(asmcode::VANDPS, AVX_REG6, AVX_REG6, AVX_REG7);
#endif

  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG4); // a <= c && c <= b

  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG5, AVX_REG5, AVX_REG1);
  code.add(asmcode::VMULPS, AVX_REG6, AVX_REG6, AVX_REG2);

  code.add(asmcode::VORPS, AVX_REG0, AVX_REG3, AVX_REG5);
  code.add(asmcode::VORPS, AVX_REG0, AVX_REG0, AVX_REG6);

  // avx_reg0 contains h

  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG5, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG6, asmcode::MEM_RCX, AVX_CELLS(2));
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG4); // p-a
  code.add(asmcode::VSUBPS, AVX_REG2, AVX_REG2, AVX_REG5);
  code.add(asmcode::VSUBPS, AVX_REG3, AVX_REG3, AVX_REG6);
  code.add(asmcode::VMOVAPS, AVX_REG7, asmcode::MEM_R8);
  code.add(asmcode::VSUBPS, AVX_REG4, AVX_REG7, AVX_REG4); // b - a
  code.add(asmcode::VMOVAPS, AVX_REG7, asmcode::MEM_R8, AVX_CELLS(1));
  code.add(asmcode::VSUBPS, AVX_REG5, AVX_REG7, AVX_REG5); // b - a
  code.add(asmcode::VMOVAPS, AVX_REG7, asmcode::MEM_R8, AVX_CELLS(2));
  code.add(asmcode::VSUBPS, AVX_REG6, AVX_REG7, AVX_REG6); // b - a
  //return length( pa - ba*h ) - mix(r1,r2,h*h*(3.0-2.0*h));

  code.add(asmcode::VMULPS, AVX_REG4, AVX_REG4, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG5, AVX_REG5, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG6, AVX_REG6, AVX_REG0);
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG4);
  code.add(asmcode::VSUBPS, AVX_REG2, AVX_REG2, AVX_REG5);
  code.add(asmcode::VSUBPS, AVX_REG3, AVX_REG3, AVX_REG6);

  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG1);
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG2, AVX_REG2);
  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG3);
  code.add(asmcode::VADDPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG1, AVX_REG1, AVX_REG3);
  code.add(asmcode::VSQRTPS, AVX_REG1, AVX_REG1); // length( pa - ba*h )

  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(1)); // r1
  code.add(asmcode::VMOVAPS, AVX_REG3, MEM_STACK_REGISTER); // r2


  //h * h * (3.f - 2.f * h);
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, 0x40000000); // 2
  code.add(asmcode::VBROADCASTSS, AVX_REG4, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, 0x40400000); // 3
  code.add(asmcode::VBROADCASTSS, AVX_REG5, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG4, AVX_REG4, AVX_REG0); // 2*h
  code.add(asmcode::VSUBPS, AVX_REG4, AVX_REG5, AVX_REG4); // 3-2*h
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG0); // h*h
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG4); // h*h*(3-2*h)  

  code.add(asmcode::VMOVAPS, AVX_REG5, ONEF_BITS);
  code.add(asmcode::VSUBPS, AVX_REG5, AVX_REG5, AVX_REG0); // 1-a
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG2, AVX_REG5); // x*(1-a)
  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG0); // y*a
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG3, AVX_REG2); // mix(r1,r2,h*h*(3.0-2.0*h));

  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG1, AVX_REG0);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(4));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);


  code.add(asmcode::COMMENT, "END PRIMITIVE sdstick");
  }

void primitive_sdcapsule(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::COMMENT, "BEGIN PRIMITIVE sdcapsule");
  /*
float sdCapsule( vec3 p, vec3 a, vec3 b, float r )
{
  vec3 pa = p - a, ba = b - a;
  float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
  return length( pa - ba*h ) - r;
}
  */

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(3)); // ptr to vec3 p
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(2)); // ptr to vec3 a
  code.add(asmcode::MOV, asmcode::R8, MEM_STACK_REGISTER, AVX_CELLS(1)); // ptr to vec3 b

  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG5, asmcode::MEM_RCX, AVX_CELLS(2));
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG3); // p-a
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG4);
  code.add(asmcode::VSUBPS, AVX_REG2, AVX_REG2, AVX_REG5);
  code.add(asmcode::VMOVAPS, AVX_REG6, asmcode::MEM_R8);
  code.add(asmcode::VSUBPS, AVX_REG3, AVX_REG6, AVX_REG3); // b - a
  code.add(asmcode::VMOVAPS, AVX_REG6, asmcode::MEM_R8, AVX_CELLS(1));
  code.add(asmcode::VSUBPS, AVX_REG4, AVX_REG6, AVX_REG4); // b - a
  code.add(asmcode::VMOVAPS, AVX_REG6, asmcode::MEM_R8, AVX_CELLS(2));
  code.add(asmcode::VSUBPS, AVX_REG5, AVX_REG6, AVX_REG5); // b - a

  code.add(asmcode::VMULPS, AVX_REG6, AVX_REG0, AVX_REG3);
  code.add(asmcode::VMULPS, AVX_REG7, AVX_REG1, AVX_REG4);
  code.add(asmcode::VADDPS, AVX_REG6, AVX_REG6, AVX_REG7);
  code.add(asmcode::VMULPS, AVX_REG7, AVX_REG2, AVX_REG5);
  code.add(asmcode::VADDPS, AVX_REG6, AVX_REG6, AVX_REG7); // dot(pa, ba)

  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG3);
  code.add(asmcode::VMULPS, AVX_REG4, AVX_REG4, AVX_REG4);
  code.add(asmcode::VMULPS, AVX_REG5, AVX_REG5, AVX_REG5);
  code.add(asmcode::VADDPS, AVX_REG3, AVX_REG3, AVX_REG4);
  code.add(asmcode::VADDPS, AVX_REG3, AVX_REG3, AVX_REG5); // dot(ba, ba)

  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG6, AVX_REG3);

  // clamp
  code.add(asmcode::VXORPS, AVX_REG1, AVX_REG1, AVX_REG1); // 0
  code.add(asmcode::VMOVAPS, AVX_REG2, ONEF_BITS); // 1

  // c a b clamp returns c if a < c and c < b, or a if c < a, or b if c > b

#ifdef AVX512
  code.add(asmcode::VXORPS, AVX_REG3, AVX_REG3, AVX_REG3);
  code.add(asmcode::VXORPS, AVX_REG4, AVX_REG4, AVX_REG4);
  code.add(asmcode::VXORPS, AVX_REG5, AVX_REG5, AVX_REG5);
  code.add(asmcode::VXORPS, AVX_REG6, AVX_REG6, AVX_REG6);
  code.add(asmcode::VMOVAPS, AVX_REG7, ONEF_BITS);
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2); // a <= c
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::k1, AVX_REG7);

  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG2, asmcode::NUMBER, 2); // c <= b
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::k1, AVX_REG7);

  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1); // c < a
  code.add(asmcode::VMOVAPS, AVX_REG5, asmcode::k1, AVX_REG7);

  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG2, AVX_REG0, asmcode::NUMBER, 1); // b < c
  code.add(asmcode::VMOVAPS, AVX_REG6, asmcode::k1, AVX_REG7);

#else
  code.add(asmcode::VCMPPS, AVX_REG3, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2); // a <= c
  code.add(asmcode::VCMPPS, AVX_REG4, AVX_REG0, AVX_REG2, asmcode::NUMBER, 2); // c <= b
  code.add(asmcode::VCMPPS, AVX_REG5, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1); // c < a
  code.add(asmcode::VCMPPS, AVX_REG6, AVX_REG2, AVX_REG0, asmcode::NUMBER, 1); // b < c

  code.add(asmcode::VCVTDQ2PS, AVX_REG3, AVX_REG3);
  code.add(asmcode::VCVTDQ2PS, AVX_REG4, AVX_REG4);
  code.add(asmcode::VCVTDQ2PS, AVX_REG5, AVX_REG5);
  code.add(asmcode::VCVTDQ2PS, AVX_REG6, AVX_REG6);

  code.add(asmcode::VMOVAPS, AVX_REG7, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG3, AVX_REG3, AVX_REG7);
  code.add(asmcode::VANDPS, AVX_REG4, AVX_REG4, AVX_REG7);
  code.add(asmcode::VANDPS, AVX_REG5, AVX_REG5, AVX_REG7);
  code.add(asmcode::VANDPS, AVX_REG6, AVX_REG6, AVX_REG7);
#endif

  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG4); // a <= c && c <= b

  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG5, AVX_REG5, AVX_REG1);
  code.add(asmcode::VMULPS, AVX_REG6, AVX_REG6, AVX_REG2);

  code.add(asmcode::VORPS, AVX_REG0, AVX_REG3, AVX_REG5);
  code.add(asmcode::VORPS, AVX_REG0, AVX_REG0, AVX_REG6);

  // avx_reg0 contains h

  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RAX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG3, asmcode::MEM_RAX, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG4, asmcode::MEM_RCX);
  code.add(asmcode::VMOVAPS, AVX_REG5, asmcode::MEM_RCX, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG6, asmcode::MEM_RCX, AVX_CELLS(2));
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG4); // p-a
  code.add(asmcode::VSUBPS, AVX_REG2, AVX_REG2, AVX_REG5);
  code.add(asmcode::VSUBPS, AVX_REG3, AVX_REG3, AVX_REG6);
  code.add(asmcode::VMOVAPS, AVX_REG7, asmcode::MEM_R8);
  code.add(asmcode::VSUBPS, AVX_REG4, AVX_REG7, AVX_REG4); // b - a
  code.add(asmcode::VMOVAPS, AVX_REG7, asmcode::MEM_R8, AVX_CELLS(1));
  code.add(asmcode::VSUBPS, AVX_REG5, AVX_REG7, AVX_REG5); // b - a
  code.add(asmcode::VMOVAPS, AVX_REG7, asmcode::MEM_R8, AVX_CELLS(2));
  code.add(asmcode::VSUBPS, AVX_REG6, AVX_REG7, AVX_REG6); // b - a
  //return length( pa - ba*h ) - r;

  code.add(asmcode::VMULPS, AVX_REG4, AVX_REG4, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG5, AVX_REG5, AVX_REG0);
  code.add(asmcode::VMULPS, AVX_REG6, AVX_REG6, AVX_REG0);
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG4);
  code.add(asmcode::VSUBPS, AVX_REG2, AVX_REG2, AVX_REG5);
  code.add(asmcode::VSUBPS, AVX_REG3, AVX_REG3, AVX_REG6);

  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG1);
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG2, AVX_REG2);
  code.add(asmcode::VMULPS, AVX_REG3, AVX_REG3, AVX_REG3);
  code.add(asmcode::VADDPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG1, AVX_REG1, AVX_REG3);
  code.add(asmcode::VSQRTPS, AVX_REG1, AVX_REG1); // length( pa - ba*h )

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER); // r

  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG1, AVX_REG0);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END PRIMITIVE sdcapsule");
  }

prim_map generate_primitives_map()
  {
  prim_map pm;
  pm.insert(std::pair<std::string, prim_fun>("+", &primitive_add));
  pm.insert(std::pair<std::string, prim_fun>("-", &primitive_sub));
  pm.insert(std::pair<std::string, prim_fun>("*", &primitive_mul));
  pm.insert(std::pair<std::string, prim_fun>("/", &primitive_div));
  pm.insert(std::pair<std::string, prim_fun>("z*", &primitive_complex_mul));
  pm.insert(std::pair<std::string, prim_fun>("z+", &primitive_complex_add));
  pm.insert(std::pair<std::string, prim_fun>("z-", &primitive_complex_sub));

  pm.insert(std::pair<std::string, prim_fun>("push", &primitive_return_stack_push));
  pm.insert(std::pair<std::string, prim_fun>("pop", &primitive_return_stack_pop));
  pm.insert(std::pair<std::string, prim_fun>(">r", &primitive_return_stack_push));
  pm.insert(std::pair<std::string, prim_fun>("r>", &primitive_return_stack_pop));
  pm.insert(std::pair<std::string, prim_fun>("r@", &primitive_return_stack_top));
  pm.insert(std::pair<std::string, prim_fun>("rt@", &primitive_return_stack_top_fetch));
  pm.insert(std::pair<std::string, prim_fun>("rp@", &primitive_return_stack_pointer_fetch));

  pm.insert(std::pair<std::string, prim_fun>("drop", &primitive_drop));
  pm.insert(std::pair<std::string, prim_fun>("swap", &primitive_swap));
  pm.insert(std::pair<std::string, prim_fun>("dup", &primitive_dup));
  pm.insert(std::pair<std::string, prim_fun>("over", &primitive_over));
  pm.insert(std::pair<std::string, prim_fun>("rot", &primitive_rot));
  pm.insert(std::pair<std::string, prim_fun>("-rot", &primitive_minrot));
  pm.insert(std::pair<std::string, prim_fun>("2drop", &primitive_twodrop));
  pm.insert(std::pair<std::string, prim_fun>("2dup", &primitive_twodup));
  pm.insert(std::pair<std::string, prim_fun>("2swap", &primitive_twoswap));
  pm.insert(std::pair<std::string, prim_fun>("min", &primitive_min));
  pm.insert(std::pair<std::string, prim_fun>("max", &primitive_max));

  pm.insert(std::pair<std::string, prim_fun>("sin", &primitive_sin));
  pm.insert(std::pair<std::string, prim_fun>("cos", &primitive_cos));
  pm.insert(std::pair<std::string, prim_fun>("tan", &primitive_tan));
  pm.insert(std::pair<std::string, prim_fun>("exp", &primitive_exp));
  pm.insert(std::pair<std::string, prim_fun>("log", &primitive_log));
  pm.insert(std::pair<std::string, prim_fun>("**", &primitive_pow));
  pm.insert(std::pair<std::string, prim_fun>("pow", &primitive_pow));
  pm.insert(std::pair<std::string, prim_fun>("atan2", &primitive_atan2));
  pm.insert(std::pair<std::string, prim_fun>("sqrt", &primitive_sqrt));
  pm.insert(std::pair<std::string, prim_fun>("abs", &primitive_abs));
  pm.insert(std::pair<std::string, prim_fun>("negate", &primitive_negate));
  pm.insert(std::pair<std::string, prim_fun>("and", &primitive_and));
  pm.insert(std::pair<std::string, prim_fun>("or", &primitive_or));
  pm.insert(std::pair<std::string, prim_fun>("xor", &primitive_xor));
  pm.insert(std::pair<std::string, prim_fun>("not", &primitive_not));

  pm.insert(std::pair<std::string, prim_fun>("fm/mod", &primitive_fmod));

  pm.insert(std::pair<std::string, prim_fun>("floor", &primitive_floor));
  pm.insert(std::pair<std::string, prim_fun>("ceil", &primitive_ceil));
  pm.insert(std::pair<std::string, prim_fun>("round", &primitive_round));
  pm.insert(std::pair<std::string, prim_fun>("trunc", &primitive_truncate));
  pm.insert(std::pair<std::string, prim_fun>("fcast", &primitive_fcast));
  pm.insert(std::pair<std::string, prim_fun>("icast", &primitive_icast));

  pm.insert(std::pair<std::string, prim_fun>("#+", &primitive_addi));
  pm.insert(std::pair<std::string, prim_fun>("#-", &primitive_subi));
  pm.insert(std::pair<std::string, prim_fun>("#*", &primitive_muli));
  pm.insert(std::pair<std::string, prim_fun>("#/", &primitive_divi));
  pm.insert(std::pair<std::string, prim_fun>("#<<", &primitive_shiftlefti));
  pm.insert(std::pair<std::string, prim_fun>("#>>", &primitive_shiftrighti));

  pm.insert(std::pair<std::string, prim_fun>("=", &primitive_equ));
  pm.insert(std::pair<std::string, prim_fun>("<>", &primitive_nequ));
  pm.insert(std::pair<std::string, prim_fun>("<", &primitive_lt));
  pm.insert(std::pair<std::string, prim_fun>(">", &primitive_gt));
  pm.insert(std::pair<std::string, prim_fun>("<=", &primitive_le));
  pm.insert(std::pair<std::string, prim_fun>(">=", &primitive_ge));

  pm.insert(std::pair<std::string, prim_fun>("f=", &primitive_fequ));
  pm.insert(std::pair<std::string, prim_fun>("f<>", &primitive_fnequ));
  pm.insert(std::pair<std::string, prim_fun>("f<", &primitive_flt));
  pm.insert(std::pair<std::string, prim_fun>("f>", &primitive_fgt));
  pm.insert(std::pair<std::string, prim_fun>("f<=", &primitive_fle));
  pm.insert(std::pair<std::string, prim_fun>("f>=", &primitive_fge));

  pm.insert(std::pair<std::string, prim_fun>("st@", &primitive_stack_top_fetch));
  pm.insert(std::pair<std::string, prim_fun>("sp@", &primitive_stack_pointer_fetch));
  pm.insert(std::pair<std::string, prim_fun>("here", &primitive_here));
  pm.insert(std::pair<std::string, prim_fun>(",", &primitive_comma));

  pm.insert(std::pair<std::string, prim_fun>("@", &primitive_fetch));
  pm.insert(std::pair<std::string, prim_fun>("!", &primitive_store));
  pm.insert(std::pair<std::string, prim_fun>("#+!", &primitive_addstorei));
  pm.insert(std::pair<std::string, prim_fun>("#-!", &primitive_substorei));

  pm.insert(std::pair<std::string, prim_fun>("if", &primitive_if));
  pm.insert(std::pair<std::string, prim_fun>("then", &primitive_then));
  pm.insert(std::pair<std::string, prim_fun>("else", &primitive_else));

  pm.insert(std::pair<std::string, prim_fun>("begin", &primitive_begin));
  pm.insert(std::pair<std::string, prim_fun>("while", &primitive_while));
  pm.insert(std::pair<std::string, prim_fun>("repeat", &primitive_repeat));

  pm.insert(std::pair<std::string, prim_fun>("clamp", &primitive_clamp));
  pm.insert(std::pair<std::string, prim_fun>("within", &primitive_within));
  pm.insert(std::pair<std::string, prim_fun>("vec3!", &primitive_vec3_store));
  pm.insert(std::pair<std::string, prim_fun>("vec2!", &primitive_vec2_store));
  pm.insert(std::pair<std::string, prim_fun>("mat2x2!", &primitive_mat2x2_store));
  pm.insert(std::pair<std::string, prim_fun>("mat3x3!", &primitive_mat3x3_store));
  pm.insert(std::pair<std::string, prim_fun>("mul2x2", &primitive_mul2x2));
  pm.insert(std::pair<std::string, prim_fun>("mul3x3", &primitive_mul3x3));
  pm.insert(std::pair<std::string, prim_fun>("dot3", &primitive_dot3));
  pm.insert(std::pair<std::string, prim_fun>("cross3", &primitive_cross3));
  pm.insert(std::pair<std::string, prim_fun>("add3", &primitive_add3));
  pm.insert(std::pair<std::string, prim_fun>("sub3", &primitive_sub3));
  pm.insert(std::pair<std::string, prim_fun>("mul3", &primitive_mul3));
  pm.insert(std::pair<std::string, prim_fun>("div3", &primitive_div3));
  pm.insert(std::pair<std::string, prim_fun>("scalarmul3", &primitive_scalarmul3));
  pm.insert(std::pair<std::string, prim_fun>("length3", &primitive_length3));
  pm.insert(std::pair<std::string, prim_fun>("normalize3", &primitive_normalize3));
  pm.insert(std::pair<std::string, prim_fun>("dot2", &primitive_dot2));
  pm.insert(std::pair<std::string, prim_fun>("add2", &primitive_add2));
  pm.insert(std::pair<std::string, prim_fun>("sub2", &primitive_sub2));
  pm.insert(std::pair<std::string, prim_fun>("mul2", &primitive_mul2));
  pm.insert(std::pair<std::string, prim_fun>("div2", &primitive_div2));
  pm.insert(std::pair<std::string, prim_fun>("scalarmul2", &primitive_scalarmul2));
  pm.insert(std::pair<std::string, prim_fun>("length2", &primitive_length2));
  pm.insert(std::pair<std::string, prim_fun>("normalize2", &primitive_normalize2));
  pm.insert(std::pair<std::string, prim_fun>("mix", &primitive_mix));
  pm.insert(std::pair<std::string, prim_fun>("smoothstep", &primitive_smoothstep));
  pm.insert(std::pair<std::string, prim_fun>("reflect3", &primitive_reflect3));
  pm.insert(std::pair<std::string, prim_fun>("mix3", &primitive_mix3));
  pm.insert(std::pair<std::string, prim_fun>("mix2", &primitive_mix2));

  pm.insert(std::pair<std::string, prim_fun>("smin", &primitive_smin));
  pm.insert(std::pair<std::string, prim_fun>("smax", &primitive_smax));

  pm.insert(std::pair<std::string, prim_fun>("sdsphere", &primitive_sdsphere));
  pm.insert(std::pair<std::string, prim_fun>("sdellipsoid", &primitive_sdellipsoid));
  pm.insert(std::pair<std::string, prim_fun>("sdstick", &primitive_sdstick));
  pm.insert(std::pair<std::string, prim_fun>("sdcapsule", &primitive_sdcapsule));


  return pm;
  }

VF_END