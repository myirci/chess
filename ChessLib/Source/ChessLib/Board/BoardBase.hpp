#pragma once

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Utility/IterableStack.hpp>

#include <unordered_map>

namespace chesslib 
{
	// Represents the board state before making the "move".
	struct BoardState
	{
		BoardState(const Move& mv, uint16_t hmc, int8_t castling, Square ept, Piece captured) :
			move{ mv },
			castling_rights{ castling },
			enpassant_target{ ept },
			halfmove_clock{ hmc },
			captured_piece{ captured }
		{ }

		Move move;
		uint16_t halfmove_clock;
		int8_t castling_rights;
		Square enpassant_target;
		Piece captured_piece;
	};

	

	class BoardBase
	{
	public:

		using MoveStack = utility::IterableStack<BoardState>;

		// getters
		Color GetActiveColor() const noexcept          { return _active_color; }
		Square GetEnPassantSquare() const noexcept     { return _enpassant_target; }
		uint16_t GetHalfMoveClock() const noexcept     { return _halfmove_clock; }
		uint16_t GetFullMoveClock() const noexcept     { return _fullmove_clock; }
		bool IsCastlingAvailable() const noexcept      { return _castling_rights != 0; }
		bool QueryCastling(Castling c) const noexcept  { return _castling_rights & static_cast<int8_t>(c); }
		const MoveStack& GetMoveStack() const noexcept { return _move_stack; }

		// setters
		void SetActiveColor(Color side_to_move)	noexcept { _active_color = side_to_move; }
		void SetEnPassantSquare(Square ep) noexcept      { _enpassant_target = ep; }
		void SetHalfMoveClock(uint16_t hmc) noexcept     { _halfmove_clock = hmc; }
		void SetFullMoveClock(uint16_t fmc)	noexcept     { _fullmove_clock = fmc; }
		void SetCastling(Castling c, bool flag) noexcept {
			_castling_rights = flag 
				? _castling_rights | static_cast<int8_t>(c) 
				: _castling_rights & ~static_cast<int8_t>(c);
		}

	protected:

		BoardBase() noexcept :
			_active_color{ 0 },
			_castling_rights{ 0 },
			_enpassant_target{ squareset::None },
			_halfmove_clock{ 0 },
			_fullmove_clock{ 1 }
		{ }

		void PushToMoveStack(const Move& mv, Piece captured = pieceset::None)
		{
			_move_stack.push({mv, _halfmove_clock, _castling_rights, _enpassant_target, captured});
		}

		Color	        	  _active_color;
		int8_t			      _castling_rights;
		Square			      _enpassant_target;
		uint16_t		      _halfmove_clock;
		uint16_t		      _fullmove_clock;
		mutable MoveStack     _move_stack;
	};
}
