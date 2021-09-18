#pragma once

#include<ChessLib/Chess/BoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>

#include <array>
#include <unordered_map>
#include <memory>
#include <vector>

namespace chesslib::x88board
{
	class x88Board : public BoardBaseWithPieces
	{
	public:
		constexpr static int BOARDSIZE = 128;
		using BoardArray = std::array<Square, BOARDSIZE>;

		const BoardArray& GetBoard() const;
		BoardArray& GetBoard();

		std::vector<Move> GenerateMoves() const;

	protected:
		
		x88Board();
		BoardArray board;

		friend std::unique_ptr<x88Board> make_unique_board(std::string_view fen);
		friend std::shared_ptr<x88Board> make_shared_board(std::string_view fen);
	};

	std::unique_ptr<x88Board> make_unique_board(std::string_view fen);
	std::shared_ptr<x88Board> make_shared_board(std::string_view fen);
}
