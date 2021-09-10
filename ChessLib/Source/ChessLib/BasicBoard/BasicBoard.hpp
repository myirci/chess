#pragma once

#include <ChessLib/Chess/BoardBase.hpp>

#include <array>
#include <unordered_map>
#include <string_view>

namespace chesslib::basic_board
{
	class BasicBoard : public BoardBase
	{
	public:

		constexpr static int BOARDSIZE = 64;
		using BoardArray = std::array<Square, BOARDSIZE>;

		BasicBoard(std::string_view fen);

		const BoardArray& GetBoard() const;
		
	private:

		using const_rank_iterator = std::vector<std::string_view>::const_iterator;
		void SetBoard(const_rank_iterator first, const_rank_iterator last);

		BoardArray board;
	};
}


