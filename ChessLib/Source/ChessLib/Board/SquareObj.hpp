#pragma once

#include <ChessLib/Chess/Definitions.hpp>

namespace chesslib::objboard
{
	struct PieceObj;

	struct SquareObj
	{
		SquareObj(Square code = Empty) : _code{ code }, _piece{ nullptr } { }
		Square    _code;							
		PieceObj* _piece;
	};
}