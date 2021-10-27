#pragma once

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Utility/IterableStack.hpp>

#include <unordered_map>

namespace chesslib 
{
	class BoardBase
	{
	public:

		// Last made move and board states to remember before making the move.
		// This information is required to be able to take the move back.
		struct State
		{
			State(const Move& mv, uint16_t hmc, int8_t castling, Square ept, Piece captured) :
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

		using MoveStack = utility::IterableStack<State>;

		// key			: pinned piece location
		// value.first	: attacker location
		// value.second : direction from pinned piece to the attacker
		using PinMap = std::unordered_map<Square, std::pair<Square, Direction>>;
		
		// 0: attacker location
		// 1: direction from king location to the attacker
		// 2: distance from king location to attacker (dist >= 1, number of moves for king to reach the attacker)
		using CheckList = std::vector<std::tuple<Square, Direction, Distance>>;

		// getters
		inline Color GetActiveColor() const			 { return _active_color; }
		inline Square GetEnPassantSquare() const	 { return _enpassant_target; }
		inline uint16_t GetHalfMoveClock() const	 { return _halfmove_clock; }
		inline uint16_t GetFullMoveClock() const	 { return _fullmove_clock; }
		inline bool IsCastlingAvailable() const		 { return _castling_rights != 0; }
		inline bool QueryCastling(Castling c) const	 { return _castling_rights & static_cast<int8_t>(c); }

		inline const MoveStack& GetMoveStack() const { return _move_stack; }
		inline const PinMap& GetPins() const		 { return _pins; }
		inline const CheckList& GetChecks() const	 { return _checks; }

		// setters
		inline void SetActiveColor(Color side_to_move)	{ _active_color = side_to_move; }
		inline void SetEnPassantSquare(Square ep)		{ _enpassant_target = ep; }
		inline void SetHalfMoveClock(uint16_t hmc)		{ _halfmove_clock = hmc; }
		inline void SetFullMoveClock(uint16_t fmc)		{ _fullmove_clock = fmc; }
		inline void SetCastling(Castling c, bool flag)  {
			_castling_rights = flag 
				? _castling_rights | static_cast<int8_t>(c) 
				: _castling_rights & ~static_cast<int8_t>(c);
		}

	protected:

		BoardBase() :
			_active_color{ 0 },
			_castling_rights{ 0 },
			_enpassant_target{ squareset::None },
			_halfmove_clock{ 0 },
			_fullmove_clock{ 1 }
		{ }

		inline void ClearChecksAndPins() const			  { _pins.clear(); _checks.clear(); };
		inline bool IsPiecePinned(Square piece_loc) const { return _pins.find(piece_loc) != _pins.end(); }

		Direction GetPinDirection(Square piece_loc) const 
		{
			auto it = _pins.find(piece_loc);
			return it == _pins.end() ? direction::None : it->second.second;
		}
		
		void PushToMoveStack(const Move& mv, Piece captured = pieceset::None) 
		{
			_move_stack.push({ mv, _halfmove_clock, _castling_rights, _enpassant_target, captured });
		}

		Color _active_color;
		int8_t _castling_rights;
		Square _enpassant_target;
		uint16_t _halfmove_clock;
		uint16_t _fullmove_clock;

		MoveStack _move_stack;

		mutable PinMap _pins;
		mutable CheckList _checks;
	};
}
