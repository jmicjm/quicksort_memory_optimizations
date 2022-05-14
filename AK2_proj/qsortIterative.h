#pragma once
#include <span>
#include <algorithm>
#include <vector>
#include "memory_measurement.h"


template<typename T, T* (*partition)(std::span<T>)>
void qsortIterative(std::span<T> data)
{
	std::vector<std::span<T>> ranges;
	ranges.push_back(data);

	while (ranges.size() > 0)
	{
		auto current = ranges.back();
		ranges.pop_back();

		T* pivot = partition(current);

		std::span<T> left{ &*current.begin(), pivot };
		std::span<T> right{ pivot, &*current.end() };

		if (left.size() >= 2) ranges.push_back(left);
		if (right.size() >= 2) ranges.push_back(right);

		qsort_heap = static_cast<int64_t>(ranges.capacity() * sizeof(std::span<T>));
		max_heap = std::max(max_heap, qsort_heap);
		max_sp = std::min(max_sp, stackPTR());
	}
}