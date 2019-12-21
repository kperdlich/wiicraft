#pragma once

#include "stdint.h"

namespace core {

void Assert(const char* expression, const char* file, int32_t line, const char *format, ...);

#define ASSERT_TEXT(e, format,...) \
    ((e) ? (void)0 : core::Assert(#e, __FILE__, __LINE__, format, ##__VA_ARGS__))

#define ASSERT(e) \
    ((e) ? (void)0 : core::Assert(#e, __FILE__, __LINE__, nullptr))

}





