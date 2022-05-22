#pragma once

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Board/PieceObj.hpp>

#include <memory>
#include <array>

namespace chesslib::objboard
{
	constexpr Direction N{ 0 }, NE{ 1 }, E{ 2 }, SE{ 3 }, S{ 4 }, SW{ 5 }, W{ 6 }, NW{ 7 };

	struct SquareObj
	{
		SquareObj(Square code,
			std::array<Square, 8>&& neighbors,
			std::array<Square, 8>&& knight_jumps) :
			_code{ code },
			_neighbors{ std::move(neighbors) },
			_knight_jumps{ std::move(knight_jumps) },
			_piece{ nullptr }
		{ }

		SquareObj() :
			_code { Empty },
			_piece { nullptr },
			_neighbors{ {Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty} },
			_knight_jumps{ {Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty} }
		{ }

		Square _code;							
		std::array<Square, 8> _neighbors;		
		std::array<Square, 8> _knight_jumps;
		PieceObj* _piece;
	};
}