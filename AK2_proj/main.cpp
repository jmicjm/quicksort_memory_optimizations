#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <string_view>
#include <span>
#include <chrono>

#include "partition.h"
#include "partitionNoPivotCopy.h"

#include "qsortIterative.h"
#include "qsortIterativeNoRedundancy.h"
#include "qsortRecursive.h"



int rval(int min, int max)
{
	static std::mt19937 rng(std::random_device{}());
	return std::uniform_int_distribution<>{min, max}(rng);
}

template<typename T, typename qsort_f>
void test(T data, qsort_f qsort, std::string_view description)
{
	auto sp = stackPTR();
	max_sp = stackPTR();
	max_heap = 0;
	auto b = std::chrono::steady_clock::now();

	qsort(data);

	auto e = std::chrono::steady_clock::now();
	std::cout << description << ":\nstack - " << (sp - max_sp) << " heap - " << max_heap << "\n";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(e - b).count() << " ms\n";
	std::cout << "sorted?: " << std::is_sorted(data.begin(), data.end()) << "\n\n";
}


int main()
{
	std::vector<std::string> vec;

	for (int i = 0; i < 1e6; i++)
	{
		std::string str(rval(1, 16), ' ');
		for (auto& ch : str) ch = rval('a', 'z');

		vec.push_back(str);
	}
	test(vec, qsortRecursive<std::string, partition>, "recursive");
	test(vec, qsortRecursive<std::string, partitionNoPivotCopy>, "recursive, no pivot copy");
	test(vec, qsortIterative<std::string, partition>, "iterative");
	test(vec, qsortIterative<std::string, partitionNoPivotCopy>, "iterative, no pivot copy");
	test(vec, qsortIterativeNoRedundancy<std::string, partition>, "iterative no redundancy");
	test(vec, qsortIterativeNoRedundancy<std::string, partitionNoPivotCopy>, "iterative no redundancy, no pivot copy");

	return 0;
}