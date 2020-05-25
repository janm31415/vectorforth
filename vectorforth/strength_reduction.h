#pragma once

#include "expand.h"
#include "vectorforth_api.h"
#include <vector>

VF_BEGIN

// replace expensive operations with cheaper ones
VECTOR_FORTH_API void strength_reduction(std::vector<expanded_token>& words);

VF_END