#pragma once

#include <ChessLib/Chess/Definitions.hpp>

#include <unordered_map>

namespace chesslib 
{
	template <typename BoardType>
	class ChecksAndPins
	{
	public:

		// key			: pinned piece location
		// value.first	: attacker location
		// value.second : direction from pinned piece to the attacker
		using Pins = std::unordered_map<Square, std::pair<Square, Direction>>;

		// 0: attacker location
		// 1: direction from king location to the attacker
		// 2: distance from king location to attacker (dist >= 1, number of moves for king to reach the attacker)
		using Checks = std::vector<std::tuple<Square, Direction, Distance>>;

		bool IsPinned(Square piece_loc) const noexcept { return _pins.contains(piece_loc); }

		Direction GetPinDirection(Square piece_loc) const noexcept
		{
			auto it = _pins.find(piece_loc);
			return it == _pins.end() ? Empty : it->second.second;
		}

		template <Color Attacker>
		void Compute(const BoardType& board, Square king_pos)
		{
			// get the board array.
			const auto& _board = board.GetBoard();

			// clear the data structure.
			Clear();

			Compute<Attacker, true>(king_pos, btraits::StraightDirections);
			Compute<Attacker, false>(king_pos, btraits::DiagonalDirections);

			for (Direction dir : btraits::KnightJumps) 
			{
				Square next{ king_pos + dir };
				if constexpr (std::is_same<BoardType, basic_board::BasicBoard>) 
				{
					if (!basic_board::BasicBoard::IsInside(king_pos, next)
						continue;
				}
				else
				{
					if (!BoardType::IsInside(next))
						continue;
				}

				if (_board[next] == ctraits::Knight)
					_checks.emplace_back(next, Empty, 0);
			}	
		}

	private:

		Pins	_pins;
		Checks	_checks;
		
		void Clear() noexcept
		{
			_pins.clear();
			_checks.clear();
		}

		template<Color Attacker, bool IsStraightMovingPiece>
		bool IsNonKingAttacker(Square sq) 
		{
			if constexpr (IsStraightMovingPiece)
				return _board[sq] == ctraits::Rook || _board[sq] == ctraits::Queen;
			else
				return _board[sq] == ctraits::Bishop || _board[sq] == ctraits::Queen ||
				       dist == 1 && _board[next] == ctraits::Pawn &&
				       (dir == bptraits::ReverseAttackDirections[0] || 
						dir == bptraits::ReverseAttackDirections[1]);
		}

		template<Color Attacker, bool IsStraightMovingPiece>
		void Compute(Square king_pos, const std::array<Direction, 4>& attack_directions)
		{
			// using ctraits = traits::color_traits<Attacker>;
			// using bptraits = traits::board_piece_traits<BoardType, ctraits::Pawn>;

			for (Direction dir : attack_directions)
			{
				Square pin_loc{ Empty };
				Distance dist{ 1 };

				Square next{ king_pos + dir };
				while (true) 
				{
					if constexpr (std::is_same<BoardType, basic_board::BasicBoard>)
					{
						if (!basic_board::BasicBoard::IsInside(next - dir, next)
							break;
					}
					else
					{
						if (!BoardType::IsInside(next))
							break;
					}

					bool is_under_attack{ false }; // excluding king attacks
					
					if constexpr (IsStraightMovingPiece)
						is_under_attack = _board[next] == ctraits::Rook || 
						                  _board[next] == ctraits::Queen;
					else
						is_under_attack = _board[next] == ctraits::Bishop || 
										  _board[next] == ctraits::Queen ||
										  dist == 1 && _board[next] == ctraits::Pawn &&
										  (dir == bptraits::ReverseAttackDirections[0] || 
										   dir == bptraits::ReverseAttackDirections[1]);

					if (is_under_attack)
					{
						if (pin_loc != Empty)
							_pins.emplace(pin_loc, std::make_pair(next, dir));
						else
							_checks.emplace_back(next, dir, dist);
						break;
					}
					else
					{
						if (pin_loc == Empty)
							pin_loc = next;
						else
							break;
					}

					next += dir;
					dist++;
				}
			}
		}
	};
}