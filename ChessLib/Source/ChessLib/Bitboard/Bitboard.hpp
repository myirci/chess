#pragma once

#include <ChessLib/Chess/BoardBase.hpp>

#include <array>

namespace chesslib::bitboard 
{
	using Bitset = uint64_t;

	constexpr Index Pawn{ 0 }, Rook{ 1 }, Knight{ 2 }, Bishop{ 3 }, Queen{ 4 }, King{ 5 };

	class BitBoard : public BoardBase
	{

	private:

		std::array<Bitset, 6> _white_pieces;
		std::array<Bitset, 6> _black_pieces;
	};
}