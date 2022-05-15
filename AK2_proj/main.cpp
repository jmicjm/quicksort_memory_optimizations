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

template<typename T, typename qsort_f>
void test(data_samples<T> samples, qsort_f qsort, std::string_view description)
{
	auto sum_sp = 0;
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

		sum_sp += (sp - max_sp);
		sum_heap += max_heap;
	}

	std::cout << description << ":\nstack - " << sum_sp / samples.size() << " heap - " << sum_heap / samples.size() << "\n";
	std::cout << time / samples.size() << " ms\n";
	std::cout << "sorted?: " << sorted << "\n\n";
}


int main()
{
	data_samples<std::string> data_samples(16);

	for (auto& sample : data_samples)
	{
		for (int i = 0; i < 1e6; i++)
		{
			std::string str(rval(1, 16), ' ');
			for (auto& ch : str) ch = rval('a', 'z');

			sample.push_back(str);
		}
	}

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

	return 0;
}