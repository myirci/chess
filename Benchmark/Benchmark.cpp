#include "BitIndexBenchmark.hpp"
#include "PinnedPieceLookupBenchmark.h"
#include <ChessLib/MoveGenerator/Connectivity.hpp>

#include <iostream>
#include <string_view>

// Benchmarks
void bit_index_find_benchmark();
void do_pinned_piece_lookup_benchmark();

// Computations
void compute_connectivities();
void compute_moves();

int main() 
{
	// bit_index_find_benchmark();
	// do_pinned_piece_lookup_benchmark();
	// compute_connectivities();
	compute_moves();
	return 0;
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

void compute_connectivities() 
{
	chesslib::precompute::Connectivity con;
	con.Export("../Benchmark/data/connections.txt");
}

void compute_moves() 
{
	chesslib::precompute::PrecomputeMoves pcm;
	pcm.ComputeRookMoves();
	pcm.ComputeBishopMoves();
	pcm.ComputeKnightMoves();
	pcm.ComputeKingMoves();
	pcm.ComputeWhitePawnMoves();
	pcm.ComputeBlackPawnMoves();
	pcm.Export_PieceMoves("../Benchmark/data");
}