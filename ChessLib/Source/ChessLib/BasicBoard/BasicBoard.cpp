#include <ChessLib/BasicBoard/BasicBoard.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

#include <stdexcept>

namespace chesslib::basic_board
{
	// Explicit initializations
	template void BasicBoard::GenerateMovesImplementation<color::White>(MoveList&) const;
	template void BasicBoard::GenerateMovesImplementation<color::Black>(MoveList&) const;

#pragma region static_methods

	bool BasicBoard::IsInside(Square curr, Square next)
	{
		return next < 64 && next >= 0 && std::abs(get_file(next) - get_file(curr)) <= 2;
	}

	template<Color Attacker>
	bool BasicBoard::IsNonKingStraightAttack(Piece p)
	{
		using ctraits = traits::color_traits<Attacker>;
		return color::get_color(p) == Attacker && (p == ctraits::Rook || p == ctraits::Queen);
	}

	template<Color Attacker>
	bool BasicBoard::IsNonKingDiagonalAttack(Piece p, Direction dir, Distance dist)
	{
		using ctraits = traits::color_traits<Attacker>;
		using bptraits = traits::board_piece_traits<BasicBoard, ctraits::Pawn>;

		return
			color::get_color(p) == Attacker &&
			(p == ctraits::Bishop || p == ctraits::Queen || dist == 1 && p == ctraits::Pawn &&
			(dir == bptraits::ReverseAttackDirections[0] || dir == bptraits::ReverseAttackDirections[1]));
	}

	template<Color Attacker>
	bool BasicBoard::IsStraightAttack(Piece p, Distance dist)
	{
		using ctraits = traits::color_traits<Attacker>;
		return color::get_color(p) == Attacker &&
			(p == ctraits::Rook || p == ctraits::Queen || p == ctraits::King && dist == 1);
	}

	template<Color Attacker>
	bool BasicBoard::IsDiagonalAttack(Piece p, Direction dir, Distance dist)
	{
		using ctraits = traits::color_traits<Attacker>;
		using bptraits = traits::board_piece_traits<BasicBoard, ctraits::Pawn>;

		return color::get_color(p) == Attacker && (p == ctraits::Bishop || p == ctraits::Queen ||
			dist == 1 && (p == ctraits::King || (p == ctraits::Pawn &&
		    (dir == bptraits::ReverseAttackDirections[0] || dir == bptraits::ReverseAttackDirections[1]))));
	}

#pragma endregion

	BasicBoard::BasicBoard() : board{} { }

	const BasicBoard::BoardArray& BasicBoard::GetBoard() const { return board; }

	BasicBoard::BoardArray& BasicBoard::GetBoard() { return board; }

	void BasicBoard::MakeMove(const Move& move) 
	{
		PushToMoveStack(move);

		auto side_to_move = GetActiveColor();
		if (side_to_move == color::White)
			MakeMoveImplementation<color::White>(move);
		else
			MakeMoveImplementation<color::Black>(move);
	}

	void BasicBoard::UnMakeMove()
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
			UnMakeMove<color::White>(previous_state.move);
		else
			UnMakeMove<color::Black>(previous_state.move);

		_move_stack.pop();
	}

	MoveList BasicBoard::GenerateMoves() const 
	{
		MoveList moves;

		auto side_to_move = GetActiveColor();
		if (side_to_move == color::White)
			GenerateMovesImplementation<color::White>(moves);
		else
			GenerateMovesImplementation<color::Black>(moves);

		return moves;
	}

	template<Color Clr>
	void BasicBoard::MakeMoveImplementation(const Move& move)
	{
		using ctraits = traits::color_traits<Clr>;
		using octraits = traits::color_traits<ctraits::Opposite>;
		using bptraits = traits::board_piece_traits<BasicBoard, ctraits::Pawn>;
		using bctraits = traits::board_color_traits<BasicBoard, Clr>;
		using boctraits = traits::board_color_traits<BasicBoard, ctraits::Opposite>;

		Square from{ move.GetFrom() }, to{ move.GetTo() };
		Piece captured{ move.GetCapturedPiece() };
		MoveType mtype{ move.GetMoveType() };

		// update half move clock
		_halfmove_clock = board[from] == ctraits::Pawn || move.IsCapture() ? 0 : _halfmove_clock + 1;

		// update full move clock 
		if (Clr == color::Black)
			_fullmove_clock++;

		// update castling rights
		if (IsCastlingAvailable()) 
		{
			if (board[from] == ctraits::King)
			{
				SetCastling(ctraits::KingSideCastling, false);
				SetCastling(ctraits::QueenSideCastling, false);
			}
			else if (board[from] == ctraits::Rook)
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
				board[removed_pawn_pos] = squareset::Empty;
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

			RemovePiece<Clr>(board[from], from);
			AddNewPiece<Clr>(promoted_piece, to);
			board[to] = promoted_piece;
			board[from] = squareset::Empty;
		}

		// update enpassant target square
		_enpassant_target = mtype == MoveType::Double_Pawn_Push ? from + bptraits::MoveDirection : squareset::None;

		// update side to move
		_active_color = ctraits::Opposite;
	}

	template<Color Clr>
	void BasicBoard::MakeQuiteMove(Square from, Square to) 
	{
		UpdatePiecePosition<Clr>(board[from], from, to);
		board[to] = board[from];
		board[from] = squareset::Empty;
	}

	template<Color Clr>
	void BasicBoard::UnMakeMove(const Move& move)
	{
		using ctraits = traits::color_traits<Clr>;
		using bctraits = traits::board_color_traits<BasicBoard, Clr>;
		using bptraits = traits::board_piece_traits<BasicBoard, ctraits::Pawn>;

		Square from{ move.GetFrom() }, to{ move.GetTo() };
		Piece captured{ move.GetCapturedPiece() };
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
				board[removed_pawn_pos] = captured;
			}
			else
			{
				board[to] = captured;
				AddNewPiece<ctraits::Opposite>(captured, to);
			}
		}
		else 
		{
			// promotion with/without capture
			RemovePiece<Clr>(board[to], to);
			AddNewPiece<Clr>(ctraits::Pawn, from);
			board[from] = ctraits::Pawn;
			if (captured != pieceset::None) 
			{
				board[to] = captured;
				AddNewPiece<ctraits::Opposite>(captured, to);
			}
			else 
				board[to] = squareset::Empty;
		}
	}

	template<Color Clr>
	void BasicBoard::GenerateMovesImplementation(MoveList& moves) const
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
	void BasicBoard::ComputeChecksAndPins(Square king_pos) const
	{
		ClearChecksAndPins();

		ComputeChecksAndPins<Attacker, true>(king_pos, direction::Straight);
		ComputeChecksAndPins<Attacker, false>(king_pos, direction::Diagonal);

		using ctraits = traits::color_traits<Attacker>;
		for (Direction dir : direction::KnightJumps) 
		{
			if (Square next{ king_pos + dir };
				IsInside(king_pos, next) && board[next] == ctraits::Knight)
				_checks.emplace_back(next, direction::None, 0);
		}
	}

	template<Color Attacker, bool IsStraightMovingPiece>
	void BasicBoard::ComputeChecksAndPins(Square king_pos, const std::array<Direction, 4>& attack_directions) const
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

	template<Color Clr>
	void BasicBoard::GenerateKingMoves(Square king_pos, MoveList& moves) const
	{
		using ctraits = traits::color_traits<Clr>;

		for (Direction dir : direction::All)
		{
			if (IsDirectionInvalidForKing<ctraits::Opposite>(dir))
				continue;

			if (Square next{ king_pos + dir }; 
				IsInside(king_pos, next) && board[next] && !IsUnderAttack<ctraits::Opposite>(next))
			{
				if (board[next] == squareset::Empty)
					moves.emplace_back(king_pos, next, MoveType::Quite);
				else if (color::get_color(board[next]) != Clr)
					moves.emplace_back(king_pos, next, MoveType::Capture, board[next]);
			}
		}

		if (_checks.size() > 0)
			return;

		using bctraits = traits::board_color_traits<BasicBoard, Clr>;

		if (IsKingSideCastlingPossible<Clr>())
			moves.emplace_back(king_pos, bctraits::KingSideCastleCheckSquares[1], MoveType::King_Side_Castle);

		if (IsQueenSideCastlingPossible<Clr>())
			moves.emplace_back(king_pos, bctraits::QueenSideCastleCheckSquares[1], MoveType::Queen_Side_Castle);
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

	template<Color Attacker>
	bool BasicBoard::IsUnderAttack(Square sq) const
	{
		return
			IsUnderAttack<Attacker, true>(sq, direction::Straight) ||
			IsUnderAttack<Attacker, false>(sq, direction::Diagonal) ||
			IsUnderAttackByAKnight<Attacker>(sq);
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

	template<Color Clr>
	void BasicBoard::GenerateCheckEliminatingMoves(Square king_pos, MoveList& moves) const
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
	void BasicBoard::GenerateCheckEliminatingEnPassantCaptureMoves(Square attacker_loc, MoveList& moves) const
	{
		using ctraits = traits::color_traits<Clr>;
		using bptraits = traits::board_piece_traits<BasicBoard, ctraits::Pawn>;

		if (_enpassant_target == squareset::None)
			return;

		Direction dirs[2] = { -1, 1 };
		for (auto i{ 0 }; i < 2; i++) 
		{
			Square next{ attacker_loc + dirs[i] };
			if (IsInside(next, _enpassant_target) && board[next] == ctraits::Pawn)
				moves.emplace_back(next, _enpassant_target, MoveType::En_Passant_Capture, ctraits::Opposite);
		}
	}

	template<Color Clr>
	void BasicBoard::ToSquareMoves(Square sq, MoveList& moves) const
	{
		using ctraits = traits::color_traits<Clr>;
		using bctraits = traits::board_color_traits<BasicBoard, Clr>;
		using bptraits = traits::board_piece_traits<BasicBoard, ctraits::Pawn>;

		MoveType move_type{ MoveType::Quite };
		Piece captured_piece{ pieceset::None };
		if (board[sq] != squareset::Empty)
		{
			captured_piece = board[sq];
			move_type = MoveType::Capture;
		}

		for (Direction dir : direction::Straight)
		{
			for (Square next{ sq + dir }; IsInside(next - dir, next); next += dir)
			{
				if (board[next] == squareset::Empty)
					continue;

				if ((board[next] == ctraits::Rook || board[next] == ctraits::Queen) && 
					!IsPiecePinned(next))
					moves.emplace_back(next, sq, move_type, captured_piece);

				break;
			}
		}

		for (Direction dir : direction::Diagonal)
		{
			for (Square next{ sq + dir }; IsInside(next - dir, next); next += dir)
			{
				if (board[next] == squareset::Empty)
					continue;

				if ((board[next] == ctraits::Bishop || board[next] == ctraits::Queen) && 
					!IsPiecePinned(next))
					moves.emplace_back(next, sq, move_type, captured_piece);

				break;
			}
		}

		for (Direction dir : direction::KnightJumps)
			if (Square next{ sq + dir }; IsInside(sq, next) && board[next] == ctraits::Knight &&
				!IsPiecePinned(next))
				moves.emplace_back(next, sq, move_type, captured_piece);

		if (sq >= bctraits::ValidPawnMoveSquares[0] && sq <= bctraits::ValidPawnMoveSquares[1]) 
		{
			if (move_type == MoveType::Quite) 
			{		
				Square next{ sq + bptraits::ReverseMoveDirection };

				if (board[next] == ctraits::Pawn && !IsPiecePinned(next))
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
				else if (board[next] == squareset::Empty) 
				{
					next += bptraits::ReverseMoveDirection;

					if(board[next] == ctraits::Pawn && !IsPiecePinned(next) &&
						get_rank(next) == bptraits::DoublePushRank)
						moves.emplace_back(next, sq, MoveType::Double_Pawn_Push);
				}
			}
			else 
			{
				for (Direction dir : bptraits::ReverseAttackDirections) 
				{
					if (Square next{ sq + dir }; IsInside(sq, next) && board[next] == ctraits::Pawn && 
						!IsPiecePinned(next)) 
					{
						if (get_rank(sq) == bptraits::PromotionRank)
						{
							moves.emplace_back(next, sq, MoveType::Queen_Promotion_Capture, captured_piece);
							moves.emplace_back(next, sq, MoveType::Rook_Promotion_Capture, captured_piece);
							moves.emplace_back(next, sq, MoveType::Bishop_Promotion_Capture, captured_piece);
							moves.emplace_back(next, sq, MoveType::Knight_Promotion_Capture, captured_piece);
						}
						else
						{
							moves.emplace_back(next, sq, move_type, captured_piece);
						}
					}
				}
			}
		}
	}

	template<Color Clr>
	void BasicBoard::GenerateKnightMoves(MoveList& moves) const
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
				if (IsInside(first->second, next))
				{
					if (board[next] == squareset::Empty)
						moves.emplace_back(first->second, next, MoveType::Quite);
					else if (color::get_color(board[next]) != Clr)
						moves.emplace_back(first->second, next, MoveType::Capture, board[next]);
				}
			}
		}
	}

	template<Color Clr>
	void BasicBoard::GenerateStraightMoves(MoveList& moves) const
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

					for (Square next{ first->second + dir }; IsInside(next - dir, next); next += dir)
					{
						if (board[next] == squareset::Empty)
							moves.emplace_back(first->second, next, MoveType::Quite);
						else 
						{
							if(color::get_color(board[next]) != Clr)
								moves.emplace_back(first->second, next, MoveType::Capture, board[next]);
							break;
						}
					}
				}
			}
		}
	}

	template<Color Clr>
	void BasicBoard::GenerateDiagonalMoves(MoveList& moves) const
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

					for (Square next{ first->second + dir }; IsInside(next - dir, next); next += dir)
					{
						if (board[next] == squareset::Empty)
							moves.emplace_back(first->second, next, MoveType::Quite);
						else
						{
							if (color::get_color(board[next]) != Clr)
								moves.emplace_back(first->second, next, MoveType::Capture, board[next]);
							break;
						}
					}
				}
			}
		}
	}

	template<Color Clr>
	void BasicBoard::GeneratePawnMoves(MoveList& moves) const 
	{
		using ctraits = traits::color_traits<Clr>;
		using bptraits = traits::board_piece_traits<BasicBoard, ctraits::Pawn>;

		auto [first, last] = GetPiecePositions<Clr>(ctraits::Pawn);
		for (; first != last; first++)
		{
			auto pin_dir = GetPinDirection(first->second);

			// One square forward, two square forward moves
			Square next{ first->second + bptraits::MoveDirection };
			if (board[next] == squareset::Empty && (pin_dir == direction::None ||
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
					moves.emplace_back(first->second, next, MoveType::Quite);

				if (Square pos{ next + bptraits::MoveDirection }; 
					get_rank(first->second) == bptraits::DoublePushRank && 
					board[pos] == squareset::Empty)
					moves.emplace_back(first->second, pos, MoveType::Double_Pawn_Push);
			}

			// Captures
			for (int i{ 0 }; i < 2; i++) 
			{
				Square next{ first->second + bptraits::AttackDirections[i] };
				if (IsInside(first->second, next) && board[next] != squareset::Empty &&
					color::get_color(board[next]) != Clr &&
					(pin_dir == direction::None || pin_dir == bptraits::AttackDirections[i]))
				{
					if (get_rank(next) == bptraits::PromotionRank) 
					{
						moves.emplace_back(first->second, next, MoveType::Queen_Promotion_Capture, board[next]);
						moves.emplace_back(first->second, next, MoveType::Rook_Promotion_Capture, board[next]);
						moves.emplace_back(first->second, next, MoveType::Knight_Promotion_Capture, board[next]);
						moves.emplace_back(first->second, next, MoveType::Bishop_Promotion_Capture, board[next]);
					}
					else 
						moves.emplace_back(first->second, next, MoveType::Capture, board[next]);
				}
			}
		}
	}

	template<Color Clr>
	void BasicBoard::GenerateEnPassantCaptureMoves(Square king_pos, MoveList& moves) const
	{
		if (_enpassant_target == squareset::None)
			return;

		using ctraits = traits::color_traits<Clr>;
		using bptraits = traits::board_piece_traits<BasicBoard, ctraits::Pawn>;
		using otraits = traits::color_traits<ctraits::Opposite>;

		for (int i{ 0 }; i < 2; i++)
		{
			Square pos{ _enpassant_target + bptraits::ReverseAttackDirections[i] };
			if (!IsInside(_enpassant_target, pos) || board[pos] != ctraits::Pawn)
				continue;

			auto pin_dir = GetPinDirection(pos);
			if (pin_dir != direction::None && pin_dir != bptraits::AttackDirections[i])
				continue;
			
			if (get_rank(king_pos) != get_rank(pos))
			{
				moves.emplace_back(pos, _enpassant_target, MoveType::En_Passant_Capture, otraits::Pawn);
			}
			else 
			{
				Square ppos{ _enpassant_target + bptraits::ReverseMoveDirection };
				Direction dir = king_pos > pos ? direction::W : direction::E;
				for (Square next{ king_pos + dir }; IsInside(next - dir, next); next += dir)
				{
					if (next == pos || next == ppos || board[next] == squareset::Empty)
						continue;

					if (board[next] != otraits::Rook && board[next] != otraits::Queen)
						moves.emplace_back(pos, _enpassant_target, MoveType::En_Passant_Capture, otraits::Pawn);

					break;
				}
			}
		}
	}

#pragma region factory_functions

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

#pragma endregion

}