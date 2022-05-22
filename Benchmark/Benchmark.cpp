#include "BitIndexBenchmark.hpp"

#include <iostream>
#include <string_view>

void bit_index_find_benchmark();

int main() 
{
	bit_index_find_benchmark();
}

void bit_index_find_benchmark() 
{
	if (!bit_index_benchmark::test())
	{
		std::cout << "Test failed!" << std::endl;
		return;
	}
	bit_index_benchmark::benchmark(100000);
}

