#pragma once

#include <ChessLib/Chess/BoardBase.hpp>

#include <array>
#include <string_view>
#include <memory>

namespace chesslib::basic_board
{
	class BasicBoard : public BoardBaseWithPieces
	{
	public:

		constexpr static int BOARDSIZE = 64;
		using BoardArray = std::array<Square, BOARDSIZE>;

		const BoardArray& GetBoard() const;
		BoardArray& GetBoard();

		// void MakeMove(Move const& mv);
		// void UnmakeMove(Move const& mv);
		
	protected:

		BasicBoard();
		BoardArray board;

		friend std::unique_ptr<BasicBoard> make_unique_board(std::string_view fen);
		friend std::shared_ptr<BasicBoard> make_shared_board(std::string_view fen);
	};

	std::unique_ptr<BasicBoard> make_unique_board(std::string_view fen);
	std::shared_ptr<BasicBoard> make_shared_board(std::string_view fen);
}


