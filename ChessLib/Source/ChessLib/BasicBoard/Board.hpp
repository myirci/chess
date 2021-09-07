#pragma once

#include <ChessLib/Chess/BoardBase.hpp>

#include <array>
#include <unordered_map>
#include <string_view>

namespace chesslib::basic_board
{
	class Board :public BoardBase
	{
	public:
		
		using BoardArray = std::array<Square, 64>;
		using PieceMap = std::unordered_multimap<Piece, Square>;

		Board(std::string_view fen);

		const BoardArray& GetBoard() const;
		const PieceMap& GetWhitePieces() const;
		const PieceMap& GetBlackPieces() const;

	private:
		BoardArray board;
		PieceMap white_pieces;
		PieceMap black_pieces;
	};
}


