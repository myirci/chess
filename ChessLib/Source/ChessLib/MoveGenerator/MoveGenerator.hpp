#pragma once

#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/TypeTraits.hpp>

#include <ChessLib/MoveGenerator/ChecksAndPins.hpp>

namespace chesslib
{
	template <typename BoardType>
	class MoveGenerator 
	{
	public:

		MoveList GenerateMoves(const BoardType& board)
		{
			MoveList moves;

			auto stm{ board.GetActiveColor() };
			if (stm == color::White)
				GenerateMoves<color::White>(board, moves);
			else
				GenerateMoves<color::Black>(board, moves);

			return moves;
		}

	private:

		ChecksAndPins<BoardType> _checks_and_pins;

		template<Color Clr>
		void GenerateMoves(const BoardType& board, MoveList& moves)
		{
			using ctraits = traits::color_traits<Clr>;

			auto king_pos{ board.GetKingPosition<Clr>() };
		}
	};
}