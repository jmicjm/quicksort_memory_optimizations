#pragma once
#include <cstdint>

extern "C" int64_t stackPTR();

inline static int64_t max_sp = 0;
inline static int64_t max_heap = 0;
inline static int64_t qsort_heap = 0;