#include <ChessLib/Board/x88Board.hpp>

namespace chesslib
{
	/*
	template<Color Clr>
	void x88Board::GenerateMovesImplementation(MoveList& moves) const
	{
		using ctraits = traits::color_traits<Clr>;

		auto king_pos{ GetKingPosition<Clr>() };
		ComputeChecksAndPins<ctraits::Opposite>(king_pos);

		GenerateKingMoves<Clr>(king_pos, moves);

		auto num_checks{ _checks.size() };
		if (num_checks == 0)
		{
			GenerateStraightMoves<Clr>(moves);
			GenerateDiagonalMoves<Clr>(moves);
			GenerateKnightMoves<Clr>(moves);
			GeneratePawnMoves<Clr>(moves);
			GenerateEnPassantCaptureMoves<Clr>(king_pos, moves);
		}
		else if (num_checks == 1)
		{
			GenerateCheckEliminatingMoves<Clr>(king_pos, moves);
		}
		else if (num_checks != 2)
			throw std::logic_error("Number of checks can not be different than 0, 1, 2.");
	}

	template<Color Attacker>
	void x88Board::ComputeChecksAndPins(Square king_pos) const
	{
		ClearChecksAndPins();

		ComputeChecksAndPins<Attacker, true>(king_pos, direction::Straight);
		ComputeChecksAndPins<Attacker, false>(king_pos, direction::Diagonal);

		using ctraits = traits::color_traits<Attacker>;
		for (Direction dir : direction::KnightJumps)
			if (Square next{ king_pos + dir }; IsInside(next) && _board[next] == ctraits::Knight)
				_checks.emplace_back(next, Empty, 0);
	}

	template<Color Attacker, bool IsStraightMovingPiece>
	void x88Board::ComputeChecksAndPins(Square king_pos, const std::array<Direction, 4>& attack_directions) const
	{
		using ctraits = traits::color_traits<Attacker>;
		using bptraits = traits::board_piece_traits<x88Board, ctraits::Pawn>;

		for (Direction dir : attack_directions)
		{
			Square pin_loc{ Empty };
			Distance dist{ 1 };
			for (Square next{ king_pos + dir }; IsInside(next); next += dir, dist++)
			{
				if (_board[next] == Empty)
					continue;

				bool is_non_king_attack{ false };
				if constexpr (IsStraightMovingPiece)
					is_non_king_attack = (_board[next] == ctraits::Rook || _board[next] == ctraits::Queen);
				else
					is_non_king_attack = _board[next] == ctraits::Bishop || _board[next] == ctraits::Queen ||
					dist == 1 && _board[next] == ctraits::Pawn &&
					(dir == bptraits::ReverseAttackDirections[0] || dir == bptraits::ReverseAttackDirections[1]);

				if (is_non_king_attack)
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
			}
		}
	}

	template<Color Clr>
	void x88Board::GenerateKingMoves(Square king_pos, MoveList& moves) const
	{
		using ctraits = traits::color_traits<Clr>;

		for (Direction dir : direction::All)
		{
			if (IsDirectionInvalidForKing<ctraits::Opposite>(dir))
				continue;

			if (Square next{ king_pos + dir };
				IsInside(next) && _board[next] && !IsUnderAttack<ctraits::Opposite>(next))
			{
				if (_board[next] == Empty)
					moves.emplace_back(king_pos, next);
				else if (color::get_color(_board[next]) != Clr)
					moves.emplace_back(king_pos, next, MoveType::Capture);
			}
		}

		if (_checks.size() > 0)
			return;

		using bctraits = traits::board_color_traits<x88Board, Clr>;

		if (IsKingSideCastlingPossible<Clr>())
			moves.emplace_back(king_pos, bctraits::KingSideCastleCheckSquares[1], MoveType::King_Side_Castle);

		if (IsQueenSideCastlingPossible<Clr>())
			moves.emplace_back(king_pos, bctraits::QueenSideCastleCheckSquares[1], MoveType::Queen_Side_Castle);
	}

	template<Color Attacker>
	bool x88Board::IsDirectionInvalidForKing(Direction dir) const
	{
		using ctraits = traits::color_traits<Attacker>;

		return std::any_of(_checks.begin(), _checks.end(),
			[dir, this](const auto& check)
			{
				return
					std::get<1>(check) == dir && std::get<2>(check) != 1 ||
					std::get<1>(check) == direction::Reverse(dir) &&
					_board[std::get<0>(check)] != ctraits::Pawn;
			});
	}

	template<Color Attacker>
	bool x88Board::IsUnderAttack(Square sq) const
	{
		return
			IsUnderAttack<Attacker, true>(sq, direction::Straight) ||
			IsUnderAttack<Attacker, false>(sq, direction::Diagonal) ||
			IsUnderAttackByAKnight<Attacker>(sq);
	}

	template<Color Attacker, bool IsStraightMovingPiece>
	bool x88Board::IsUnderAttack(Square sq, const std::array<Direction, 4>& attack_directions) const
	{
		using ctraits = traits::color_traits<Attacker>;
		using bptraits = traits::board_piece_traits<x88Board, ctraits::Pawn>;

		for (Direction dir : attack_directions)
		{
			Distance dist{ 1 };
			for (Square next{ sq + dir }; IsInside(next); next += dir, dist++)
			{
				if (_board[next] == Empty)
					continue;

				if constexpr (IsStraightMovingPiece)
				{
					if (_board[next] == ctraits::Rook ||
						_board[next] == ctraits::Queen ||
						_board[next] == ctraits::King && dist == 1)
						return true;
				}
				else
				{
					if (_board[next] == ctraits::Bishop || _board[next] == ctraits::Queen ||
						dist == 1 && (_board[next] == ctraits::King || (_board[next] == ctraits::Pawn &&
							(dir == bptraits::ReverseAttackDirections[0] || dir == bptraits::ReverseAttackDirections[1]))))
						return true;
				}

				break;
			}
		}

		return false;
	}

	template<Color Attacker>
	bool x88Board::IsUnderAttackByAKnight(Square sq) const
	{
		using ctraits = traits::color_traits<Attacker>;

		for (Direction dir : direction::KnightJumps)
			if (Square next{ sq + dir }; IsInside(next) && _board[next] == ctraits::Knight)
				return true;

		return false;
	}

	template<Color Clr>
	bool x88Board::IsKingSideCastlingPossible() const
	{
		using bctraits = traits::board_color_traits<x88Board, Clr>;
		using ctraits = traits::color_traits<Clr>;

		return
			QueryCastling(ctraits::KingSideCastling) &&
			_board[bctraits::KingSideCastleCheckSquares[0]] == Empty &&
			_board[bctraits::KingSideCastleCheckSquares[1]] == Empty &&
			!IsUnderAttack<ctraits::Opposite>(bctraits::KingSideCastleCheckSquares[0]) &&
			!IsUnderAttack<ctraits::Opposite>(bctraits::KingSideCastleCheckSquares[1]);
	}

	template<Color Clr>
	bool x88Board::IsQueenSideCastlingPossible() const
	{
		using bctraits = traits::board_color_traits<x88Board, Clr>;
		using ctraits = traits::color_traits<Clr>;

		return
			QueryCastling(ctraits::QueenSideCastling) &&
			_board[bctraits::QueenSideCastleCheckSquares[0]] == Empty &&
			_board[bctraits::QueenSideCastleCheckSquares[1]] == Empty &&
			_board[bctraits::QueenSideCastleCheckSquares[2]] == Empty &&
			!IsUnderAttack<ctraits::Opposite>(bctraits::QueenSideCastleCheckSquares[0]) &&
			!IsUnderAttack<ctraits::Opposite>(bctraits::QueenSideCastleCheckSquares[1]);
	}

	template<Color Clr>
	void x88Board::GenerateCheckEliminatingMoves(Square king_pos, MoveList& moves) const
	{
		auto [attacker_loc, king_to_square_dir, dist] = _checks[0];

		GenerateCheckEliminatingEnPassantCaptureMoves<Clr>(attacker_loc, moves);

		if (king_to_square_dir != Empty)
		{
			Square next{ king_pos + king_to_square_dir };
			for (Index i{ 0 }; i < dist; i++, next += king_to_square_dir)
				ToSquareMoves<Clr>(next, moves);
		}
		else
			ToSquareMoves<Clr>(attacker_loc, moves);
	}

	template<Color Clr>
	void x88Board::GenerateCheckEliminatingEnPassantCaptureMoves(Square attacker_loc, MoveList& moves) const
	{
		using ctraits = traits::color_traits<Clr>;
		using bptraits = traits::board_piece_traits<x88Board, ctraits::Pawn>;

		if (_enpassant_target == Empty || _board[attacker_loc] != bptraits::Opposite)
			return;

		Direction dirs[2] = { -1, 1 };
		for (auto i{ 0 }; i < 2; i++)
		{
			Square next{ attacker_loc + dirs[i] };
			if (auto pin_dir = GetPinDirection(next);
				pin_dir == Empty &&
				_board[next] == ctraits::Pawn &&
				IsInside(_enpassant_target))
				moves.emplace_back(next, _enpassant_target, MoveType::En_Passant_Capture);
		}
	}

	template<Color Clr>
	void x88Board::ToSquareMoves(Square sq, MoveList& moves) const
	{
		using ctraits = traits::color_traits<Clr>;
		using bctraits = traits::board_color_traits<x88Board, Clr>;
		using bptraits = traits::board_piece_traits<x88Board, ctraits::Pawn>;

		MoveType move_type{ MoveType::Quite };
		Piece captured_piece{ Empty };
		if (_board[sq] != Empty)
		{
			captured_piece = _board[sq];
			move_type = MoveType::Capture;
		}

		for (Direction dir : direction::Straight)
		{
			for (Square next{ sq + dir }; IsInside(next); next += dir)
			{
				if (_board[next] == Empty)
					continue;

				if ((_board[next] == ctraits::Rook || _board[next] == ctraits::Queen) &&
					!IsPiecePinned(next))
					moves.emplace_back(next, sq, move_type);

				break;
			}
		}

		for (Direction dir : direction::Diagonal)
		{
			for (Square next{ sq + dir }; IsInside(next); next += dir)
			{
				if (_board[next] == Empty)
					continue;

				if ((_board[next] == ctraits::Bishop || _board[next] == ctraits::Queen) &&
					!IsPiecePinned(next))
					moves.emplace_back(next, sq, move_type);

				break;
			}
		}

		for (Direction dir : direction::KnightJumps)
			if (Square next{ sq + dir }; IsInside(next) && _board[next] == ctraits::Knight &&
				!IsPiecePinned(next))
				moves.emplace_back(next, sq, move_type);

		if (sq >= bctraits::ValidPawnMoveSquares[0] && sq <= bctraits::ValidPawnMoveSquares[1])
		{
			if (move_type == MoveType::Quite)
			{
				Square next{ sq + bptraits::ReverseMoveDirection };

				if (_board[next] == ctraits::Pawn && !IsPiecePinned(next))
				{
					if (get_rank(sq) == bptraits::PromotionRank)
					{
						moves.emplace_back(next, sq, MoveType::Queen_Promotion);
						moves.emplace_back(next, sq, MoveType::Rook_Promotion);
						moves.emplace_back(next, sq, MoveType::Bishop_Promotion);
						moves.emplace_back(next, sq, MoveType::Knight_Promotion);
					}
					else
					{
						moves.emplace_back(next, sq, move_type);
					}
				}
				else if (_board[next] == Empty)
				{
					next += bptraits::ReverseMoveDirection;

					if (_board[next] == ctraits::Pawn && !IsPiecePinned(next) &&
						get_rank(next) == bptraits::DoublePushRank)
						moves.emplace_back(next, sq, MoveType::Double_Pawn_Push);
				}
			}
			else
			{
				for (Direction dir : bptraits::ReverseAttackDirections)
				{
					if (Square next{ sq + dir }; IsInside(next) && _board[next] == ctraits::Pawn &&
						!IsPiecePinned(next))
					{
						if (get_rank(sq) == bptraits::PromotionRank)
						{
							moves.emplace_back(next, sq, MoveType::Queen_Promotion_Capture);
							moves.emplace_back(next, sq, MoveType::Rook_Promotion_Capture);
							moves.emplace_back(next, sq, MoveType::Bishop_Promotion_Capture);
							moves.emplace_back(next, sq, MoveType::Knight_Promotion_Capture);
						}
						else
						{
							moves.emplace_back(next, sq, move_type);
						}
					}
				}
			}
		}
	}

	template<Color Clr>
	void x88Board::GenerateKnightMoves(MoveList& moves) const
	{
		using ctraits = traits::color_traits<Clr>;

		auto [first, last] = GetPiecePositions<Clr>(ctraits::Knight);

		for (; first != last; first++)
		{
			if (IsPiecePinned(first->second))
				continue;

			for (Direction dir : direction::KnightJumps)
			{
				Square next{ first->second + dir };
				if (IsInside(next))
				{
					if (_board[next] == Empty)
						moves.emplace_back(first->second, next);
					else if (color::get_color(_board[next]) != Clr)
						moves.emplace_back(first->second, next, MoveType::Capture);
				}
			}
		}
	}

	template<Color Clr>
	void x88Board::GenerateStraightMoves(MoveList& moves) const
	{
		using ctraits = traits::color_traits<Clr>;
		for (int i = 0; i < 2; i++)
		{
			auto [first, last] = GetPiecePositions<Clr>(ctraits::StraightMovingPieces[i]);
			for (; first != last; first++)
			{
				auto pin_dir = GetPinDirection(first->second);

				for (Direction dir : direction::Straight)
				{
					if (pin_dir != Empty && dir != pin_dir && direction::Reverse(dir) != pin_dir)
						continue;

					for (Square next{ first->second + dir }; IsInside(next); next += dir)
					{
						if (_board[next] == Empty)
							moves.emplace_back(first->second, next);
						else
						{
							if (color::get_color(_board[next]) != Clr)
								moves.emplace_back(first->second, next, MoveType::Capture);
							break;
						}
					}
				}
			}
		}
	}

	template<Color Clr>
	void x88Board::GenerateDiagonalMoves(MoveList& moves) const
	{
		using ctraits = traits::color_traits<Clr>;
		for (int i = 0; i < 2; i++)
		{
			auto [first, last] = GetPiecePositions<Clr>(ctraits::DiagonalMovingPieces[i]);
			for (; first != last; first++)
			{
				auto pin_dir = GetPinDirection(first->second);

				for (Direction dir : direction::Diagonal)
				{
					if (pin_dir != Empty && dir != pin_dir && direction::Reverse(dir) != pin_dir)
						continue;

					for (Square next{ first->second + dir }; IsInside(next); next += dir)
					{
						if (_board[next] == Empty)
							moves.emplace_back(first->second, next);
						else
						{
							if (color::get_color(_board[next]) != Clr)
								moves.emplace_back(first->second, next, MoveType::Capture);
							break;
						}
					}
				}
			}
		}
	}

	template<Color Clr>
	void x88Board::GeneratePawnMoves(MoveList& moves) const
	{
		using ctraits = traits::color_traits<Clr>;
		using bptraits = traits::board_piece_traits<x88Board, ctraits::Pawn>;

		auto [first, last] = GetPiecePositions<Clr>(ctraits::Pawn);
		for (; first != last; first++)
		{
			auto pin_dir = GetPinDirection(first->second);

			// One square forward, two square forward moves
			Square next{ first->second + bptraits::MoveDirection };
			if (_board[next] == Empty && (pin_dir == Empty ||
				pin_dir == bptraits::MoveDirection || pin_dir == bptraits::ReverseMoveDirection))
			{
				Rank r = get_rank(next);
				if (r == bptraits::PromotionRank)
				{
					moves.emplace_back(first->second, next, MoveType::Queen_Promotion);
					moves.emplace_back(first->second, next, MoveType::Rook_Promotion);
					moves.emplace_back(first->second, next, MoveType::Knight_Promotion);
					moves.emplace_back(first->second, next, MoveType::Bishop_Promotion);
				}
				else
					moves.emplace_back(first->second, next);

				if (Square pos{ next + bptraits::MoveDirection };
					get_rank(first->second) == bptraits::DoublePushRank &&
					_board[pos] == Empty)
					moves.emplace_back(first->second, pos, MoveType::Double_Pawn_Push);
			}

			// Captures
			for (int i{ 0 }; i < 2; i++)
			{
				Square next{ first->second + bptraits::AttackDirections[i] };
				if (IsInside(next) && _board[next] != Empty &&
					color::get_color(_board[next]) != Clr &&
					(pin_dir == Empty || pin_dir == bptraits::AttackDirections[i]))
				{
					if (get_rank(next) == bptraits::PromotionRank)
					{
						moves.emplace_back(first->second, next, MoveType::Queen_Promotion_Capture);
						moves.emplace_back(first->second, next, MoveType::Rook_Promotion_Capture);
						moves.emplace_back(first->second, next, MoveType::Knight_Promotion_Capture);
						moves.emplace_back(first->second, next, MoveType::Bishop_Promotion_Capture);
					}
					else
						moves.emplace_back(first->second, next, MoveType::Capture);
				}
			}
		}
	}

	template<Color Clr>
	void x88Board::GenerateEnPassantCaptureMoves(Square king_pos, MoveList& moves) const
	{
		if (_enpassant_target == Empty)
			return;

		using ctraits = traits::color_traits<Clr>;
		using bptraits = traits::board_piece_traits<x88Board, ctraits::Pawn>;
		using otraits = traits::color_traits<ctraits::Opposite>;

		for (int i{ 0 }; i < 2; i++)
		{
			Square pos{ _enpassant_target + bptraits::ReverseAttackDirections[i] };
			if (!IsInside(pos) || _board[pos] != ctraits::Pawn)
				continue;

			auto pin_dir = GetPinDirection(pos);
			if (pin_dir != Empty && pin_dir != bptraits::AttackDirections[i])
				continue;

			if (get_rank(king_pos) != get_rank(pos))
			{
				moves.emplace_back(pos, _enpassant_target, MoveType::En_Passant_Capture);
			}
			else
			{
				Square ppos{ _enpassant_target + bptraits::ReverseMoveDirection };
				Direction dir = king_pos > pos ? direction::W : direction::E;
				bool make_move{ true };
				for (Square next{ king_pos + dir }; IsInside(next); next += dir)
				{
					if (next == pos || next == ppos || _board[next] == Empty)
						continue;

					if (_board[next] == otraits::Rook || _board[next] == otraits::Queen)
						make_move = false;
					break;
				}

				if (make_move)
					moves.emplace_back(pos, _enpassant_target, MoveType::En_Passant_Capture);
			}
		}
	}
	*/
}
