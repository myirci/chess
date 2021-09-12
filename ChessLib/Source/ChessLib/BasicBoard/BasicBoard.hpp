#pragma once

#include <ChessLib/Chess/BoardBase.hpp>

#include <array>
#include <unordered_map>
#include <string_view>
#include <memory>

namespace chesslib::basic_board
{
	class BasicBoard : public BoardBase
	{
	public:

		constexpr static int BOARDSIZE = 64;
		using BoardArray = std::array<Square, BOARDSIZE>;

		const BoardArray& GetBoard() const;
		BoardArray& GetBoard();
		
	protected:

		BasicBoard();
		BoardArray board;

		friend std::unique_ptr<BasicBoard> make_unique_basic_board(std::string_view fen);
		friend std::shared_ptr<BasicBoard> make_shared_basic_board(std::string_view fen);
	};

	std::unique_ptr<BasicBoard> make_unique_basic_board(std::string_view fen);
	std::shared_ptr<BasicBoard> make_shared_basic_board(std::string_view fen);
}


