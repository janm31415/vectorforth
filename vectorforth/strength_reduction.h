#pragma once

#include "expand.h"
#include <vector>

VF_BEGIN

// replace expensive operations with cheaper ones
void strength_reduction(std::vector<expanded_token>& words);

VF_END