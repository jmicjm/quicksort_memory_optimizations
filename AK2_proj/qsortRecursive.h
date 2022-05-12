#pragma once
#include <span>
#include <algorithm>
#include "memory_measurement.h"


template<typename T, T* (*partition)(std::span<T>)>
void qsortRecursive(std::span<T> data)
{
	if (data.size() <= 1) return;

	T* p = partition(data);

	max_sp = std::min(max_sp, stackPTR());

	qsortRecursive<T, partition>(data.first(p - &*data.begin()));
	qsortRecursive<T, partition>(data.last(&*data.end() - p));
}