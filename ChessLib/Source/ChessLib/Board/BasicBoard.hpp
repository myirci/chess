#pragma once

#include <ChessLib/Board/SimpleBoard.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/ColorTraits.hpp>
#include <ChessLib/Chess/BoardColorTraits.hpp>
#include <ChessLib/Utility/Utility.hpp>

#include <array>
#include <string_view>
#include <memory>
#include <vector>

namespace chesslib
{
	class BoardFactory;

	class BasicBoard : 
		public SimpleBoard
	{
	public: 

		static constexpr bool IsInside(Square curr, Square next) noexcept
		{
			return next < BOARDSIZE && next >= 0 && utility::numeric::abs(GetFile(next) - GetFile(curr)) <= 2;
		}

	protected:

		BasicBoard() noexcept : SimpleBoard() { _board.fill(Empty); }

		friend BoardFactory;
	};
}