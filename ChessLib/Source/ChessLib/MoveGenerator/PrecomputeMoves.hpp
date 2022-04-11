#pragma once

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/LightMove.hpp>
#include <ChessLib/Board/BasicBoard.hpp>
#include <ChessLib/Chess/TypeTraits.hpp>

namespace chesslib::movegenerator 
{
	class PrecomputeMoves
	{
	public:
		/*
		LightMoveList ComputeDiagonallySlidingPieceMoves()
		{
			return ComputeSlidingPieceMoves(basic_board::direction::Diagonal);
		}

		LightMoveList ComputeStraightSlidingPieceMoves()
		{
			return ComputeSlidingPieceMoves(basic_board::direction::Straight);
		}

		LightMoveList ComputeKnightMoves()
		{
			return ComputeNonSlidingPieceMoves(basic_board::direction::KnightJumps);
		}

		LightMoveList ComputeKingMoves()
		{
			auto moves = ComputeNonSlidingPieceMoves(basic_board::direction::All);
			moves.emplace_back(squareset::e1, squareset::g1, MoveType::King_Side_Castle);
			moves.emplace_back(squareset::e1, squareset::c1, MoveType::Queen_Side_Castle);
			moves.emplace_back(squareset::e8, squareset::g8, MoveType::King_Side_Castle);
			moves.emplace_back(squareset::e8, squareset::c8, MoveType::Queen_Side_Castle);
			return moves;
		}

		LightMoveList ComputePawnMoves()
		{
			LightMoveList moves;
			ComputePawnMoves<color::White>(moves);
			ComputePawnMoves<color::Black>(moves);
			return moves;
		}

	protected:

		template <Color CLR>
		void ComputePawnMoves(LightMoveList& moves) 
		{
			using ctraits = traits::color_traits<CLR>;
			using bptraits = traits::board_piece_traits<basic_board::BasicBoard, ctraits::Pawn>;

			Square const * order = nullptr;
			if constexpr (CLR == color::White)
				order = basic_board::bottom_to_top_order;
			else
				order = basic_board::top_to_bottom_order;

			// Double pawn push
			for (int i = 8; i < 16; i++)
			{
				Square from = order[i];
				Square next = from + 2 * bptraits::MoveDirection;
				moves.emplace_back(from, next, MoveType::Double_Pawn_Push);
			}

			// Single push and normal capture
			for (int i = 8; i < 48; i++)
			{
				Square from = order[i];
				Square next = from + bptraits::MoveDirection;
				moves.emplace_back(from, next);

				for (int j = 0; j < 2; j++)
				{
					next = from + bptraits::AttackDirections[j];
					if (basic_board::BasicBoard::IsInside(from, next))
						moves.emplace_back(from, next, MoveType::Capture);
				}
			}

			// En passant capture
			for (int i = 32; i < 40; i++)
			{
				Square from = order[i];
				for (int j = 0; j < 2; j++)
				{
					Square next = from + bptraits::AttackDirections[j];
					if (basic_board::BasicBoard::IsInside(from, next))
						moves.emplace_back(from, next, MoveType::En_Passant_Capture);
				}
			}

			// Promotion
			for (int i = 48; i < 56; i++)
			{
				Square from = order[i];
				Square next = from + bptraits::MoveDirection;
				moves.emplace_back(from, next, MoveType::Queen_Promotion);
				moves.emplace_back(from, next, MoveType::Knight_Promotion);
				moves.emplace_back(from, next, MoveType::Rook_Promotion);
				moves.emplace_back(from, next, MoveType::Bishop_Promotion);

				for (int j = 0; j < 2; j++)
				{
					next = from + bptraits::AttackDirections[j];
					if (basic_board::BasicBoard::IsInside(from, next))
					{
						moves.emplace_back(from, next, MoveType::Queen_Promotion_Capture);
						moves.emplace_back(from, next, MoveType::Knight_Promotion_Capture);
						moves.emplace_back(from, next, MoveType::Rook_Promotion_Capture);
						moves.emplace_back(from, next, MoveType::Bishop_Promotion_Capture);
					}
				}
			}
		}

		LightMoveList ComputeNonSlidingPieceMoves(const std::array<Direction, 8>& dirs)
		{
			LightMoveList moves;
			for (int i = 0; i < 64; i++)
			{
				Square from = basic_board::bottom_to_top_order[i];

				for (Direction dir : dirs) 
					if(Square next{ from + dir }; basic_board::BasicBoard::IsInside(from, next))
						AddMoves(from, next, moves);
			}
			return moves;
		}
		
		LightMoveList ComputeSlidingPieceMoves(const std::array<Direction, 4>& dirs)
		{
			LightMoveList moves;
			for (int i = 0; i < 64; i++)
			{
				Square from = basic_board::bottom_to_top_order[i];

				for (Direction dir : dirs)
					for (Square next{ from + dir }; basic_board::BasicBoard::IsInside(next - dir, next); next += dir)
						AddMoves(from, next, moves);
			}
			return moves;
		}
	
		void AddMoves(Square from, Square next, LightMoveList& moves)
		{
			moves.emplace_back(from, next, MoveType::Quite);
			moves.emplace_back(from, next, MoveType::Capture);
		}
		*/
	};
}

