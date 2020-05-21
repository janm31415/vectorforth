#pragma once

#include "namespace.h"
#include <asm/asmcode.h>
#include <string>

VF_BEGIN

std::string label_to_string(uint64_t lab);
void store_registers(ASM::asmcode& code);
void load_registers(ASM::asmcode& code);

VF_END