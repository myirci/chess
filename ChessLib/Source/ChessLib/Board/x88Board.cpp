#include <ChessLib/Board/x88Board.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

namespace chesslib
{
	/*
	void x88Board::MakeMove(const Move& move)
	{
		auto side_to_move = GetActiveColor();
		if (side_to_move == color::White)
			MakeMoveImplementation<color::White>(move);
		else
			MakeMoveImplementation<color::Black>(move);
	}

	template<Color Clr>
	void x88Board::MakeMoveImplementation(const Move& move)
	{
		using ctraits = traits::color_traits<Clr>;
		using octraits = traits::color_traits<ctraits::Opposite>;
		using bptraits = traits::board_piece_traits<x88Board, ctraits::Pawn>;
		using bctraits = traits::board_color_traits<x88Board, Clr>;
		using boctraits = traits::board_color_traits<x88Board, ctraits::Opposite>;

		Square from{ move.GetFrom() }, to{ move.GetTo() };

		Piece captured{ pieceset::None };
		if (move.IsCapture())
			captured = move.IsEnPassantCapture() ? octraits::Pawn : _board[to];

		MoveType mtype{ move.GetMoveType() };

		// push move to the stack
		PushToMoveStack(move, captured);

		// update half move clock
		_halfmove_clock = _board[from] == ctraits::Pawn || move.IsCapture() ? 0 : _halfmove_clock + 1;

		// update full move clock 
		if (Clr == color::Black)
			_fullmove_clock++;

		// update castling rights
		if (IsCastlingAvailable())
		{
			if (_board[from] == ctraits::King)
			{
				SetCastling(ctraits::KingSideCastling, false);
				SetCastling(ctraits::QueenSideCastling, false);
			}
			else if (_board[from] == ctraits::Rook)
			{
				if (from == bctraits::KingSideRookInitialPosition)
					SetCastling(ctraits::KingSideCastling, false);
				else if (from == bctraits::QueenSideRookInitialPosition)
					SetCastling(ctraits::QueenSideCastling, false);
			}

			if (captured == octraits::Rook)
			{
				if (to == boctraits::KingSideRookInitialPosition)
					SetCastling(octraits::KingSideCastling, false);
				else if (to == boctraits::QueenSideRookInitialPosition)
					SetCastling(octraits::QueenSideCastling, false);
			}
		}

		// update board and piece locations
		if (!move.IsPromotion() && !move.IsCapture())
		{
			// Move types: quite, double pawn push, castling 
			MakeQuiteMove<Clr>(from, to);

			if (mtype == MoveType::King_Side_Castle)
				MakeQuiteMove<Clr>(bctraits::KingSideRookInitialPosition, bctraits::KingSideRookPositionAfterCastling);
			else if (mtype == MoveType::Queen_Side_Castle)
				MakeQuiteMove<Clr>(bctraits::QueenSideRookInitialPosition, bctraits::QueenSideRookPositionAfterCastling);
		}
		else if (!move.IsPromotion())
		{
			// Move types: capture, enpassant capture
			if (mtype == MoveType::En_Passant_Capture)
			{
				Square removed_pawn_pos{ _enpassant_target + bptraits::ReverseMoveDirection };
				RemovePiece<ctraits::Opposite>(captured, removed_pawn_pos);
				_board[removed_pawn_pos] = squareset::Empty;
			}
			else
				RemovePiece<ctraits::Opposite>(captured, to);

			MakeQuiteMove<Clr>(from, to);
		}
		else
		{
			// Promotion with/without capture
			Piece promoted_piece{ ctraits::Queen };
			if (mtype == MoveType::Knight_Promotion || mtype == MoveType::Knight_Promotion_Capture)
				promoted_piece = ctraits::Knight;
			else if (mtype == MoveType::Rook_Promotion || mtype == MoveType::Rook_Promotion_Capture)
				promoted_piece = ctraits::Rook;
			else if (mtype == MoveType::Bishop_Promotion || mtype == MoveType::Bishop_Promotion_Capture)
				promoted_piece = ctraits::Bishop;

			if (move.IsCapture())
				RemovePiece<ctraits::Opposite>(captured, to);

			RemovePiece<Clr>(_board[from], from);
			AddNewPiece<Clr>(promoted_piece, to);
			_board[to] = promoted_piece;
			_board[from] = squareset::Empty;
		}

		// update enpassant target square
		_enpassant_target = mtype == MoveType::Double_Pawn_Push ? from + bptraits::MoveDirection : squareset::None;

		// update side to move
		_active_color = ctraits::Opposite;
	}

	template<Color Clr>
	void x88Board::MakeQuiteMove(Square from, Square to)
	{
		UpdatePiecePosition<Clr>(_board[from], from, to);
		_board[to] = _board[from];
		_board[from] = squareset::Empty;
	}

	void x88Board::UnMakeMove()
	{
		// get the move and board state before the move was made.
		const auto& previous_state = _move_stack.top();

		// restore side to move
		_active_color = color::get_opposite_color(_active_color);

		// restore castling rights, en-passant location and half move clock
		_castling_rights = previous_state.castling_rights;
		_enpassant_target = previous_state.enpassant_target;
		_halfmove_clock = previous_state.halfmove_clock;

		// restore the full move clock
		if (_active_color == color::Black)
			_fullmove_clock--;

		// restore the board and piece positions
		if (_active_color == color::White)
			UnMakeMove<color::White>(previous_state.move, previous_state.captured_piece);
		else
			UnMakeMove<color::Black>(previous_state.move, previous_state.captured_piece);

		_move_stack.pop();
	}

	template<Color Clr>
	void x88Board::UnMakeMove(const Move& move, Piece captured)
	{
		using ctraits = traits::color_traits<Clr>;
		using bctraits = traits::board_color_traits<x88Board, Clr>;
		using bptraits = traits::board_piece_traits<x88Board, ctraits::Pawn>;

		Square from{ move.GetFrom() }, to{ move.GetTo() };
		MoveType mtype{ move.GetMoveType() };

		if (!move.IsPromotion() && !move.IsCapture())
		{
			// Move types: quite, double pawn push, castling
			MakeQuiteMove<Clr>(to, from);

			if (mtype == MoveType::King_Side_Castle)
				MakeQuiteMove<Clr>(bctraits::KingSideRookPositionAfterCastling, bctraits::KingSideRookInitialPosition);
			else if (mtype == MoveType::Queen_Side_Castle)
				MakeQuiteMove<Clr>(bctraits::QueenSideRookPositionAfterCastling, bctraits::QueenSideRookInitialPosition);
		}
		else if (!move.IsPromotion())
		{
			// Move types: capture, enpassant capture
			MakeQuiteMove<Clr>(to, from);

			if (mtype == MoveType::En_Passant_Capture)
			{
				Square removed_pawn_pos{ _enpassant_target + bptraits::ReverseMoveDirection };
				AddNewPiece<ctraits::Opposite>(captured, removed_pawn_pos);
				_board[removed_pawn_pos] = captured;
			}
			else
			{
				_board[to] = captured;
				AddNewPiece<ctraits::Opposite>(captured, to);
			}
		}
		else
		{
			// promotion with/without capture
			RemovePiece<Clr>(_board[to], to);
			AddNewPiece<Clr>(ctraits::Pawn, from);
			_board[from] = ctraits::Pawn;
			if (captured != pieceset::None)
			{
				_board[to] = captured;
				AddNewPiece<ctraits::Opposite>(captured, to);
			}
			else
				_board[to] = squareset::Empty;
		}
	}

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
				_checks.emplace_back(next, direction::None, 0);
	}

	template<Color Attacker, bool IsStraightMovingPiece>
	void x88Board::ComputeChecksAndPins(Square king_pos, const std::array<Direction, 4>& attack_directions) const
	{
		using ctraits = traits::color_traits<Attacker>;
		using bptraits = traits::board_piece_traits<x88Board, ctraits::Pawn>;

		for (Direction dir : attack_directions)
		{
			Square pin_loc{ squareset::None };
			Distance dist{ 1 };
			for (Square next{ king_pos + dir }; IsInside(next); next += dir, dist++)
			{
				if (_board[next] == squareset::Empty)
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
				if (_board[next] == squareset::Empty)
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
				if (_board[next] == squareset::Empty)
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
			_board[bctraits::KingSideCastleCheckSquares[0]] == squareset::Empty &&
			_board[bctraits::KingSideCastleCheckSquares[1]] == squareset::Empty &&
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
			_board[bctraits::QueenSideCastleCheckSquares[0]] == squareset::Empty &&
			_board[bctraits::QueenSideCastleCheckSquares[1]] == squareset::Empty &&
			_board[bctraits::QueenSideCastleCheckSquares[2]] == squareset::Empty &&
			!IsUnderAttack<ctraits::Opposite>(bctraits::QueenSideCastleCheckSquares[0]) &&
			!IsUnderAttack<ctraits::Opposite>(bctraits::QueenSideCastleCheckSquares[1]);
	}

	template<Color Clr>
	void x88Board::GenerateCheckEliminatingMoves(Square king_pos, MoveList& moves) const
	{
		auto [attacker_loc, king_to_square_dir, dist] = _checks[0];

		GenerateCheckEliminatingEnPassantCaptureMoves<Clr>(attacker_loc, moves);

		if (king_to_square_dir != direction::None)
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

		if (_enpassant_target == squareset::None || _board[attacker_loc] != bptraits::Opposite)
			return;

		Direction dirs[2] = { -1, 1 };
		for (auto i{ 0 }; i < 2; i++)
		{
			Square next{ attacker_loc + dirs[i] };
			if (auto pin_dir = GetPinDirection(next);
				pin_dir == direction::None &&
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
		Piece captured_piece{ pieceset::None };
		if (_board[sq] != squareset::Empty)
		{
			captured_piece = _board[sq];
			move_type = MoveType::Capture;
		}

		for (Direction dir : direction::Straight)
		{
			for (Square next{ sq + dir }; IsInside(next); next += dir)
			{
				if (_board[next] == squareset::Empty)
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
				if (_board[next] == squareset::Empty)
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
				else if (_board[next] == squareset::Empty)
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
					if (_board[next] == squareset::Empty)
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
					if (pin_dir != direction::None && dir != pin_dir && direction::Reverse(dir) != pin_dir)
						continue;

					for (Square next{ first->second + dir }; IsInside(next); next += dir)
					{
						if (_board[next] == squareset::Empty)
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
					if (pin_dir != direction::None && dir != pin_dir && direction::Reverse(dir) != pin_dir)
						continue;

					for (Square next{ first->second + dir }; IsInside(next); next += dir)
					{
						if (_board[next] == squareset::Empty)
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
			if (_board[next] == squareset::Empty && (pin_dir == direction::None ||
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
					_board[pos] == squareset::Empty)
					moves.emplace_back(first->second, pos, MoveType::Double_Pawn_Push);
			}

			// Captures
			for (int i{ 0 }; i < 2; i++)
			{
				Square next{ first->second + bptraits::AttackDirections[i] };
				if (IsInside(next) && _board[next] != squareset::Empty &&
					color::get_color(_board[next]) != Clr &&
					(pin_dir == direction::None || pin_dir == bptraits::AttackDirections[i]))
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
		if (_enpassant_target == squareset::None)
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
			if (pin_dir != direction::None && pin_dir != bptraits::AttackDirections[i])
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
					if (next == pos || next == ppos || _board[next] == squareset::Empty)
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
