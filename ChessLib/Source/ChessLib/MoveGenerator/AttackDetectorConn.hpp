#pragma once

#include <ChessLib/MoveGenerator/AttackDetectorBase.hpp>
#include <ChessLib/MoveGenerator/Connections.hpp>

namespace chesslib 
{
	template <typename BoardType>
	class AttackDetectorConn : public AttackDetectorBase<BoardType>
	{
	public:

		template <Color Attacker>
		void ComputeChecksAndPins(const BoardType& board, Square king_pos);

		template<Color Attacker>
		bool IsUnderAttack(const BoardType& board, Square sq) const;

	private:

		template <Color Attacker, Direction Dir>
		void ComputeSlidingPieceChecksAndPins(const BoardType& board, Square king_pos);

		template<Color Attacker>
		void ComputeKnightChecks(const BoardType& board, Square king_pos);

		template<Color Attacker>
		void ComputePawnChecks(const BoardType& board, Square king_pos);

		template <Color Attacker, Direction Dir>
		bool IsUnderAttack(const BoardType& board, Square sq) const;

		template<Color Attacker>
		bool IsUnderPawnAttack(const BoardType& board, Square sq) const;
	};

	template <typename BoardType>
	template <Color Attacker>
	void AttackDetectorConn<BoardType>::ComputeChecksAndPins(const BoardType& board, Square king_pos)
	{
		this->Clear();

		#define SlidingChecks(Dir) ComputeSlidingPieceChecksAndPins<Attacker, Dir>(board, king_pos)

		SlidingChecks(BoardType::N); 
		SlidingChecks(BoardType::NE); 
		SlidingChecks(BoardType::E); 
		SlidingChecks(BoardType::SE);
		SlidingChecks(BoardType::S); 
		SlidingChecks(BoardType::SW); 
		SlidingChecks(BoardType::W); 
		SlidingChecks(BoardType::NW);
		ComputeKnightChecks<Attacker>(board, king_pos);
		ComputePawnChecks<Attacker>(board, king_pos);
	}

	template <typename BoardType>
	template <Color Attacker, Direction Dir>
	void AttackDetectorConn<BoardType>::ComputeSlidingPieceChecksAndPins(const BoardType& board, Square king_pos)
	{
		using ctraits = traits::color_traits<Attacker>;

		Square next{ Empty}, pin_loc{ Empty };
		Piece p{ Empty };

		conn::View<Dir> view{ king_pos };
		
		for (int8_t i = 0; i < view.GetSize(); i++) 
		{
			next = view.Next();
			p = board.GetPiece(next);
			if (p == Empty)
				continue;

			bool cond{ false };
			if constexpr (Dir == BoardType::N || Dir == BoardType::E || Dir == BoardType::S || Dir == BoardType::W)
				cond = (p == ctraits::Rook || p == ctraits::Queen);
			else if constexpr (Dir == BoardType::NE || Dir == BoardType::SE || Dir == BoardType::SW || Dir == BoardType::NW)
				cond = (p == ctraits::Bishop || p == ctraits::Queen);

			if (cond)
			{
				if (pin_loc != Empty)
					this->_pins[this->_numPins++] = { next, pin_loc, direction::Reverse(Dir) };
				else
					this->_checks[this->_numChecks++] = { next, king_pos, direction::Reverse(Dir) };
				return;
			}
			else
			{
				if (pin_loc == Empty && color::get_color(p) == ctraits::Opposite)
					pin_loc = next;
				else
					return;
			}
		}
	}

	template <typename BoardType>
	template<Color Attacker>
	void AttackDetectorConn<BoardType>::ComputeKnightChecks(const BoardType& board, Square king_pos)
	{
		using ctraits = traits::color_traits<Attacker>;
		Square next{ Empty };
		
		conn::View<conn::KnightJumps> view{ king_pos };
		for (int8_t i = 0; i < view.GetSize(); i++)
		{
			next = view.Next();
			if (board.GetPiece(next) == ctraits::Knight) 
			{
				this->_checks[this->_numChecks++] = { next, king_pos, Empty };
				return;
			}
		}
	}

	template <typename BoardType>
	template<Color Attacker>
	void AttackDetectorConn<BoardType>::ComputePawnChecks(const BoardType& board, Square king_pos)
	{
		using bctraits = traits::board_color_traits<BoardType, Attacker>;
		if (king_pos < bctraits::ValidPawnMoveSquares[0] || king_pos > bctraits::ValidPawnMoveSquares[1])
			return;

		using ctraits = traits::color_traits<Attacker>;
		conn::View<conn::PawnDirectionTrait<ctraits::Opposite>::Dir> view{ king_pos };
		Square next = view.Next(3);
		if (next != Empty && board.GetPiece(next) == ctraits::Pawn)
		{
			this->_checks[this->_numChecks++] = { next, king_pos, king_pos - next };
			return;
		}

		next = view.Next(1);
		if (next != Empty && board.GetPiece(next) == ctraits::Pawn)
			this->_checks[this->_numChecks++] = { next, king_pos, king_pos - next };
	}

	template <typename BoardType>
	template<Color Attacker>
	bool AttackDetectorConn<BoardType>::IsUnderAttack(const BoardType& board, Square sq) const
	{ 
		#define Attack(Dir) IsUnderAttack<Attacker, Dir>(board, sq)
		return 
			Attack(BoardType::N) || Attack(BoardType::NE) || Attack(BoardType::E) || Attack(BoardType::SE) ||
			Attack(BoardType::S) || Attack(BoardType::SW) || Attack(BoardType::W) || Attack(BoardType::NW) ||
			Attack(conn::KnightJumps) ||
			Attack(conn::AllDir) || // King attacks
			IsUnderPawnAttack<Attacker>(board, sq);
	}

	template <typename BoardType>
	template <Color Attacker, Direction Dir>
	bool AttackDetectorConn<BoardType>::IsUnderAttack(const BoardType& board, Square sq) const
	{
		using ctraits = traits::color_traits<Attacker>;
		Square next{ Empty };
		Piece p{ Empty };
		conn::View<Dir> view{ sq };
		for (int8_t i = 0; i < view.GetSize(); i++) 
		{
			next = view.Next();
			p = board.GetPiece(next);
			if (p == Empty)
				continue;

			if constexpr (Dir == BoardType::N || Dir == BoardType::E || Dir == BoardType::S || Dir == BoardType::W)
				return p == ctraits::Rook || p == ctraits::Queen;
			else if constexpr (Dir == BoardType::NE || Dir == BoardType::SE || Dir == BoardType::SW || Dir == BoardType::NW)
				return p == ctraits::Bishop || p == ctraits::Queen;
			else if constexpr (Dir == conn::KnightJumps) 
			{
				if (p == ctraits::Knight)
					return true;
			}
			else if constexpr (Dir == conn::AllDir) 
			{
				if (p == ctraits::King)
					return true;
			}
		}
		return false;
	}

	template <typename BoardType>
	template<Color Attacker>
	bool AttackDetectorConn<BoardType>::IsUnderPawnAttack(const BoardType& board, Square sq) const
	{
		using bctraits = traits::board_color_traits<BoardType, Attacker>;
		if (sq < bctraits::ValidPawnMoveSquares[0] || sq > bctraits::ValidPawnMoveSquares[1])
			return false;

		using ctraits = traits::color_traits<Attacker>;
		conn::View<conn::PawnDirectionTrait<ctraits::Opposite>::Dir> view{ sq };
		Square next[2] = { view.Next(3) , view.Next(1) };

		for (int i = 0; i < 2; i++) 
			if (next[i] != Empty && board.GetPiece(next[i]) == ctraits::Pawn)
				return true;
		return false;
	}
}