#pragma once

#include "namespace.h"
#include "context.h"
#include "vectorforth_api.h"

#include <iostream>

VF_BEGIN

VECTOR_FORTH_API void print_stack(std::ostream& os, const context& ctxt);

VECTOR_FORTH_API void print_data_space(std::ostream& os, const context& ctxt);

VF_END