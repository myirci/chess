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

	template void BasicBoard::GenerateKingMoves<color::White>(Square, MoveList&) const;
    template void BasicBoard::GenerateKingMoves<color::Black>(Square, MoveList&) const;

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
				_checks.emplace_back(next, direction::None, 0);
	}
	
	template<Color Clr>
	void BasicBoard::GenerateKingMoves(Square king_pos, MoveList& moves) const
	{
		using ctraits = traits::color_traits<Clr>;

		for (Direction dir : direction::All) 
		{
			if (IsDirectionInvalidForKing<ctraits::Opposite>(dir))
				continue;

			if (Square next{ king_pos + dir }; IsInside(king_pos, next) && board[next] && !IsUnderAttack<ctraits::Opposite>(next))
			{
				if (board[next] == squareset::Empty)
					moves.emplace_back(king_pos, next, MoveType::Quite);
				else if(color::get_color(board[next]) != Clr)
					moves.emplace_back(king_pos, next, MoveType::Capture, board[next]);
			}
		}

		if (_checks.size() > 0) 
			return;

		using bctraits = traits::board_color_traits<BasicBoard, Clr>;

		if(IsKingSideCastlingPossible<Clr>())
			moves.emplace_back(king_pos, bctraits::KingSideCastleCheckSquares[1], MoveType::King_Side_Castle);

		if (IsQueenSideCastlingPossible<Clr>())
			moves.emplace_back(king_pos, bctraits::QueenSideCastleCheckSquares[1], MoveType::Queen_Side_Castle);
	}

	template<Color Clr>
	void BasicBoard::ToSquareNonKingMoves(Square sq, MoveList& moves) const
	{
		if(color::get_color(board[sq] == Clr))
			throw std::logic_error("Illegal move generation request.");

		MoveType move_type{ MoveType::Quite };
		Piece captured_piece{ pieceset::None };
		if (board[sq] != squareset::Empty)
		{
			captured_piece = board[sq];
			move_type = MoveType::Capture;
		}

		using ctraits = traits::color_traits<Clr>;
		using octraits = traits::color_traits<color::get_opposite_color(Clr)>;

		for (Direction dir : direction::Straight)
		{
			for (Square next{ sq + dir }; IsInside(next - dir, next); next += dir)
			{
				if (board[next] == squareset::Empty)
					continue;

				if (color::get_color(board[next]) == Clr &&
					(board[next] == ctraits::Rook || board[next] == ctraits::Queen) &&
					!IsPiecePinned(next))
					moves.emplace(next, sq, move_type, captured_piece);
				break;
			}
		}
	}

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
						_checks.emplace_back(next, dir, dist);
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
	bool BasicBoard::IsNonKingStraightAttack(Piece p) const
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
	bool BasicBoard::IsNonKingDiagonalAttack(Piece p, Direction dir, Distance dist) const
	{
		using ctraits = traits::color_traits<Attacker>;
		using bptraits = traits::board_piece_traits<BasicBoard, ctraits::Pawn>;

		return
			color::get_color(p) == Attacker &&
			(p == ctraits::Bishop || p == ctraits::Queen || dist == 1 && p == ctraits::Pawn &&
				(dir == direction::Reverse(bptraits::AttackDirections[0]) || 
			     dir == direction::Reverse(bptraits::AttackDirections[1])));
	}

	template<Color Attacker>
	bool BasicBoard::IsDirectionInvalidForKing(Direction dir) const
	{
		using ctraits = traits::color_traits<Attacker>;

		return std::any_of(_checks.begin(), _checks.end(),
			[dir, this](const auto& check)
			{
				return
					std::get<1>(check) == dir && std::get<2>(check) != 1 ||
					std::get<1>(check) == direction::Reverse(dir) &&
					board[std::get<0>(check)] != ctraits::Pawn;
			});
	}

	template<Color Clr>
	bool BasicBoard::IsKingSideCastlingPossible() const
	{
		using bctraits = traits::board_color_traits<BasicBoard, Clr>;
		using ctraits = traits::color_traits<Clr>;

		return
			QueryCastling(ctraits::KingSideCastling) &&
			board[bctraits::KingSideCastleCheckSquares[0]] == squareset::Empty &&
			board[bctraits::KingSideCastleCheckSquares[1]] == squareset::Empty &&
			!IsUnderAttack<ctraits::Opposite>(bctraits::KingSideCastleCheckSquares[0]) &&
			!IsUnderAttack<ctraits::Opposite>(bctraits::KingSideCastleCheckSquares[1]);
	}

	template<Color Clr>
	bool BasicBoard::IsQueenSideCastlingPossible() const
	{
		using bctraits = traits::board_color_traits<BasicBoard, Clr>;
		using ctraits = traits::color_traits<Clr>;

		return
			QueryCastling(ctraits::QueenSideCastling) &&
			board[bctraits::QueenSideCastleCheckSquares[0]] == squareset::Empty &&
			board[bctraits::QueenSideCastleCheckSquares[1]] == squareset::Empty &&
			board[bctraits::QueenSideCastleCheckSquares[2]] == squareset::Empty &&
			!IsUnderAttack<ctraits::Opposite>(bctraits::QueenSideCastleCheckSquares[0]) &&
			!IsUnderAttack<ctraits::Opposite>(bctraits::QueenSideCastleCheckSquares[1]);
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