#pragma once

#include<ChessLib/Chess/Definitions.hpp>

namespace chesslib 
{
	class BoardBase
	{
	public:

		uint16_t GetHalfMoveClock() const;
		uint16_t GetFullMoveClock() const;
		Color GetActiveColor() const;
		Square GetEnPassantSquare() const;
		bool IsCastlingAvailable() const;
		bool QueryCastling(Castling c) const;
		void SetCastling(Castling c, bool flag);

		void SetActiveColor(char side_to_move);
		void SetCastlingRights(std::string_view castling_availability);
		void SetEnPassantSquare(Square ep);
		void SetHalfMoveClock(std::string_view hmc);
		void SetFullMoveClock(std::string_view fmc);

	protected:

		BoardBase();

		Color _active_color;
		int8_t _castling_rights;
		Square _enpassant_target;
		uint16_t _halfmove_clock;
		uint16_t _fullmove_clock;
	};

	class BoardBaseWithPieces : public BoardBase
	{
	public:
		using PieceMap = std::unordered_multimap<Piece, Square>;
	
		const PieceMap& GetWhitePieces() const;
		PieceMap& GetWhitePieces();

		const PieceMap& GetBlackPieces() const;
		PieceMap& GetBlackPieces();

	protected:

		BoardBaseWithPieces();

		PieceMap _white_pieces;
		PieceMap _black_pieces;
	};
}
