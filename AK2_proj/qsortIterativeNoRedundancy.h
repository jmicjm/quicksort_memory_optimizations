#pragma once
#include <span>
#include <algorithm>
#include <vector>
#include "memory_measurement.h"

template<typename T, T* (*partition)(std::span<T>)>
void qsortIterativeNoRedundancy(std::span<T> data)
{
	std::vector<T*> ranges;
	ranges.push_back(&*data.begin());
	ranges.push_back(&*data.end());

	while (ranges.size() > 1)
	{
		auto top = ranges.back();
		ranges.pop_back();

		if (!top || !ranges.back()) continue;

		T* pivot = partition({ ranges.back(), top });

		if (pivot - ranges.back() >= 2) ranges.push_back(pivot);
		else if (ranges.back()) ranges.push_back(nullptr);

		if (top - pivot >= 2)
		{
			if (!ranges.back()) ranges.push_back(pivot);
			ranges.push_back(top);
		}

		qsort_heap = static_cast<int64_t>(ranges.capacity() * sizeof(T*));
		max_heap = std::max(max_heap, qsort_heap);
		max_sp = std::min(max_sp, stackPTR());
	}
}