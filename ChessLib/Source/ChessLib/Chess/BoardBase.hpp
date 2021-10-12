#pragma once

#include<ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Utility/IterableStack.hpp>

#include <unordered_map>
#include <optional>

namespace chesslib 
{
	class BoardBase
	{
	public:

		// Last made move and board states to remember before making the move.
		// This information is required to be able to take the move back.
		struct State
		{
			State(const Move& mv, int8_t castling, Square ept, uint16_t hmc) :
				move{ mv },
				castling_rights{ castling },
				enpassant_target{ ept },
				halfmove_clock{ hmc } { }

			Move move;
			int8_t castling_rights;
			Square enpassant_target;
			uint16_t halfmove_clock;
		};

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

		const PinMap& GetPins() const { return _pins; }
		const CheckList& GetChecks() const { return _checks; }
		const utility::IterableStack<State>& GetMoveStack() const { return _move_stack;  }

	protected:

		BoardBase();

		inline void ClearChecksAndPins() const { _pins.clear(); _checks.clear(); };

		inline bool IsPiecePinned(Square piece_loc) const { return _pins.find(piece_loc) != _pins.end(); }

		Direction GetPinDirection(Square piece_loc) const;

		void PushToMoveStack(const Move& mv);

		Color _active_color;
		int8_t _castling_rights;
		Square _enpassant_target;
		uint16_t _halfmove_clock;
		uint16_t _fullmove_clock;

		utility::IterableStack<State> _move_stack;

		mutable PinMap _pins;
		mutable CheckList _checks;
	};

	class BoardBaseWithPieces : public BoardBase
	{
	public:
		using PieceMap = std::unordered_multimap<Piece, Square>;
		using EqualPieceRange = std::pair<PieceMap::const_iterator, PieceMap::const_iterator>;

		const PieceMap& GetWhitePieces() const;
		PieceMap& GetWhitePieces();

		const PieceMap& GetBlackPieces() const;
		PieceMap& GetBlackPieces();

	protected:

		BoardBaseWithPieces();

		template <Color Clr>
		Square GetKingPosition() const;

		template <Color Clr>
		EqualPieceRange GetPiecePositions(Piece p) const;

		template <Color Clr>
		void UpdatePiecePosition(Piece p, Square current_pos, Square new_pos);

		template <Color Clr>
		void RemovePiece(Piece p, Square current_pos);

		template <Color Clr>
		void AddNewPiece(Piece p, Square pos);

		PieceMap _white_pieces;
		PieceMap _black_pieces;
	};
}
