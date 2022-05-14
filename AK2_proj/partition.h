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

	constexpr static auto T_capacity = T{}.capacity();
	auto partition_heap = pivot.capacity() > T_capacity ? static_cast<int64_t>((pivot.capacity()+1) * sizeof(T::value_type)) : 0;
	max_heap = std::max(max_heap, qsort_heap + partition_heap);
	max_sp = std::min(max_sp, stackPTR());

	return &*data.begin() + i;
}