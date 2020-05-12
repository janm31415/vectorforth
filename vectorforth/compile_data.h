#pragma once

#include <stdint.h>
#include "namespace.h"

#include <string>
#include <vector>

VF_BEGIN

struct compile_data
  {
  uint64_t label;
  std::vector<std::string> else_label;
  std::vector<std::string> then_label;
  std::vector<std::string> begin_label;
  std::vector<std::string> repeat_label;
  };

inline compile_data create_compile_data()
  {
  compile_data cd;
  cd.label = 0;
  return cd;
  }

VF_END