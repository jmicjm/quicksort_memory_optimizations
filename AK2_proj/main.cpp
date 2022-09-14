#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <string_view>
#include <span>
#include <chrono>

#include "partition.h"
#include "inlinePartition.h"
#include "partitionNoPivotCopy.h"
#include "inlinePartitionNoPivotCopy.h"

#include "qsortIterative.h"
#include "qsortIterativeNoRedundancy.h"
#include "qsortIterativeNoRedundancyStack.h"
#include "qsortRecursive.h"

template<typename T>
using data_samples = std::vector<std::vector<T>>;

int rval(int min, int max)
{
	static std::mt19937 rng(std::random_device{}());
	return std::uniform_int_distribution<>{min, max}(rng);
}

template<typename T>
void testAll(const data_samples<T>& data_samples);


int main()
{
	constexpr auto sample_count = 1;
	constexpr auto sample_size = 1e6;

	data_samples<std::string> random_data_samples(sample_count);
	for (auto& sample : random_data_samples)
	{
		for (int i = 0; i < sample_size; i++)
		{
			std::string str(rval(1, 16), ' ');
			for (auto& ch : str) ch = rval('a', 'z');

			sample.push_back(str);
		}
	}
	std::cout << "==random data order\n";
	testAll(random_data_samples);

	auto increasing_data_samples = std::move(random_data_samples);	
	for (auto& sample : increasing_data_samples)
	{
		std::sort(sample.begin(), sample.end());
	}
	std::cout << "==increasing data order\n";
	testAll(increasing_data_samples);

	auto decreasing_data_samples = std::move(increasing_data_samples);
	for (auto& sample : decreasing_data_samples)
	{
		std::sort(sample.begin(), sample.end(), [](const auto& a, const auto& b) { return a >= b; });
	}
	std::cout << "==decreasing data order\n";
	testAll(decreasing_data_samples);

	data_samples<std::string> equal_data_samples(sample_count);
	for (auto& sample : equal_data_samples)
	{
		for (int i = 0; i < sample_size; i++)
		{
			sample.push_back(std::string(16, 'a'));
		}
	}
	std::cout << "==equal data\n";
	testAll(equal_data_samples);

	data_samples<std::string> interleaved_data_samples(sample_count);
	for (auto& sample : interleaved_data_samples)
	{
		for (int i = 0; i < sample_size; i++)
		{
			sample.push_back(std::string(16, i%2 ? 'a': 'b'));
		}
	}
	std::cout << "==interleaved data\n";
	testAll(interleaved_data_samples);
}

template<typename T, typename qsort_f>
void test(data_samples<T> samples, qsort_f qsort, std::string_view description)
{
	auto sum_stack = 0;
	auto sum_heap = 0;

	auto sorted = true;
	auto time = 0;

	for (auto& sample : samples)
	{
		auto sp = stackPTR();
		max_sp = sp;
		max_heap = 0;
		qsort_heap = 0;

		auto b = std::chrono::steady_clock::now();
		qsort(sample);
		auto e = std::chrono::steady_clock::now();
		time += std::chrono::duration_cast<std::chrono::milliseconds>(e - b).count();

		sorted &= std::is_sorted(sample.begin(), sample.end());
		sum_stack += (sp - max_sp);
		sum_heap += max_heap;
	}

	std::cout << description << ":\nstack - " << sum_stack / samples.size() << " heap - " << sum_heap / samples.size() << "\n";
	std::cout << time / samples.size() << " ms\n";
	std::cout << "sorted?: " << sorted << "\n\n";
}

template<typename T>
void testAll(const data_samples<T>& data_samples)
{
	test(data_samples, qsortRecursive<std::string, partition>, "recursive");
	test(data_samples, qsortRecursive<std::string, inlinePartition>, "recursive,\ninline partition");

	test(data_samples, qsortRecursive<std::string, partitionNoPivotCopy>, "recursive,\nno pivot copy");
	test(data_samples, qsortRecursive<std::string, inlinePartitionNoPivotCopy>, "recursive,\ninline partition,\nno pivot copy");


	test(data_samples, qsortIterative<std::string, partition>, "iterative");
	test(data_samples, qsortIterative<std::string, inlinePartition>, "iterative,\ninline partition");

	test(data_samples, qsortIterative<std::string, partitionNoPivotCopy>, "iterative,\nno pivot copy");
	test(data_samples, qsortIterative<std::string, inlinePartitionNoPivotCopy>, "iterative,\ninline partition,\nno pivot copy");


	test(data_samples, qsortIterativeNoRedundancy<std::string, partition>, "iterative no redundancy");
	test(data_samples, qsortIterativeNoRedundancy<std::string, inlinePartition>, "iterative no redundancy,\ninline partition");

	test(data_samples, qsortIterativeNoRedundancy<std::string, partitionNoPivotCopy>, "iterative no redundancy,\nno pivot copy");
	test(data_samples, qsortIterativeNoRedundancy<std::string, inlinePartitionNoPivotCopy>, "iterative no redundancy,\ninline partition,\nno pivot copy");


	test(data_samples, qsortIterativeNoRedundancyStack<std::string, partition>, "iterative no redundancy stack");
	test(data_samples, qsortIterativeNoRedundancyStack<std::string, inlinePartition>, "iterative no redundancy stack,\ninline partition");

	test(data_samples, qsortIterativeNoRedundancyStack<std::string, partitionNoPivotCopy>, "iterative no redundancy stack,\nno pivot copy");
	test(data_samples, qsortIterativeNoRedundancyStack<std::string, inlinePartitionNoPivotCopy>, "iterative no redundancy stack,\ninline partition,\nno pivot copy");
}

