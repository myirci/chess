#pragma once

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Board/BasicBoard/Square.hpp>
#include <ChessLib/Board/BasicBoard/BasicBoard.hpp>

namespace chesslib::movegenerator 
{
	enum class ColorsToCompute 
	{
		White,
		Black,
		White_Black
	};

	// Computes all possible moves for all pieces for the given color(s).
	template <ColorsToCompute clr>
	class PrecomputeMoves
	{
	public:
		
		MoveList ComputeMoves() 
		{
			MoveList moves;
			ComputeStraightSlidingPieceMoves(moves);
			ComputeDiagonallySlidingPieceMoves(moves);
			ComputeKnightMoves(moves);
			ComputeKingMoves(moves);
		}
		
		int ComputeDiagonallySlidingPieceMoves(MoveList& moves) 
		{
			return ComputeSlidingPieceMoves(moves, basic_board::direction::Diagonal);
		}

		int ComputeStraightSlidingPieceMoves(MoveList& moves) 
		{
			return ComputeSlidingPieceMoves(moves, basic_board::direction::Straight);
		}

		int ComputeKnightMoves(MoveList& moves) 
		{
			return ComputeNonSlidingPieceMoves(moves, basic_board::direction::KnightJumps);
		}

		int ComputeKingMoves(MoveList& moves) 
		{
			int num_moves = ComputeNonSlidingPieceMoves(moves, basic_board::direction::All);

			if constexpr (clr == ColorsToCompute::White || clr == ColorsToCompute::White_Black) 
			{
				num_moves += 2;
				moves.emplace_back(squareset::e1, squareset::f1, MoveType::King_Side_Castle);
				moves.emplace_back(squareset::e1, squareset::c1, MoveType::Queen_Side_Castle);
			}
			
			if constexpr (clr == ColorsToCompute::Black || clr == ColorsToCompute::White_Black)
			{
				num_moves += 2;
				moves.emplace_back(squareset::e8, squareset::f8, MoveType::King_Side_Castle);
				moves.emplace_back(squareset::e8, squareset::c8, MoveType::Queen_Side_Castle);
			}
			return num_moves;
		}

		int ComputePawnMoves(MoveList& moves) 
		{
			return 0;
		}

	protected:

		int ComputeNonSlidingPieceMoves(MoveList & moves, const std::array<Direction, 8>& dirs)
		{
			int num_moves{ 0 };

			for (int i = 0; i < 64; i++)
			{
				Square from = basic_board::bottom_to_top_order[i];

				for (Direction dir : dirs) 
				{
					Square next{ from + dir };
					if(basic_board::BasicBoard::IsInside(from, next))
						AddMoves(from, next, moves, num_moves);
				}
			}

			return num_moves;
		}
		
		int ComputeSlidingPieceMoves(MoveList& moves, const std::array<Direction, 4>& dirs) 
		{
			int num_moves{ 0 };

			for (int i = 0; i < 64; i++)
			{
				Square from = basic_board::bottom_to_top_order[i];

				for (Direction dir : dirs)
					for (Square next{ from + dir }; basic_board::BasicBoard::IsInside(next - dir, next); next += dir)
						AddMoves(from, next, moves, num_moves);
			}

			return num_moves;
		}
	
		void AddMoves(Square from, Square next, MoveList& moves, int& num_moves)
		{
			num_moves++;
			moves.emplace_back(from, next, MoveType::Quite);

			if constexpr (clr == ColorsToCompute::White || clr == ColorsToCompute::White_Black)
			{
				num_moves += 5;
				moves.emplace_back(from, next, MoveType::Capture);
			}

			if constexpr (clr == ColorsToCompute::Black || clr == ColorsToCompute::White_Black)
			{
				num_moves += 5;
				moves.emplace_back(from, next, MoveType::Capture);
			}
		}
	};
}

