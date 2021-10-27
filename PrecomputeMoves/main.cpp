#include <fstream>
#include <iostream>
#include <string>

#include <ChessLib/MoveGenerator/PrecomputeMoves.hpp>
#include <ChessLib/Chess/Move.hpp>

using namespace chesslib::movegenerator;

int main() 
{
	std::string_view fpath{""};

	// PrecomputeMoves<ColorsToCompute::White> move_gen;

	// chesslib::MoveList moves;
	// move_gen.ComputeStraightSlidingPieceMoves(moves);
	// move_gen.ComputeDiagonallySlidingPieceMoves(moves);
	// move_gen.ComputeKnightMoves(moves);
	// std::cout << moves.size() << std::endl;
}