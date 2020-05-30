#pragma once

#include "vectorforth_api.h"

#include "namespace.h"
#include <asm/asmcode.h>
#include <stdint.h>

VF_BEGIN


#ifdef _WIN32
__declspec(align(64)) // windows alignment
#endif
struct context
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

  char* binding_space_pointer; // offset 112

  char* here_pointer; // 120

  uint64_t signbit[8]; // offset 128
  uint64_t not_signbit[8]; // offset 192
  uint64_t all_bits[8]; // offset 256
  uint64_t no_bits[8]; // offset 320
  uint64_t onef_bits[8]; // offset 384

  uint64_t store_zmm6[8]; // offset 448
  uint64_t store_zmm7[8]; // offset 512

  char* memory_allocated;  
  }
#ifndef _WIN32 // linux alignment in gcc
__attribute__ ((aligned (64)))
#endif
  ;


VECTOR_FORTH_API context create_context(uint64_t stack_size_in_bytes, uint64_t binding_space_in_bytes, uint64_t data_space_size_in_bytes);
VECTOR_FORTH_API void destroy_context(const context& ctxt);

VF_END
