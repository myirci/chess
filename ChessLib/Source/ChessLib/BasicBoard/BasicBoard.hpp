#pragma once

#include <ChessLib/Chess/BoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>

#include <array>
#include <string_view>
#include <memory>
#include <vector>

namespace chesslib::basic_board
{
	class BasicBoard : public BoardBaseWithPieces
	{
	public:

		constexpr static int BOARDSIZE = 64;
		using BoardArray = std::array<Square, BOARDSIZE>;

		const BoardArray& GetBoard() const;
		BoardArray& GetBoard();

		std::vector<Move> GenerateMoves() const;
		
	protected:

		BasicBoard();
		BoardArray board;

		template <Color SideToMove>
		void GenerateMoves(std::vector<Move>& moves) const;

		

		template <Color SideToMove>
		Square GetKingPosition() const;

		friend std::unique_ptr<BasicBoard> make_unique_board(std::string_view fen);
		friend std::shared_ptr<BasicBoard> make_shared_board(std::string_view fen);
	};

	std::unique_ptr<BasicBoard> make_unique_board(std::string_view fen);
	std::shared_ptr<BasicBoard> make_shared_board(std::string_view fen);
}


