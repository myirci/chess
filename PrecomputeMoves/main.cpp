#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <fstream>

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/MoveGenerator/PrecomputeMoves.hpp>
#include <ChessLib/Chess/LightMove.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

#include <ChessLib/MoveGenerator/MoveConnectivity.hpp>

using namespace chesslib;
using namespace chesslib::movegenerator;

// Pawn moves	  : 440
// Knight moves   : 672
// King Moves	  : 844
// Straight Moves : 1792
// Diagonal Moves : 1120

bool export_moves(std::string_view fpath, LightMoveList& moves);

void precompute_moves_v1();
void precompute_moves_v2();

int main() 
{
	// precompute_moves_v1();
	precompute_moves_v2();
}

void precompute_moves_v1() 
{
	std::string_view fpath{ "" };

	PrecomputeMoves move_gen;
	auto comp = [](const LightMove& mv1, LightMove& mv2)
	{
		if (mv1.GetFrom() < mv2.GetFrom())
			return true;

		if (mv1.GetFrom() == mv2.GetFrom())
		{
			if (mv1.GetTo() < mv2.GetTo())
				return true;

			if (mv1.GetTo() == mv2.GetTo())
				return mv1.GetMoveType() < mv2.GetMoveType();
		}

		return false;
	};

	auto king_moves = move_gen.ComputeKingMoves();
	std::sort(king_moves.begin(), king_moves.end(), comp);
	if (!export_moves("KingMoves.txt", king_moves))
		std::cout << "Error exporting king moves" << std::endl;

	auto pawn_moves = move_gen.ComputePawnMoves();
	std::sort(pawn_moves.begin(), pawn_moves.end(), comp);
	if (!export_moves("PawnMoves.txt", pawn_moves))
		std::cout << "Error exporting pawn moves" << std::endl;

	auto knight_moves = move_gen.ComputeKnightMoves();
	std::sort(knight_moves.begin(), knight_moves.end(), comp);
	if (!export_moves("KnightMoves.txt", knight_moves))
		std::cout << "Error exporting knight moves" << std::endl;

	auto diagonal_moves = move_gen.ComputeDiagonallySlidingPieceMoves();
	std::sort(diagonal_moves.begin(), diagonal_moves.end(), comp);
	if (!export_moves("DiagonalMoves.txt", diagonal_moves))
		std::cout << "Error exporting diagonal moves" << std::endl;

	auto straight_moves = move_gen.ComputeStraightSlidingPieceMoves();
	std::sort(straight_moves.begin(), straight_moves.end(), comp);
	if (!export_moves("StraightMoves.txt", straight_moves))
		std::cout << "Error exporting straight moves" << std::endl;
}

void precompute_moves_v2() 
{
	auto connections = chesslib::movegenerator::MoveConnectivity::ComputeConnections();
	
	std::ofstream ofile("Connectivity.txt");
	if (!ofile.is_open()) 
	{
		std::cout << "File cound not be opened\n";
		return;
	}

	for (const auto& connection : connections)
		ofile << connection;

	ofile.close();
}

bool export_moves(std::string_view fpath, LightMoveList& moves) 
{
	std::ofstream ofile(fpath.data());
	if (!ofile.is_open())
		return false;

	for (const auto& mv : moves)
		ofile << utility::chess::to_string_long<basic_board::BasicBoard>(mv) << std::endl;

	ofile.close();
	return true;
}