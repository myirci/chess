#pragma once

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/MoveGenerator/Connections.hpp>

namespace chesslib::objboard
{
	struct PieceObj;

	struct SquareObj
	{
		constexpr explicit SquareObj(Square code = Empty) : 
			_code{ code }, 
			_piece{ nullptr },
			_neighbors { conn::details::GetDirectNeighbors(code) }
		{ }

		Square				  _code;							
		PieceObj*			  _piece;
		conn::NeighborSquares _neighbors;
	};
}