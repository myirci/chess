#include "BoardBase.hpp"

namespace chesslib
{
	BoardBase::BoardBase() :
		active_color{ 0 },
		castling_rights{ 0 },
		en_passant_target{ squareset::Empty },
		half_move_clock{ 0 },
		full_move_clock{ 1 }	
	{ }

	Color BoardBase::GetActiveColor() const { return active_color; }

	Square BoardBase::GetEnPassantSquare() const { return en_passant_target; };

	uint16_t BoardBase::GetHalfMoveClock() const { return half_move_clock; };

	uint16_t BoardBase::GetFullMoveClock() const { return full_move_clock; };

	bool BoardBase::QueryCastling(Castling c) const { return castling_rights & static_cast<int8_t>(c); }

	void BoardBase::SetCastling(Castling c, bool flag)
	{
		castling_rights = flag
			? castling_rights | static_cast<int8_t>(c)
			: castling_rights & ~static_cast<int8_t>(c);
	}
}