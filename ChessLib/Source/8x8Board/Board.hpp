#pragma once

#include <array>
#include <string_view>

#include<Chess/Definitions.hpp>

namespace chesslib 
{
	class Board
	{
	public:
		Board();
		Board(std::string_view piece_placement);
	private:
		// std::array<Square, 64> board;
	};
}


