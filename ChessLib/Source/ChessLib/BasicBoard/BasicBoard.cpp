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
			IsUnderAttackByAStraightMovingPiece<Attacker>(sq) ||
			IsUnderAttackByADiagonallyMovingPiece<Attacker>(sq) ||
			IsUnderAttackByAKnight<Attacker>(sq);
	}

	template<Color Attacker>
	void BasicBoard::ComputeChecksAndPins(Square king_pos)
	{
		using ctraits = traits::color_traits<Attacker>;
		using bptraits = traits::board_piece_traits<BasicBoard, ctraits::Pawn>;

		BoardBase::ClearChecksAndPins();

		for (Direction dir : direction::Straight)
		{
			Square pin_loc{ squareset::None };
			for (Square next{ king_pos + dir }; IsInside(next - dir, next); next += dir)
			{
				if (board[next] == squareset::Empty)
					continue;
				else if (color::get_color(board[next]) == Attacker && (board[next] == ctraits::Rook || board[next] == ctraits::Queen))
				{
					if (pin_loc != squareset::None)
						_pins.emplace(pin_loc, std::make_pair(next, dir));
					else
						_checks.emplace(next, dir);
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

		for (Direction dir : direction::Diagonal)
		{
			Distance dist{ 1 };
			Square pin_loc{ squareset::None };
			for (Square next{ king_pos + dir }; IsInside(next - dir, next); next += dir, dist++)
			{
				if (board[next] == squareset::Empty)
					continue;
				else if (color::get_color(board[next]) == Attacker)
				{
					if (board[next] == ctraits::Bishop ||
						board[next] == ctraits::Queen ||
						dist == 1 &&
						(board[next] == ctraits::King ||
							(board[next] == ctraits::Pawn &&
								(dir == direction::Reverse(bptraits::AttackDirections[0]) ||
									dir == direction::Reverse(bptraits::AttackDirections[1]))))) 
					{
						if (pin_loc != squareset::None)
							_pins.emplace(pin_loc, std::make_pair(next, dir));
						else
							_checks.emplace(next, dir);
					}
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

		for (Direction dir : direction::KnightJumps)
			if (Square next{ king_pos + dir }; IsInside(king_pos, next) && board[next] == ctraits::Knight)
				_checks.emplace(next, direction::None);
	}

	/*
	template<Color SideToMove>
	void BasicBoard::ToSquareMoves(Square sq, MoveList& moves) const
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
			Distance dist{ 1 };
			for (Square next{ sq + dir }; IsInside(next - dir, next); next += dir, dist++)
			{
				if (board[next] == squareset::Empty)
					continue;
				else if (color::get_color(board[next]) == SideToMove)
				{
					if (board[next] == ctraits::Rook || board[next] == ctraits::Queen || board[next] == ctraits::King && dist == 1)
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

	template<Color Attacker>
	bool BasicBoard::IsUnderAttackByAStraightMovingPiece(Square sq) const 
	{
		using ctraits = traits::color_traits<Attacker>;

		for (Direction dir : direction::Straight)
		{
			Distance dist{ 1 };
			for (Square next{ sq + dir }; IsInside(next - dir, next); next += dir, dist++)
			{
				if (board[next] == squareset::Empty)
					continue;
				else if (color::get_color(board[next]) == Attacker)
				{
					if (board[next] == ctraits::Rook || board[next] == ctraits::Queen || board[next] == ctraits::King && dist == 1)
						return true;
					break;
				}
				else
					break;
			}
		}

		return false;
	}

	template<Color Attacker>
	bool BasicBoard::IsUnderAttackByADiagonallyMovingPiece(Square sq) const 
	{
		using ctraits = traits::color_traits<Attacker>;
		using bptraits = traits::board_piece_traits<BasicBoard, ctraits::Pawn>;

		for (Direction dir : direction::Diagonal)
		{
			Distance dist{ 1 };
			for (Square next{ sq + dir }; IsInside(next - dir, next); next += dir, dist++)
			{
				if (board[next] == squareset::Empty)
					continue;
				else if (color::get_color(board[next]) == Attacker)
				{
					if (board[next] == ctraits::Bishop || 
						board[next] == ctraits::Queen ||
						dist == 1 && 
						(board[next] == ctraits::King || 
						(board[next] == ctraits::Pawn && 
							(dir == direction::Reverse(bptraits::AttackDirections[0]) || 
							 dir == direction::Reverse(bptraits::AttackDirections[1])))))
						return true;
					break;
				}
				else
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