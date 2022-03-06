#pragma once

#include <ChessLib/Board/BoardBase.hpp>

#include <array>

namespace chesslib::bitboard 
{
	using Bitset = uint64_t;

	class BitBoard : public BoardBase
	{
		
	private:

		static constexpr Index Pawn{ 0 }, Rook{ 1 }, Knight{ 2 }, Bishop{ 3 }, Queen{ 4 }, King{ 5 };

		std::array<Bitset, 6> _white_pieces;
		std::array<Bitset, 6> _black_pieces;
		Bitset _white_all_pieces;
		Bitset _black_all_pieces;
	};
}