///////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////////

#include "AsmCodeTests.h"
#include <stdint.h>
#include <asm/asmcode.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif
#include "test_assert.h"

ASM_BEGIN


struct asmcode_fixture
  {
  asmcode code;
  uint8_t buffer[255];
  };

namespace
  {
  std::string _uchar_to_hex(unsigned char i)
    {
    std::string hex;
    int h1 = (i >> 4) & 0x0f;
    if (h1 < 10)
      hex += '0' + char(h1);
    else
      hex += 'A' + char(h1) - 10;
    int h2 = (i) & 0x0f;
    if (h2 < 10)
      hex += '0' + char(h2);
    else
      hex += 'A' + char(h2) - 10;
    return hex;
    }

  void _check_buffer(uint8_t* buffer, uint64_t size, std::vector<uint8_t> expected)
    {
    bool error = false;
    TEST_EQ(expected.size(), size);
    if (size != expected.size())
      error = true;
    if (size == expected.size())
      {
      for (size_t i = 0; i < size; ++i)
        {
        TEST_EQ(expected[i], buffer[i]);
        if (expected[i] != buffer[i])
          error = true;
        }
      }
    if (error)
      {
      printf("expect : ");
      for (size_t i = 0; i < expected.size(); ++i)
        {
        printf("%s ", _uchar_to_hex(expected[i]).c_str());
        }
      printf("\nactual : ");
      for (size_t i = 0; i < size; ++i)
        {
        printf("%s ", _uchar_to_hex(buffer[i]).c_str());
        }
      printf("\n");
      }
    }


  void asmcode_mov_rax_number()
    {
    asmcode code; uint8_t buffer[255];
    double d = 5.0;
    code.add(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, *reinterpret_cast<uint64_t*>(&d));
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x40 });
    }

  void asmcode_mov_rax_number_2()
    {
    asmcode code; uint8_t buffer[255];
    int64_t d = 5;
    code.add(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, d);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xC7, 0xC0, 0x05, 0x00, 0x00, 0x00 });
    }

  void asmcode_mov_rbx_number()
    {
    asmcode code; uint8_t buffer[255];
    double d = 5.0;
    code.add(asmcode::MOV, asmcode::RBX, asmcode::NUMBER, *reinterpret_cast<uint64_t*>(&d));
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xBB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x40 });
    }

  void asmcode_mov_r15_number()
    {
    asmcode code; uint8_t buffer[255];
    double d = 5.0;
    code.add(asmcode::MOV, asmcode::R15, asmcode::NUMBER, *reinterpret_cast<uint64_t*>(&d));
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x49, 0xBF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x40 });
    }

  void asmcode_mov_rax_rcx()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::RAX, asmcode::RCX);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0xC8 });
    }

  void asmcode_mov_rcx_rax()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::RCX, asmcode::RAX);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0xC1 });
    }

  void asmcode_mov_rax_memrsp_8()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::RAX, asmcode::MEM_RSP, -8);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x8B, 0x44, 0x24, 0xF8 });
    }

  void asmcode_mov_rax_memrsp_4096()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::RAX, asmcode::MEM_RSP, -4096);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x8B, 0x84, 0x24, 0x00, 0xF0, 0xFF, 0xFF });
    }

  void asmcode_mov_memrsp_8_rax()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::MEM_RSP, -8, asmcode::RAX);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x44, 0x24, 0xF8 });
    }

  void asmcode_mov_memrsp_4096_rax()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::MEM_RSP, -4096, asmcode::RAX);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x84, 0x24, 0x00, 0xF0, 0xFF, 0xFF });
    }

  void asmcode_mov_memrsp_8_rcx()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::MEM_RSP, -8, asmcode::RCX);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x4C, 0x24, 0xF8 });
    }

  void asmcode_mov_r14_mem_r13()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::R14, asmcode::MEM_R13);
    code.add(asmcode::MOV, asmcode::R13, asmcode::MEM_R14);
    code.add(asmcode::MOV, asmcode::R13, asmcode::MEM_R13);
    code.add(asmcode::MOV, asmcode::R14, asmcode::MEM_R14);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4D, 0x8B, 0x75, 0x00 });  //<== BUG: TO FIX
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4D, 0x8B, 0x2E });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4D, 0x8B, 0x6D, 0x00 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4D, 0x8B, 0x36 });
    }

  void asmcode_movq_xmm0_rax()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOVQ, asmcode::XMM0, asmcode::RAX);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x48, 0x0F, 0x6E, 0xC0 });
    }

  void asmcode_movq_rax_xmm0()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOVQ, asmcode::RAX, asmcode::XMM0);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x48, 0x0F, 0x7E, 0xC0 });
    }

  void asmcode_mov_rsp_rdx()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::RSP, asmcode::RDX);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0xD4 });
    }

  void asmcode_mov_rbp_r8()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::RBP, asmcode::R8);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4C, 0x89, 0xC5 });
    }

  void asmcode_and_rsp_number()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::AND, asmcode::RSP, asmcode::NUMBER, 0xFFFFFFFFFFFFFFF0);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xE4, 0xF0 });
    }

  void asmcode_and_rsp_number_32()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::AND, asmcode::RSP, asmcode::NUMBER, 0xABCDEF);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x81, 0xE4, 0xEF, 0xCD, 0xAB, 0x00 });
    }

  void asmcode_and_rbp_number()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::AND, asmcode::RBP, asmcode::NUMBER, 0xFFFFFFFFFFFFFFF0);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xE5, 0xF0 });
    }

  void asmcode_and_rbp_number_32()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::AND, asmcode::RBP, asmcode::NUMBER, 0xABCDEF);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x81, 0xE5, 0xEF, 0xCD, 0xAB, 0x00 });
    }




  void asmcode_or_rsp_number()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::OR, asmcode::RSP, asmcode::NUMBER, 0xFFFFFFFFFFFFFFF0);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xCC, 0xF0 });
    }

  void asmcode_or_rsp_number_32()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::OR, asmcode::RSP, asmcode::NUMBER, 0xABCDEF);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x81, 0xCC, 0xEF, 0xCD, 0xAB, 0x00 });
    }

  void asmcode_or_rbp_number()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::OR, asmcode::RBP, asmcode::NUMBER, 0xFFFFFFFFFFFFFFF0);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xCD, 0xF0 });
    }

  void asmcode_or_rbp_number_32()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::OR, asmcode::RBP, asmcode::NUMBER, 0xABCDEF);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x81, 0xCD, 0xEF, 0xCD, 0xAB, 0x00 });
    }

  void asmcode_add_rbp_number_15()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::ADD, asmcode::RBP, asmcode::NUMBER, 15);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xC5, 0x0F });
    }

  void asmcode_add_rbp_number_4096()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::ADD, asmcode::RBP, asmcode::NUMBER, 4096);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x81, 0xC5, 0x00, 0x10, 0x00, 0x00 });
    }

  void asmcode_sub_rbp_number_15()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::SUB, asmcode::RBP, asmcode::NUMBER, 15);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xED, 0x0F });
    }

  void asmcode_sub_rbp_number_4096()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::SUB, asmcode::RBP, asmcode::NUMBER, 4096);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x81, 0xED, 0x00, 0x10, 0x00, 0x00 });
    }

  void asmcode_xor_rax_rax()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::XOR, asmcode::RAX, asmcode::RAX);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x31, 0xC0 });
    }

  void asmcode_xor_r14_r14()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::XOR, asmcode::R14, asmcode::R14);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4D, 0x31, 0xF6 });
    }

  void asmcode_pop_rbp()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::POP, asmcode::RBP);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x5D });
    }

  void asmcode_push_rbp()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::PUSH, asmcode::RBP);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x55 });
    }

  void asmcode_ret()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::RET);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC3 });
    }

  void asmcode_intro_1()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::RBX, asmcode::R9);
    code.add(asmcode::ADD, asmcode::RBX, asmcode::NUMBER, 7);
    code.add(asmcode::AND, asmcode::RBX, asmcode::NUMBER, 0xFFFFFFFFFFFFFFF8);
    code.add(asmcode::XOR, asmcode::R14, asmcode::R14);
    code.add(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, 9843185);
    code.add(asmcode::MOV, asmcode::MEM_RBX, asmcode::RAX);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4C, 0x89, 0xCB });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xC3, 0x07 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xE3, 0xF8 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4D, 0x31, 0xF6 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xC7, 0xC0, 0xF1, 0x31, 0x96, 0x00 });
    size = code.get_instructions_list().front()[5].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x03 });
    }


  void asmcode_store_registers()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::MEM_R10, 8, asmcode::RBX);
    code.add(asmcode::MOV, asmcode::MEM_R10, 32, asmcode::RDI);
    code.add(asmcode::MOV, asmcode::MEM_R10, 40, asmcode::RSI);
    code.add(asmcode::MOV, asmcode::MEM_R10, 48, asmcode::RSP);
    code.add(asmcode::MOV, asmcode::MEM_R10, 56, asmcode::RBP);
    code.add(asmcode::MOV, asmcode::MEM_R10, 96, asmcode::R12);
    code.add(asmcode::MOV, asmcode::MEM_R10, 104, asmcode::R13);
    code.add(asmcode::MOV, asmcode::MEM_R10, 112, asmcode::R14);
    code.add(asmcode::MOV, asmcode::MEM_R10, 120, asmcode::R15);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x49, 0x89, 0x5A, 0x08 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x49, 0x89, 0x7A, 0x20 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x49, 0x89, 0x72, 0x28 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x49, 0x89, 0x62, 0x30 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x49, 0x89, 0x6A, 0x38 });
    size = code.get_instructions_list().front()[5].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4D, 0x89, 0x62, 0x60 });
    size = code.get_instructions_list().front()[6].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4D, 0x89, 0x6A, 0x68 });
    size = code.get_instructions_list().front()[7].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4D, 0x89, 0x72, 0x70 });
    size = code.get_instructions_list().front()[8].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4D, 0x89, 0x7A, 0x78 });
    }


  void asmcode_load_registers()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::RBX, asmcode::MEM_R10, 8);
    code.add(asmcode::MOV, asmcode::RDI, asmcode::MEM_R10, 32);
    code.add(asmcode::MOV, asmcode::RSI, asmcode::MEM_R10, 40);
    code.add(asmcode::MOV, asmcode::RSP, asmcode::MEM_R10, 48);
    code.add(asmcode::MOV, asmcode::RBP, asmcode::MEM_R10, 56);
    code.add(asmcode::MOV, asmcode::R12, asmcode::MEM_R10, 96);
    code.add(asmcode::MOV, asmcode::R13, asmcode::MEM_R10, 104);
    code.add(asmcode::MOV, asmcode::R14, asmcode::MEM_R10, 112);
    code.add(asmcode::MOV, asmcode::R15, asmcode::MEM_R10, 120);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x49, 0x8B, 0x5A, 0x08 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x49, 0x8B, 0x7A, 0x20 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x49, 0x8B, 0x72, 0x28 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x49, 0x8B, 0x62, 0x30 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x49, 0x8B, 0x6A, 0x38 });
    size = code.get_instructions_list().front()[5].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4D, 0x8B, 0x62, 0x60 });
    size = code.get_instructions_list().front()[6].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4D, 0x8B, 0x6A, 0x68 });
    size = code.get_instructions_list().front()[7].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4D, 0x8B, 0x72, 0x70 });
    size = code.get_instructions_list().front()[8].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4D, 0x8B, 0x7A, 0x78 });
    }

  void asmcode_prepare_external_function_call()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, 48);
    code.add(asmcode::PUSH, asmcode::R10);
    code.add(asmcode::PUSH, asmcode::RBP);
    code.add(asmcode::MOV, asmcode::RBP, asmcode::RSP);
    code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, 32 + 8);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xEC, 0x30 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x41, 0x52 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x55 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0xE5 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xEC, 0x28 });
    }

  void finalize_external_function_call()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, 32 + 8);
    code.add(asmcode::POP, asmcode::RBP);
    code.add(asmcode::POP, asmcode::R10);
    code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, 48);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xC4, 0x28 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x5D });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x41, 0x5A });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xC4, 0x30 });
    }

  void code_get_stack_item()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::RAX, asmcode::MEM_RSP, -1064);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x8B, 0x84, 0x24, 0xD8, 0xFB, 0xFF, 0xFF });
    }

  void save_rax_on_stack()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::MEM_RSP, -1064, asmcode::RAX);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x84, 0x24, 0xD8, 0xFB, 0xFF, 0xFF });
    }

  void convert_short_string_to_string()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::MEM_RBP, asmcode::RAX);
    code.add(asmcode::MOV, asmcode::RAX, asmcode::RBP);
    code.add(asmcode::ADD, asmcode::RBP, asmcode::NUMBER, 8);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x45, 0x00 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0xE8 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xC5, 0x08 });
    }

  void asmcode_call()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::CALL, asmcode::RBX);
    code.add(asmcode::CALL, asmcode::MEM_RBX);
    code.add(asmcode::CALL, asmcode::NUMBER, 64);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xFF, 0xD3 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xFF, 0x13 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xE8, 0x40, 0x00, 0x00, 0x00 });
    }

  void asmcode_add_7_and_3()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, 7);
    code.add(asmcode::MOV, asmcode::RCX, asmcode::NUMBER, 3);
    code.add(asmcode::ADD, asmcode::RAX, asmcode::RCX);
    code.add(asmcode::RET);
    uint64_t size = 0;
    const auto& commands = code.get_instructions_list().front();
    for (const auto& c : commands)
      size += c.fill_opcode(buffer);

#ifdef _WIN32
    unsigned char* my_func = (unsigned char*)malloc(size);
#else
    unsigned char* my_func = (unsigned char*)mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
    unsigned char* ptr = my_func;
    for (const auto& c : commands)
      {
      ptr += c.fill_opcode(ptr);
      }
    unsigned long i;
#ifdef _WIN32
    auto success = VirtualProtect((void *)(my_func), size, 0x40/*PAGE_EXECUTE_READWRITE*/, (unsigned long *)&i);
    TEST_ASSERT(success);
#endif
    typedef uint64_t(*fun_ptr)(void);

    uint64_t res = ((fun_ptr)(my_func))();

    TEST_EQ(res, uint64_t(10));

#ifdef _WIN32
    free(my_func);
#else
    munmap((void*)(my_func), size);
#endif
    }

  void asmcode_add_two_integers()
    {
    asmcode code; uint8_t buffer[255];
#ifdef _WIN32
    code.add(asmcode::MOV, asmcode::RAX, asmcode::RCX); // windows calling convention
    code.add(asmcode::ADD, asmcode::RAX, asmcode::RDX);
#else
    code.add(asmcode::MOV, asmcode::RAX, asmcode::RSI); // linux calling convention
    code.add(asmcode::ADD, asmcode::RAX, asmcode::RDI);
#endif
    code.add(asmcode::RET);
    uint64_t size = 0;
    const auto& commands = code.get_instructions_list().front();
    for (const auto& c : commands)
      size += c.fill_opcode(buffer);

#ifdef _WIN32
    unsigned char* my_func = (unsigned char*)malloc(size);
#else
    unsigned char* my_func = (unsigned char*)mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif

    unsigned char* ptr = my_func;
    for (const auto& c : commands)
      {
      ptr += c.fill_opcode(ptr);
      }

    unsigned long i;
#ifdef _WIN32
    auto success = VirtualProtect((void *)(my_func), size, 0x40/*PAGE_EXECUTE_READWRITE*/, (unsigned long *)&i);
    TEST_ASSERT(success);
#endif
    typedef uint64_t(*fun_ptr)(uint64_t a, uint64_t b);

    uint64_t res = ((fun_ptr)(my_func))(3, 7);
    TEST_EQ(res, uint64_t(10));
    res = ((fun_ptr)(my_func))(100, 211);
    TEST_EQ(res, uint64_t(311));

#ifdef _WIN32
    free(my_func);
#else
    munmap((void*)(my_func), size);
#endif
    }

  void asmcode_cmp()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::CMP, asmcode::RAX, asmcode::NUMBER, 1024);
    code.add(asmcode::CMP, asmcode::RBX, asmcode::NUMBER, 1024);
    code.add(asmcode::CMP, asmcode::RAX, asmcode::NUMBER, 8);
    code.add(asmcode::CMP, asmcode::RBX, asmcode::NUMBER, 8);
    code.add(asmcode::CMP, asmcode::RAX, asmcode::RCX);
    code.add(asmcode::CMP, asmcode::RBX, asmcode::RCX);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x3D, 0x00, 0x04, 0x00, 0x00 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x81, 0xFB, 0x00, 0x04, 0x00, 0x00 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xF8, 0x08 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xFB, 0x08 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x39, 0xC8 });
    size = code.get_instructions_list().front()[5].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x39, 0xCB });
    }


  void asmcode_dec_inc()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::INC, asmcode::RAX);
    code.add(asmcode::INC, asmcode::RBX);
    code.add(asmcode::DEC, asmcode::RCX);
    code.add(asmcode::DEC, asmcode::RDX);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xFF, 0xC0 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xFF, 0xC3 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xFF, 0xC9 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xFF, 0xCA });
    }

  void asmcode_mul()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MUL, asmcode::RCX);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xF7, 0xE1 });
    }

  void asmcode_div()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::DIV, asmcode::RCX);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xF7, 0xF1 });
    }

  void asmcode_nop()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::NOP);
    auto first_instruction = code.get_instructions_list().front().front();
    auto size = first_instruction.fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x90 });
    }

  void asmcode_cvt_sd_si()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::CVTSI2SD, asmcode::XMM0, asmcode::RAX);
    code.add(asmcode::CVTSI2SD, asmcode::XMM1, asmcode::RAX);
    code.add(asmcode::CVTTSD2SI, asmcode::RAX, asmcode::XMM0);
    code.add(asmcode::CVTTSD2SI, asmcode::RCX, asmcode::XMM1);
    code.add(asmcode::CVTSI2SS, asmcode::XMM0, asmcode::RAX);
    code.add(asmcode::CVTSI2SS, asmcode::XMM1, asmcode::RAX);
    code.add(asmcode::CVTSD2SS, asmcode::XMM0, asmcode::XMM0);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF2, 0x48, 0x0F, 0x2A, 0xC0 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF2, 0x48, 0x0F, 0x2A, 0xC8 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF2, 0x48, 0x0F, 0x2C, 0xC0 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF2, 0x48, 0x0F, 0x2C, 0xC9 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF3, 0x48, 0x0F, 0x2A, 0xC0 });
    size = code.get_instructions_list().front()[5].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF3, 0x48, 0x0F, 0x2A, 0xC8 });
    size = code.get_instructions_list().front()[6].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF2, 0x0F, 0x5A, 0xC0 });
    }

  void asmcode_cmp_bytes()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::CMP, asmcode::BYTE_MEM_RCX, asmcode::AL);
    code.add(asmcode::CMP, asmcode::BYTE_MEM_RAX, asmcode::AL);
    code.add(asmcode::CMP, asmcode::BYTE_MEM_RSP, asmcode::AL);

    code.add(asmcode::CMP, asmcode::BYTE_MEM_RCX, 8, asmcode::AL);
    code.add(asmcode::CMP, asmcode::BYTE_MEM_RAX, 8, asmcode::AL);
    code.add(asmcode::CMP, asmcode::BYTE_MEM_RSP, -8, asmcode::AL);

    code.add(asmcode::CMP, asmcode::BYTE_MEM_RCX, asmcode::NUMBER, 0);
    code.add(asmcode::CMP, asmcode::BYTE_MEM_RAX, asmcode::NUMBER, 0);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x38, 0x01 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x38, 0x00 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x38, 0x04, 0x24 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x38, 0x41, 0x08 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x38, 0x40, 0x08 });
    size = code.get_instructions_list().front()[5].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x38, 0x44, 0x24, 0xF8 });

    size = code.get_instructions_list().front()[6].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x80, 0x39, 0x00 });
    size = code.get_instructions_list().front()[7].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x80, 0x38, 0x00 });
    }


  void asmcode_mov_bytes()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::AL, asmcode::BYTE_MEM_RDX);
    code.add(asmcode::MOV, asmcode::BYTE_MEM_RDX, asmcode::AL);
    code.add(asmcode::MOV, asmcode::DL, asmcode::BYTE_MEM_RBP);
    code.add(asmcode::MOV, asmcode::BYTE_MEM_RBP, asmcode::DL);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x8A, 0x02 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x88, 0x02 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x8A, 0x55, 0x00 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x88, 0x55, 0x00 });
    }


  void asmcode_sse_basic_ops()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::ADDSD, asmcode::XMM0, asmcode::XMM1);
    code.add(asmcode::SUBSD, asmcode::XMM0, asmcode::XMM1);
    code.add(asmcode::DIVSD, asmcode::XMM0, asmcode::XMM1);
    code.add(asmcode::MULSD, asmcode::XMM0, asmcode::XMM1);

    code.add(asmcode::ADDSS, asmcode::XMM0, asmcode::XMM1);
    code.add(asmcode::ADDPS, asmcode::XMM0, asmcode::XMM1);
    code.add(asmcode::SUBPS, asmcode::XMM0, asmcode::XMM1);
    code.add(asmcode::MULPS, asmcode::XMM0, asmcode::XMM1);
    code.add(asmcode::DIVPS, asmcode::XMM0, asmcode::XMM1);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF2, 0x0F, 0x58, 0xC1 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF2, 0x0F, 0x5C, 0xC1 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF2, 0x0F, 0x5E, 0xC1 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF2, 0x0F, 0x59, 0xC1 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF3, 0x0F, 0x58, 0xC1 });
    size = code.get_instructions_list().front()[5].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x0F, 0x58, 0xC1 });
    size = code.get_instructions_list().front()[6].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x0F, 0x5C, 0xC1 });
    size = code.get_instructions_list().front()[7].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x0F, 0x59, 0xC1 });
    size = code.get_instructions_list().front()[8].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x0F, 0x5E, 0xC1 });
    }

  void asmcode_sse_cmp_ops()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::CMPEQPD, asmcode::XMM0, asmcode::XMM1);
    code.add(asmcode::CMPLTPD, asmcode::XMM0, asmcode::XMM1);
    code.add(asmcode::CMPLEPD, asmcode::XMM0, asmcode::XMM1);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x0F, 0xC2, 0xC1, 0x00 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x0F, 0xC2, 0xC1, 0x01 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x0F, 0xC2, 0xC1, 0x02 });
    }

  void asmcode_sse_other_ops()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOVMSKPD, asmcode::RAX, asmcode::XMM0);
    code.add(asmcode::SQRTPD, asmcode::XMM1, asmcode::XMM0);
    code.add(asmcode::SQRTPS, asmcode::XMM1, asmcode::XMM0);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x0F, 0x50, 0xC0 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x0F, 0x51, 0xC8 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x0F, 0x51, 0xC8 });
    }

  void asmcode_fpu()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::FLD, asmcode::MEM_RSP, -8);
    code.add(asmcode::FLD, asmcode::MEM_RAX, 0);

    code.add(asmcode::FADD);
    code.add(asmcode::FADD, asmcode::ST0, asmcode::ST5);
    code.add(asmcode::FADD, asmcode::ST5, asmcode::ST0);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xDD, 0x44, 0x24, 0xF8 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xDD, 0x00 });

    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xDE, 0xC1 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xD8, 0xC5 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xDC, 0xC5 });
    }

  void asmcode_string_literals()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, 0x6F77206F6C6C6548);
    code.add(asmcode::MOV, asmcode::MEM_RBP, asmcode::RAX);
    code.add(asmcode::ADD, asmcode::RBP, asmcode::NUMBER, 0x0000000000000008);
    code.add(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, 0x0000000021646C72);
    code.add(asmcode::MOV, asmcode::MEM_RBP, asmcode::RAX);
    code.add(asmcode::ADD, asmcode::RBP, asmcode::NUMBER, 0x0000000000000008);
    code.add(asmcode::MOV, asmcode::RAX, asmcode::RBP);
    code.add(asmcode::SUB, asmcode::RAX, asmcode::NUMBER, 0x0000000000000010);
    code.add(asmcode::MOV, asmcode::MEM_RBX, 8, asmcode::RAX);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xB8, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x77, 0x6F });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x45, 0x00 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xC5, 0x08 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xC7, 0xC0, 0x72, 0x6C, 0x64, 0x21 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x45, 0x00 });
    size = code.get_instructions_list().front()[5].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xC5, 0x08 });
    size = code.get_instructions_list().front()[6].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0xE8 });
    size = code.get_instructions_list().front()[7].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xE8, 0x10 });
    size = code.get_instructions_list().front()[8].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x43, 0x08 });
    }


  void asmcode_print_statement()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::RAX, asmcode::MEM_RBX, 8);
    code.add(asmcode::MOV, asmcode::RCX, asmcode::RAX);
    code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, 0);
    code.add(asmcode::PUSH, asmcode::R10);
    code.add(asmcode::PUSH, asmcode::RBP);
    code.add(asmcode::MOV, asmcode::RBP, asmcode::RSP);
    code.add(asmcode::SUB, asmcode::RSP, asmcode::NUMBER, 0x0000000000000028);

    code.add(asmcode::ADD, asmcode::R14, asmcode::RAX);
    code.add(asmcode::XOR, asmcode::R14, asmcode::R14);

    code.add(asmcode::ADD, asmcode::RSP, asmcode::NUMBER, 0);
    code.add(asmcode::POP, asmcode::RBP);
    code.add(asmcode::POP, asmcode::R10);

    uint64_t size;

    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x8B, 0x43, 0x08 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0xC1 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xEC, 0x00 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x41, 0x52 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x55 });
    size = code.get_instructions_list().front()[5].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0xE5 });
    size = code.get_instructions_list().front()[6].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xEC, 0x28 });
    size = code.get_instructions_list().front()[7].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x49, 0x01, 0xC6 });
    size = code.get_instructions_list().front()[8].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x4D, 0x31, 0xF6 });
    size = code.get_instructions_list().front()[9].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xC4, 0x00 });
    size = code.get_instructions_list().front()[10].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x5D });
    size = code.get_instructions_list().front()[11].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x41, 0x5A });
    }


  void asmcode_string_literals_tst()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, 0x0000000000747374);
    code.add(asmcode::MOV, asmcode::MEM_RBP, 0, asmcode::RAX);
    code.add(asmcode::ADD, asmcode::RBP, asmcode::NUMBER, 0x0000000000000008);
    code.add(asmcode::MOV, asmcode::RAX, asmcode::RBP);
    code.add(asmcode::SUB, asmcode::RAX, asmcode::NUMBER, 0x0000000000000008);
    code.add(asmcode::MOV, asmcode::MEM_RBX, 8, asmcode::RAX);
    code.add(asmcode::RET);

    code.add(asmcode::MOV, asmcode::R10, asmcode::RCX);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xC7, 0xC0, 0x74, 0x73, 0x74, 0x00 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x45, 0x00 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xC5, 0x08 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0xE8 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xE8, 0x08 });
    size = code.get_instructions_list().front()[5].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x43, 0x08 });
    size = code.get_instructions_list().front()[6].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC3 });

    size = code.get_instructions_list().front()[7].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x49, 0x89, 0xCA });
    }


  void asmcode_eq()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::MEM_RSP, -8, asmcode::RAX);
    code.add(asmcode::MOV, asmcode::RAX, asmcode::MEM_RSP, -8);
    code.add(asmcode::MOVQ, asmcode::XMM0, asmcode::RAX);
    code.add(asmcode::MOVQ, asmcode::XMM1, asmcode::RAX);
    code.add(asmcode::CMPEQPD, asmcode::XMM0, asmcode::XMM1);
    code.add(asmcode::MOVMSKPD, asmcode::RAX, asmcode::XMM0);
    code.add(asmcode::CMP, asmcode::RAX, asmcode::NUMBER, 0x0000000000000003);
    code.add(asmcode::MOVMSKPD, asmcode::RAX, asmcode::XMM1);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x44, 0x24, 0xF8 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x8B, 0x44, 0x24, 0xF8 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x48, 0x0F, 0x6E, 0xC0 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x48, 0x0F, 0x6E, 0xC8 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x0F, 0xC2, 0xC1, 0x00 });
    size = code.get_instructions_list().front()[5].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x0F, 0x50, 0xC0 });
    size = code.get_instructions_list().front()[6].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x83, 0xF8, 0x03 });
    size = code.get_instructions_list().front()[7].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x0F, 0x50, 0xC1 });

    }

  void asmcode_int()
    {
    asmcode code; uint8_t buffer[255];

    code.add(asmcode::MOVQ, asmcode::XMM0, asmcode::RAX);
    code.add(asmcode::CVTTSD2SI, asmcode::RAX, asmcode::XMM0);
    code.add(asmcode::CVTSI2SD, asmcode::XMM0, asmcode::RAX);
    code.add(asmcode::MOVQ, asmcode::RAX, asmcode::XMM0);
    code.add(asmcode::CVTTSS2SI, asmcode::RAX, asmcode::XMM0);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x48, 0x0F, 0x6E, 0xC0 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF2, 0x48, 0x0F, 0x2C, 0xC0 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF2, 0x48, 0x0F, 0x2A, 0xC0 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x48, 0x0F, 0x7E, 0xC0 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF3, 0x48, 0x0F, 0x2C, 0xC0 });
    }

  void asmcode_checks()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::RAX, asmcode::MEM_RBX, 8);
    code.add(asmcode::MOV, asmcode::MEM_R12, asmcode::RAX);
    code.add(asmcode::NEG, asmcode::RAX);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x8B, 0x43, 0x08 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x49, 0x89, 0x04, 0x24 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xF7, 0xD8 });
    }

  void asmcode_byte_boundary()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::MEM_RBX, 128, asmcode::RAX);
    code.add(asmcode::MOV, asmcode::MEM_RBX, -128, asmcode::RAX);
    code.add(asmcode::MOV, asmcode::MEM_RSP, -128, asmcode::RAX);
    code.add(asmcode::MOV, asmcode::MEM_RSP, 128, asmcode::RAX);
    code.add(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, 0x00000000ffffffff);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x83, 0x80, 0x00, 0x00, 0x00 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x43, 0x80 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x44, 0x24, 0x80 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x89, 0x84, 0x24, 0x80, 0x00, 0x00, 0x00 });

    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xB8, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 });
    }

  void asmcode_32bit_tests()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::EAX, asmcode::NUMBER, 0xabcdef00);
    code.add(asmcode::MOV, asmcode::ECX, asmcode::EAX);
    code.add(asmcode::MOV, asmcode::ECX, asmcode::MEM_EAX);
    code.add(asmcode::MOV, asmcode::MEM_EAX, asmcode::ECX);
    code.add(asmcode::MOV, asmcode::ECX, asmcode::MEM_EAX, 128);
    code.add(asmcode::MOV, asmcode::MEM_EAX, 128, asmcode::ECX);
    code.add(asmcode::MOV, asmcode::ECX, asmcode::MEM_ESP, -128);
    code.add(asmcode::MOV, asmcode::MEM_ESP, -128, asmcode::ECX);
    code.add(asmcode::MOVD, asmcode::XMM0, asmcode::ECX);
    code.add(asmcode::MOVD, asmcode::XMM1, asmcode::EDX);
    code.add(asmcode::MOVD, asmcode::ECX, asmcode::XMM0);
    code.add(asmcode::MOVD, asmcode::EDX, asmcode::XMM1);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xB8, 0x00, 0xEF, 0xCD, 0xAB });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x89, 0xC1 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x67, 0x8B, 0x08 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x67, 0x89, 0x08 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x67, 0x8B, 0x88, 0x80, 0x00, 0x00, 0x00 });
    size = code.get_instructions_list().front()[5].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x67, 0x89, 0x88, 0x80, 0x00, 0x00, 0x00 });
    size = code.get_instructions_list().front()[6].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x67, 0x8B, 0x4C, 0x24, 0x80 });
    size = code.get_instructions_list().front()[7].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x67, 0x89, 0x4C, 0x24, 0x80 });
    size = code.get_instructions_list().front()[8].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x0F, 0x6E, 0xC1 });
    size = code.get_instructions_list().front()[9].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x0F, 0x6E, 0xCA });
    size = code.get_instructions_list().front()[10].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x0F, 0x7E, 0xC1 });
    size = code.get_instructions_list().front()[11].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x0F, 0x7E, 0xCA });
    }


  void asmcode_shufps()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::SHUFPS, asmcode::XMM0, asmcode::XMM0, asmcode::NUMBER, 0);
    code.add(asmcode::SHUFPS, asmcode::XMM1, asmcode::XMM0, asmcode::NUMBER, 0x1B);
    code.add(asmcode::SHUFPS, asmcode::XMM0, asmcode::XMM1, asmcode::NUMBER, 0xAA);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x0F, 0xC6, 0xC0, 0x00 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x0F, 0xC6, 0xC8, 0x1B });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x0F, 0xC6, 0xC1, 0xAA });
    }

  void asmcode_xorps()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::XORPS, asmcode::XMM0, asmcode::XMM0);
    code.add(asmcode::XORPS, asmcode::XMM1, asmcode::XMM0);
    code.add(asmcode::XORPS, asmcode::XMM0, asmcode::XMM1);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x0F, 0x57, 0xC0 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x0F, 0x57, 0xC8 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x0F, 0x57, 0xC1 });
    }

  void asmcode_movdqa()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOVDQA, asmcode::XMM0, asmcode::MEM_RSP, -32);
    code.add(asmcode::MOVDQA, asmcode::MEM_RSP, -32, asmcode::XMM0);
    code.add(asmcode::MOVAPS, asmcode::XMM0, asmcode::MEM_RSP, -32);
    code.add(asmcode::MOVAPS, asmcode::MEM_RSP, -32, asmcode::XMM0);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x0F, 0x6F, 0x44, 0x24, 0xE0 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x0F, 0x7F, 0x44, 0x24, 0xE0 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x0F, 0x28, 0x44, 0x24, 0xE0 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x0F, 0x29, 0x44, 0x24, 0xE0 });
    }

  void asmcode_vaddps()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VADDPS, asmcode::YMM1, asmcode::YMM2, asmcode::YMM3);
    code.add(asmcode::VADDPS, asmcode::YMM1, asmcode::YMM2, asmcode::MEM_RSP, -256);
    code.add(asmcode::VADDPS, asmcode::YMM0, asmcode::YMM7, asmcode::MEM_RAX, 64);

    code.add(asmcode::VADDPS, asmcode::ZMM1, asmcode::ZMM2, asmcode::ZMM3);
    code.add(asmcode::VADDPS, asmcode::ZMM1, asmcode::ZMM2, asmcode::MEM_RSP, -256);
    code.add(asmcode::VADDPS, asmcode::ZMM0, asmcode::ZMM7, asmcode::MEM_RAX, 64);
    code.add(asmcode::VADDPS, asmcode::ZMM0, asmcode::ZMM7, asmcode::MEM_RAX, 64000);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xEC, 0x58, 0xCB });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xEC, 0x58, 0x8C, 0x24, 0x00, 0xFF, 0xFF, 0xFF });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xC4, 0x58, 0x40, 0x40 });

    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF1, 0x6C, 0x48, 0x58, 0xCB });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF1, 0x6C, 0x48, 0x58, 0x4C, 0x24, 0xFC });
    size = code.get_instructions_list().front()[5].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF1, 0x44, 0x48, 0x58, 0x40, 0x01 });
    size = code.get_instructions_list().front()[6].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF1, 0x44, 0x48, 0x58, 0x80, 0x00, 0xFA, 0x00, 0x00 });
    }

  void asmcode_vsubps()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VSUBPS, asmcode::YMM1, asmcode::YMM2, asmcode::YMM3);
    code.add(asmcode::VSUBPS, asmcode::YMM1, asmcode::YMM2, asmcode::MEM_RSP, -256);
    code.add(asmcode::VSUBPS, asmcode::YMM0, asmcode::YMM7, asmcode::MEM_RAX, 64);


    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xEC, 0x5C, 0xCB });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xEC, 0x5C, 0x8C, 0x24, 0x00, 0xFF, 0xFF, 0xFF });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xC4, 0x5C, 0x40, 0x40 });

    }


  void asmcode_vmulps()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VMULPS, asmcode::YMM1, asmcode::YMM2, asmcode::YMM3);
    code.add(asmcode::VMULPS, asmcode::YMM1, asmcode::YMM2, asmcode::MEM_RSP, -256);
    code.add(asmcode::VMULPS, asmcode::YMM0, asmcode::YMM7, asmcode::MEM_RAX, 64);


    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xEC, 0x59, 0xCB });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xEC, 0x59, 0x8C, 0x24, 0x00, 0xFF, 0xFF, 0xFF });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xC4, 0x59, 0x40, 0x40 });

    }


  void asmcode_vdivps()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VDIVPS, asmcode::YMM1, asmcode::YMM2, asmcode::YMM3);
    code.add(asmcode::VDIVPS, asmcode::YMM1, asmcode::YMM2, asmcode::MEM_RSP, -256);
    code.add(asmcode::VDIVPS, asmcode::YMM0, asmcode::YMM7, asmcode::MEM_RAX, 64);


    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xEC, 0x5E, 0xCB });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xEC, 0x5E, 0x8C, 0x24, 0x00, 0xFF, 0xFF, 0xFF });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xC4, 0x5E, 0x40, 0x40 });

    }

  void asmcode_vbroadcastss()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VBROADCASTSS, asmcode::YMM0, asmcode::DWORD_MEM_RSP, -32);
    code.add(asmcode::VBROADCASTSS, asmcode::YMM5, asmcode::DWORD_MEM_RCX, 1024);

    code.add(asmcode::VBROADCASTSS, asmcode::ZMM0, asmcode::XMM0);

    code.add(asmcode::VBROADCASTSS, asmcode::YMM0, asmcode::DWORD_MEM_RBP);
    code.add(asmcode::VBROADCASTSS, asmcode::ZMM0, asmcode::DWORD_MEM_RBP);

    code.add(asmcode::VBROADCASTSS, asmcode::ZMM0, asmcode::DWORD_MEM_RBP, -4);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC4, 0xE2, 0x7D, 0x18, 0x44, 0x24, 0xE0 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC4, 0xE2, 0x7D, 0x18, 0xA9, 0x00, 0x04, 0x00, 0x00 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF2, 0x7D, 0x48, 0x18, 0xC0 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC4, 0xE2, 0x7D, 0x18, 0x45, 0x00 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF2, 0x7D, 0x48, 0x18, 0x45, 0x00 });
    size = code.get_instructions_list().front()[5].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF2, 0x7D, 0x48, 0x18, 0x45, 0xFF });
    }

  void asmcode_vmovaps()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VMOVAPS, asmcode::YMM0, asmcode::YMM1);
    code.add(asmcode::VMOVAPS, asmcode::YMM5, asmcode::MEM_RCX, 1024);
    code.add(asmcode::VMOVAPS, asmcode::YMM1, asmcode::MEM_RSP, -8);
    code.add(asmcode::VMOVAPS, asmcode::MEM_RCX, 1024, asmcode::YMM0);
    code.add(asmcode::VMOVAPS, asmcode::MEM_RSP, -8, asmcode::YMM7);

    code.add(asmcode::VMOVAPS, asmcode::ZMM1, asmcode::MEM_RBP);
    code.add(asmcode::VMOVAPS, asmcode::ZMM0, asmcode::MEM_RBP, 64);
    code.add(asmcode::VMOVAPS, asmcode::ZMM2, asmcode::MEM_RBP, 128);
    code.add(asmcode::VMOVAPS, asmcode::MEM_RBP, -128, asmcode::ZMM0);

    code.add(asmcode::VMOVAPS, asmcode::ZMM0, asmcode::k1, asmcode::ZMM1);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xFC, 0x29, 0xC8 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xFC, 0x28, 0xA9, 0x00, 0x04, 0x00, 0x00 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xFC, 0x28, 0x4C, 0x24, 0xF8 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xFC, 0x29, 0x81, 0x00, 0x04, 0x00, 0x00 });
    size = code.get_instructions_list().front()[4].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xFC, 0x29, 0x7C, 0x24, 0xF8 });
    size = code.get_instructions_list().front()[5].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF1, 0x7C, 0x48, 0x28, 0x4D, 0x00 });
    size = code.get_instructions_list().front()[6].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF1, 0x7C, 0x48, 0x28, 0x45, 0x01 });
    size = code.get_instructions_list().front()[7].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF1, 0x7C, 0x48, 0x28, 0x55, 0x02 });
    size = code.get_instructions_list().front()[8].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF1, 0x7C, 0x48, 0x29, 0x45, 0xFE });
    size = code.get_instructions_list().front()[9].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF1, 0x7C, 0x49, 0x29, 0xC8 });
    }

  void asmcode_vsqrtps()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VSQRTPS, asmcode::YMM0, asmcode::YMM1);
    code.add(asmcode::VSQRTPS, asmcode::YMM5, asmcode::MEM_RCX, 1024);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xFC, 0x51, 0xC1 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xFC, 0x51, 0xA9, 0x00, 0x04, 0x00, 0x00 });
    }

  void asmcode_vshufps()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VSHUFPS, asmcode::YMM0, asmcode::YMM1, asmcode::YMM2, asmcode::NUMBER, 0);
    code.add(asmcode::VSHUFPS, asmcode::XMM0, asmcode::XMM1, asmcode::XMM2, asmcode::NUMBER, 0);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xF4, 0xC6, 0xC2, 0x00 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xF0, 0xC6, 0xC2, 0x00 });
    }

  void asmcode_vxorps()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VXORPS, asmcode::YMM0, asmcode::YMM1, asmcode::YMM2);
    code.add(asmcode::VXORPS, asmcode::ZMM0, asmcode::ZMM1, asmcode::ZMM2);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xF4, 0x57, 0xC2 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF1, 0x74, 0x48, 0x57, 0xC2 });
    }


  void asmcode_vperm2f128()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VPERM2F128, asmcode::YMM0, asmcode::YMM1, asmcode::YMM2, asmcode::NUMBER, 4);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC4, 0xE3, 0x75, 0x06, 0xC2, 0x04 });
    }

  void asmcode_vcvtsi2ss()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VCVTSI2SS, asmcode::XMM0, asmcode::XMM1, asmcode::RAX);
    code.add(asmcode::VCVTSD2SS, asmcode::XMM0, asmcode::XMM1, asmcode::XMM2);
    code.add(asmcode::VCVTSD2SS, asmcode::XMM0, asmcode::XMM0, asmcode::XMM0);
    code.add(asmcode::VCVTSS2SI, asmcode::RAX, asmcode::XMM0);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC4, 0xE1, 0xF2, 0x2A, 0xC0 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xF3, 0x5A, 0xC2 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xFB, 0x5A, 0xC0 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC4, 0xE1, 0xFA, 0x2C, 0xC0 });
    }


  void asmcode_vmov()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VMOVD, asmcode::XMM0, asmcode::EAX);
    code.add(asmcode::VMOVQ, asmcode::XMM0, asmcode::RAX);
    code.add(asmcode::VMOVD, asmcode::EAX, asmcode::XMM0);
    code.add(asmcode::VMOVQ, asmcode::RAX, asmcode::XMM0);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xF9, 0x6E, 0xC0 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC4, 0xE1, 0xF9, 0x6E, 0xC0 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xF9, 0x7E, 0xC0 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC4, 0xE1, 0xF9, 0x7E, 0xC0 });
    }

  void asmcode_and_8bit()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::AND, asmcode::AL, asmcode::NUMBER, 3);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x24, 0x03 });
    }

  void asmcode_cmp_8bit()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::CMP, asmcode::AL, asmcode::NUMBER, 3);
    code.add(asmcode::CMP, asmcode::CL, asmcode::NUMBER, 3);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x3c, 0x03 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x80, 0xF9, 0x03 });
    }

  void asmcode_shl_shr_sal_sar()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::SHL, asmcode::RAX, asmcode::NUMBER, 3);
    code.add(asmcode::SAL, asmcode::RAX, asmcode::NUMBER, 3);
    code.add(asmcode::SHR, asmcode::RAX, asmcode::NUMBER, 3);
    code.add(asmcode::SAR, asmcode::RAX, asmcode::NUMBER, 3);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xC1, 0xE0, 0x03 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xC1, 0xE0, 0x03 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xC1, 0xE8, 0x03 });
    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xC1, 0xF8, 0x03 });
    }

  void asmcode_sete()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::SETE, asmcode::AL);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x0F, 0x94, 0xC0 });
    }


  void asmcode_movss()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOVSS, asmcode::XMM0, asmcode::XMM1);

    code.add(asmcode::VMOVSS, asmcode::XMM0, asmcode::MEM_EAX);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF3, 0x0F, 0x11, 0xC8 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x67, 0xC5, 0xFA, 0x10, 0x00 });

    }

  void asmcode_movsd()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOVSD, asmcode::XMM0, asmcode::XMM1);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xF2, 0x0F, 0x11, 0xC8 });
    }

  void asmcode_movzx()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOVZX, asmcode::RAX, asmcode::AL);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x0F, 0xB6, 0xC0 });
    }

  void asmcode_idiv()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::IDIV, asmcode::R11);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x49, 0xF7, 0xFB });
    }

  void asmcode_cqo()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::CQO);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x99 });
    }

  void asmcode_jmp()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::JMP, asmcode::R11);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x41, 0xFF, 0xE3 });
    }

  void asmcode_or_r15mem()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::OR, asmcode::BYTE_MEM_R15, asmcode::AL);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x41, 0x08, 0x07 });
    }

  void mov_constant_number()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::MOV, asmcode::DWORD_MEM_RBP, -8, asmcode::NUMBER, 1);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC7, 0x45, 0xF8, 0x01, 0x00, 0x00, 0x00 });
    }

  void test_test()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::TEST, asmcode::RAX, asmcode::NUMBER, 1);
    code.add(asmcode::TEST, asmcode::RCX, asmcode::NUMBER, 1);
    code.add(asmcode::TEST, asmcode::RCX, asmcode::RDX);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xA9, 0x01, 0x00, 0x00, 0x00 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0xF7, 0xC1, 0x01, 0x00, 0x00, 0x00 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x48, 0x85, 0xD1 });
    }

  void test_ucomisd()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::UCOMISD, asmcode::XMM0, asmcode::XMM1);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x66, 0x0F, 0x2E, 0xC1 });
    }

  void asmcode_vcmpps()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VCMPPS, asmcode::YMM0, asmcode::YMM0, asmcode::YMM1, asmcode::NUMBER, 255);

    code.add(asmcode::VCMPPS, asmcode::K0, asmcode::ZMM0, asmcode::ZMM1, asmcode::NUMBER, 0);
    code.add(asmcode::VCMPPS, asmcode::K3, asmcode::ZMM2, asmcode::ZMM7, asmcode::NUMBER, 255);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xFC, 0xC2, 0xC1, 0xFF });

    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF1, 0x7C, 0x48, 0xC2, 0xC1, 0x00 });
    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF1, 0x6C, 0x48, 0xC2, 0xDF, 0xFF });
    }

  void asmcode_vlogicps()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VXORPS, asmcode::YMM0, asmcode::YMM1, asmcode::YMM2);
    code.add(asmcode::VANDPS, asmcode::YMM0, asmcode::YMM1, asmcode::YMM2);
    code.add(asmcode::VORPS, asmcode::YMM0, asmcode::YMM1, asmcode::YMM2);
    code.add(asmcode::VANDNPS, asmcode::YMM0, asmcode::YMM1, asmcode::YMM2);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xF4, 0x57, 0xC2 });

    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xF4, 0x54, 0xC2 });

    size = code.get_instructions_list().front()[2].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xF4, 0x56, 0xC2 });

    size = code.get_instructions_list().front()[3].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xF4, 0x55, 0xC2 });
    }

  void asmcode_vroundps()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VROUNDPS, asmcode::YMM0, asmcode::YMM1, asmcode::NUMBER, 8);
    code.add(asmcode::VRNDSCALEPS, asmcode::ZMM0, asmcode::ZMM1, asmcode::NUMBER, 8);
    
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC4, 0xE3, 0x7D, 0x08, 0xC1, 0x08 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0x62, 0xF3, 0x7D, 0x48, 0x08, 0xC1, 0x08 });
    }

  void asmcode_vcvtps2dq()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VCVTPS2DQ, asmcode::YMM0, asmcode::YMM1);
    code.add(asmcode::VCVTDQ2PS, asmcode::YMM0, asmcode::YMM1);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xFD, 0x5B, 0xC1 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xFC, 0x5B, 0xC1 });
    }

  void asmcode_vminmaxps()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VMINPS, asmcode::YMM0, asmcode::YMM0, asmcode::YMM1);
    code.add(asmcode::VMAXPS, asmcode::YMM0, asmcode::YMM0, asmcode::YMM1);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xFC, 0x5D, 0xC1 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xFC, 0x5F, 0xC1 });
    }

  void asmcode_vmovmskps()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::VMOVMSKPS, asmcode::RAX, asmcode::YMM0);
    code.add(asmcode::VMOVMSKPS, asmcode::RAX, asmcode::YMM1);
    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xFC, 0x50, 0xC0 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xFC, 0x50, 0xC1 });
    }

  void asmcode_kmovw()
    {
    asmcode code; uint8_t buffer[255];
    code.add(asmcode::KMOVW, asmcode::EAX, asmcode::K1);
    code.add(asmcode::KMOVW, asmcode::ECX, asmcode::K3);

    uint64_t size;
    size = code.get_instructions_list().front()[0].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xF8, 0x93, 0xC1 });
    size = code.get_instructions_list().front()[1].fill_opcode(buffer);
    _check_buffer(buffer, size, { 0xC5, 0xF8, 0x93, 0xCB });
    }

  }

ASM_END


void run_all_asm_code_tests()
  {
  using namespace ASM;  
  asmcode_mov_rax_number();
  asmcode_mov_rax_number_2();
  asmcode_mov_rbx_number();
  asmcode_mov_r15_number();
  asmcode_mov_rax_rcx();
  asmcode_mov_rcx_rax();
  asmcode_mov_rax_memrsp_8();
  asmcode_mov_rax_memrsp_4096();
  asmcode_mov_memrsp_8_rax();
  asmcode_mov_memrsp_4096_rax();
  asmcode_mov_memrsp_8_rcx();
  asmcode_mov_r14_mem_r13();
  asmcode_movq_xmm0_rax();
  asmcode_movq_rax_xmm0();
  asmcode_mov_rsp_rdx();
  asmcode_mov_rbp_r8();
  asmcode_and_rsp_number();
  asmcode_and_rsp_number_32();
  asmcode_and_rbp_number();
  asmcode_and_rbp_number_32();
  asmcode_or_rsp_number();
  asmcode_or_rsp_number_32();
  asmcode_or_rbp_number();
  asmcode_or_rbp_number_32();
  asmcode_add_rbp_number_15();
  asmcode_add_rbp_number_4096();
  asmcode_sub_rbp_number_15();
  asmcode_sub_rbp_number_4096();
  asmcode_xor_rax_rax();
  asmcode_xor_r14_r14();
  asmcode_pop_rbp();
  asmcode_push_rbp();
  asmcode_ret();
  asmcode_intro_1();
  asmcode_store_registers();
  asmcode_load_registers();
  asmcode_prepare_external_function_call();
  finalize_external_function_call();
  code_get_stack_item();
  save_rax_on_stack();
  convert_short_string_to_string();
  asmcode_call();
  asmcode_add_7_and_3();
  asmcode_add_two_integers();
  asmcode_cmp();
  asmcode_dec_inc();
  asmcode_mul();
  asmcode_div();
  asmcode_nop();
  asmcode_cvt_sd_si();
  asmcode_cmp_bytes();
  asmcode_mov_bytes();
  asmcode_sse_basic_ops();
  asmcode_sse_cmp_ops();
  asmcode_sse_other_ops();
  asmcode_fpu();
  asmcode_string_literals();
  asmcode_print_statement();
  asmcode_string_literals_tst();
  asmcode_eq();
  asmcode_int();
  asmcode_checks();
  asmcode_byte_boundary();
  asmcode_32bit_tests();
  asmcode_shufps();
  asmcode_xorps();
  asmcode_movdqa();
  asmcode_vaddps();
  asmcode_vsubps();
  asmcode_vmulps();
  asmcode_vdivps();
  asmcode_vbroadcastss();
  asmcode_vmovaps();
  asmcode_vsqrtps();
  asmcode_vshufps();
  asmcode_vxorps();
  asmcode_vperm2f128();
  asmcode_vcvtsi2ss();
  asmcode_vmov();
  asmcode_and_8bit();
  asmcode_cmp_8bit();
  asmcode_shl_shr_sal_sar();
  asmcode_sete();
  asmcode_movss();
  asmcode_movsd();
  asmcode_movzx();
  asmcode_idiv();
  asmcode_cqo();
  asmcode_jmp();
  asmcode_or_r15mem();
  mov_constant_number();
  test_test();
  test_ucomisd();
  asmcode_vcmpps();
  asmcode_vlogicps();
  asmcode_vroundps();
  asmcode_vcvtps2dq();
  asmcode_vminmaxps();
  asmcode_vmovmskps();
  asmcode_kmovw();
  }