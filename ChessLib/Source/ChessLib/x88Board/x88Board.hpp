#pragma once

#include<ChessLib/Chess/BoardBase.hpp>

#include <array>
#include <unordered_map>

namespace chesslib::x88board
{
	class Board : public BoardBase
	{
	public:
		constexpr static int BOARDSIZE = 128;
		using BoardArray = std::array<Square, BOARDSIZE>;
		
		Board(std::string_view fen);

		const BoardArray& GetBoard() const;

	private:
		
		using const_rank_iterator = std::vector<std::string_view>::const_iterator;
		void SetBoard(const_rank_iterator first, const_rank_iterator last);

		BoardArray board;
	};
}
