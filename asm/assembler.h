#pragma once

#include "namespace.h"
#include "asm_api.h"
#include "asmcode.h"
#include <map>
#include <string>
#include <stdint.h>

ASM_BEGIN

struct first_pass_data
  {
  first_pass_data() : size(0), data_size(0) {}

  uint64_t size, data_size;
  std::map<std::string, uint64_t> label_to_address;
  std::map<std::string, uint64_t> external_to_address;
  std::map<std::string, std::pair<uint64_t, uint64_t>> dq_to_pair_offset_value;
  };

ASSEMBLER_API void* assemble(uint64_t& size, first_pass_data& d, asmcode& code, const std::map<std::string, uint64_t>& externals);
ASSEMBLER_API void* assemble(uint64_t& size, first_pass_data& d, asmcode& code);
ASSEMBLER_API void* assemble(uint64_t& size, asmcode& code, const std::map<std::string, uint64_t>& externals);
ASSEMBLER_API void* assemble(uint64_t& size, asmcode& code);

ASSEMBLER_API void free_assembled_function(void* f, uint64_t size);

ASM_END
