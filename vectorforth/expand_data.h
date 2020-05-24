#pragma once

#include <stdint.h>
#include "namespace.h"

VF_BEGIN

struct expand_data
  {  
  uint64_t binding_space_offset; // keeps track of the number of variables created so far
  bool create_called; // is set to true when "create" keyword is encountered, until the next unknown word, which will then become a variable
  bool to_called; // is set to true when "to" keyword is encountered, until the next word which should be a value 
  };

inline expand_data create_expand_data()
  {
  expand_data ed;  
  ed.binding_space_offset = 0;
  ed.create_called = false;
  ed.to_called = false;
  return ed;
  }

VF_END