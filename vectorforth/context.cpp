#include "context.h"

VF_BEGIN

context create_context(uint64_t stack_size_in_bytes, uint64_t binding_space_in_bytes, uint64_t data_space_size_in_bytes)
  {
  binding_space_in_bytes = (binding_space_in_bytes + 63) & ~63;
  const uint64_t alignment = 256; // must be a power of two
  context c;
  c.memory_allocated = (char*)malloc(stack_size_in_bytes + alignment - 1 + data_space_size_in_bytes + binding_space_in_bytes);
  c.aligned_stack_top = (char*)((uintptr_t)(c.memory_allocated + stack_size_in_bytes + alignment - 1) & ~(uintptr_t)(alignment - 1)); // align stack top to "alignment" byte boundary  
  c.aligned_stack_bottom = (char*)((uintptr_t)(c.memory_allocated + alignment - 1) & ~(uintptr_t)(alignment - 1));
  c.stack_pointer = c.aligned_stack_top;

  c.binding_space_pointer = c.aligned_stack_top;

  c.here_pointer = c.aligned_stack_top + binding_space_in_bytes;
#ifdef AVX512
  char* data_space_pointer = c.here_pointer + 64;
#else
  char* data_space_pointer = c.here_pointer + 32;
#endif

  *((uint64_t*)c.here_pointer) = (uint64_t)((void*)data_space_pointer);


  uint64_t sb = 0x8000000080000000;
  c.signbit[0] = sb;
  c.signbit[1] = sb;
  c.signbit[2] = sb;
  c.signbit[3] = sb;
  c.signbit[4] = sb;
  c.signbit[5] = sb;
  c.signbit[6] = sb;
  c.signbit[7] = sb;
  c.not_signbit[0] = ~sb;
  c.not_signbit[1] = ~sb;
  c.not_signbit[2] = ~sb;
  c.not_signbit[3] = ~sb;
  c.not_signbit[4] = ~sb;
  c.not_signbit[5] = ~sb;
  c.not_signbit[6] = ~sb;
  c.not_signbit[7] = ~sb;
  c.all_bits[0] = 0xffffffffffffffff;
  c.all_bits[1] = 0xffffffffffffffff;
  c.all_bits[2] = 0xffffffffffffffff;
  c.all_bits[3] = 0xffffffffffffffff;
  c.all_bits[4] = 0xffffffffffffffff;
  c.all_bits[5] = 0xffffffffffffffff;
  c.all_bits[6] = 0xffffffffffffffff;
  c.all_bits[7] = 0xffffffffffffffff;
  c.no_bits[0] = 0x0;
  c.no_bits[1] = 0x0;
  c.no_bits[2] = 0x0;
  c.no_bits[3] = 0x0;
  c.no_bits[4] = 0x0;
  c.no_bits[5] = 0x0;
  c.no_bits[6] = 0x0;
  c.no_bits[7] = 0x0;
  return c;
  }


void destroy_context(const context& ctxt)
  {
  free(ctxt.memory_allocated);
  }

VF_END