#include "superoperators.h"
#include "primitives.h"

VF_BEGIN

using namespace ASM;

void superoperator_address_subi(asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR #addr #-");

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, et.int_value);
  code.add(asmcode::SUB, asmcode::RAX, asmcode::R11);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR #addr #-");
  }

void superoperator_address_addi(asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR #addr #+");

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, et.int_value);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::R11);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR #addr #+");
  }

void superoperator_address_muli(asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR #addr #*");

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, et.int_value);
  code.add(asmcode::IMUL, asmcode::R11);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR #addr #*");
  }

void superoperator_address_shiftlefti(asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR #addr #<<");

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RCX, asmcode::NUMBER, et.int_value);
  code.add(asmcode::SHL, asmcode::RAX, asmcode::CL);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR #addr #<<");
  }

void superoperator_address_shiftrighti(asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR #addr #>>");

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RCX, asmcode::NUMBER, et.int_value);
  code.add(asmcode::SHR, asmcode::RAX, asmcode::CL);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR #addr #>>");
  }

void superoperator_address_subi_fetch(asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR #addr #- @");

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, et.int_value);
  code.add(asmcode::SUB, asmcode::RAX, asmcode::R11);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR #addr #- @");
  }

void superoperator_address_addi_fetch(asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR #addr #+ @");

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, et.int_value);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::R11);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR #addr #+ @");
  }

void superoperator_stacktopfetch_address_subi_fetch(asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR st@ #addr #- @");

  code.add(asmcode::MOV, asmcode::RAX, STACK_TOP);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, et.int_value);
  code.add(asmcode::SUB, asmcode::RAX, asmcode::R11);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR st@ #addr #- @");
  }


void superoperator_stacktopfetch_address_subi_fetch_twice(asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR st@ #addr #- @ st@ #addr #- @");

  code.add(asmcode::MOV, asmcode::RAX, STACK_TOP);
  code.add(asmcode::MOV, asmcode::R11, asmcode::NUMBER, et.int_value);
  code.add(asmcode::MOV, asmcode::RCX, asmcode::NUMBER, et.reserve1);
  code.add(asmcode::SUB, asmcode::RCX, asmcode::R11);
  code.add(asmcode::SUB, asmcode::RAX, asmcode::R11);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::SUB, asmcode::RAX, asmcode::RCX);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(1), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, -AVX_CELLS(2), AVX_REG1);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));


  code.add(asmcode::COMMENT, "END SUPEROPERATOR st@ #addr #- @ st@ #addr #- @");
  }

void superoperator_float_add(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float +");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float +");
  }

void superoperator_float_sub(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float -");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float -");
  }

void superoperator_float_mul(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float *");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float *");
  }

void superoperator_float_div(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float /");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float /");
  }

void superoperator_float_add_float_add(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float + float +");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float + float +");
  }
void superoperator_float_sub_float_add(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float - float +");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float - float +");
  }
void superoperator_float_mul_float_add(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float * float +");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float * float +");
  }
void superoperator_float_div_float_add(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float / float +");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float / float +");
  }
void superoperator_float_add_float_sub(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float + float -");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float + float -");
  }
void superoperator_float_sub_float_sub(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float - float -");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float - float -");
  }
void superoperator_float_mul_float_sub(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float * float -");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float * float -");
  }
void superoperator_float_div_float_sub(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float / float -");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float / float -");
  }
void superoperator_float_add_float_mul(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float + float *");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float + float *");
  }
void superoperator_float_sub_float_mul(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float - float *");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float - float *");
  }
void superoperator_float_mul_float_mul(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float * float *");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float * float *");
  }
void superoperator_float_div_float_mul(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float / float *");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float / float *");
  }
void superoperator_float_add_float_div(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float + float /");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float + float /");
  }
void superoperator_float_sub_float_div(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float - float /");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float - float /");
  }
void superoperator_float_mul_float_div(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float * float /");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float * float /");
  }
void superoperator_float_div_float_div(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float / float /");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);

  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float / float /");
  }

void superoperator_float_add_add(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float + +");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float + +");
  }
void superoperator_float_add_sub(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float + -");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float + -");
  }
void superoperator_float_add_mul(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float + *");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float + *");
  }
void superoperator_float_add_div(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float + /");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float + /");
  }
void superoperator_float_sub_add(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float - +");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float - +");
  }
void superoperator_float_sub_sub(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float - -");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float - -");
  }
void superoperator_float_sub_mul(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float - *");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float - *");
  }
void superoperator_float_sub_div(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float - /");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float - /");
  }
void superoperator_float_mul_add(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float * +");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float * +");
  }
void superoperator_float_mul_sub(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float * -");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float * -");
  }
void superoperator_float_mul_mul(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float * *");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float * *");
  }
void superoperator_float_mul_div(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float * /");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float * /");
  }
void superoperator_float_div_add(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float / +");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float / +");
  }
void superoperator_float_div_sub(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float / -");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float / -");
  }
void superoperator_float_div_mul(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float / *");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float / *");
  }
void superoperator_float_div_div(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float / /");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG1, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float / /");
  }

void superoperator_swap_drop(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR swap drop");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR swap drop");
  }

void superoperator_swap_over(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR swap over");
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(1));

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG1);

  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG1);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR swap over");
  }

void superoperator_swap_over_address_addi_store(ASM::asmcode& code, const expanded_token& et)
  {
  // speeds up vec3!
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR swap over #addr #+ store");

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RAX);
  code.add(asmcode::MOV, asmcode::RCX, asmcode::NUMBER, et.int_value);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::RCX);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR swap over #addr #+ store");
  }

void superoperator_address_here_fetch_swap_here_addstorei(ASM::asmcode& code, const expanded_token& et)
  {
  // speeds up allot
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR address here @ swap here #+!");

  code.add(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, et.int_value);
  code.add(asmcode::MOV, asmcode::RCX, MEM_HERE);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, -AVX_CELLS(1), asmcode::RCX);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::ADD, MEM_HERE, asmcode::RAX);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR address here @ swap here #+!");
  }

void superoperator_here_fetch_swap_here_addstorei(ASM::asmcode& code, const expanded_token& et)
  {
  // speeds up allot
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR here @ swap here #+!");

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER);
  code.add(asmcode::MOV, asmcode::RCX, MEM_HERE);
  code.add(asmcode::MOV, MEM_STACK_REGISTER, asmcode::RCX);
  code.add(asmcode::ADD, MEM_HERE, asmcode::RAX);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR here @ swap here #+!");
  }

void superoperator_dup_floor_sub(ASM::asmcode& code, const expanded_token& et)
  {
  // speeds up fract
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup floor -");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
#ifdef AVX512
  code.add(asmcode::VRNDSCALEPS, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1 + 8);
#else
  code.add(asmcode::VROUNDPS, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1);
#endif
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup floor -");
  }

void superoperator_variable_fetch(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR variable @");
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::MOV, asmcode::RCX, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RCX);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR variable @");
  }

void superoperator_variable_integer_addi_fetch(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR variable #addr #+ @");
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::MOV, asmcode::RCX, asmcode::MEM_RAX);
  code.add(asmcode::ADD, asmcode::RCX, asmcode::NUMBER, et.int_value);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RCX);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR variable #addr #+ @");
  }

void superoperator_rtpopop(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR r> r>");

  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RSP);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RSP, AVX_CELLS(1));
  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG1);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR r> r>");
  }

void superoperator_rtpopopop(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR r> r> r>");

  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RSP);
  code.add(asmcode::VMOVAPS, AVX_REG1, asmcode::MEM_RSP, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG2, asmcode::MEM_RSP, AVX_CELLS(2));
  code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(2), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG2);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR r> r> r>");
  }

void superoperator_muladd(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR * +");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR * +");
  }

void superoperator_mulsub(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR * -");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR * -");
  }

void superoperator_muldiv(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR * /");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR * /");
  }

void superoperator_mulmul(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR * *");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR * *");
  }

void superoperator_addmul(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR + *");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VADDPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR + *");

  }
void superoperator_addsub(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR + -");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VADDPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR + -");
  }
void superoperator_adddiv(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR + /");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VADDPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR + /");
  }

void superoperator_addadd(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR + +");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VADDPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR + +");
  }

void superoperator_subadd(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR - +");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR - +");
  }

void superoperator_submul(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR - *");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR - *");
  }

void superoperator_subdiv(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR - /");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR - /");
  }

void superoperator_subsub(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR - -");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR - -");
  }

void superoperator_divadd(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR / +");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VDIVPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR / +");
  }

void superoperator_divmul(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR / *");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VDIVPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR / *");
  }

void superoperator_divsub(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR / -");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VDIVPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR / -");
  }

void superoperator_divdiv(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR / /");
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VDIVPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR / /");
  }

void superoperator_mulmulmul(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR * * *");
  code.add(asmcode::VMOVAPS, AVX_REG3, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(3));
  code.add(asmcode::VMULPS, AVX_REG2, AVX_REG2, AVX_REG3);
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VMULPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR * * *");
  }

void superoperator_addaddadd(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR + + +");
  code.add(asmcode::VMOVAPS, AVX_REG3, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(3));
  code.add(asmcode::VADDPS, AVX_REG2, AVX_REG2, AVX_REG3);
  code.add(asmcode::VADDPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR + + +");
  }

void superoperator_subsubsub(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR - - -");
  code.add(asmcode::VMOVAPS, AVX_REG3, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(3));
  code.add(asmcode::VSUBPS, AVX_REG2, AVX_REG2, AVX_REG3);
  code.add(asmcode::VSUBPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR - - -");
  }

void superoperator_divdivdiv(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR / / /");
  code.add(asmcode::VMOVAPS, AVX_REG3, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(3));
  code.add(asmcode::VDIVPS, AVX_REG2, AVX_REG2, AVX_REG3);
  code.add(asmcode::VDIVPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VDIVPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR / / /");
  }

void superoperator_dupup(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup dup");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup dup");
  }
void superoperator_dupupup(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup dup dup");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(2), AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup dup dup");
  }

void superoperator_dupupupup(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup dup dup dup");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(4));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(1), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(2), AVX_REG0);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_CELLS(3), AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup dup dup dup");
  }

void superoperator_addmuladd(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR + * +");
  code.add(asmcode::VMOVAPS, AVX_REG3, MEM_STACK_REGISTER);
  code.add(asmcode::VMOVAPS, AVX_REG2, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG1, MEM_STACK_REGISTER, AVX_CELLS(2));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(3));
  code.add(asmcode::VADDPS, AVX_REG2, AVX_REG2, AVX_REG3);
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG1, AVX_REG2);
  code.add(asmcode::VADDPS, AVX_REG0, AVX_REG0, AVX_REG1);
  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR + * +");
  }

void superoperator_rtpush_2dup_addr_addi_fetch(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR >r 2dup #addr #+ @");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, asmcode::MEM_RSP, AVX_REG0);

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER, AVX_CELLS(2));

  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::MOV, asmcode::RCX, asmcode::NUMBER, et.int_value);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::RCX);

  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR >r 2dup #addr #+ @");
  }

void superoperator_rtpush_addr_addi_fetch(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR >r #addr #+ @");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, asmcode::MEM_RSP, AVX_REG0);

  code.add(asmcode::MOV, asmcode::RAX, MEM_STACK_REGISTER, AVX_CELLS(1));
  code.add(asmcode::MOV, asmcode::RCX, asmcode::NUMBER, et.int_value);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::RCX);

  code.add(asmcode::ADD, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR >r #addr #+ @");
  }

void superoperator_var_float_equ(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR var float =");
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR var float =");
  }

void superoperator_var_float_nequ(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR var float <>");
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 4);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 4);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR var float <>");
  }
void superoperator_var_float_lt(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR var float <");
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR var float <");
  }

void superoperator_var_float_le(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR var float <=");
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 2);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 2);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR var float <=");
  }

void superoperator_var_float_gt(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR var float >");
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR var float >");
  }

void superoperator_var_float_ge(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR var float >=");
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR var float >=");
  }

void superoperator_var_float_fequ(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR var float f=");
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR var float f=");
  }

void superoperator_var_float_fnequ(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR var float f<>");
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 4);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 4);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR var float f<>");
  }
void superoperator_var_float_flt(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR var float f<");
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR var float f<");
  }

void superoperator_var_float_fle(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR var float f<=");
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 2);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 2);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR var float f<=");
  }

void superoperator_var_float_fgt(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR var float f>");
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR var float f>");
  }

void superoperator_var_float_fge(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR var float f>=");
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::MEM_RAX);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR var float f>=");
  }

void superoperator_dup_float_lt(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup float <");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup float <");
  }

void superoperator_dup_float_gt(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup float >");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1);
#endif  
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup float >");
  }

void superoperator_dup_float_le(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup float <=");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 2);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 2);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup float <=");
  }

void superoperator_dup_float_ge(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup float >=");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2);
#endif  
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup float >=");
  }

void superoperator_dup_float_equ(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup float =");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup float =");
  }

void superoperator_dup_float_nequ(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup float <>");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 4);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ALL_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 4);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup float <>");
  }

void superoperator_dup_float_fequ(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup float f=");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 0);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup float f=");
  }

void superoperator_dup_float_fnequ(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup float f<>");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 4);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 4);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup float f<>");
  }

void superoperator_dup_float_flt(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup float f<");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 1);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup float f<");
  }


void superoperator_dup_float_fgt(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup float f>");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 1);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif  
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup float f>");
  }

void superoperator_dup_float_fle(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup float f<=");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG0, AVX_REG1, asmcode::NUMBER, 2);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG0, AVX_REG1, asmcode::NUMBER, 2);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup float f<=");
  }


void superoperator_dup_float_fge(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR dup float f>=");

  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);

#ifdef AVX512
  code.add(asmcode::VCMPPS, asmcode::K1, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2);
  code.add(asmcode::VXORPS, AVX_REG0, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, ONEF_BITS);
  code.add(asmcode::VMOVAPS, AVX_REG0, asmcode::k1, AVX_REG1);
#else
  code.add(asmcode::VCMPPS, AVX_REG0, AVX_REG1, AVX_REG0, asmcode::NUMBER, 2);
  code.add(asmcode::VCVTDQ2PS, AVX_REG0, AVX_REG0);
  code.add(asmcode::VMOVAPS, AVX_REG1, NOT_SIGN_BIT);
  code.add(asmcode::VANDPS, AVX_REG0, AVX_REG0, AVX_REG1);
#endif  
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR dup float f>=");
  }

void superoperator_float_fmmod_drop(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float fm/mod drop");
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_STACK_REGISTER);
  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER);
  code.add(asmcode::VDIVPS, AVX_REG2, AVX_REG0, AVX_REG1); // quotient
#ifdef AVX512
  code.add(asmcode::VRNDSCALEPS, AVX_REG2, AVX_REG2, asmcode::NUMBER, 8 + 1); // floored quotient
#else
  code.add(asmcode::VROUNDPS, AVX_REG2, AVX_REG2, asmcode::NUMBER, 1); // floored quotient
#endif
  code.add(asmcode::VMULPS, AVX_REG1, AVX_REG2, AVX_REG1); // floored quotient n3 times n1
  code.add(asmcode::VSUBPS, AVX_REG0, AVX_REG0, AVX_REG1); // remainder n2
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR float fm/mod drop");
  }

void superoperator_here_fetch(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR here @");
  code.add(asmcode::SUB, STACK_REGISTER, asmcode::NUMBER, AVX_CELLS(1));
  code.add(asmcode::VMOVAPS, AVX_REG0, MEM_HERE);
  code.add(asmcode::VMOVAPS, MEM_STACK_REGISTER, AVX_REG0);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR here @");
  }

void superoperator_float_comma(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float ,");
  code.add(asmcode::MOV, asmcode::RAX, MEM_HERE);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, asmcode::DWORD_MEM_RAX, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG0, asmcode::DWORD_MEM_RAX);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
#ifdef AVX512
  code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, 64);
#else
  code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, 32);
#endif
  code.add(asmcode::MOV, MEM_HERE, asmcode::RAX);

  code.add(asmcode::COMMENT, "END SUPEROPERATOR float ,");
  }

void superoperator_float_comma_float_comma(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float , float ,");
  code.add(asmcode::MOV, asmcode::RAX, MEM_HERE);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, asmcode::DWORD_MEM_RAX, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG0, asmcode::DWORD_MEM_RAX);
  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, asmcode::DWORD_MEM_RAX, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, asmcode::DWORD_MEM_RAX);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, AVX_CELLS(2));
  code.add(asmcode::MOV, MEM_HERE, asmcode::RAX);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR float , float ,");
  }

void superoperator_float_comma_float_comma_float_comma(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float , float , float ,");
  code.add(asmcode::MOV, asmcode::RAX, MEM_HERE);

  uint32_t v = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, asmcode::DWORD_MEM_RAX, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG0, asmcode::DWORD_MEM_RAX);
  v = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  code.add(asmcode::MOV, asmcode::DWORD_MEM_RAX, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, asmcode::DWORD_MEM_RAX);
  v = *(reinterpret_cast<const uint32_t*>(&et.f[2]));
  code.add(asmcode::MOV, asmcode::DWORD_MEM_RAX, asmcode::NUMBER, v);
  code.add(asmcode::VBROADCASTSS, AVX_REG2, asmcode::DWORD_MEM_RAX);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RAX, AVX_CELLS(2), AVX_REG2);
  code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, AVX_CELLS(3));
  code.add(asmcode::MOV, MEM_HERE, asmcode::RAX);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR float , float , float ,");
  }

void superoperator_float_float_float_var_vec3store(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float float float var vec3!");

  uint32_t v1 = *(reinterpret_cast<const uint32_t*>(&et.f[2]));
  uint32_t v2 = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  uint32_t v3 = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  /*
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1), asmcode::NUMBER, v1);
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4, asmcode::NUMBER, v2);
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 8, asmcode::NUMBER, v3);
  code.add(asmcode::VBROADCASTSS, AVX_REG2, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1));
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4);
  code.add(asmcode::VBROADCASTSS, AVX_REG0, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 8);
  */
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1), asmcode::NUMBER, v1);
  code.add(asmcode::VBROADCASTSS, AVX_REG2, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1));
  if (v1 == v2)
    {
    code.add(asmcode::VMOVAPS, AVX_REG1, AVX_REG2);
    }
  else
    {
    code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4, asmcode::NUMBER, v2);
    code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4);
    }
  if (v1 == v3)
    {
    code.add(asmcode::VMOVAPS, AVX_REG0, AVX_REG2);
    }
  else if (v2 == v3)
    {
    code.add(asmcode::VMOVAPS, AVX_REG0, AVX_REG1);
    }
  else
    {
    code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 8, asmcode::NUMBER, v3);
    code.add(asmcode::VBROADCASTSS, AVX_REG0, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 8);
    }

  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::MOV, asmcode::RCX, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_CELLS(2), AVX_REG2);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR float float float var vec3!");
  }

void superoperator_float_float_var_vec2store(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float float var vec2!");

  uint32_t v1 = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  uint32_t v2 = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1), asmcode::NUMBER, v1);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1));

  if (v1 == v2)
    {
    code.add(asmcode::VMOVAPS, AVX_REG0, AVX_REG1);
    }
  else
    {
    code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4, asmcode::NUMBER, v2);
    code.add(asmcode::VBROADCASTSS, AVX_REG0, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4);
    }
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::MOV, asmcode::RCX, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR float float var vec2!");
  }

void superoperator_float_float_float_float_var_mat2x2store(ASM::asmcode& code, const expanded_token& et)
  {
  code.add(asmcode::COMMENT, "BEGIN SUPEROPERATOR float float float float var mat2x2!");

  uint32_t v1 = *(reinterpret_cast<const uint32_t*>(&et.f[3]));
  uint32_t v2 = *(reinterpret_cast<const uint32_t*>(&et.f[2]));
  uint32_t v3 = *(reinterpret_cast<const uint32_t*>(&et.f[1]));
  uint32_t v4 = *(reinterpret_cast<const uint32_t*>(&et.f[0]));
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1), asmcode::NUMBER, v1);
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4, asmcode::NUMBER, v2);
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 8, asmcode::NUMBER, v3);
  code.add(asmcode::MOV, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 12, asmcode::NUMBER, v4);
  code.add(asmcode::VBROADCASTSS, AVX_REG3, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1));
  code.add(asmcode::VBROADCASTSS, AVX_REG2, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 4);
  code.add(asmcode::VBROADCASTSS, AVX_REG1, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 8);
  code.add(asmcode::VBROADCASTSS, AVX_REG0, DWORD_MEM_STACK_REGISTER, -AVX_CELLS(1) - 12);
  code.add(asmcode::MOV, asmcode::RAX, CONSTANT_SPACE_POINTER);
  if (et.variable_address)
    code.add(asmcode::ADD, asmcode::RAX, asmcode::NUMBER, et.variable_address);
  code.add(asmcode::MOV, asmcode::RCX, asmcode::MEM_RAX);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_REG0);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_CELLS(1), AVX_REG1);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_CELLS(2), AVX_REG2);
  code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, AVX_CELLS(3), AVX_REG3);
  code.add(asmcode::COMMENT, "END SUPEROPERATOR float float float float var mat2x2!");
  }

namespace
  {
  bool is_float(std::vector<expanded_token>::iterator it)
    {
    return it->t == expanded_token::ET_FLOAT;
    }

  bool is_primitive(std::vector<expanded_token>::iterator it)
    {
    return it->t == expanded_token::ET_PRIMITIVE;
    }

  bool is_integer(std::vector<expanded_token>::iterator it)
    {
    return it->t == expanded_token::ET_INTEGER;
    }

  bool is_variable(std::vector<expanded_token>::iterator it)
    {
    return it->t == expanded_token::ET_VARIABLE;
    }

  bool is_subi(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_subi);
    }

  bool is_addi(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_addi);
    }

  bool is_muli(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_muli);
    }

  bool is_shiftlefti(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_shiftlefti);
    }

  bool is_shiftrighti(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_shiftrighti);
    }

  bool is_fetch(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_fetch);
    }

  bool is_stack_top_fetch(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_stack_top_fetch);
    }

  bool is_mul(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_mul);
    }

  bool is_div(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_div);
    }

  bool is_add(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_add);
    }

  bool is_sub(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_sub);
    }

  bool is_swap(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_swap);
    }

  bool is_over(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_over);
    }

  bool is_drop(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_drop);
    }

  bool is_fmmod(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_fmod);
    }

  bool is_dup(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_dup);
    }

  bool is_2dup(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_twodup);
    }

  bool is_store(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_store);
    }

  bool is_addstorei(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_addstorei);
    }

  bool is_here(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_here);
    }

  bool is_comma(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_comma);
    }

  bool is_floor(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_floor);
    }

  bool is_return_stack_pop(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_return_stack_pop);
    }

  bool is_return_stack_push(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_return_stack_push);
    }

  bool is_lt(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_lt);
    }

  bool is_gt(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_gt);
    }

  bool is_le(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_le);
    }

  bool is_ge(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_ge);
    }

  bool is_equ(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_equ);
    }

  bool is_nequ(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_nequ);
    }

  bool is_flt(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_flt);
    }

  bool is_fgt(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_fgt);
    }

  bool is_fle(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_fle);
    }

  bool is_fge(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_fge);
    }

  bool is_fequ(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_fequ);
    }

  bool is_fnequ(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_fnequ);
    }

  bool is_vec3_store(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_vec3_store);
    }

  bool is_vec2_store(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_vec2_store);
    }

  bool is_mat2x2_store(std::vector<expanded_token>::iterator it)
    {
    return is_primitive(it) && (it->prim == &primitive_mat2x2_store);
    }

  std::vector<expanded_token>::iterator combine_variable_ops(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(it, words.end());
    if (sz >= 4)
      {
      auto it1 = it + 1;
      auto it2 = it + 2;
      auto it3 = it + 3;
      if (is_integer(it1) && is_addi(it2) && is_fetch(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->int_value = it1->int_value;
        it->supop = &superoperator_variable_integer_addi_fetch;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      }
    if (sz >= 3)
      {
      auto it1 = it + 1;
      auto it2 = it + 2;
      if (is_float(it1))
        {
        if (is_lt(it2))
          {
          it->f[0] = it1->f[0];
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_var_float_lt;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_le(it2))
          {
          it->f[0] = it1->f[0];
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_var_float_le;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_gt(it2))
          {
          it->f[0] = it1->f[0];
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_var_float_gt;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_ge(it2))
          {
          it->f[0] = it1->f[0];
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_var_float_ge;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_equ(it2))
          {
          it->f[0] = it1->f[0];
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_var_float_equ;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_nequ(it2))
          {
          it->f[0] = it1->f[0];
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_var_float_nequ;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }

        if (is_flt(it2))
          {
          it->f[0] = it1->f[0];
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_var_float_flt;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_fle(it2))
          {
          it->f[0] = it1->f[0];
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_var_float_fle;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_fgt(it2))
          {
          it->f[0] = it1->f[0];
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_var_float_fgt;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_fge(it2))
          {
          it->f[0] = it1->f[0];
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_var_float_fge;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_fequ(it2))
          {
          it->f[0] = it1->f[0];
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_var_float_fequ;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_fnequ(it2))
          {
          it->f[0] = it1->f[0];
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_var_float_fnequ;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        }
      }
    if (sz >= 2)
      {
      auto it1 = it + 1;
      if (is_fetch(it1))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_variable_fetch;
        *it1 = *it;
        it = words.erase(it, it1);
        return it;
        }
      }
    return it;
    }

  std::vector<expanded_token>::iterator combine_floats(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(it, words.end());
    if (sz >= 6)
      {
      auto it1 = it + 1;
      auto it2 = it + 2;
      auto it3 = it + 3;
      auto it4 = it + 4;
      auto it5 = it + 5;
      if (is_comma(it1) && is_float(it2) && is_comma(it3) && is_float(it4) && is_comma(it5))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->f[2] = it4->f[0];
        it->supop = &superoperator_float_comma_float_comma_float_comma;
        *it5 = *it;
        it = words.erase(it, it5);
        return it;
        }

      if (is_float(it3) && is_float(it2) && is_float(it1))
        {
        if (is_variable(it4) && is_mat2x2_store(it5))
          {
          it->f[1] = it1->f[0];
          it->f[2] = it2->f[0];
          it->f[3] = it3->f[0];
          it->variable_address = it4->variable_address;
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_float_float_float_var_mat2x2store;
          *it5 = *it;
          it = words.erase(it, it5);
          return it;
          }
        if (is_float(it5) && is_float(it4))
          {
          it->f[1] = it1->f[0];
          it->f[2] = it2->f[0];
          it->f[3] = it3->f[0];
          it->f[4] = it4->f[0];
          it->f[5] = it5->f[0];
          it->t = expanded_token::ET_FLOAT6;
          *it5 = *it;
          it = words.erase(it, it5);
          return it;
          }
        }
      }
    if (sz >= 5)
      {
      auto it1 = it + 1;
      auto it2 = it + 2;
      auto it3 = it + 3;
      auto it4 = it + 4;
      if (is_float(it1))
        {
        if (is_float(it2))
          {
          if (is_variable(it3) && is_vec3_store(it4))
            {
            it->f[1] = it1->f[0];
            it->f[2] = it2->f[0];
            it->variable_address = it3->variable_address;
            it->t = expanded_token::ET_SUPEROPERATOR;
            it->supop = &superoperator_float_float_float_var_vec3store;
            *it4 = *it;
            it = words.erase(it, it4);
            return it;
            }
          if (is_float(it3) && is_float(it4))
            {
            it->f[1] = it1->f[0];
            it->f[2] = it2->f[0];
            it->f[3] = it3->f[0];
            it->f[4] = it4->f[0];
            it->t = expanded_token::ET_FLOAT5;
            *it4 = *it;
            it = words.erase(it, it4);
            return it;
            }
          }
        }
      }
    if (sz >= 4)
      {
      auto it1 = it + 1;
      auto it2 = it + 2;
      auto it3 = it + 3;
      if (is_comma(it1) && is_float(it2) && is_comma(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_comma_float_comma;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      if (is_float(it1))
        {
        if (is_variable(it2) && is_vec2_store(it3))
          {
          it->f[1] = it1->f[0];
          it->variable_address = it2->variable_address;
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_float_var_vec2store;
          *it3 = *it;
          it = words.erase(it, it3);
          return it;
          }
        if (is_float(it2) && is_float(it3))
          {
          it->f[1] = it1->f[0];
          it->f[2] = it2->f[0];
          it->f[3] = it3->f[0];
          it->t = expanded_token::ET_FLOAT4;
          *it3 = *it;
          it = words.erase(it, it3);
          return it;
          }
        }

      if (is_add(it1) && is_float(it2) && is_add(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_add_float_add;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      if (is_add(it1) && is_float(it2) && is_sub(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_add_float_sub;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      if (is_add(it1) && is_float(it2) && is_mul(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_add_float_mul;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      if (is_add(it1) && is_float(it2) && is_div(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_add_float_div;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }

      if (is_sub(it1) && is_float(it2) && is_add(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_sub_float_add;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      if (is_sub(it1) && is_float(it2) && is_sub(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_sub_float_sub;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      if (is_sub(it1) && is_float(it2) && is_mul(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_sub_float_mul;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      if (is_sub(it1) && is_float(it2) && is_div(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_sub_float_div;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }

      if (is_mul(it1) && is_float(it2) && is_add(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_mul_float_add;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      if (is_mul(it1) && is_float(it2) && is_sub(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_mul_float_sub;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      if (is_mul(it1) && is_float(it2) && is_mul(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_mul_float_mul;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      if (is_mul(it1) && is_float(it2) && is_div(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_mul_float_div;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }

      if (is_div(it1) && is_float(it2) && is_add(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_div_float_add;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      if (is_div(it1) && is_float(it2) && is_sub(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_div_float_sub;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      if (is_div(it1) && is_float(it2) && is_mul(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_div_float_mul;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      if (is_div(it1) && is_float(it2) && is_div(it3))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->f[1] = it2->f[0];
        it->supop = &superoperator_float_div_float_div;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      }
    if (sz >= 3)
      {
      auto it1 = it + 1;
      auto it2 = it + 2;
      if (is_float(it2) && is_float(it1))
        {
        it->f[1] = it1->f[0];
        it->f[2] = it2->f[0];
        it->t = expanded_token::ET_FLOAT3;
        *it2 = *it;
        it = words.erase(it, it2);
        return it;
        }
      if (is_fmmod(it1) && is_drop(it2))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_float_fmmod_drop;
        *it2 = *it;
        it = words.erase(it, it2);
        return it;
        }
      if (is_add(it1))
        {
        if (is_add(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_add_add;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_sub(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_add_sub;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_mul(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_add_mul;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_div(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_add_div;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        }
      if (is_sub(it1))
        {
        if (is_add(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_sub_add;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_sub(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_sub_sub;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_mul(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_sub_mul;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_div(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_sub_div;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        }
      if (is_mul(it1))
        {
        if (is_add(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_mul_add;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_sub(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_mul_sub;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_mul(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_mul_mul;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_div(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_mul_div;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        }
      if (is_div(it1))
        {
        if (is_add(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_div_add;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_sub(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_div_sub;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_mul(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_div_mul;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_div(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_float_div_div;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        }
      }
    if (sz >= 2)
      {
      auto it1 = it + 1;
      if (is_comma(it1))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_float_comma;
        *it1 = *it;
        it = words.erase(it, it1);
        return it;
        }
      if (is_float(it1))
        {
        it->f[1] = it1->f[0];
        it->t = expanded_token::ET_FLOAT2;
        *it1 = *it;
        it = words.erase(it, it1);
        return it;
        }
      if (is_add(it1))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_float_add;
        *it1 = *it;
        it = words.erase(it, it1);
        return it;
        }
      if (is_sub(it1))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_float_sub;
        *it1 = *it;
        it = words.erase(it, it1);
        return it;
        }
      if (is_mul(it1))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_float_mul;
        *it1 = *it;
        it = words.erase(it, it1);
        return it;
        }
      if (is_div(it1))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_float_div;
        *it1 = *it;
        it = words.erase(it, it1);
        return it;
        }
      }
    return it;
    } // combine_floats


  std::vector<expanded_token>::iterator combine_primitives(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(it, words.end());
    if (sz >= 8)
      {
      auto it1 = it + 1;
      auto it2 = it + 2;
      auto it3 = it + 3;
      auto it4 = it + 4;
      auto it5 = it + 5;
      auto it6 = it + 6;
      auto it7 = it + 7;
      if (is_stack_top_fetch(it) && is_integer(it1) && is_subi(it2) && is_fetch(it3) && is_stack_top_fetch(it4) && is_integer(it5) && is_subi(it6) && is_fetch(it7))
        {
        it->int_value = it1->int_value;
        it->reserve1 = it5->int_value;
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_stacktopfetch_address_subi_fetch_twice;
        *it7 = *it;
        it = words.erase(it, it7);
        return it;
        }
      }
    if (sz >= 5)
      {
      auto it1 = it + 1;
      auto it2 = it + 2;
      auto it3 = it + 3;
      auto it4 = it + 4;
      if (is_swap(it))
        {
        if (is_over(it1) && is_integer(it2) && is_addi(it3) && is_store(it4))
          {
          it->int_value = it2->int_value;
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_swap_over_address_addi_store;
          *it4 = *it;
          it = words.erase(it, it4);
          return it;
          }
        }
      if (is_return_stack_push(it))
        {
        if (is_2dup(it1) && is_integer(it2) && is_addi(it3) && is_fetch(it4))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->int_value = it2->int_value;
          it->supop = &superoperator_rtpush_2dup_addr_addi_fetch;
          *it4 = *it;
          it = words.erase(it, it4);
          return it;
          }
        }
      if (is_here(it))
        {
        if (is_fetch(it1) && is_swap(it2) && is_here(it3) && is_addstorei(it4))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_here_fetch_swap_here_addstorei;
          *it4 = *it;
          it = words.erase(it, it4);
          return it;
          }
        }
      }
    if (sz >= 4)
      {
      auto it1 = it + 1;
      auto it2 = it + 2;
      auto it3 = it + 3;
      if (is_stack_top_fetch(it) && is_integer(it1) && is_subi(it2) && is_fetch(it3))
        {
        it->int_value = it1->int_value;
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_stacktopfetch_address_subi_fetch;
        *it3 = *it;
        it = words.erase(it, it3);
        return it;
        }
      if (is_dup(it))
        {
        if (is_dup(it1) && is_dup(it2) && is_dup(it3))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_dupupupup;
          *it3 = *it;
          it = words.erase(it, it3);
          return it;
          }
        }
      if (is_return_stack_push(it))
        {
        if (is_integer(it1) && is_addi(it2) && is_fetch(it3))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->int_value = it1->int_value;
          it->supop = &superoperator_rtpush_addr_addi_fetch;
          *it3 = *it;
          it = words.erase(it, it3);
          return it;
          }
        }
      }
    if (sz >= 3)
      {
      auto it1 = it + 1;
      auto it2 = it + 2;
      if (is_dup(it))
        {
        if (is_floor(it1) && is_sub(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_dup_floor_sub;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_float(it1))
          {
          if (is_equ(it2))
            {
            it->t = expanded_token::ET_SUPEROPERATOR;
            it->f[0] = it1->f[0];
            it->supop = &superoperator_dup_float_equ;
            *it2 = *it;
            it = words.erase(it, it2);
            return it;
            }
          if (is_nequ(it2))
            {
            it->t = expanded_token::ET_SUPEROPERATOR;
            it->f[0] = it1->f[0];
            it->supop = &superoperator_dup_float_nequ;
            *it2 = *it;
            it = words.erase(it, it2);
            return it;
            }
          if (is_lt(it2))
            {
            it->t = expanded_token::ET_SUPEROPERATOR;
            it->f[0] = it1->f[0];
            it->supop = &superoperator_dup_float_lt;
            *it2 = *it;
            it = words.erase(it, it2);
            return it;
            }
          if (is_le(it2))
            {
            it->t = expanded_token::ET_SUPEROPERATOR;
            it->f[0] = it1->f[0];
            it->supop = &superoperator_dup_float_le;
            *it2 = *it;
            it = words.erase(it, it2);
            return it;
            }
          if (is_gt(it2))
            {
            it->t = expanded_token::ET_SUPEROPERATOR;
            it->f[0] = it1->f[0];
            it->supop = &superoperator_dup_float_gt;
            *it2 = *it;
            it = words.erase(it, it2);
            return it;
            }
          if (is_ge(it2))
            {
            it->t = expanded_token::ET_SUPEROPERATOR;
            it->f[0] = it1->f[0];
            it->supop = &superoperator_dup_float_ge;
            *it2 = *it;
            it = words.erase(it, it2);
            return it;
            }

          if (is_fequ(it2))
            {
            it->t = expanded_token::ET_SUPEROPERATOR;
            it->f[0] = it1->f[0];
            it->supop = &superoperator_dup_float_fequ;
            *it2 = *it;
            it = words.erase(it, it2);
            return it;
            }
          if (is_fnequ(it2))
            {
            it->t = expanded_token::ET_SUPEROPERATOR;
            it->f[0] = it1->f[0];
            it->supop = &superoperator_dup_float_fnequ;
            *it2 = *it;
            it = words.erase(it, it2);
            return it;
            }
          if (is_flt(it2))
            {
            it->t = expanded_token::ET_SUPEROPERATOR;
            it->f[0] = it1->f[0];
            it->supop = &superoperator_dup_float_flt;
            *it2 = *it;
            it = words.erase(it, it2);
            return it;
            }
          if (is_fle(it2))
            {
            it->t = expanded_token::ET_SUPEROPERATOR;
            it->f[0] = it1->f[0];
            it->supop = &superoperator_dup_float_fle;
            *it2 = *it;
            it = words.erase(it, it2);
            return it;
            }
          if (is_fgt(it2))
            {
            it->t = expanded_token::ET_SUPEROPERATOR;
            it->f[0] = it1->f[0];
            it->supop = &superoperator_dup_float_fgt;
            *it2 = *it;
            it = words.erase(it, it2);
            return it;
            }
          if (is_fge(it2))
            {
            it->t = expanded_token::ET_SUPEROPERATOR;
            it->f[0] = it1->f[0];
            it->supop = &superoperator_dup_float_fge;
            *it2 = *it;
            it = words.erase(it, it2);
            return it;
            }
          }
        }
      if (is_return_stack_pop(it))
        {
        if (is_return_stack_pop(it1) && is_return_stack_pop(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_rtpopopop;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        }
      if (is_add(it))
        {
        if (is_add(it1) && is_add(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_addaddadd;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        if (is_mul(it1) && is_add(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_addmuladd;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        }
      if (is_sub(it))
        {
        if (is_sub(it1) && is_sub(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_subsubsub;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        }
      if (is_mul(it))
        {
        if (is_mul(it1) && is_mul(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_mulmulmul;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        }
      if (is_div(it))
        {
        if (is_div(it1) && is_div(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_divdivdiv;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        }
      if (is_dup(it))
        {
        if (is_dup(it1) && is_dup(it2))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_dupupup;
          *it2 = *it;
          it = words.erase(it, it2);
          return it;
          }
        }
      }
    if (sz >= 2)
      {
      auto it1 = it + 1;
      if (is_here(it) && is_fetch(it1))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_here_fetch;
        *it1 = *it;
        it = words.erase(it, it1);
        return it;
        }
      if (is_swap(it))
        {
        if (is_over(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_swap_over;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        if (is_drop(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_swap_drop;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        }
      if (is_return_stack_pop(it))
        {
        if (is_return_stack_pop(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_rtpopop;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        }
      if (is_add(it))
        {
        if (is_add(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_addadd;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        if (is_mul(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_addmul;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        if (is_sub(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_addsub;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        if (is_div(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_adddiv;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        }
      if (is_sub(it))
        {
        if (is_sub(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_subsub;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        if (is_add(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_subadd;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        if (is_mul(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_submul;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        if (is_div(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_subdiv;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        }
      if (is_mul(it))
        {
        if (is_mul(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_mulmul;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        if (is_add(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_muladd;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        if (is_sub(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_mulsub;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        if (is_div(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_muldiv;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        }
      if (is_div(it))
        {
        if (is_div(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_divdiv;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        if (is_add(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_divadd;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        if (is_mul(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_divmul;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        if (is_sub(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_divsub;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        }
      if (is_dup(it))
        {
        if (is_dup(it1))
          {
          it->t = expanded_token::ET_SUPEROPERATOR;
          it->supop = &superoperator_dupup;
          *it1 = *it;
          it = words.erase(it, it1);
          return it;
          }
        }
      }
    return it;
    }

  std::vector<expanded_token>::iterator combine_address_ops(std::vector<expanded_token>& words, std::vector<expanded_token>::iterator it)
    {
    auto sz = std::distance(it, words.end());
    if (sz >= 6)
      {
      auto it1 = it + 1;
      auto it2 = it + 2;
      auto it3 = it + 3;
      auto it4 = it + 4;
      auto it5 = it + 5;
      if (is_here(it1) && is_fetch(it2) && is_swap(it3) && is_here(it4) && is_addstorei(it5))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_address_here_fetch_swap_here_addstorei;
        *it5 = *it;
        it = words.erase(it, it5);
        return it;
        }
      }
    if (sz >= 3)
      {
      auto it1 = it + 1;
      auto it2 = it + 2;
      if (is_subi(it1) && is_fetch(it2))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_address_subi_fetch;
        *it2 = *it;
        it = words.erase(it, it2);
        return it;
        }
      if (is_addi(it1) && is_fetch(it2))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_address_addi_fetch;
        *it2 = *it;
        it = words.erase(it, it2);
        return it;
        }
      }
    if (sz >= 2)
      {
      auto it1 = it + 1;
      if (is_subi(it1))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_address_subi;
        *it1 = *it;
        it = words.erase(it, it1);
        return it;
        }
      if (is_addi(it1))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_address_addi;
        *it1 = *it;
        it = words.erase(it, it1);
        return it;
        }
      if (is_muli(it1))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_address_muli;
        *it1 = *it;
        it = words.erase(it, it1);
        return it;
        }
      if (is_shiftlefti(it1))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_address_shiftlefti;
        *it1 = *it;
        it = words.erase(it, it1);
        return it;
        }
      if (is_shiftrighti(it1))
        {
        it->t = expanded_token::ET_SUPEROPERATOR;
        it->supop = &superoperator_address_shiftrighti;
        *it1 = *it;
        it = words.erase(it, it1);
        return it;
        }
      }
    return it;
    }

  } // anonymous namespace

void superoperators(std::vector<expanded_token>& words)
  {
  auto it = words.begin();
  while (it != words.end())
    {
    if (is_float(it))
      {
      it = combine_floats(words, it);
      }
    if (is_primitive(it))
      {
      it = combine_primitives(words, it);
      }
    if (is_integer(it))
      {
      it = combine_address_ops(words, it);
      }
    if (is_variable(it))
      {
      it = combine_variable_ops(words, it);
      }
    ++it;
    }
  }

VF_END