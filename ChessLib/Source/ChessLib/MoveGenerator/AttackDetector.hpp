#pragma once

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/ColorTraits.hpp>
#include <ChessLib/Chess/BoardColorTraits.hpp>
#include <ChessLib/Board/BoardFunctions.hpp>
#include <unordered_map>

namespace chesslib 
{
	template <typename BoardType>
	class AttackDetector
	{
	public:

		using BoardArrayType = typename BoardType::BoardArray;

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

		const check& GetFirstCheck() const { return _checks[0]; }

		std::uint8_t GetNumberOfChecks() const noexcept { return _numChecks; }

		bool IsPinned(Square piece_loc) const noexcept 
		{ 
			for (int i = 0; i < _numPins; i++)
				if (_pins[i].pinned == piece_loc)
					return true;
			return false;
		}

		Direction GetPinDirection(Square piece_loc) const noexcept
		{
			for (int i = 0; i < _numPins; i++)
				if (_pins[i].pinned == piece_loc)
					return _pins[i].pinner_dir;
			return Empty;
		}

		template <Color Attacker>
		void ComputeChecksAndPins(const BoardType& board, Square king_pos)
		{
			Clear();	

			// There can be only one checking knight
			if (Square knight_pos = IsUnderKnightAttack<Attacker>(board, king_pos); knight_pos != Empty)
				_checks[_numChecks++] = { knight_pos, Empty, Empty };

			ComputeSlidingPieceChecksAndPins<Attacker, true>(board, king_pos);
			ComputeSlidingPieceChecksAndPins<Attacker, false>(board, king_pos);
		}

		template<Color Attacker>
		bool IsUnderAttack(const BoardType& board, Square sq) const
		{
			return 
				IsUnderSlidingPieceAttack<Attacker, true>(board, sq) ||
				IsUnderSlidingPieceAttack<Attacker, false>(board, sq) ||
				IsUnderKnightAttack<Attacker>(board, sq) != Empty;
		}

		template<Color Attacker>
		bool CanKingMoveInThisDirection(const BoardType& board, Direction dir) const
		{
			const auto& brd = board.GetBoard();

			for (int i = 0; i < _numChecks; i++) 
			{
				if (_checks[i].checker_dir == dir && _checks[i].checker_dist != 1 ||
					_checks[i].checker_dir == direction::Reverse(dir) &&
					brd[_checks[i].checker] != traits::color_traits<Attacker>::Pawn)
					return false;
			}
			return true;
		}

	private:
		std::uint8_t _numPins;
		std::uint8_t _numChecks;
		pin		     _pins[8];
		check	     _checks[2];

		inline void Clear() noexcept
		{
			_numPins = 0;
			_numChecks = 0;
		}

		template<bool IsStraightAttackingPiece>
		inline const std::array<Direction, 4>& GetDirectionArray() const
		{
			if constexpr (IsStraightAttackingPiece)
				return BoardType::StraightDirections;
			else
				return BoardType::DiagonalDirections;
		}

		template<Color Attacker>
		Square IsUnderKnightAttack(const BoardType& board, Square sq) const
		{
			const auto& brd = board.GetBoard();
			Square next{ Empty };

			for (Direction dir : BoardType::KnightJumps)
			{
				next = sq + dir;
				if (!IsInside<BoardType>(next, sq))
					continue;

				if (brd[next] == traits::color_traits<Attacker>::Knight)
					return next;
			}

			return Empty;
		}

		template<Color Attacker, bool IsStraightAttackingPiece>
		void ComputeSlidingPieceChecksAndPins(const BoardType& board, Square king_pos)
		{
			using ctraits = traits::color_traits<Attacker>;
			using bctraits = traits::board_color_traits<BoardType, Attacker>;
		
			const auto& brd = board.GetBoard();

			for (Direction dir : GetDirectionArray<IsStraightAttackingPiece>())
			{
				Square pin_loc{ Empty };
				Distance dist{ 1 };
				for (Square next{ king_pos + dir }; IsInside<BoardType>(next, next - dir); next += dir, dist++)
				{
					if (brd[next] == Empty)
						continue;

					bool under_attack{ false };
					if constexpr (IsStraightAttackingPiece)
						under_attack = (brd[next] == ctraits::Rook ||
										brd[next] == ctraits::Queen);
					else
						under_attack = brd[next] == ctraits::Bishop ||
									   brd[next] == ctraits::Queen ||
						               dist == 1 && brd[next] == ctraits::Pawn &&
									  (dir == bctraits::PawnReverseAttackDirections[0] ||
									   dir == bctraits::PawnReverseAttackDirections[1]);

					if (under_attack)
					{
						if (pin_loc != Empty)
							_pins[_numPins++] = { pin_loc, next, dir };
						else
							_checks[_numChecks++] = { next, dir, dist };
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
	
		template<Color Attacker, bool IsStraightAttackingPiece>
		bool IsUnderSlidingPieceAttack(const BoardType& board, Square sq) const
		{
			using ctraits = traits::color_traits<Attacker>;
			using bctraits = traits::board_color_traits<BoardType, Attacker>;

			const auto& brd = board.GetBoard();
			for (Direction dir : GetDirectionArray<IsStraightAttackingPiece>())
			{
				Distance dist{ 1 };
				for (Square next{ sq + dir }; IsInside<BoardType>(next, next - dir); next += dir, dist++)
				{
					if (brd[next] == Empty)
						continue;

					if constexpr (IsStraightAttackingPiece)
					{
						if (brd[next] == ctraits::Rook || 
							brd[next] == ctraits::Queen || 
							brd[next] == ctraits::King && dist == 1)
							return true;
					}
					else
					{
						if (brd[next] == ctraits::Bishop || 
							brd[next] == ctraits::Queen ||
							dist == 1 && (brd[next] == ctraits::King || 
								(brd[next] == ctraits::Pawn && 
									(dir == bctraits::PawnReverseAttackDirections[0] ||
									 dir == bctraits::PawnReverseAttackDirections[1]))))
							return true;
					}
					break;
				}
			}

			return false;
		}
	};
}