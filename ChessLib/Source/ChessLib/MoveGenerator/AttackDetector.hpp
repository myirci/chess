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
		Square IsUnderKnightAttack(const BoardType& board, Square sq) const;

		template<Color Attacker>
		bool IsUnderStraightAttack(const BoardType& board, Square sq) const;

		template<Color Attacker>
		bool IsUnderDiagonalAttack(const BoardType& board, Square sq) const;

		template<Color Attacker>
		void ComputeStraightChecksAndPins(const BoardType& board, Square king_pos);

		template<Color Attacker>
		void ComputeDiagonalChecksAndPins(const BoardType& board, Square king_pos);
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

		if (Square knight_pos = IsUnderKnightAttack<Attacker>(board, king_pos); knight_pos != Empty)
			this->_checks[this->_numChecks++] = { knight_pos, Empty, Empty };

		ComputeStraightChecksAndPins<Attacker>(board, king_pos);
		ComputeDiagonalChecksAndPins<Attacker>(board, king_pos);
	}

	template <typename BoardType>
	template<Color Attacker>
	bool AttackDetector<BoardType>::IsUnderAttack(const BoardType& board, Square sq) const
	{
		return
			IsUnderStraightAttack<Attacker>(board, sq) ||
			IsUnderDiagonalAttack<Attacker>(board, sq) ||
			IsUnderKnightAttack<Attacker>(board, sq) != Empty;
	}

	template <typename BoardType>
	template<Color Attacker>
	Square AttackDetector<BoardType>::IsUnderKnightAttack(const BoardType& board, Square sq) const
	{
		Square next{ Empty };
		for (Direction dir : BoardType::KnightJumps)
		{
			next = sq + dir;
			if (IsInside<BoardType>(next, sq) && 
				board.GetPiece(next) == traits::color_traits<Attacker>::Knight)
				return next;
		}

		return Empty;
	}

	template <typename BoardType>
	template<Color Attacker>
	bool AttackDetector<BoardType>::IsUnderStraightAttack(const BoardType& board, Square sq) const
	{
		using ctraits = traits::color_traits<Attacker>;

		for (Direction dir : BoardType::StraightDirections)
		{
			Distance dist{ 1 };
			for (Square next{ sq + dir }; IsInside<BoardType>(next, next - dir); next += dir, dist++) 
			{
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if (p == ctraits::Rook || p == ctraits::Queen || p == ctraits::King && dist == 1)
					return true;

				break;
			}	
		}

		return false;
	}

	template <typename BoardType>
	template<Color Attacker>
	bool AttackDetector<BoardType>::IsUnderDiagonalAttack(const BoardType& board, Square sq) const
	{
		using ctraits = traits::color_traits<Attacker>;
		using bctraits = traits::board_color_traits<BoardType, Attacker>;

		for (Direction dir : BoardType::DiagonalDirections)
		{
			Distance dist{ 1 };
			for (Square next{ sq + dir }; IsInside<BoardType>(next, next - dir); next += dir, dist++)
			{
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if (p == ctraits::Bishop || p == ctraits::Queen || dist == 1 && 
					(p == ctraits::King || (p == ctraits::Pawn && (dir == bctraits::PawnReverseAttackDirections[0] || dir == bctraits::PawnReverseAttackDirections[1]))))
					return true;
				
				break;
			}
		}

		return false;
	}

	template <typename BoardType>
	template<Color Attacker>
	void AttackDetector<BoardType>::ComputeStraightChecksAndPins(const BoardType& board, Square king_pos)
	{
		using ctraits = traits::color_traits<Attacker>;
		
		for (Direction dir : BoardType::StraightDirections)
		{
			Square pin_loc{ Empty };
			Distance dist{ 1 };
			for (Square next{ king_pos + dir }; IsInside<BoardType>(next, next - dir); next += dir, dist++)
			{
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if (p == ctraits::Rook || p == ctraits::Queen)
				{
					if (pin_loc != Empty)
						this->_pins[this->_numPins++] = { pin_loc, next, dir };
					else
						this->_checks[this->_numChecks++] = { next, dir, dist };
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
	void AttackDetector<BoardType>::ComputeDiagonalChecksAndPins(const BoardType& board, Square king_pos)
	{
		using ctraits = traits::color_traits<Attacker>;
		using bctraits = traits::board_color_traits<BoardType, Attacker>;

		for (Direction dir : BoardType::DiagonalDirections)
		{
			Square pin_loc{ Empty };
			Distance dist{ 1 };
			for (Square next{ king_pos + dir }; IsInside<BoardType>(next, next - dir); next += dir, dist++)
			{
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if (p == ctraits::Bishop || p == ctraits::Queen ||
					dist == 1 && p == ctraits::Pawn && (dir == bctraits::PawnReverseAttackDirections[0] || dir == bctraits::PawnReverseAttackDirections[1]))
				{
					if (pin_loc != Empty)
						this->_pins[this->_numPins++] = { pin_loc, next, dir };
					else
						this->_checks[this->_numChecks++] = { next, dir, dist };
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
#pragma endregion

#pragma region AttackDetector_ObjBoardSpecializations

	template <>
	template<Color Attacker>
	Square AttackDetector<objboard::ObjBoard>::IsUnderKnightAttack(const objboard::ObjBoard& board, Square sq) const
	{
		auto idx = Connections::KnightSquareIndexes[sq];
		auto num = Connections::KnightConnections[idx];
		for (int i = 0; i < num; i++) 
			if (board.GetPiece(Connections::KnightConnections[++idx]) == traits::color_traits<Attacker>::Knight)
				return Connections::KnightConnections[idx];
		return Empty;
	}

	template <>
	template<Color Attacker>
	bool AttackDetector<objboard::ObjBoard>::IsUnderStraightAttack(const objboard::ObjBoard& board, Square sq) const
	{
		using ctraits = traits::color_traits<Attacker>;

		auto dir_idx = Connections::StraightSquareIndexes[sq];
		auto idx = dir_idx + 4;
		for (Direction dir : ChessBoard::StraightDirections)
		{
			Distance dist{ 1 };
			for (int i = 0; i < Connections::StraightConnections[dir_idx]; i++, dist++)
			{
				auto p = board.GetPiece(Connections::StraightConnections[idx++]);
				if (p == Empty)
					continue;

				if (p == ctraits::Rook || p == ctraits::Queen || p == ctraits::King && dist == 1)
					return true;

				break;
			}
			dir_idx++;
		}

		return false;
	}

	template <>
	template<Color Attacker>
	bool AttackDetector<objboard::ObjBoard>::IsUnderDiagonalAttack(const objboard::ObjBoard& board, Square sq) const
	{
		using ctraits = traits::color_traits<Attacker>;
		using bctraits = traits::board_color_traits<objboard::ObjBoard, Attacker>;

		auto dir_idx = Connections::DiagonalSquareIndexes[sq];
		auto idx = dir_idx + 4;

		for (Direction dir : objboard::ObjBoard::DiagonalDirections)
		{
			Distance dist{ 1 };
			for (int i = 0; i < Connections::DiagonalConnections[dir_idx]; i++, dist++)
			{
				auto p = board.GetPiece(Connections::DiagonalConnections[idx++]);
				if (p == Empty)
					continue;

				if (p == ctraits::Bishop || p == ctraits::Queen || dist == 1 &&
				   (p == ctraits::King || (p == ctraits::Pawn && (dir == bctraits::PawnReverseAttackDirections[0] || dir == bctraits::PawnReverseAttackDirections[1]))))
					return true;

				break;
			}
			dir_idx++;
		}

		return false;
	}

	template <>
	template<Color Attacker>
	void AttackDetector<objboard::ObjBoard>::ComputeStraightChecksAndPins(const objboard::ObjBoard& board, Square king_pos)
	{
		using ctraits = traits::color_traits<Attacker>;

		auto dir_idx = Connections::StraightSquareIndexes[king_pos];
		auto idx = dir_idx + 4;

		for (Direction dir : objboard::ObjBoard::StraightDirections)
		{
			Square pin_loc{ Empty };
			Distance dist{ 1 };

			for (int i = 0; i < Connections::StraightConnections[dir_idx]; i++, dist++)
			{
				auto next = Connections::StraightConnections[idx++];
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if (p == ctraits::Rook || p == ctraits::Queen)
				{
					if (pin_loc != Empty)
						this->_pins[this->_numPins++] = { pin_loc, next, dir };
					else
						this->_checks[this->_numChecks++] = { next, dir, dist };
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

	template <>
	template<Color Attacker>
	void AttackDetector<objboard::ObjBoard>::ComputeDiagonalChecksAndPins(const objboard::ObjBoard& board, Square king_pos)
	{
		using ctraits = traits::color_traits<Attacker>;
		using bctraits = traits::board_color_traits<objboard::ObjBoard, Attacker>;

		auto dir_idx = Connections::StraightSquareIndexes[king_pos];
		auto idx = dir_idx + 4;

		for (Direction dir : objboard::ObjBoard::DiagonalDirections)
		{
			Square pin_loc{ Empty };
			Distance dist{ 1 };
			for (int i = 0; i < Connections::DiagonalConnections[dir_idx]; i++, dist++)
			{
				auto next = Connections::DiagonalConnections[idx++];
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if (p == ctraits::Bishop || p == ctraits::Queen ||
					dist == 1 && p == ctraits::Pawn && (dir == bctraits::PawnReverseAttackDirections[0] || dir == bctraits::PawnReverseAttackDirections[1]))
				{
					if (pin_loc != Empty)
						this->_pins[this->_numPins++] = { pin_loc, next, dir };
					else
						this->_checks[this->_numChecks++] = { next, dir, dist };
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
#pragma endregion
}