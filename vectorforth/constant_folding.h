#pragma once

#include "expand.h"
#include "vectorforth_api.h"
#include <vector>

VF_BEGIN

VECTOR_FORTH_API void constant_folding(std::vector<expanded_token>& words);

VF_END