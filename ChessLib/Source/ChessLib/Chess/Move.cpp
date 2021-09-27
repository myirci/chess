#include "Move.hpp"

namespace chesslib 
{
	Move::Move(Square loc_from, Square loc_to, MoveType move_type, Piece captured_piece /* = pieceset::None*/) :
		from{ loc_from }, 
		to{ loc_to }, 
		mtype{ move_type }, 
		captured{ captured_piece } { }

	Square Move::GetFrom() const { return from; }

	Square Move::GetTo() const { return to; }

	MoveType Move::GetMoveType() const { return mtype; }

	Piece Move::GetCapturedPiece() const { return captured; }

	bool Move::IsPromotion() const { return (static_cast<int8_t>(mtype) & 0x08) > 0; }

	bool Move::IsCapture() const { return (static_cast<int8_t>(mtype) & 0x04) > 0; }

	bool Move::IsCastle() const { return mtype == MoveType::King_Side_Castle || mtype == MoveType::Queen_Side_Castle; }

	bool Move::IsEnPassantCapture() const { return mtype == MoveType::En_Passant_Capture; }

	bool operator == (const Move m1, const Move m2) 
	{
		return
			m1.from == m2.from &&
			m1.to == m2.to &&
			m1.mtype == m2.mtype &&
			m1.captured == m2.captured;
	}
}