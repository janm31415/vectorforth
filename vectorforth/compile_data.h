#pragma once

#include <stdint.h>
#include "namespace.h"

#include <string>
#include <vector>

VF_BEGIN

struct compile_data
  {
  uint64_t label;
  uint64_t constant_space_offset;
  bool create_called; // is set to true when "create" keyword is encountered, until the next unknown word, which will then become a variable
  bool to_called; // is set to true when "to" keyword is encountered, until the next word which should be a value
  std::vector<std::string> else_label;
  std::vector<std::string> then_label;
  std::vector<std::string> begin_label;
  std::vector<std::string> repeat_label;
  };

inline compile_data create_compile_data()
  {
  compile_data cd;
  cd.label = 0;
  cd.constant_space_offset = 0;
  cd.create_called = false;
  cd.to_called = false;
  return cd;
  }

VF_END