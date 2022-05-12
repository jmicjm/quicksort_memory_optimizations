#pragma once
#include <span>
#include "memory_measurement.h"


template<typename T>
T* partition(std::span<T> data)
{
	T pivot = data[data.size() / 2];

	int i, j;
	for (i = 0, j = data.size() - 1;; i++, j--)
	{
		while (data[i] < pivot) i++;
		while (data[j] > pivot) j--;

		if (i >= j) break;

		std::swap(data[i], data[j]);
	}

	max_heap = std::max(max_heap, (int64_t)(pivot.capacity() * sizeof(char)));
	max_sp = std::min(max_sp, stackPTR());

	return &*data.begin() + i;
}