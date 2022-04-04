
#include "PieceObj.hpp"

#include <ChessLib/Chess/Definitions.hpp>

#include <stdexcept>

namespace chesslib::objboard 
{
	std::unique_ptr<PieceObj> make_piece(Piece code, Square loc)
	{
		switch (code)
		{
			case pieceset::WhitePawn:
			case pieceset::BlackPawn:
				return std::make_unique<Pawn>(code, loc);
			case pieceset::WhiteRook:
			case pieceset::BlackRook:
				return std::make_unique<Rook>(code, loc);
			case pieceset::WhiteKnight:
			case pieceset::BlackKnight:
				return std::make_unique<Knight>(code, loc);
			case pieceset::WhiteBishop:
			case pieceset::BlackBishop:
				return std::make_unique<Bishop>(code, loc);
			case pieceset::WhiteQueen:
			case pieceset::BlackQueen:
				return std::make_unique<Queen>(code, loc);
			case pieceset::WhiteKing:
			case pieceset::BlackKing:
				return std::make_unique<King>(code, loc);
			default:
				throw std::logic_error("Undefined piece code.");
				break;
		}

		return nullptr;
	}
}
