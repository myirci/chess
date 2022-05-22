#pragma once

#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Chess/Definitions.hpp>

#include <ChessLib/MoveGenerator/ChecksAndPins.hpp>

namespace chesslib
{
	template <typename BoardType>
	class MoveGenerator 
	{
	public:

		static MoveList GenerateMoves(const BoardType& board)
		{
			MoveList moves;

			if (board.GetActiveColor() == color::White)
				GenerateMoves<color::White>(board, moves);
			else
				GenerateMoves<color::Black>(board, moves);

			return moves;
		}

	private:

		ChecksAndPins<BoardType> _checks_and_pins;

		template<Color SideToMove>
		void GenerateMoves(const BoardType& board, MoveList& moves)
		{
			using ctraits = traits::color_traits<SideToMove>;

			auto king_pos{ board.GetKingPosition<SideToMove>() };
		}
	};
}