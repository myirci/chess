#include <ChessLib/BasicBoard/BasicBoard.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

#include <stdexcept>

namespace chesslib::basic_board
{
	// Explicit initializations
	template bool BasicBoard::IsUnderAttack<color::White>(Square) const;
	template bool BasicBoard::IsUnderAttack<color::Black>(Square) const;

	template void BasicBoard::ComputeChecksAndPins<color::White>(Square);
	template void BasicBoard::ComputeChecksAndPins<color::Black>(Square);

	BasicBoard::BasicBoard() : board{} { }

	const BasicBoard::BoardArray& BasicBoard::GetBoard() const { return board; }

	BasicBoard::BoardArray& BasicBoard::GetBoard() { return board; }

	template<Color Attacker>
	bool BasicBoard::IsUnderAttack(Square sq) const
	{	
		return 
			IsUnderAttack<Attacker, true>(sq, direction::Straight) || 
			IsUnderAttack<Attacker, false>(sq, direction::Diagonal) ||
			IsUnderAttackByAKnight<Attacker>(sq);
	}

	template<Color Attacker>
	void BasicBoard::ComputeChecksAndPins(Square king_pos)
	{
		BoardBase::ClearChecksAndPins();

		ComputeChecksAndPins<Attacker, true>(king_pos, direction::Straight);
		ComputeChecksAndPins<Attacker, false>(king_pos, direction::Diagonal);

		using ctraits = traits::color_traits<Attacker>;
		for (Direction dir : direction::KnightJumps)
			if (Square next{ king_pos + dir }; IsInside(king_pos, next) && board[next] == ctraits::Knight)
				_checks.emplace_back(next, direction::None);
	}
	
	/*
	template<Color SideToMove>
	void BasicBoard::ToSquareNonKingMoves(Square sq, MoveList& moves) const
	{
		if(color::get_color(board[sq] == SideToMove))
			throw std::logic_error("Illegal move generation request.");

		MoveType move_type{ MoveType::Quite };
		Piece captured_piece{ pieceset::None };
		if (board[sq] != squareset::Empty)
		{
			captured_piece = board[sq];
			move_type = MoveType::Capture;
		}

		using stm_traits = traits::color_traits<SideToMove>;
		using opp_traits = traits::color_traits<color::get_opposite_color(SideToMove)>;

		for (Direction dir : direction::Straight)
		{
			for (Square next{ sq + dir }; IsInside(next - dir, next); next += dir, dist++)
			{
				if (board[next] == squareset::Empty)
					continue;
				else if (color::get_color(board[next]) == SideToMove)
				{
					if (board[next] == stm_traits::Rook || board[next] == stm_traits::Queen)
						return true;
					break;
				}
				else
					break;
			}
		}
	}
	*/

	bool BasicBoard::IsInside(Square curr, Square next)
	{
		return next < 64 && next >= 0 && std::abs(get_file(next) - get_file(curr)) <= 2;
	}
	
	template<Color Attacker, bool IsStraightMovingPiece>
	void BasicBoard::ComputeChecksAndPins(Square king_pos, const std::array<Direction, 4>& attack_directions)
	{
		for (Direction dir : attack_directions)
		{
			Square pin_loc{ squareset::None };
			Distance dist{ 1 };
			for (Square next{ king_pos + dir }; IsInside(next - dir, next); next += dir, dist++)
			{
				if (board[next] == squareset::Empty)
					continue;

				bool is_non_king_attack{ false };
				if constexpr (IsStraightMovingPiece)
					is_non_king_attack = IsNonKingStraightAttack<Attacker>(board[next]);
				else
					is_non_king_attack = IsNonKingDiagonalAttack<Attacker>(board[next], dir, dist);
				
				if (is_non_king_attack)
				{
					if (pin_loc != squareset::None)
						_pins.emplace(pin_loc, std::make_pair(next, dir));
					else
						_checks.emplace_back(next, dir);
					break;
				}
				else
				{
					if (pin_loc == squareset::None)
						pin_loc = next;
					else
						break;
				}
			}
		}
	}

	template<Color Attacker, bool IsStraightMovingPiece>
	bool BasicBoard::IsUnderAttack(Square sq, const std::array<Direction, 4>& attack_directions) const
	{
		for (Direction dir : attack_directions) 
		{
			Distance dist{ 1 };
			for (Square next{ sq + dir }; IsInside(next - dir, next); next += dir, dist++)
			{
				if (board[next] == squareset::Empty)
					continue;

				if constexpr (IsStraightMovingPiece) 
				{
					if (IsStraightAttack<Attacker>(board[next], dist))
						return true;
				}
				else 
				{
					if (IsDiagonalAttack<Attacker>(board[next], dir, dist))
						return true;
				}

				break;
			}
		}

		return false;
	}

	template<Color Attacker>
	bool BasicBoard::IsUnderAttackByAKnight(Square sq) const 
	{
		using ctraits = traits::color_traits<Attacker>;
		
		for (Direction dir : direction::KnightJumps)
			if (Square next{ sq + dir }; IsInside(sq, next) && board[next] == ctraits::Knight)
				return true;
		
		return false;
	}

	template<Color Attacker>
	bool BasicBoard::IsStraightAttack(Piece p, Distance dist) const
	{
		using ctraits = traits::color_traits<Attacker>;
		return color::get_color(p) == Attacker && 
			(p == ctraits::Rook || p == ctraits::Queen || p == ctraits::King && dist == 1);
	}

	template<Color Attacker>
	inline bool BasicBoard::IsNonKingStraightAttack(Piece p) const
	{
		using ctraits = traits::color_traits<Attacker>;
		return color::get_color(p) == Attacker && (p == ctraits::Rook || p == ctraits::Queen);
	}

	template<Color Attacker>
	bool BasicBoard::IsDiagonalAttack(Piece p, Direction dir, Distance dist) const
	{
		using ctraits = traits::color_traits<Attacker>;
		using bptraits = traits::board_piece_traits<BasicBoard, ctraits::Pawn>;

		return color::get_color(p) == Attacker && (p == ctraits::Bishop || p == ctraits::Queen ||
			dist == 1 && (p == ctraits::King || (p == ctraits::Pawn &&
				(dir == direction::Reverse(bptraits::AttackDirections[0]) ||
				 dir == direction::Reverse(bptraits::AttackDirections[1])))));
	}

	template<Color Attacker>
	inline bool BasicBoard::IsNonKingDiagonalAttack(Piece p, Direction dir, Distance dist) const
	{
		using ctraits = traits::color_traits<Attacker>;
		using bptraits = traits::board_piece_traits<BasicBoard, ctraits::Pawn>;

		return
			color::get_color(p) == Attacker &&
			(p == ctraits::Bishop || p == ctraits::Queen || dist == 1 && p == ctraits::Pawn &&
				(dir == direction::Reverse(bptraits::AttackDirections[0]) || 
			     dir == direction::Reverse(bptraits::AttackDirections[1])));
	}

	std::unique_ptr<BasicBoard> make_unique_board(std::string_view fen) 
	{
		auto brd = std::unique_ptr<BasicBoard>(new BasicBoard());
		utility::chess::set_board(*brd, fen);
		return brd;
	}
	
	std::shared_ptr<BasicBoard> make_shared_board(std::string_view fen) 
	{
		auto brd = std::shared_ptr<BasicBoard>(new BasicBoard());
		utility::chess::set_board(*brd, fen);
		return brd;
	}
}