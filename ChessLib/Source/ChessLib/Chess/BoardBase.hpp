#pragma once

#include<ChessLib/Chess/Definitions.hpp>

namespace chesslib 
{
	class BoardBase
	{
	public:

		using PieceMap = std::unordered_multimap<Piece, Square>;

		uint16_t GetHalfMoveClock() const;
		uint16_t GetFullMoveClock() const;
		Color GetActiveColor() const;
		Square GetEnPassantSquare() const;
		bool IsCastlingAvailable() const;
		bool QueryCastling(Castling c) const;
		void SetCastling(Castling c, bool flag);

		const PieceMap& GetWhitePieces() const;
		const PieceMap& GetBlackPieces() const;

	protected:

		void SetActiveColor(char side_to_move);
		void SetCastlingRights(std::string_view castling_availability);
		void SetEnPassantSquare(Square ep);
		void SetHalfMoveClock(std::string_view hmc);
		void SetFullMoveClock(std::string_view fmc);

		BoardBase();

		Color active_color;
		int8_t castling_rights;
		Square en_passant_target;
		uint16_t half_move_clock;
		uint16_t full_move_clock;

		PieceMap white_pieces;
		PieceMap black_pieces;
	};
}
