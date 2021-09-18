#include "Bitboard.hpp"
#include <ChessLib/Chess/BoardBase.hpp>

#include <array>

namespace chesslib::bitboard
{
	using Bitset = uint64_t;

	class BitBoard : public BoardBase 
	{

	private:

		std::array<Bitset, 6> _white_pieces;
		std::array<Bitset, 6> _black_pieces;
	};
}