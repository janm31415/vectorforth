#pragma once

#include "vectorforth_api.h"

#include "namespace.h"
#include <asm/asmcode.h>
#include <stdint.h>

VF_BEGIN

__declspec(align(32)) struct context
  {
  context() : memory_allocated(nullptr) {}

  void* rbx; // offset 0
  void* rdi; // offset 8
  void* rsi; // offset 16
  void* rsp; // offset 24
  void* rbp; // offset 32
  void* r12; // offset 40
  void* r13; // offset 48
  void* r14; // offset 56
  void* r15; // offset 64  
  
  char* aligned_stack_top; // offset 72
  char* aligned_stack_bottom; // offset 80
  char* stack_pointer; // offset 88

  char* rsp_save; // offset 96
  char* rsp_top; //offset 104

  uint64_t padding[2]; // 112, 120

  uint64_t signbit[4]; // offset 128
  uint64_t not_signbit[4]; // offset 160
  uint64_t all_bits[4]; // offset 192

  char* memory_allocated;  
  };


VECTOR_FORTH_API context create_context(uint64_t stack_size_in_bytes);
VECTOR_FORTH_API void destroy_context(const context& ctxt);

VF_END
