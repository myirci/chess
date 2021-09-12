#pragma once

#include<ChessLib/Chess/BoardBase.hpp>

#include <array>
#include <unordered_map>
#include <memory>

namespace chesslib::x88board
{
	class Board : public BoardBase
	{
	public:
		constexpr static int BOARDSIZE = 128;
		using BoardArray = std::array<Square, BOARDSIZE>;

		const BoardArray& GetBoard() const;
		BoardArray& GetBoard();

	protected:
		
		Board();
		BoardArray board;

		friend std::unique_ptr<Board> make_unique_x88board(std::string_view fen);
		friend std::shared_ptr<Board> make_shared_x88board(std::string_view fen);
	};

	std::unique_ptr<Board> make_unique_x88board(std::string_view fen);
	std::shared_ptr<Board> make_shared_x88board(std::string_view fen);
}
