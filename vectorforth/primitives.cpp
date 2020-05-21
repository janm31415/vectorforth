#include "primitives.h"
#include "context_defs.h"
#include "asm_aux.h"
#include "avx_mathfun.h"

#include <cassert>

using namespace ASM;

VF_BEGIN



void primitive_addi(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::R11, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::ADD, asmcode::RAX, asmcode::R11);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);  
  }

void primitive_subi(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::R11, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::SUB, asmcode::RAX, asmcode::R11);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_muli(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::R11, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::IMUL, asmcode::R11);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_divi(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::R11, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::XOR, asmcode::RDX, asmcode::RDX);
  code.add(asmcode::CQO);
  code.add(asmcode::IDIV, asmcode::R11);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_add(asmcode& code, compile_data&)
  {  

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_sub(asmcode& code, compile_data&)
  {  

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_mul(asmcode& code, compile_data&)
  {

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_complex_add(asmcode& code, compile_data&)
  {
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
  }


void primitive_complex_sub(asmcode& code, compile_data&)
  {
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
  }

void primitive_complex_mul(asmcode& code, compile_data&)
  {
  //y0 y1 y2 y3

  // result y0y2 - y1y3  y0y3 + y1y2

  code.add(asmcode::VMOVAPS, AVX_REG3, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(3));
    
  code.add(asmcode::VMULPS, asmcode::YMM4, AVX_REG0, AVX_REG2); //y0y2
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG3); //y0y3
  code.add(asmcode::VMULPS, asmcode::YMM5, AVX_REG1, AVX_REG3); //y1y3
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG2); //y1y2

  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1); //y0y3 + y1y2
  code.add(asmcode::VSUBPS, AVX_REG1, asmcode::YMM4, asmcode::YMM5); //y0y2 - y1y3
  
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_div(asmcode& code, compile_data&)
  {

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_drop(asmcode& code, compile_data&)
  {
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  }

void primitive_swap(asmcode& code, compile_data&)
  {

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG1);

  }

void primitive_dup(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);  
  }

void primitive_over(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_rot(asmcode& code, compile_data&)
  {
  // a b c >> b c a

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(2));

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(2), AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG2);

  }

void primitive_minrot(asmcode& code, compile_data&)
  {

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(2));

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(2), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG2);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG1);
  }

void primitive_twodrop(asmcode& code, compile_data&)
  {
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  }

void primitive_twodup(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(2), AVX_REG0);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  }

void primitive_twoswap(asmcode& code, compile_data&)
  {

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG3, MEM_STACK_REGISTER, AVX_CELLS(3));

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG2);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG3);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(2), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(3), AVX_REG1);

  }

void primitive_incri(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::INC, asmcode::RAX);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_decri(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::DEC, asmcode::RAX);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_incr32i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, 32);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_decr32i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::SUB, asmcode::RAX, asmcode::NUMBER, 32);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_incr64i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, 64);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_decr64i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::SUB, asmcode::RAX, asmcode::NUMBER, 64);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }


void primitive_incr96i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, 96);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_decr96i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::SUB, asmcode::RAX, asmcode::NUMBER, 96);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_incr128i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, 128);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_decr128i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::SUB, asmcode::RAX, asmcode::NUMBER, 128);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_incr160i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, 160);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_decr160i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::SUB, asmcode::RAX, asmcode::NUMBER, 160);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_incr192i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, 192);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_decr192i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::SUB, asmcode::RAX, asmcode::NUMBER, 192);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_incr224i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, 224);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_decr224i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::SUB, asmcode::RAX, asmcode::NUMBER, 224);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_incr256i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, 256);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_decr256i(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::SUB, asmcode::RAX, asmcode::NUMBER, 256);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_sin(asmcode& code, compile_data&)
  {
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
  }

void primitive_cos(asmcode& code, compile_data&)
  {
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
  }

void primitive_tan(asmcode& code, compile_data&)
  {
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
  }

void primitive_exp(asmcode& code, compile_data&)
  {
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
  }

void primitive_log(asmcode& code, compile_data&)
  {
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
  }

void primitive_pow(asmcode& code, compile_data&)
  {
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
  }

void primitive_atan2(asmcode& code, compile_data&)
  {
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
  }

void primitive_sqrt(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VSQRTPS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_abs(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_negate(asmcode& code, compile_data&)
  {

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, SIGN_BIT);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_and(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_or(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VORPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_xor(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_not(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_equ(asmcode& code, compile_data&)
  {

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  }

void primitive_nequ(asmcode& code, compile_data&)
  {


  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 4);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_lt(asmcode& code, compile_data&)
  {

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_gt(asmcode& code, compile_data&)
  {

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_le(asmcode& code, compile_data&)
  {

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 2);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_ge(asmcode& code, compile_data&)
  {

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_fequ(asmcode& code, compile_data&)
  {
  //1.f if a is equal to b, else 0.f
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_fnequ(asmcode& code, compile_data&)
  {
  //1.f if a is not equal to b, else 0.f
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 4);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_flt(asmcode& code, compile_data&)
  {
  //1.f if a is less than b, else 0.f
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_fgt(asmcode& code, compile_data&)
  {
  //1.f if a is greater than b, else 0.f
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_fle(asmcode& code, compile_data&)
  {
  //1.f if a is less than or equal to b, else 0.f
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 2);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_fge(asmcode& code, compile_data&)
  {
  //1.f if a is greater than or equal to b, else 0.f
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_stack_top_fetch(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, STACK_TOP);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_stack_pointer_fetch(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, STACK_REGISTER);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_fetch(asmcode& code, compile_data&)
  {

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_store(asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
  }

void primitive_addstorei(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER); 
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::ADD, asmcode::MEM_RAX, asmcode::RCX);
  }

void primitive_substorei(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RCX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::SUB, asmcode::MEM_RAX, asmcode::RCX);
  }

void primitive_here(ASM::asmcode& code, compile_data&)
  {
  //code.add(asmcode::MOV, asmcode::RAX, HERE);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, HERE);
  }

void primitive_cells(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::SHL, asmcode::RAX, asmcode::NUMBER, 5);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_comma(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));  
  code.add(asmcode::MOV, asmcode::RAX, MEM_HERE);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, 32);
  code.add(asmcode::MOV, MEM_HERE, asmcode::RAX);
  }

void primitive_floor(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VROUNDPS, AVX_REG0, AVX_REG0, asmcode::NUMBER, 1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_ceil(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VROUNDPS, AVX_REG0, AVX_REG0, asmcode::NUMBER, 2);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_round(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VROUNDPS, AVX_REG0, AVX_REG0, asmcode::NUMBER, 0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_truncate(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VROUNDPS, AVX_REG0, AVX_REG0, asmcode::NUMBER, 3);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_fcast(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_icast(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::VCVTPS2DQ, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_fmod(asmcode& code, compile_data&)
  {
  /*
  (d1 n1 -- n2 n3)
  Divide d1 by n1, giving the floored quotient n3 and the remainder n2. Input and output stack arguments are signed.
  An ambiguous condition exists if n1 is zero or if the quotient lies outside the range of a single-cell signed integer.
  */

  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER); // n1
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1)); // d1
  code.add(asmcode::VDIVPS, AVX_REG2, AVX_REG0, AVX_REG1); // quotient
  code.add(asmcode::VROUNDPS, AVX_REG2, AVX_REG2, asmcode::NUMBER, 1); // floored quotient n3
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG2, AVX_REG1); // floored quotient n3 times n1
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1); // remainder n2
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG2);
  }

void primitive_min(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMINPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_max(asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMAXPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_return_stack_push(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, asmcode::MEM_RSP, AVX_REG0);
  }

void primitive_return_stack_pop(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RSP);
  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_return_stack_top(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, RSP_TOP);
  code.add(asmcode::SUB, asmcode::RAX, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  }

void primitive_return_stack_top_fetch(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, RSP_TOP);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_return_stack_pointer_fetch(ASM::asmcode& code, compile_data&)
  {
  code.add(asmcode::MOV, asmcode::RAX, asmcode::RSP);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  }

void primitive_if(ASM::asmcode& code, compile_data& cd)
  {

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


  code.add(asmcode::VMOVMSKPS, asmcode::RAX, AVX_REG0); // if rax == 0, then all conditions are false, if rax == 255, all conditions are true
  code.add(asmcode::TEST, asmcode::RAX, asmcode::RAX);
  code.add(asmcode::JE, cd.else_label.back()); // if rax == 0, skip to the end of the else code  
  }

void primitive_then(ASM::asmcode& code, compile_data& cd)
  {
  /*
  return stack looks like this:
  true branch value a, true branch value b, ..., test condition , address of stack register before branch
  */

  auto loop = label_to_string(cd.label++);
  auto loop_end = label_to_string(cd.label++);
  
  code.add(asmcode::MOV, asmcode::RCX, asmcode::MEM_RSP); // address of stack register before branch
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RSP, AVX_CELLS(1)); // test condition
  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(2));

  code.add(asmcode::VMOVMSKPS, asmcode::RAX, AVX_REG1); // if rax == 0, then all conditions are false, if rax == 255, all conditions are true
  code.add(asmcode::TEST, asmcode::RAX, asmcode::RAX);
  code.add(asmcode::JE, loop_end);

  code.add(asmcode::MOV, asmcode::RAX, STACK_REGISTER); // address of current stack register in rax
  code.add(asmcode::SUB, asmcode::RCX, asmcode::RAX);
  code.add(asmcode::SHR, asmcode::RCX, asmcode::NUMBER, 5); // divide rcx by 32: now rcx contains number of items added on the stack

  code.add(asmcode::MOV, asmcode::RDX, STACK_REGISTER);

  code.add(asmcode::LABEL, loop);
  code.add(asmcode::TEST, asmcode::RCX, asmcode::RCX);
  code.add(asmcode::JES, loop_end);
  code.add(asmcode::DEC, asmcode::RCX);
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RDX);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RSP);

  //_mm_or_ps(_mm_and_ps(mask.m128, updated_values.m128), _mm_andnot_ps(mask.m128, original.m128));
  code.add(asmcode::VANDPS, AVX_REG3, AVX_REG1, AVX_REG0);
  code.add(asmcode::VANDNPS, asmcode::YMM4, AVX_REG1, AVX_REG2);
  code.add(asmcode::VORPS, AVX_REG0, AVX_REG3, asmcode::YMM4);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RDX, AVX_REG0);

  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::ADD, asmcode::RDX, asmcode::NUMBER, AVX_CELLS(1));  
  code.add(asmcode::JMPS, loop);
  code.add(asmcode::LABEL, loop_end);

  code.add(asmcode::LABEL, cd.then_label.back());
  cd.then_label.pop_back();
  }

void primitive_else(ASM::asmcode& code, compile_data& cd)
  {
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

  code.add(asmcode::VMOVMSKPS, asmcode::RAX, AVX_REG1); // if rax == 0, then all conditions are false, if rax == 255, all conditions are true
  code.add(asmcode::CMP, asmcode::RAX, asmcode::NUMBER, 255); 
  code.add(asmcode::JE, cd.then_label.back());

  code.add(asmcode::MOV, asmcode::RAX, STACK_REGISTER); // address of current stack register in rax
  code.add(asmcode::SUB, asmcode::RCX, asmcode::RAX);  
  code.add(asmcode::SHR, asmcode::RCX, asmcode::NUMBER, 5); // divide rcx by 32: now rcx contains number of items added on the stack  

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

  }

void primitive_begin(ASM::asmcode& code, compile_data& cd)
  {
  cd.begin_label.push_back(label_to_string(cd.label++));
  cd.repeat_label.push_back(label_to_string(cd.label++));
  code.add(asmcode::LABEL, cd.begin_label.back());
  }

void primitive_while(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER); // pop ymm0 from stack
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));

  /*
  The begin while repeat test only fails if it fails for all values in the simd vector
  */

  code.add(asmcode::VMOVMSKPS, asmcode::RAX, AVX_REG0); // if rax == 0, then all conditions are false
  code.add(asmcode::TEST, asmcode::RAX, asmcode::RAX);
  code.add(asmcode::JE, cd.repeat_label.back()); // if rax == 0, skip to the end of the begin while repeat code, otherwise continue
  }

void primitive_repeat(ASM::asmcode& code, compile_data& cd)
  {
  code.add(asmcode::JMP, cd.begin_label.back());
  code.add(asmcode::LABEL, cd.repeat_label.back());
  cd.begin_label.pop_back();
  cd.repeat_label.pop_back();
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
  //pm.insert(std::pair<std::string, prim_fun>("Z*", &primitive_complex_mul));
  //pm.insert(std::pair<std::string, prim_fun>("Z+", &primitive_complex_add));
  //pm.insert(std::pair<std::string, prim_fun>("Z-", &primitive_complex_sub));

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

  //pm.insert(std::pair<std::string, prim_fun>("DROP", &primitive_drop));
  //pm.insert(std::pair<std::string, prim_fun>("SWAP", &primitive_swap));
  //pm.insert(std::pair<std::string, prim_fun>("DUP", &primitive_dup));
  //pm.insert(std::pair<std::string, prim_fun>("OVER", &primitive_over));
  //pm.insert(std::pair<std::string, prim_fun>("ROT", &primitive_rot));
  //pm.insert(std::pair<std::string, prim_fun>("-ROT", &primitive_minrot));
  //pm.insert(std::pair<std::string, prim_fun>("2DROP", &primitive_twodrop));
  //pm.insert(std::pair<std::string, prim_fun>("2DUP", &primitive_twodup));
  //pm.insert(std::pair<std::string, prim_fun>("2SWAP", &primitive_twoswap));  
  //pm.insert(std::pair<std::string, prim_fun>("MIN", &primitive_min));
  //pm.insert(std::pair<std::string, prim_fun>("MAX", &primitive_max));

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

  //pm.insert(std::pair<std::string, prim_fun>("SIN", &primitive_sin));
  //pm.insert(std::pair<std::string, prim_fun>("COS", &primitive_cos));
  //pm.insert(std::pair<std::string, prim_fun>("EXP", &primitive_exp));
  //pm.insert(std::pair<std::string, prim_fun>("LOG", &primitive_log));
  //pm.insert(std::pair<std::string, prim_fun>("**", &primitive_pow));
  //pm.insert(std::pair<std::string, prim_fun>("POW", &primitive_pow));
  //pm.insert(std::pair<std::string, prim_fun>("ATAN2", &primitive_atan2));
  //pm.insert(std::pair<std::string, prim_fun>("SQRT", &primitive_sqrt));
  //pm.insert(std::pair<std::string, prim_fun>("ABS", &primitive_abs));
  //pm.insert(std::pair<std::string, prim_fun>("NEGATE", &primitive_negate));
  //pm.insert(std::pair<std::string, prim_fun>("AND", &primitive_and));
  //pm.insert(std::pair<std::string, prim_fun>("OR", &primitive_or));
  //pm.insert(std::pair<std::string, prim_fun>("XOR", &primitive_xor));
  //pm.insert(std::pair<std::string, prim_fun>("NOT", &primitive_not));

  //pm.insert(std::pair<std::string, prim_fun>("FM/MOD", &primitive_fmod));
  pm.insert(std::pair<std::string, prim_fun>("fm/mod", &primitive_fmod));

  //pm.insert(std::pair<std::string, prim_fun>("FLOOR", &primitive_floor));
  //pm.insert(std::pair<std::string, prim_fun>("CEIL", &primitive_ceil));
  //pm.insert(std::pair<std::string, prim_fun>("ROUND", &primitive_round));
  //pm.insert(std::pair<std::string, prim_fun>("TRUNC", &primitive_truncate));
  //pm.insert(std::pair<std::string, prim_fun>("FCAST", &primitive_fcast));
  //pm.insert(std::pair<std::string, prim_fun>("ICAST", &primitive_icast));

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
  pm.insert(std::pair<std::string, prim_fun>("#1+", &primitive_incri));
  pm.insert(std::pair<std::string, prim_fun>("#1-", &primitive_decri));
  pm.insert(std::pair<std::string, prim_fun>("#32+", &primitive_incr32i));
  pm.insert(std::pair<std::string, prim_fun>("#32-", &primitive_decr32i));
  pm.insert(std::pair<std::string, prim_fun>("#64+", &primitive_incr64i));
  pm.insert(std::pair<std::string, prim_fun>("#64-", &primitive_decr64i));
  pm.insert(std::pair<std::string, prim_fun>("#96+", &primitive_incr96i));
  pm.insert(std::pair<std::string, prim_fun>("#96-", &primitive_decr96i));
  pm.insert(std::pair<std::string, prim_fun>("#128+", &primitive_incr128i));
  pm.insert(std::pair<std::string, prim_fun>("#128-", &primitive_decr128i));
  pm.insert(std::pair<std::string, prim_fun>("#160+", &primitive_incr160i));
  pm.insert(std::pair<std::string, prim_fun>("#160-", &primitive_decr160i));
  pm.insert(std::pair<std::string, prim_fun>("#192+", &primitive_incr192i));
  pm.insert(std::pair<std::string, prim_fun>("#192-", &primitive_decr192i));
  pm.insert(std::pair<std::string, prim_fun>("#224+", &primitive_incr224i));
  pm.insert(std::pair<std::string, prim_fun>("#224-", &primitive_decr224i));
  pm.insert(std::pair<std::string, prim_fun>("#256+", &primitive_incr256i));
  pm.insert(std::pair<std::string, prim_fun>("#256-", &primitive_decr256i));

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
  

  //pm.insert(std::pair<std::string, prim_fun>("ST@", &primitive_stack_top_fetch));
  //pm.insert(std::pair<std::string, prim_fun>("SP@", &primitive_stack_pointer_fetch));

  pm.insert(std::pair<std::string, prim_fun>("st@", &primitive_stack_top_fetch));
  pm.insert(std::pair<std::string, prim_fun>("sp@", &primitive_stack_pointer_fetch));
  pm.insert(std::pair<std::string, prim_fun>("here", &primitive_here));
  pm.insert(std::pair<std::string, prim_fun>("cells", &primitive_cells));
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

  return pm;
  }

VF_END