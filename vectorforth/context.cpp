#include "context.h"

VF_BEGIN

context create_context(uint64_t stack_size_in_bytes, uint64_t binding_space_in_bytes, uint64_t data_space_size_in_bytes)
  {
  binding_space_in_bytes = (binding_space_in_bytes + 31) & ~31;
  const uint64_t alignment = 256; // must be a power of two
  context c;
  c.memory_allocated = (char*)malloc(stack_size_in_bytes + alignment - 1 + data_space_size_in_bytes + binding_space_in_bytes);
  c.aligned_stack_top = (char*)((uintptr_t)(c.memory_allocated + stack_size_in_bytes + alignment - 1) & ~(uintptr_t)(alignment-1)); // align stack top to "alignment" byte boundary  
  c.aligned_stack_bottom = (char*)((uintptr_t)(c.memory_allocated + alignment - 1) & ~(uintptr_t)(alignment-1));
  c.stack_pointer = c.aligned_stack_top;

  c.binding_space_pointer = c.aligned_stack_top;

  c.here_pointer = c.aligned_stack_top + binding_space_in_bytes;
  char* data_space_pointer = c.here_pointer+32;

  *((uint64_t*)c.here_pointer) = (uint64_t)((void*)data_space_pointer);


  uint64_t sb = 0x8000000080000000;
  c.signbit[0] = sb;
  c.signbit[1] = sb;
  c.signbit[2] = sb;
  c.signbit[3] = sb;
  c.not_signbit[0] = ~sb;
  c.not_signbit[1] = ~sb;
  c.not_signbit[2] = ~sb;
  c.not_signbit[3] = ~sb;
  c.all_bits[0] = 0xffffffffffffffff;
  c.all_bits[1] = 0xffffffffffffffff;
  c.all_bits[2] = 0xffffffffffffffff;
  c.all_bits[3] = 0xffffffffffffffff;
  return c;
  }


void destroy_context(const context& ctxt)
  {
  free(ctxt.memory_allocated);
  }

VF_END