#pragma once

#include<ChessLib/Chess/Definitions.hpp>

#include <unordered_map>

namespace chesslib 
{
	class BoardBase
	{
	public:

		// key			: pinned piece location
		// value.first	: attacker location
		// value.second : direction from pinned piece to the attacker
		using PinMap = std::unordered_map<Square, std::pair<Square, Direction>>;
		
		// 0: attacker location
		// 1: direction from king location to the attacker
		// 2: distance from king location to attacker (dist >= 1, number of moves for king to reach the attacker)
		using CheckList = std::vector<std::tuple<Square, Direction, Distance>>;

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

		const PinMap& GetPins() const;
		const CheckList& GetChecks() const;

	protected:

		BoardBase();

		void inline ClearChecksAndPins() { _pins.clear(); _checks.clear(); };

		Color _active_color;
		int8_t _castling_rights;
		Square _enpassant_target;
		uint16_t _halfmove_clock;
		uint16_t _fullmove_clock;

		PinMap _pins;
		CheckList _checks;
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
