#include "assembler.h"

#include <stdint.h>
#include <map>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif

ASM_BEGIN

namespace
  {

  void first_pass(first_pass_data& data, asmcode& code, const std::map<std::string, uint64_t>& externals)
    {
    uint8_t buffer[255];
    data.size = 0;
    data.label_to_address.clear();
    for (auto it = code.get_instructions_list().begin(); it != code.get_instructions_list().end(); ++it)
      {
      std::vector<std::pair<size_t, int>> nops_to_add;
      for (size_t i = 0; i < it->size(); ++i)
        {
        auto instr = (*it)[i];
        switch (instr.oper)
          {
          case asmcode::CALL:
          {
          auto it2 = externals.find(instr.text);
          if (it2 != externals.end())
            {
            instr.oper = asmcode::MOV;
            instr.operand1 = asmcode::RAX;
            instr.operand2 = asmcode::NUMBER;
            instr.operand2_mem = it2->second;
            data.size += instr.fill_opcode(buffer);
            instr.oper = asmcode::CALL;
            instr.operand1 = asmcode::RAX;
            instr.operand2 = asmcode::EMPTY;
            data.size += instr.fill_opcode(buffer);
            }
          else
            {
            if (instr.operand1 == asmcode::EMPTY)
              {
              instr.operand1 = asmcode::NUMBER;
              instr.operand1_mem = 0x11111111;
              }
            data.size += instr.fill_opcode(buffer);
            }
          break;
          }
          case asmcode::JMP:
          case asmcode::JE:
          case asmcode::JL:
          case asmcode::JLE:
          case asmcode::JG:
          case asmcode::JA:
          case asmcode::JB:
          case asmcode::JGE:
          case asmcode::JNE:
          {
          if (instr.operand1 == asmcode::EMPTY)
            {
            instr.operand1 = asmcode::NUMBER;
            instr.operand1_mem = 0x11111111;
            }
          data.size += instr.fill_opcode(buffer);
          break;
          }
          case asmcode::JMPS:
          case asmcode::JES:
          case asmcode::JLS:
          case asmcode::JLES:
          case asmcode::JGS:
          case asmcode::JAS:
          case asmcode::JBS:
          case asmcode::JGES:
          case asmcode::JNES:
          {
          instr.operand1 = asmcode::NUMBER;
          instr.operand1_mem = 0x11;
          data.size += instr.fill_opcode(buffer);
          break;
          }
          case asmcode::LABEL:
            data.label_to_address[instr.text] = data.size; break;
          case asmcode::LABEL_ALIGNED:
            if (data.size & 7)
              {
              int nr_of_nops = 8 - (data.size & 7);
              data.size += nr_of_nops;
              nops_to_add.emplace_back(i, nr_of_nops);
              }
            data.label_to_address[instr.text] = data.size; break;
          case asmcode::GLOBAL:
            if (data.size & 7)
              {
              int nr_of_nops = 8 - (data.size & 7);
              data.size += nr_of_nops;
              nops_to_add.emplace_back(i, nr_of_nops);
              }
            data.label_to_address[instr.text] = data.size; break;
          case asmcode::DQ:
          {
          data.dq_to_pair_offset_value[instr.text] = std::pair<uint64_t, uint64_t>(data.data_size, instr.operand1_mem);
          data.data_size += 8; // reserve memory for 64 bit data
          break;
          }
          case asmcode::EXTERN:
          {
          auto it2 = externals.find(instr.text);
          if (it2 == externals.end())
            throw std::logic_error("error: external is not defined");
          data.external_to_address[instr.text] = it2->second;
          break;
          }
          default:
            data.size += instr.fill_opcode(buffer); break;
          }
        }
      size_t nops_offset = 0;
      for (auto nops : nops_to_add)
        {
        std::vector<asmcode::instruction> nops_instructions(nops.second, asmcode::instruction(asmcode::NOP));
        it->insert(it->begin() + nops_offset + nops.first, nops_instructions.begin(), nops_instructions.end());
        nops_offset += nops.second;
        }
      }
    }


  uint8_t* second_pass(uint8_t* func, const first_pass_data& data, const asmcode& code)
    {
    uint64_t address_start = (uint64_t)(reinterpret_cast<uint64_t*>(func));
    uint8_t* start = func;
    for (auto it = code.get_instructions_list().begin(); it != code.get_instructions_list().end(); ++it)
      {
      for (asmcode::instruction instr : *it)
        {
        if (instr.operand1 == asmcode::VARIABLE)
          {
          auto it2 = data.dq_to_pair_offset_value.find(instr.text);
          if (it2 == data.dq_to_pair_offset_value.end())
            throw std::logic_error("error: variable is not defined");
          instr.operand1_mem = address_start + data.size + it2->second.first;
          }
        if (instr.operand2 == asmcode::VARIABLE)
          {
          auto it2 = data.dq_to_pair_offset_value.find(instr.text);
          if (it2 == data.dq_to_pair_offset_value.end())
            throw std::logic_error("error: variable is not defined");
          instr.operand2_mem = address_start + data.size + it2->second.first;
          }
        if (instr.operand1 == asmcode::LABELADDRESS)
          {
          auto it2 = data.label_to_address.find(instr.text);
          if (it2 == data.label_to_address.end())
            throw std::logic_error("error: label is not defined");
          instr.operand1_mem = address_start + it2->second;
          }
        if (instr.operand2 == asmcode::LABELADDRESS)
          {
          auto it2 = data.label_to_address.find(instr.text);
          if (it2 == data.label_to_address.end())
            throw std::logic_error("error: label is not defined");
          instr.operand2_mem = address_start + it2->second;
          }
        if (instr.operand1 == asmcode::MOFFS64)
          {
          auto it2 = data.dq_to_pair_offset_value.find(instr.text);
          if (it2 == data.dq_to_pair_offset_value.end())
            throw std::logic_error("error: dq variable is not defined");
          instr.operand1_mem = address_start + data.size + it2->second.first;
          }
        if (instr.operand2 == asmcode::MOFFS64)
          {
          auto it2 = data.dq_to_pair_offset_value.find(instr.text);
          if (it2 == data.dq_to_pair_offset_value.end())
            throw std::logic_error("error: dq variable is not defined");
          instr.operand2_mem = address_start + data.size + it2->second.first;
          }
        switch (instr.oper)
          {
          case asmcode::CALL:
          {
          if (instr.operand1 != asmcode::EMPTY)
            break;
          auto it_label = data.label_to_address.find(instr.text);
          auto it_external = data.external_to_address.find(instr.text);
          if (it_external != data.external_to_address.end())
            {
            asmcode::instruction extra_instr(asmcode::MOV, asmcode::RAX, asmcode::NUMBER, it_external->second);
            func += extra_instr.fill_opcode(func);
            instr.operand1 = asmcode::RAX;
            }
          else if (it_label != data.label_to_address.end())
            {
            int64_t address = (int64_t)it_label->second;
            int64_t current = (int64_t)(func - start);
            instr.operand1 = asmcode::NUMBER;
            instr.operand1_mem = (int64_t(address - current - 5));
            }
          else
            throw std::logic_error("second_pass error: call target does not exist");
          break;
          }
          case asmcode::JE:
          case asmcode::JL:
          case asmcode::JLE:
          case asmcode::JG:
          case asmcode::JA:
          case asmcode::JB:
          case asmcode::JGE:
          case asmcode::JNE:
          {
          if (instr.operand1 != asmcode::EMPTY)
            break;
          auto it2 = data.label_to_address.find(instr.text);
          if (it2 == data.label_to_address.end())
            throw std::logic_error("second_pass error: label does not exist");
          int64_t address = (int64_t)it2->second;
          int64_t current = (int64_t)(func - start);
          instr.operand1 = asmcode::NUMBER;
          instr.operand1_mem = (int64_t(address - current - 6));
          break;
          }
          case asmcode::JMP:
          {
          if (instr.operand1 != asmcode::EMPTY)
            break;
          auto it2 = data.label_to_address.find(instr.text);
          if (it2 == data.label_to_address.end())
            throw std::logic_error("second_pass error: label does not exist");
          int64_t address = (int64_t)it2->second;
          int64_t current = (int64_t)(func - start);
          instr.operand1 = asmcode::NUMBER;
          instr.operand1_mem = (int64_t(address - current - 5));
          break;
          }
          case asmcode::JES:
          case asmcode::JLS:
          case asmcode::JLES:
          case asmcode::JGS:
          case asmcode::JAS:
          case asmcode::JBS:
          case asmcode::JGES:
          case asmcode::JNES:
          case asmcode::JMPS:
          {
          if (instr.operand1 != asmcode::EMPTY)
            break;
          auto it2 = data.label_to_address.find(instr.text);
          if (it2 == data.label_to_address.end())
            throw std::logic_error("second_pass error: label does not exist");
          int64_t address = (int64_t)it2->second;
          int64_t current = (int64_t)(func - start);
          instr.operand1 = asmcode::NUMBER;
          instr.operand1_mem = (int64_t(address - current - 2));
          if ((int64_t)instr.operand1_mem > 127 || (int64_t)instr.operand1_mem < -128)
            throw std::logic_error("second_pass error: jump short is too far");
          break;
          }
          }
        func += instr.fill_opcode(func);
        }
      }
    while ((uint64_t)(func - start) < data.size)
      {
      asmcode::instruction dummy(asmcode::NOP);
      func += dummy.fill_opcode(func);
      }
    return func;
    }
  }

void* assemble(uint64_t& size, first_pass_data& d, asmcode& code, const std::map<std::string, uint64_t>& externals)
  {
  d.external_to_address.clear();
  d.label_to_address.clear();
  d.dq_to_pair_offset_value.clear();
  first_pass(d, code, externals);

  void *compiled_func;
#ifdef _WIN32
  compiled_func = VirtualAlloc(0, d.size + d.data_size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#else
  compiled_func = mmap(NULL, d.size + d.data_size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif

  if (!compiled_func)
    throw std::runtime_error("Could not allocate virtual memory");

  uint8_t* func_end = second_pass((uint8_t*)compiled_func, d, code);

  uint64_t size_used = func_end - (uint8_t*)compiled_func;

  if (size_used != d.size)
    {
    throw std::logic_error("error: error in size computation.");
    }

  for (auto it = d.dq_to_pair_offset_value.begin(); it != d.dq_to_pair_offset_value.end(); ++it)
    {
    uint64_t offset = it->second.first;
    uint64_t value = it->second.second;
    *(uint64_t*)(func_end + offset) = value;
    }

  size = d.size + d.data_size;

#ifdef _WIN32
  unsigned long i;
  auto success = VirtualProtect((void *)(compiled_func), d.size + d.data_size, 0x40, (unsigned long *)&i);

  if (!success)
    return NULL;
#endif
  return compiled_func;
  }

void* assemble(uint64_t& size, asmcode& code, const std::map<std::string, uint64_t>& externals)
  {
  first_pass_data d;
  return assemble(size, d, code, externals);
  }

void* assemble(uint64_t& size, first_pass_data& d, asmcode& code)
  {
  std::map<std::string, uint64_t> externals;
  return assemble(size, d, code, externals);
  }

void* assemble(uint64_t& size, asmcode& code)
  {
  std::map<std::string, uint64_t> externals;
  return assemble(size, code, externals);
  }

void free_assembled_function(void* f, uint64_t size)
  {
#ifdef _WIN32
  (void*)size;
  VirtualFree(f, 0, MEM_RELEASE);
#else
  munmap(f, size);
#endif
  }

ASM_END
