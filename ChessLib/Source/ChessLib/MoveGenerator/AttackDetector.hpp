#pragma once

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/ColorTraits.hpp>
#include <ChessLib/Chess/BoardColorTraits.hpp>
#include <ChessLib/Board/BoardFunctions.hpp>
#include <ChessLib/Board/ObjBoard.hpp>
#include <ChessLib/MoveGenerator/Connections.hpp>
#include <unordered_map>

namespace chesslib 
{
	
#pragma region AttackDetector
	template <typename BoardType>
	class AttackDetector
	{
	public:

		struct check
		{
			Square    checker;		// checker location
			Direction checker_dir;  // direction from king to checker
			Distance  checker_dist; // distance from king to checker
		};

		struct pin
		{
			Square    pinned;	  // pinned piece location
			Square    pinner;	  // attacker location
			Direction pinner_dir; // direction from pinned piece to the attacker
		};

		const check& GetFirstCheck() const;

		std::uint8_t GetNumberOfChecks() const noexcept;

		bool IsPinned(Square piece_loc) const noexcept;

		Direction GetPinDirection(Square piece_loc) const noexcept;

		template<Color Attacker>
		bool CanKingMoveInThisDirection(const BoardType& board, Direction dir) const;

		template <Color Attacker>
		void ComputeChecksAndPins(const BoardType& board, Square king_pos);

		template<Color Attacker>
		bool IsUnderAttack(const BoardType& board, Square sq) const;

	private:

		std::uint8_t _numPins;
		std::uint8_t _numChecks;
		pin		     _pins[8];
		check	     _checks[2];

		inline void Clear() noexcept;

		template<Color Attacker>
		bool IsUnderStraightSlidingPieceAttack(const BoardType& board, Square sq) const;

		template<Color Attacker>
		bool IsUnderDiagonallySlidingPieceAttack(const BoardType& board, Square sq) const;

		template<Color Attacker>
		bool InUnderKingAttack(const BoardType& board, Square sq) const;

		template<Color Attacker>
		bool IsUnderKnightAttack(const BoardType& board, Square sq) const;

		template<Color Attacker>
		bool IsUnderPawnAttack(const BoardType& board, Square sq) const;

		template<Color Attacker>
		void ComputeStraightSlidingPieceChecksAndPins(const BoardType& board, Square king_pos);

		template<Color Attacker>
		void ComputeDiagonallySlidingPieceChecksAndPins(const BoardType& board, Square king_pos);

		template<Color Attacker>
		void ComputeKnightChecks(const BoardType& board, Square king_pos);

		template<Color Attacker>
		void ComputePawnChecks(const BoardType& board, Square king_pos);
	};

	template <typename BoardType>
	const typename AttackDetector<BoardType>::check& AttackDetector<BoardType>::GetFirstCheck() const { return _checks[0]; }

	template <typename BoardType>
	std::uint8_t AttackDetector<BoardType>::GetNumberOfChecks() const noexcept { return _numChecks; }

	template <typename BoardType>
	bool AttackDetector<BoardType>::IsPinned(Square piece_loc) const noexcept
	{
		for (int i = 0; i < _numPins; i++)
			if (_pins[i].pinned == piece_loc)
				return true;
		return false;
	}

	template <typename BoardType>
	Direction AttackDetector<BoardType>::GetPinDirection(Square piece_loc) const noexcept
	{
		for (int i = 0; i < _numPins; i++)
			if (_pins[i].pinned == piece_loc)
				return _pins[i].pinner_dir;
		return Empty;
	}

	template <typename BoardType>
	template<Color Attacker>
	bool AttackDetector<BoardType>::CanKingMoveInThisDirection(const BoardType& board, Direction dir) const
	{
		for (int i = 0; i < this->_numChecks; i++)
		{
			if (this->_checks[i].checker_dir == dir && this->_checks[i].checker_dist != 1 ||
				this->_checks[i].checker_dir == direction::Reverse(dir) &&
				board.GetPiece(this->_checks[i].checker) != traits::color_traits<Attacker>::Pawn)
				return false;
		}
		return true;
	}

	template <typename BoardType>
	void AttackDetector<BoardType>::Clear() noexcept
	{
		_numPins = 0;
		_numChecks = 0;
	}

	template <typename BoardType>
	template <Color Attacker>
	void AttackDetector<BoardType>::ComputeChecksAndPins(const BoardType& board, Square king_pos)
	{
		this->Clear();

		ComputeStraightSlidingPieceChecksAndPins<Attacker>(board, king_pos);
		ComputeDiagonallySlidingPieceChecksAndPins<Attacker>(board, king_pos);
		ComputeKnightChecks<Attacker>(board, king_pos);
		ComputePawnChecks<Attacker>(board, king_pos);
	}

	template <typename BoardType>
	template<Color Attacker>
	void AttackDetector<BoardType>::ComputeStraightSlidingPieceChecksAndPins(const BoardType& board, Square king_pos)
	{
		using ctraits = traits::color_traits<Attacker>;
		for (Direction dir : BoardType::StraightDirections)
		{
			Square pin_loc{ Empty };
			for (Square next{ king_pos + dir }; IsInside<BoardType>(next, next - dir); next += dir)
			{
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if (p == ctraits::Rook || p == ctraits::Queen)
				{
					if (pin_loc != Empty)
						this->_pins[this->_numPins++] = { pin_loc, next, dir };
					else
						this->_checks[this->_numChecks++] = { next, dir, (next - king_pos) / dir};
					break;
				}
				else
				{
					if (pin_loc == Empty)
						pin_loc = next;
					else
						break;
				}
			}
		}
	}

	template <typename BoardType>
	template<Color Attacker>
	void AttackDetector<BoardType>::ComputeDiagonallySlidingPieceChecksAndPins(const BoardType& board, Square king_pos)
	{
		using ctraits = traits::color_traits<Attacker>;
		for (Direction dir : BoardType::DiagonalDirections)
		{
			Square pin_loc{ Empty };
			for (Square next{ king_pos + dir }; IsInside<BoardType>(next, next - dir); next += dir)
			{
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if (p == ctraits::Bishop || p == ctraits::Queen)
				{
					if (pin_loc != Empty)
						this->_pins[this->_numPins++] = { pin_loc, next, dir };
					else
						this->_checks[this->_numChecks++] = { next, dir, (next - king_pos) / dir };
					break;
				}
				else
				{
					if (pin_loc == Empty)
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
				this->_checks[this->_numChecks++] = { next, Empty, Empty };
				return;
			}
		}
	}

	template <typename BoardType>
	template<Color Attacker>
	void AttackDetector<BoardType>::ComputePawnChecks(const BoardType& board, Square king_pos)
	{
		using ctraits = traits::color_traits<Attacker>;
		using bctraits = traits::board_color_traits<BoardType, Attacker>;
		for (int i = 0; i < 2; i++)
		{
			Square next = king_pos + bctraits::PawnReverseAttackDirections[i];
			if (IsInside<BoardType>(next, king_pos) && board.GetPiece(next) == ctraits::Pawn) 
			{
				this->_checks[this->_numChecks++] = { next, bctraits::PawnReverseAttackDirections[i], (Distance)1 };
				return;
			}
		}
	}

	template <typename BoardType>
	template<Color Attacker>
	bool AttackDetector<BoardType>::IsUnderAttack(const BoardType& board, Square sq) const
	{ 
		return
			IsUnderStraightSlidingPieceAttack<Attacker>(board, sq) ||
			IsUnderDiagonallySlidingPieceAttack<Attacker>(board, sq) ||
			IsUnderKnightAttack<Attacker>(board, sq) ||
			IsUnderPawnAttack<Attacker>(board, sq) ||
			InUnderKingAttack<Attacker>(board, sq);
	}

	template <typename BoardType>
	template<Color Attacker>
	bool AttackDetector<BoardType>::IsUnderStraightSlidingPieceAttack(const BoardType& board, Square sq) const
	{
		using ctraits = traits::color_traits<Attacker>;
		for (Direction dir : BoardType::StraightDirections)
		{
			for (Square next{ sq + dir }; IsInside<BoardType>(next, next - dir); next += dir)
			{
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if (p == ctraits::Rook || p == ctraits::Queen)
					return true;

				break;
			}
		}
		return false;
	}

	template <typename BoardType>
	template<Color Attacker>
	bool AttackDetector<BoardType>::IsUnderDiagonallySlidingPieceAttack(const BoardType& board, Square sq) const
	{
		using ctraits = traits::color_traits<Attacker>;
		for (Direction dir : BoardType::DiagonalDirections)
		{
			for (Square next{ sq + dir }; IsInside<BoardType>(next, next - dir); next += dir)
			{
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if (p == ctraits::Bishop || p == ctraits::Queen)
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
		using ctraits = traits::color_traits<Attacker>;
		using bctraits = traits::board_color_traits<BoardType, Attacker>;
		for (int i = 0; i < 2; i++)
		{
			Square next = sq + bctraits::PawnReverseAttackDirections[i];
			if (IsInside<BoardType>(next, sq) && board.GetPiece(next) == ctraits::Pawn)
				return true;
		}
		return false;
	}

	
	/*
	template <typename BoardType>
	template <Color Attacker, Direction Dir>
	void AttackDetector<BoardType>::ComputeChecksAndPinsImpl(const BoardType& board, Square king_pos)
	{
		using ctraits = traits::color_traits<Attacker>;
		using bctraits = traits::board_color_traits<BoardType, Attacker>;

		Square pin_loc{ Empty }, next{ Empty };
		Distance dist{ 1 };
		conn::View<Dir> view{ king_pnos };
		for (int8_t i = 0; i < view.GetSize(); i++, dist++) 
		{
			next = view.Next();
			auto p = board.GetPiece(next);
			if (p == Empty)
				continue;

			bool cond{ false };
			if constexpr (Dir == ChessBoard::N || Dir == ChessBoard::E || Dir == ChessBoard::S || Dir == ChessBoard::W) 
			{
				cond = (p == ctraits::Rook || p == ctraits::Queen);
			}
			else if constexpr (Dir == ChessBoard::NE || Dir == ChessBoard::SE || Dir == ChessBoard::SW || Dir == ChessBoard::NW)
			{
				cond = (p == ctraits::Bishop || p == ctraits::Queen;
				if constexpr (Dir == bctraits::PawnReverseAttackDirections[0] || bctraits::PawnReverseAttackDirections[1])
					cond = cond || (dist == 1 && p == ctraits::Pawn);
			}
			else if constexpr (Dir == conn::KnightJumps)
			{
				cond = (p == ctraits::Knight);
			}

			if (cond) 
			{
				if (pin_loc == Empty)
					this->_checks[this->_numChecks++] = { next, Dir, dist };
				else
					this->_pins[this->_numPins++] = { pin_loc, next, Dir };
				return;
			}
			else 
			{
				if (pin_loc == Empty)
					pin_loc = next;
				else
					return;
			}
		}
	}
	*/
#pragma endregion
}