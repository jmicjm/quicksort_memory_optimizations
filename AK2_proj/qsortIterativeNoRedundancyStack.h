#pragma once
#include <span>
#include <algorithm>
#include <vector>
#include <numeric>
#include <malloc.h>
#include "memory_measurement.h"

template<typename T>
class Stack
{
	constexpr static auto step_size = std::lcm(sizeof(T), 16) / sizeof(T);
	T* m_data = nullptr;
	int32_t m_size = 0;
	int32_t m_capacity = 0;

public:
	__forceinline Stack()
		: m_data(static_cast<T*>(_alloca(step_size * sizeof(T)))),
		  m_size(0),
		  m_capacity(step_size) {}
	__forceinline auto push(T value)
	{
		if (m_size == m_capacity)
		{
			_alloca(step_size * sizeof(T));
			m_capacity += step_size;
		}
		m_data[-m_size] = value;
		m_size++;
	};
	__forceinline auto pop()
	{
		m_size--;
	};
	__forceinline auto top()
	{
		return m_data[-(m_size - 1)];
	};
	__forceinline auto size()
	{
		return m_size;
	}
};

template<typename T, T* (*partition)(std::span<T>)>
void qsortIterativeNoRedundancyStack(std::span<T> data)
{
	Stack<T*> ranges;	
	ranges.push(&*data.begin());
	ranges.push(&*data.end());

	while (ranges.size() > 1)
	{
		auto top = ranges.top();
		ranges.pop();

		if (!top || !ranges.top()) continue;

		T* pivot = partition({ ranges.top(), top });

		if (pivot - ranges.top() >= 2) ranges.push(pivot);
		else if (ranges.top()) ranges.push(nullptr);

		if (top - pivot >= 2)
		{
			if (!ranges.top()) ranges.push(pivot);
			ranges.push(top);
		}

		max_sp = std::min(max_sp, stackPTR());
	}
}