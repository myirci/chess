#pragma once

#include <ChessLib/MoveGenerator/AttackDetectorBase.hpp>

namespace chesslib 
{
	template <typename BoardType>
	class AttackDetector : public AttackDetectorBase<BoardType>
	{
	public:

		template <Color Attacker>
		void ComputeChecksAndPins(const BoardType& board, Square king_pos);

		template<Color Attacker>
		bool IsUnderAttack(const BoardType& board, Square sq) const;

	private:

		template<Color Attacker, typename DirectionArray>
		void ComputeSlidingPieceChecksAndPins(const BoardType& board, const DirectionArray& dir_arr, Piece attacking_piece, Square king_pos);

		template<Color Attacker>
		void ComputeKnightChecks(const BoardType& board, Square king_pos);

		template<Color Attacker>
		void ComputePawnChecks(const BoardType& board, Square king_pos);

		template<Color SideToMove, typename DirectionArray>
		bool IsUnderSlidingPieceAttack(const BoardType& board, const DirectionArray& dir_arr, Piece attacking_piece, Square sq) const;

		template<Color Attacker>
		bool InUnderKingAttack(const BoardType& board, Square sq) const;

		template<Color Attacker>
		bool IsUnderKnightAttack(const BoardType& board, Square sq) const;

		template<Color Attacker>
		bool IsUnderPawnAttack(const BoardType& board, Square sq) const;
	};

	template <typename BoardType>
	template <Color Attacker>
	void AttackDetector<BoardType>::ComputeChecksAndPins(const BoardType& board, Square king_pos)
	{
		this->Clear();

		using ctraits = traits::color_traits<Attacker>;
		ComputeSlidingPieceChecksAndPins<Attacker>(board, BoardType::AllDirections, ctraits::Queen, king_pos);
		ComputeSlidingPieceChecksAndPins<Attacker>(board, BoardType::StraightDirections, ctraits::Rook, king_pos);
		ComputeSlidingPieceChecksAndPins<Attacker>(board, BoardType::DiagonalDirections, ctraits::Bishop, king_pos);
		ComputeKnightChecks<Attacker>(board, king_pos);
		ComputePawnChecks<Attacker>(board, king_pos);
	}

	template <typename BoardType>
	template<Color Attacker, typename DirectionArray>
	void AttackDetector<BoardType>::ComputeSlidingPieceChecksAndPins(const BoardType& board, const DirectionArray& dir_arr, Piece attacking_piece, Square king_pos)
	{
		using ctraits = traits::color_traits<Attacker>;
		for (Direction dir : dir_arr) 
		{
			Square pin_loc{ Empty };
			for (Square next{ king_pos + dir }; IsInside<BoardType>(next, next - dir); next += dir)
			{
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if (p == attacking_piece)
				{
					if (pin_loc != Empty)
						this->_pins[this->_numPins++] = { next, pin_loc, direction::Reverse(dir) };
					else
						this->_checks[this->_numChecks++] = { next, king_pos, direction::Reverse(dir) };
					break;
				}
				else
				{
					if (pin_loc == Empty && color::get_color(p) == ctraits::Opposite)
						pin_loc = next;
					else
						break;
				}
			}
		}
	}

	template <typename BoardType>
	template<Color Attacker>
	void AttackDetector<BoardType>::ComputeKnightChecks(const BoardType& board, Square king_pos)
	{
		using ctraits = traits::color_traits<Attacker>;
		Square next{ Empty };
		for (Direction dir : BoardType::KnightJumps) 
		{
			if (next = king_pos + dir; IsInside<BoardType>(next, king_pos) && board.GetPiece(next) == ctraits::Knight)
			{
				this->_checks[this->_numChecks++] = { next, king_pos, Empty };
				return;
			}
		}
	}

	template <typename BoardType>
	template<Color Attacker>
	void AttackDetector<BoardType>::ComputePawnChecks(const BoardType& board, Square king_pos)
	{
		using bctraits = traits::board_color_traits<BoardType, Attacker>;
		if (king_pos < bctraits::ValidPawnMoveSquares[0] || king_pos > bctraits::ValidPawnMoveSquares[1])
			return;

		using ctraits = traits::color_traits<Attacker>;
		for (int i = 0; i < 2; i++)
		{
			Square next = king_pos + bctraits::PawnReverseAttackDirections[i];
			if (IsInside<BoardType>(next, king_pos) && board.GetPiece(next) == ctraits::Pawn) 
			{
				this->_checks[this->_numChecks++] = { next, king_pos, direction::Reverse(bctraits::PawnReverseAttackDirections[i]) };
				return;
			}
		}
	}

	template <typename BoardType>
	template<Color Attacker>
	bool AttackDetector<BoardType>::IsUnderAttack(const BoardType& board, Square sq) const
	{ 
		using ctraits = traits::color_traits<Attacker>;

		return
			IsUnderSlidingPieceAttack<Attacker>(board, BoardType::AllDirections, ctraits::Queen, sq) ||
			IsUnderSlidingPieceAttack<Attacker>(board, BoardType::StraightDirections, ctraits::Rook, sq) ||
			IsUnderSlidingPieceAttack<Attacker>(board, BoardType::DiagonalDirections, ctraits::Bishop, sq) ||
			IsUnderKnightAttack<Attacker>(board, sq) ||
			IsUnderPawnAttack<Attacker>(board, sq) ||
			InUnderKingAttack<Attacker>(board, sq);
	}

	template <typename BoardType>
	template<Color SideToMove, typename DirectionArray>
	bool AttackDetector<BoardType>::IsUnderSlidingPieceAttack(const BoardType& board, const DirectionArray& dir_arr, Piece attacking_piece, Square sq) const
	{
		for (Direction dir : dir_arr)
		{
			for (Square next{ sq + dir }; IsInside<BoardType>(next, next - dir); next += dir)
			{
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if (p == attacking_piece)
					return true;

				break;
			}
		}
		return false;
	}

	template <typename BoardType>
	template<Color Attacker>
	bool AttackDetector<BoardType>::InUnderKingAttack(const BoardType& board, Square sq) const 
	{
		using ctraits = traits::color_traits<Attacker>;
		Square next{ Empty };
		for (Direction dir : BoardType::AllDirections) 
			if (next = sq + dir; IsInside<BoardType>(next, sq) && board.GetPiece(next) == ctraits::King)
				return true;
		return false;
	}

	template <typename BoardType>
	template<Color Attacker>
	bool AttackDetector<BoardType>::IsUnderKnightAttack(const BoardType& board, Square sq) const
	{
		using ctraits = traits::color_traits<Attacker>;
		Square next{ Empty };
		for (Direction dir : BoardType::KnightJumps)
			if (next = sq + dir; IsInside<BoardType>(next, sq) && board.GetPiece(next) == ctraits::Knight)
				return true;
		return false;
	}

	template <typename BoardType>
	template<Color Attacker>
	bool AttackDetector<BoardType>::IsUnderPawnAttack(const BoardType& board, Square sq) const 
	{
		using bctraits = traits::board_color_traits<BoardType, Attacker>;
		if (sq < bctraits::ValidPawnMoveSquares[0] || sq > bctraits::ValidPawnMoveSquares[1])
			return false;

		using ctraits = traits::color_traits<Attacker>;
		for (int i = 0; i < 2; i++)
		{
			Square next = sq + bctraits::PawnReverseAttackDirections[i];
			if (IsInside<BoardType>(next, sq) && board.GetPiece(next) == ctraits::Pawn)
				return true;
		}
		return false;
	}
}