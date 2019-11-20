#pragma once

#include "stdint.h"

namespace core {

void Assert(const char* expression, const char* file, int32_t line);

#define ASSERT(e) \
    ((e) ? (void)0 : core::Assert(#e, __FILE__, __LINE__))

}





