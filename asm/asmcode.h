#pragma once

#include <list>
#include <vector>
#include <string>
#include <stdint.h>
#include <ostream>
#include "namespace.h"
#include "asm_api.h"

ASM_BEGIN

class asmcode
  {
  public:
    enum operation
      {
      ADD,
      ADDSD,
      ADDSS,
      ADDPS,
      AND,
      CALL,
      CMP,
      CMPEQPD,
      CMPLTPD,
      CMPLEPD,
      COMMENT,
      CQO,
      CDQ,
      CVTSI2SD,
      CVTSD2SS,
      CVTSI2SS,
      CVTTSD2SI,
      CVTTSS2SI,
      DATA,
      DQ,
      DEC,
      DIV,
      DIVSD,
      DIVPS,
      EXTERN,
      FABS,
      F2XM1,
      FADD,
      FADDP,
      FDIV,
      FDIVP,
      FILD,
      FIST,
      FISTPQ,
      FCHS,
      FLDQ,
      FLD,
      FLD1,
      FLDLN2,
      FLDL2E,
      FLDPI,
      FMUL,
      FMULP,
      FSIN,
      FCOS,
      FPATAN,
      FPREM,
      FPTAN,
      FRNDINT,
      FSCALE,
      FSQRT,
      FSTP,
      FSTPQ,
      FSUB,
      FSUBR,
      FSUBRP,
      FXCH,
      FYL2X,
      GLOBAL,
      IDIV,
      IMUL,
      INC,
      JMP,
      JA,
      JB,
      JE,
      JL,
      JLE,
      JG,
      JGE,
      JNE,
      JMPS, // JMP short
      JAS, // JA short
      JBS, // JB short
      JES, // JE short
      JLS, // JL short
      JLES, // JLE short
      JGS, // JG short
      JGES, // JGE short
      JNES, // JNE short
      LABEL,
      LABEL_ALIGNED,
      MOV,
      MOVAPS,
      MOVD,
      MOVQ,
      MOVDQA,
      MOVMSKPD,
      MOVSS,
      MOVSD,
      MOVZX,
      MUL,
      MULSD,
      MULPS,
      NEG,
      NOP,
      OR,
      POP,
      PUSH,
      RET,
      SAL,
      SAR,
      SECTION,
      SETE,
      SETNE,
      SETL,
      SETG,
      SETLE,
      SETGE,
      SHL,
      SHUFPS,
      SHR,
      SQRTPD,
      SQRTPS,
      SUB,
      SUBSD,
      SUBPS,
      TEST,
      UCOMISD,
      VADDPS,
      VANDPS,
      VANDNPS,
      VCMPPS,
      VCVTSI2SS,
      VCVTSD2SS,
      VCVTSS2SI,
      VCVTPS2DQ,
      VCVTDQ2PS,
      VDIVPS,
      VMULPS,
      VMOVD,
      VMOVMSKPS,
      VMOVQ,
      VORPS,
      VSUBPS,
      VSQRTPS,
      VBROADCASTSS,
      VMINPS,
      VMAXPS,
      VMOVAPS,
      VPERM2F128,
      VROUNDPS,
      VSHUFPS,
      VXORPS,
      XOR,
      XORPD,
      XORPS
      };

    enum operand
      {
      EMPTY,
      AL,
      AH,
      BL,
      BH,
      CL,
      CH,
      DL,
      DH,
      EAX,
      EBX,
      ECX,
      EDX,
      EDI,
      ESI,
      ESP,
      EBP,
      R8D,
      R9D,
      R10D,
      R11D,
      RAX,
      RBX,
      RCX,
      RDX,
      RSI,
      RDI,
      RSP,
      RBP,
      R8,
      R9,
      R10,
      R11,
      R12,
      R13,
      R14,
      R15,
      MEM_EAX,
      MEM_EBX,
      MEM_ECX,
      MEM_EDX,
      MEM_EDI,
      MEM_ESI,
      MEM_ESP,
      MEM_EBP,
      MEM_RAX,
      MEM_RBX,
      MEM_RCX,
      MEM_RDX,
      MEM_RDI,
      MEM_RSI,
      MEM_RSP,
      MEM_RBP,
      MEM_R8,
      MEM_R9,
      MEM_R10,
      MEM_R11,
      MEM_R12,
      MEM_R13,
      MEM_R14,
      MEM_R15,
      BYTE_MEM_RAX,
      BYTE_MEM_RBX,
      BYTE_MEM_RCX,
      BYTE_MEM_RDX,
      BYTE_MEM_RDI,
      BYTE_MEM_RSI,
      BYTE_MEM_RSP,
      BYTE_MEM_RBP,
      BYTE_MEM_R8,
      BYTE_MEM_R9,
      BYTE_MEM_R10,
      BYTE_MEM_R11,
      BYTE_MEM_R12,
      BYTE_MEM_R13,
      BYTE_MEM_R14,
      BYTE_MEM_R15,
      DWORD_MEM_RAX,
      DWORD_MEM_RBX,
      DWORD_MEM_RCX,
      DWORD_MEM_RDX,
      DWORD_MEM_RDI,
      DWORD_MEM_RSI,
      DWORD_MEM_RSP,
      DWORD_MEM_RBP,
      DWORD_MEM_R8,
      DWORD_MEM_R9,
      DWORD_MEM_R10,
      DWORD_MEM_R11,
      DWORD_MEM_R12,
      DWORD_MEM_R13,
      DWORD_MEM_R14,
      DWORD_MEM_R15,
      MOFFS64,
      NUMBER,
      ST0,
      ST1,
      ST2,
      ST3,
      ST4,
      ST5,
      ST6,
      ST7,
      VARIABLE,
      LABELADDRESS,
      XMM0,
      XMM1,
      XMM2,
      XMM3,
      XMM4,
      XMM5,
      XMM6,
      XMM7,
      YMM0,
      YMM1,
      YMM2,
      YMM3,
      YMM4,
      YMM5,
      YMM6,
      YMM7
      };

    struct instruction
      {
      operation oper;
      operand operand1, operand2, operand3, operand4;
      uint64_t operand1_mem, operand2_mem, operand3_mem, operand4_mem;
      std::string text;

      ASSEMBLER_API instruction();
      ASSEMBLER_API instruction(const std::string& text);
      ASSEMBLER_API instruction(operation op);
      ASSEMBLER_API instruction(operation op, operand op1);
      ASSEMBLER_API instruction(operation op, operand op1, operand op2);
      ASSEMBLER_API instruction(operation op, operand op1, operand op2, operand op3);
      ASSEMBLER_API instruction(operation op, operand op1, uint64_t op1_mem);
      ASSEMBLER_API instruction(operation op, operand op1, uint64_t op1_mem, operand op2);
      ASSEMBLER_API instruction(operation op, operand op1, uint64_t op1_mem, operand op2, uint64_t op2_mem);
      ASSEMBLER_API instruction(operation op, operand op1, operand op2, uint64_t op2_mem);
      ASSEMBLER_API instruction(operation op, operand op1, operand op2, operand op3, uint64_t op3_mem);
      ASSEMBLER_API instruction(operation op, operand op1, operand op2, operand op3, operand op4, uint64_t op4_mem);
      ASSEMBLER_API instruction(operation op, const std::string& text);
      ASSEMBLER_API instruction(operation op, operand op1, const std::string& text);
      ASSEMBLER_API instruction(operation op, operand op1, operand op2, const std::string& text);
      ASSEMBLER_API instruction(operation op, operand op1, uint64_t op1_mem, const std::string& text);
      ASSEMBLER_API instruction(operation op, operand op1, uint64_t op1_mem, operand op2, const std::string& text);
      ASSEMBLER_API instruction(operation op, operand op1, uint64_t op1_mem, operand op2, uint64_t op2_mem, const std::string& text);
      ASSEMBLER_API instruction(operation op, operand op1, operand op2, uint64_t op2_mem, const std::string& text);

      ASSEMBLER_API void stream(std::ostream& out) const;
      ASSEMBLER_API uint64_t fill_opcode(uint8_t* opcode_stream) const;
      };

    ASSEMBLER_API asmcode();
    ASSEMBLER_API ~asmcode();

    template <class... T>
    void add(T&&... val)
      {
      instruction ins(std::forward<T>(val)...);
      (*instructions_list_stack.back()).push_back(ins);
      }

    ASSEMBLER_API void push();
    ASSEMBLER_API void pop();

    ASSEMBLER_API const std::list<std::vector<instruction>>& get_instructions_list() const;
    ASSEMBLER_API std::list<std::vector<instruction>>& get_instructions_list();

    ASSEMBLER_API void stream(std::ostream& out) const;

    ASSEMBLER_API void clear();

  private:
    std::list<std::vector<instruction>> instructions_list;
    std::vector<std::list<std::vector<instruction>>::iterator> instructions_list_stack;


  };

ASM_END
