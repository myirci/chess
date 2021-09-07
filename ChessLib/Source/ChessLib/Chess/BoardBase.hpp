#pragma once

#include<ChessLib/Chess/Definitions.hpp>

namespace chesslib 
{
	class BoardBase
	{
	public:
		uint16_t GetHalfMoveClock() const;
		uint16_t GetFullMoveClock() const;
		Color GetActiveColor() const;
		Square GetEnPassantSquare() const;
		bool QueryCastling(Castling c) const;
		void SetCastling(Castling c, bool flag);

	protected:

		BoardBase();

		Color active_color;
		int8_t castling_rights;
		Square en_passant_target;
		uint16_t half_move_clock;
		uint16_t full_move_clock;
	};
}
