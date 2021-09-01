#pragma once

#include <array>
#include <unordered_map>
#include <string_view>

#include<ChessLib/Chess/Definitions.hpp>

namespace chesslib::basic_board
{
	class Board
	{
	public:
		
		using BoardArray = std::array<Square, 64>;
		using PieceMap = std::unordered_multimap<Piece, Square>;

		Board(std::string_view fen);

		const BoardArray& GetBoard() const;
		const PieceMap& GetWhitePieces() const;
		const PieceMap& GetBlackPieces() const;

		Color GetActiveColor() const;
		Square GetEnPassantSquare() const;
		uint16_t GetHalfMoveClock() const;
		uint16_t GetFullMoveClock() const;

		inline bool QueryCastling(Castling c) const;
		inline void SetCastling(Castling c, bool flag);

	private:
		BoardArray board;
		PieceMap white_pieces;
		PieceMap black_pieces;
		Color active_color;
		int8_t castling_rights;
		uint16_t half_move_clock;
		uint16_t full_move_clock;
		Square en_passant_target; 
	};
}


