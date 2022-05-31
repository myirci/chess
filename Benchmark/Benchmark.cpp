#include "BitIndexBenchmark.hpp"
#include "PinnedPieceLookupBenchmark.h"

#include <iostream>
#include <string_view>

void bit_index_find_benchmark();
void do_pinned_piece_lookup_benchmark();

int main() 
{
	// bit_index_find_benchmark();
	do_pinned_piece_lookup_benchmark();
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

void do_pinned_piece_lookup_benchmark()
{
	if (!pinned_piece_lookup_benchmark::test())
	{
		std::cout << "Test failed!" << std::endl;
		return;
	}
	pinned_piece_lookup_benchmark::benchmark(10000);
}