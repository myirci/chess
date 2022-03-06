
#include "PieceObj.hpp"

#include <ChessLib/Chess/Definitions.hpp>

#include <stdexcept>

namespace chesslib::objboard 
{
	std::shared_ptr<PieceObj> make_shared_piece(Piece code, Square loc)
	{
		switch (code)
		{
			case pieceset::WhitePawn:
			case pieceset::BlackPawn:
				return std::make_shared<Pawn>(code, loc);
			case pieceset::WhiteRook:
			case pieceset::BlackRook:
				return std::make_shared<Rook>(code, loc);
			case pieceset::WhiteKnight:
			case pieceset::BlackKnight:
				return std::make_shared<Knight>(code, loc);
			case pieceset::WhiteBishop:
			case pieceset::BlackBishop:
				return std::make_shared<Bishop>(code, loc);
			case pieceset::WhiteQueen:
			case pieceset::BlackQueen:
				return std::make_shared<Queen>(code, loc);
			case pieceset::WhiteKing:
			case pieceset::BlackKing:
				return std::make_shared<King>(code, loc);
			default:
				throw std::logic_error("Undefined piece code.");
				break;
		}

		return nullptr;
	}
}
