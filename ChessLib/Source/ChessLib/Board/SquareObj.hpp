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

		Square _code;							// should be static constexpr
		std::array<Square, 8> _neighbors;		// should be static constexpr
		std::array<Square, 8> _knight_jumps;	// should be static constexpr
		PieceObj* _piece;
	};
}