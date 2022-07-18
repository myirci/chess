#pragma once

#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Chess/Definitions.hpp>

#include <ChessLib/MoveGenerator/AttackDetector.hpp>

namespace chesslib
{
	template <typename BoardType>
	class MoveGenerator 
	{
	public:
		MoveList GenerateMoves(const BoardType& board);

	private:

		AttackDetector<BoardType> _attackDetector;

		template<Color SideToMove>
		void GenerateMoves(const BoardType& board, MoveList& moves);
	
		template<Color SideToMove>
		void GenerateKingMoves(const BoardType& board, Square king_pos, MoveList& moves);
	
		template<Color SideToMove>
		void GenerateCheckerCapturingMoves(const BoardType& board, MoveList& moves);

		template<Color SideToMove>
		void GenerateCheckBlockingMoves(const BoardType& board, MoveList& moves) const;

		template<Color SideToMove>
		void GenerateKnightMoves(const BoardType& board, MoveList& moves) const;

		template<Color SideToMove>
		void GenerateStraightMoves(const BoardType& board, MoveList& moves) const;

		template<Color SideToMove>
		inline void GenerateStraightMoves(const BoardType& board, Square pos, Direction dir, MoveList& moves) const;

		template<Color SideToMove>
		void GenerateDiagonalMoves(const BoardType& board, MoveList& moves) const;
	
		template<Color SideToMove>
		inline void GenerateDiagonalMoves(const BoardType& board, Square pos, Direction dir, MoveList& moves) const;
		
		template<Color SideToMove>
		void GeneratePawnMoves(const BoardType& board, MoveList& moves) const;
	};

	template <typename BoardType>
	MoveList MoveGenerator<BoardType>::GenerateMoves(const BoardType& board)
	{
		MoveList moves;

		if (board.GetActiveColor() == color::White)
			GenerateMoves<color::White>(board, moves);
		else
			GenerateMoves<color::Black>(board, moves);

		return moves;
	}

	template <typename BoardType>
	template <Color SideToMove>
	void MoveGenerator<BoardType>::GenerateMoves(const BoardType& board, MoveList& moves)
	{
		using ctraits = traits::color_traits<SideToMove>;

		auto king_pos = board.GetKingPosition<SideToMove>();
		_attackDetector.ComputeChecksAndPins<ctraits::Opposite>(board, king_pos);

		GenerateKingMoves<SideToMove>(board, king_pos, moves);

		auto num_checks = _attackDetector.GetNumberOfChecks();
		if (num_checks == 0)
		{
			GenerateStraightMoves<SideToMove>(board, moves);
			GenerateDiagonalMoves<SideToMove>(board, moves);
			GenerateKnightMoves<SideToMove>(board, moves);
			GeneratePawnMoves<SideToMove>(board, moves);
		}
		else if (num_checks == 1)
		{
			GenerateCheckerCapturingMoves<SideToMove>(board, moves);
			GenerateCheckBlockingMoves<SideToMove>(board, moves);
		}
		else if (num_checks != 2)
			throw std::logic_error("Number of checks can not be different than 0, 1, 2.");
	}

	template <typename BoardType>
	template <Color SideToMove>
	void MoveGenerator<BoardType>::GenerateKingMoves(const BoardType& board, Square king_pos, MoveList& moves)
	{
		using ctraits = traits::color_traits<SideToMove>;
		using bctraits = traits::board_color_traits<BoardType, SideToMove>;

		Square next{ Empty };

		for (Direction dir : BoardType::AllDirections)
		{
			next = king_pos + dir;
			if (IsInside<BoardType>(next, king_pos) &&
				_attackDetector.CanKingMoveInThisDirection<ctraits::Opposite>(board, dir) &&
				!_attackDetector.IsUnderAttack<ctraits::Opposite>(board, next))
			{
				auto p = board.GetPiece(next);
				if (p == Empty)
					moves.emplace_back(king_pos, next);
				else if (color::get_color(p) != SideToMove)
					moves.emplace_back(king_pos, next, MoveType::Capture);
			}
		}

		if (_attackDetector.GetNumberOfChecks() > 0)
			return;

		if (board.QueryCastling(ctraits::KingSideCastling) &&
			board.GetPiece(bctraits::KingSideCastleCheckSquares[0]) == Empty &&
			board.GetPiece(bctraits::KingSideCastleCheckSquares[1]) == Empty &&
			!_attackDetector.IsUnderAttack<ctraits::Opposite>(board, bctraits::KingSideCastleCheckSquares[0]) &&
			!_attackDetector.IsUnderAttack<ctraits::Opposite>(board, bctraits::KingSideCastleCheckSquares[1]))
			moves.emplace_back(king_pos, bctraits::KingSideCastleCheckSquares[1], MoveType::King_Side_Castle);

		if (board.QueryCastling(ctraits::QueenSideCastling) &&
			board.GetPiece(bctraits::QueenSideCastleCheckSquares[0]) == Empty &&
			board.GetPiece(bctraits::QueenSideCastleCheckSquares[1]) == Empty &&
			board.GetPiece(bctraits::QueenSideCastleCheckSquares[2]) == Empty &&
			!_attackDetector.IsUnderAttack<ctraits::Opposite>(board, bctraits::QueenSideCastleCheckSquares[0]) &&
			!_attackDetector.IsUnderAttack<ctraits::Opposite>(board, bctraits::QueenSideCastleCheckSquares[1]))
			moves.emplace_back(king_pos, bctraits::QueenSideCastleCheckSquares[1], MoveType::Queen_Side_Castle);
	}

	template <typename BoardType>
	template <Color SideToMove>
	void MoveGenerator<BoardType>::GenerateCheckerCapturingMoves(const BoardType& board, MoveList& moves)
	{
		using ctraits = traits::color_traits<SideToMove>;
		using octraits = traits::color_traits<ctraits::Opposite>;
		using bctraits = traits::board_color_traits<BoardType, SideToMove>;

		const auto& check = _attackDetector.GetFirstCheck();

		Direction check_dir = direction::Reverse(check.checker_dir); // checker to king direction

		for (Direction dir : BoardType::StraightDirections)
		{
			// checker cannot be captured in check - direction, this can be done by the checked king - only which is
		    // covered when generating the king-moves.
			if (dir == check_dir)
				continue;

			for (Square next{ check.checker + dir }; IsInside<BoardType>(next, next - dir); next += dir)
			{
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if ((p == ctraits::Rook || p == ctraits::Queen) && !_attackDetector.IsPinned(next))
					moves.emplace_back(next, check.checker, MoveType::Capture);

				break;
			}
		}

		for (Direction dir : BoardType::DiagonalDirections)
		{
			if (dir == check_dir)
				continue;

			for (Square next{ check.checker + dir }; IsInside<BoardType>(next, next - dir); next += dir)
			{
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if ((p == ctraits::Bishop || p == ctraits::Queen) && !_attackDetector.IsPinned(next))
					moves.emplace_back(next, check.checker, MoveType::Capture);

				break;
			}
		}

		for (Direction dir : BoardType::KnightJumps)
		{
			if (Square next{ check.checker + dir }; IsInside<BoardType>(next, check.checker) && board.GetPiece(next) == ctraits::Knight && !_attackDetector.IsPinned(next))
				moves.emplace_back(next, check.checker, MoveType::Capture);
		}

		if (check.checker >= bctraits::ValidPawnMoveSquares[0] && check.checker <= bctraits::ValidPawnMoveSquares[1])
		{
			for (Direction dir : bctraits::PawnReverseAttackDirections)
			{
				if (Square next{ check.checker + dir }; IsInside<BoardType>(next, check.checker) && board.GetPiece(next) == ctraits::Pawn && !_attackDetector.IsPinned(next))
				{
					if (BoardType::GetRank(check.checker) == bctraits::PawnPromotionRank)
					{
						moves.emplace_back(next, check.checker, MoveType::Queen_Promotion_Capture);
						moves.emplace_back(next, check.checker, MoveType::Rook_Promotion_Capture);
						moves.emplace_back(next, check.checker, MoveType::Bishop_Promotion_Capture);
						moves.emplace_back(next, check.checker, MoveType::Knight_Promotion_Capture);
					}
					else
					{
						moves.emplace_back(next, check.checker, MoveType::Capture);
					}
				}
			}

			if (Square enpassant_sq{ board.GetEnPassantSquare() }; enpassant_sq != Empty && board.GetPiece(check.checker) == octraits::Pawn)
			{
				for (Direction dir : bctraits::PawnReverseAttackDirections)
					if (Square next{ enpassant_sq + dir }; IsInside<BoardType>(check.checker, enpassant_sq) && board.GetPiece(next) == ctraits::Pawn && !_attackDetector.IsPinned(next))
						moves.emplace_back(next, enpassant_sq, MoveType::En_Passant_Capture);
			}
		}
	}

	template <typename BoardType>
	template <Color SideToMove>
	void MoveGenerator<BoardType>::GenerateCheckBlockingMoves(const BoardType& board, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;
		using bctraits = traits::board_color_traits<BoardType, SideToMove>;

		const auto& check = _attackDetector.GetFirstCheck();
		if (check.checker_dist < 2)
			return;

		Direction check_dir = direction::Reverse(check.checker_dir);

		Square sq{ check.checker + check_dir };
		for (Distance i{ 1 }; i < check.checker_dist; i++, sq += check_dir)
		{
			for (Direction dir : BoardType::StraightDirections)
			{
				if (dir == check_dir)
					continue;

				for (Square next{ sq + dir }; IsInside<BoardType>(next, next - dir); next += dir)
				{
					auto p = board.GetPiece(next);
					if (p == Empty)
						continue;

					if ((p == ctraits::Rook || p == ctraits::Queen) && !_attackDetector.IsPinned(next))
						moves.emplace_back(next, sq, MoveType::Quite);

					break;
				}
			}

			for (Direction dir : BoardType::DiagonalDirections)
			{
				if (dir == check_dir)
					continue;

				for (Square next{ sq + dir }; IsInside<BoardType>(next, next - dir); next += dir)
				{
					auto p = board.GetPiece(next);
					if (p == Empty)
						continue;

					if ((p == ctraits::Bishop || p == ctraits::Queen) && !_attackDetector.IsPinned(next))
						moves.emplace_back(next, sq, MoveType::Quite);

					break;
				}
			}

			for (Direction dir : BoardType::KnightJumps)
			{
				if (Square next{ sq + dir }; IsInside<BoardType>(next, sq) && board.GetPiece(next) == ctraits::Knight && !_attackDetector.IsPinned(next))
					moves.emplace_back(next, sq, MoveType::Quite);
			}

			if (sq >= bctraits::ValidPawnMoveSquares[0] && sq <= bctraits::ValidPawnMoveSquares[1])
			{
				Square next{ sq + bctraits::PawnReverseMoveDirection };

				if (board.GetPiece(next) == ctraits::Pawn && !_attackDetector.IsPinned(next))
				{
					if (BoardType::GetRank(sq) == bctraits::PawnPromotionRank)
					{
						moves.emplace_back(next, sq, MoveType::Queen_Promotion);
						moves.emplace_back(next, sq, MoveType::Rook_Promotion);
						moves.emplace_back(next, sq, MoveType::Bishop_Promotion);
						moves.emplace_back(next, sq, MoveType::Knight_Promotion);
					}
					else
					{
						moves.emplace_back(next, sq, MoveType::Quite);
					}
				}
				else if (board.GetPiece(next) == Empty)
				{
					next += bctraits::PawnReverseMoveDirection;
					if (board.GetPiece(next) == ctraits::Pawn && !_attackDetector.IsPinned(next) && BoardType::GetRank(next) == bctraits::PawnDoublePushRank)
						moves.emplace_back(next, sq, MoveType::Double_Pawn_Push);
				}
			}
		}
	}

	template <typename BoardType>
	template <Color SideToMove>
	void MoveGenerator<BoardType>::GenerateKnightMoves(const BoardType& board, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;
		auto [first, last] = board.GetPiecePositions<SideToMove>(ctraits::Knight);

		for (; first != last; first++)
		{
			if (_attackDetector.IsPinned(first->second))
				continue;

			for (Direction dir : BoardType::KnightJumps)
			{
				Square next{ first->second + dir };
				if (IsInside<BoardType>(next, first->second))
				{
					auto p = board.GetPiece(next);
					if (p == Empty)
						moves.emplace_back(first->second, next);
					else if (color::get_color(p) != SideToMove)
						moves.emplace_back(first->second, next, MoveType::Capture);
				}
			}
		}
	}

	template <typename BoardType>
	template <Color SideToMove>
	void MoveGenerator<BoardType>::GenerateStraightMoves(const BoardType& board, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;

		for (int i = 0; i < 2; i++)
		{
			auto [first, last] = board.GetPiecePositions<SideToMove>(ctraits::StraightMovingPieces[i]);
			for (; first != last; first++)
			{
				if (Direction pin_dir = _attackDetector.GetPinDirection(first->second); pin_dir != Empty)
				{
					// Piece is pinned. Piece can only move in the pinned direction or opposite of the pinned direction.
					for (Direction dir : BoardType::StraightDirections)
						if (dir == pin_dir || dir == direction::Reverse(pin_dir))
							GenerateStraightMoves<SideToMove>(board, first->second, dir, moves);
				}
				else
				{
					// Piece is not pinned. Piece is free to move in any direction.
					for (Direction dir : BoardType::StraightDirections)
						GenerateStraightMoves<SideToMove>(board, first->second, dir, moves);
				}
			}
		}
	}

	template <typename BoardType>
	template <Color SideToMove>
	inline void MoveGenerator<BoardType>::GenerateStraightMoves(const BoardType& board, Square pos, Direction dir, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;

		for (Square next{ pos + dir }; IsInside<BoardType>(next, next - dir); next += dir)
		{
			auto p = board.GetPiece(next);
			if (p == Empty)
				moves.emplace_back(pos, next);
			else
			{
				if (color::get_color(p) != SideToMove)
					moves.emplace_back(pos, next, MoveType::Capture);
				break;
			}
		}
	}

	template <typename BoardType>
	template <Color SideToMove>
	void MoveGenerator<BoardType>::GenerateDiagonalMoves(const BoardType& board, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;

		for (int i = 0; i < 2; i++)
		{
			auto [first, last] = board.GetPiecePositions<SideToMove>(ctraits::DiagonallyMovingPieces[i]);

			for (; first != last; first++)
			{
				if (Direction pin_dir = _attackDetector.GetPinDirection(first->second); pin_dir != Empty)
				{
					for (Direction dir : BoardType::DiagonalDirections)
						if (dir == pin_dir || dir == direction::Reverse(pin_dir))
							GenerateDiagonalMoves<SideToMove>(board, first->second, dir, moves);
				}
				else
				{
					for (Direction dir : BoardType::DiagonalDirections)
						GenerateDiagonalMoves<SideToMove>(board, first->second, dir, moves);
				}
			}
		}
	}

	template <typename BoardType>
	template <Color SideToMove>
	void MoveGenerator<BoardType>::GenerateDiagonalMoves(const BoardType& board, Square pos, Direction dir, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;

		for (Square next{ pos + dir }; IsInside<BoardType>(next, next - dir); next += dir)
		{
			auto p = board.GetPiece(next);
			if (p == Empty)
				moves.emplace_back(pos, next);
			else
			{
				if (color::get_color(p) != SideToMove)
					moves.emplace_back(pos, next, MoveType::Capture);
				break;
			}
		}
	}

	template <typename BoardType>
	template <Color SideToMove>
	void MoveGenerator<BoardType>::GeneratePawnMoves(const BoardType& board, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;
		using octraits = traits::color_traits<ctraits::Opposite>;
		using bctraits = traits::board_color_traits<BoardType, SideToMove>;

		auto [first, last] = board.GetPiecePositions<SideToMove>(ctraits::Pawn);
		for (; first != last; first++)
		{
			auto pin_dir = _attackDetector.GetPinDirection(first->second);

			// One square forward, two square forward moves
			if (Square next{ first->second + bctraits::PawnMoveDirection };
				board.GetPiece(next) == Empty &&
				(pin_dir == Empty || pin_dir == bctraits::PawnMoveDirection || pin_dir == bctraits::PawnReverseMoveDirection))
			{
				if (Rank r = BoardType::GetRank(next); r == bctraits::PawnPromotionRank)
				{
					moves.emplace_back(first->second, next, MoveType::Queen_Promotion);
					moves.emplace_back(first->second, next, MoveType::Rook_Promotion);
					moves.emplace_back(first->second, next, MoveType::Knight_Promotion);
					moves.emplace_back(first->second, next, MoveType::Bishop_Promotion);
				}
				else
					moves.emplace_back(first->second, next);

				if (Square pos{ next + bctraits::PawnMoveDirection };
					BoardType::GetRank(first->second) == bctraits::PawnDoublePushRank && board.GetPiece(pos) == Empty)
					moves.emplace_back(first->second, pos, MoveType::Double_Pawn_Push);
			}

			// Captures
			for (int i{ 0 }; i < 2; i++)
			{
				if (Square next{ first->second + bctraits::PawnAttackDirections[i] };
					IsInside<BoardType>(next, first->second) &&
					board.GetPiece(next) != Empty &&
					color::get_color(board.GetPiece(next)) != SideToMove &&
					(pin_dir == Empty || pin_dir == bctraits::PawnAttackDirections[i]))
				{
					if (BoardType::GetRank(next) == bctraits::PawnPromotionRank)
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

		Square en_passant = board.GetEnPassantSquare();
		if (en_passant == Empty)
			return;

		for (int i{ 0 }; i < 2; i++)
		{
			Square pos{ en_passant + bctraits::PawnReverseAttackDirections[i] };
			if (!IsInside<BoardType>(en_passant, pos) || board.GetPiece(pos) != ctraits::Pawn)
				continue;

			if (Direction pin_dir = _attackDetector.GetPinDirection(pos); pin_dir != Empty && pin_dir != bctraits::PawnAttackDirections[i])
				continue;

			Square king_pos = board.GetKingPosition<SideToMove>();
			if (BoardType::GetRank(king_pos) != BoardType::GetRank(pos))
				moves.emplace_back(pos, en_passant, MoveType::En_Passant_Capture);
			else
			{
				Square ppos{ en_passant + bctraits::PawnReverseMoveDirection };
				Direction dir = king_pos > pos ? BoardType::W : BoardType::E;
				bool make_move{ true };
				for (Square next{ king_pos + dir }; IsInside<BoardType>(next, next - dir); next += dir)
				{
					auto p = board.GetPiece(next);
					if (next == pos || next == ppos || p == Empty)
						continue;

					if (p == octraits::Rook || p == octraits::Queen)
						make_move = false;
					break;
				}

				if (make_move)
					moves.emplace_back(pos, en_passant, MoveType::En_Passant_Capture);
			}
		}
	}

/*
#pragma region MoveGenerator_SimpleBoardSpecialization

	template <>
	template <Color SideToMove>
	void MoveGenerator<SimpleBoard>::GenerateKingMoves(const SimpleBoard& board, Square king_pos, MoveList& moves)
	{
		using ctraits = traits::color_traits<SideToMove>;
		using bctraits = traits::board_color_traits<SimpleBoard, SideToMove>;

		auto idx = Connections::KingSquareIndexes[king_pos];
		auto bitfield = Connections::KingConnections[idx++];
		std::uint8_t mask = 1;

		Square next{ Empty };
		for (Direction dir : SimpleBoard::AllDirections)
		{
			if (bitfield & mask) 
			{
				next = Connections::KingConnections[idx++];

				if (_attackDetector.CanKingMoveInThisDirection<ctraits::Opposite>(board, dir) &&
					!_attackDetector.IsUnderAttack<ctraits::Opposite>(board, next))
				{
					auto p = board.GetPiece(next);
					if (p == Empty)
						moves.emplace_back(king_pos, next);
					else if (color::get_color(p) != SideToMove)
						moves.emplace_back(king_pos, next, MoveType::Capture);
				}
			}
			mask <<= 1;
		}

		if (_attackDetector.GetNumberOfChecks() > 0)
			return;

		if (board.QueryCastling(ctraits::KingSideCastling) &&
			board.GetPiece(bctraits::KingSideCastleCheckSquares[0]) == Empty &&
			board.GetPiece(bctraits::KingSideCastleCheckSquares[1]) == Empty &&
			!_attackDetector.IsUnderAttack<ctraits::Opposite>(board, bctraits::KingSideCastleCheckSquares[0]) &&
			!_attackDetector.IsUnderAttack<ctraits::Opposite>(board, bctraits::KingSideCastleCheckSquares[1]))
			moves.emplace_back(king_pos, bctraits::KingSideCastleCheckSquares[1], MoveType::King_Side_Castle);

		if (board.QueryCastling(ctraits::QueenSideCastling) &&
			board.GetPiece(bctraits::QueenSideCastleCheckSquares[0]) == Empty &&
			board.GetPiece(bctraits::QueenSideCastleCheckSquares[1]) == Empty &&
			board.GetPiece(bctraits::QueenSideCastleCheckSquares[2]) == Empty &&
			!_attackDetector.IsUnderAttack<ctraits::Opposite>(board, bctraits::QueenSideCastleCheckSquares[0]) &&
			!_attackDetector.IsUnderAttack<ctraits::Opposite>(board, bctraits::QueenSideCastleCheckSquares[1]))
			moves.emplace_back(king_pos, bctraits::QueenSideCastleCheckSquares[1], MoveType::Queen_Side_Castle);
	}
	
	template <>
	template <Color SideToMove>
	void MoveGenerator<SimpleBoard>::GenerateCheckerCapturingMoves(const SimpleBoard& board, MoveList& moves)
	{
		using ctraits = traits::color_traits<SideToMove>;
		using octraits = traits::color_traits<ctraits::Opposite>;
		using bctraits = traits::board_color_traits<SimpleBoard, SideToMove>;

		const auto& check = _attackDetector.GetFirstCheck();
		Direction check_dir = direction::Reverse(check.checker_dir); // checker to king direction
		
		auto dir_idx = Connections::StraightSquareIndexes[check.checker];
		auto idx = dir_idx + 4;
		for (Direction dir : SimpleBoard::StraightDirections)
		{
			// checker cannot be captured in check-direction, this can be done by the checked king-only which is 
			// covered when generating the king-moves.
			if (dir == check_dir)
			{
				idx += Connections::StraightConnections[dir_idx];
				dir_idx++;
				continue;
			}

			for (int i = 0; i < Connections::StraightConnections[dir_idx]; i++, idx++)
			{
				const auto& next = Connections::StraightConnections[idx];
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if ((p == ctraits::Rook || p == ctraits::Queen) && !_attackDetector.IsPinned(next))
					moves.emplace_back(next, check.checker, MoveType::Capture);

				break;
			}
		}

		dir_idx = Connections::DiagonalSquareIndexes[check.checker];
		idx = dir_idx + 4;
		for (Direction dir : SimpleBoard::DiagonalDirections)
		{
			// checker cannot be captured in check-direction, this can be done by the checked king-only which is 
			// covered when generating the king-moves.
			if (dir == check_dir)
			{
				idx += Connections::DiagonalConnections[dir_idx];
				dir_idx++;
				continue;
			}

			for (int i = 0; i < Connections::DiagonalConnections[dir_idx]; i++, idx++)
			{
				const auto& next = Connections::DiagonalConnections[idx];
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if ((p == ctraits::Bishop || p == ctraits::Queen) && !_attackDetector.IsPinned(next))
					moves.emplace_back(next, check.checker, MoveType::Capture);

				break;
			}
		}

		idx = Connections::KnightSquareIndexes[check.checker];
		auto num_knight_jumps = Connections::KnightConnections[idx];
		for (int i = 0; i < num_knight_jumps; i++)
		{
			const auto& next = Connections::KnightConnections[++idx];
			if (board.GetPiece(next) == ctraits::Knight && !_attackDetector.IsPinned(next))
				moves.emplace_back(next, check.checker, MoveType::Capture);
		}

		if (check.checker >= bctraits::ValidPawnMoveSquares[0] && check.checker <= bctraits::ValidPawnMoveSquares[1]) 
		{
			idx = Connections::PawnSquareIndexes[check.checker];
			if constexpr (SideToMove == color::White) 
				idx += 4;

			for (int i = idx + 2; i < idx + 4; i++) 
			{
				const auto& next = Connections::PawnConnections[i];
				if (next != Empty && board.GetPiece(next) == ctraits::Pawn && !_attackDetector.IsPinned(next))
				{
					if (SimpleBoard::GetRank(check.checker) == bctraits::PawnPromotionRank)
					{
						moves.emplace_back(next, check.checker, MoveType::Queen_Promotion_Capture);
						moves.emplace_back(next, check.checker, MoveType::Rook_Promotion_Capture);
						moves.emplace_back(next, check.checker, MoveType::Bishop_Promotion_Capture);
						moves.emplace_back(next, check.checker, MoveType::Knight_Promotion_Capture);
					}
					else
					{
						moves.emplace_back(next, check.checker, MoveType::Capture);
					}
				}
			}

			if (Square enpassant_sq{ board.GetEnPassantSquare() }; enpassant_sq != Empty && board.GetPiece(check.checker) == octraits::Pawn)
			{
				idx = Connections::PawnSquareIndexes[enpassant_sq];
				if constexpr (SideToMove == color::White)
					idx += 4;

				for (int i = idx + 2; i < idx + 4; i++) 
				{
					const auto& next = Connections::PawnConnections[i];
					if (next != Empty && board.GetPiece(next) == ctraits::Pawn && !_attackDetector.IsPinned(next))
						moves.emplace_back(next, enpassant_sq, MoveType::En_Passant_Capture);
				}
			}
		}
	}
	
	template <>
	template <Color SideToMove>
	void MoveGenerator<SimpleBoard>::GenerateCheckBlockingMoves(const SimpleBoard& board, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;
		using bctraits = traits::board_color_traits<SimpleBoard, SideToMove>;

		const auto& check = _attackDetector.GetFirstCheck();
		if (check.checker_dist < 2)
			return;

		Direction check_dir = direction::Reverse(check.checker_dir); // checker to king direction

		Square sq{ check.checker + check_dir };
		for (Distance i{ 1 }; i < check.checker_dist; i++, sq += check_dir)
		{
			auto dir_idx = Connections::StraightSquareIndexes[sq];
			auto idx = dir_idx + 4;
			for (Direction dir : SimpleBoard::StraightDirections)
			{
				// check cannot be blocked by moving a piece in check-direction.
				if (dir == check_dir)
				{
					idx += Connections::StraightConnections[dir_idx];
					dir_idx++;
					continue;
				}

				for (int i = 0; i < Connections::StraightConnections[dir_idx]; i++, idx++)
				{
					const auto& next = Connections::StraightConnections[idx];
					auto p = board.GetPiece(next);
					if (p == Empty)
						continue;

					if ((p == ctraits::Rook || p == ctraits::Queen) && !_attackDetector.IsPinned(next))
						moves.emplace_back(next, sq, MoveType::Quite);

					break;
				}
			}

			dir_idx = Connections::DiagonalSquareIndexes[sq];
			idx = dir_idx + 4;
			for (Direction dir : SimpleBoard::DiagonalDirections)
			{
				// check cannot be blocked by moving a piece in check-direction.
				if (dir == check_dir)
				{
					idx += Connections::DiagonalConnections[dir_idx];
					dir_idx++;
					continue;
				}

				for (int i = 0; i < Connections::DiagonalConnections[dir_idx]; i++, idx++)
				{
					const auto& next = Connections::DiagonalConnections[idx];
					auto p = board.GetPiece(next);
					if (p == Empty)
						continue;

					if ((p == ctraits::Bishop || p == ctraits::Queen) && !_attackDetector.IsPinned(next))
						moves.emplace_back(next, sq, MoveType::Quite);

					break;
				}
			}

			idx = Connections::KnightSquareIndexes[sq];
			auto num_knight_jumps = Connections::KnightConnections[idx];
			for (int i = 0; i < num_knight_jumps; i++)
			{
				const auto& next = Connections::KnightConnections[++idx];
				if (board.GetPiece(next) == ctraits::Knight && !_attackDetector.IsPinned(next))
					moves.emplace_back(next, sq, MoveType::Quite);
			}

			if (sq >= bctraits::ValidPawnMoveSquares[0] && sq <= bctraits::ValidPawnMoveSquares[1])
			{
				idx = Connections::PawnSquareIndexes[sq];
				if constexpr (SideToMove == color::White)
					idx += 4;

				const auto& next = Connections::PawnConnections[idx];
				if (board.GetPiece(next) == ctraits::Pawn && !_attackDetector.IsPinned(next)) 
				{
					if (SimpleBoard::GetRank(sq) == bctraits::PawnPromotionRank)
					{
						moves.emplace_back(next, sq, MoveType::Queen_Promotion);
						moves.emplace_back(next, sq, MoveType::Rook_Promotion);
						moves.emplace_back(next, sq, MoveType::Bishop_Promotion);
						moves.emplace_back(next, sq, MoveType::Knight_Promotion);
					}
					else
					{
						moves.emplace_back(next, sq, MoveType::Quite);
					}
				}
				else if (board.GetPiece(next) == Empty) 
				{
					// next += bctraits::PawnReverseMoveDirection;
					// if (board.GetPiece(next) == ctraits::Pawn && !_attackDetector.IsPinned(next) && BoardType::GetRank(next) == bctraits::PawnDoublePushRank)
					//	moves.emplace_back(next, sq, MoveType::Double_Pawn_Push);
				}
			}
		}
	}

	template <>
	template <Color SideToMove>
	void MoveGenerator<SimpleBoard>::GenerateKnightMoves(const SimpleBoard& board, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;

		auto [first, last] = board.GetPiecePositions<SideToMove>(ctraits::Knight);
		for (; first != last; first++)
		{
			if (_attackDetector.IsPinned(first->second))
				continue;

			auto idx = Connections::KnightSquareIndexes[first->second];
			auto num_knight_jumps = Connections::KnightConnections[idx];
			for (int i = 0; i < num_knight_jumps; i++) 
			{
				const auto& next = Connections::KnightConnections[++idx];
				auto p = board.GetPiece(next);
				if (p == Empty)
					moves.emplace_back(first->second, next);
				else if (color::get_color(p) != SideToMove)
					moves.emplace_back(first->second, next, MoveType::Capture);
			}
		}
	}

	template <>
	template <Color SideToMove>
	void MoveGenerator<SimpleBoard>::GenerateStraightMoves(const SimpleBoard& board, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;

		for (int i = 0; i < 2; i++) 
		{
			auto [first, last] = board.GetPiecePositions<SideToMove>(ctraits::StraightMovingPieces[i]);

			for (; first != last; first++) 
			{
				auto dir_idx = Connections::StraightSquareIndexes[first->second];
				auto idx = dir_idx + 4;

				if (Direction pin_dir = _attackDetector.GetPinDirection(first->second); pin_dir != Empty) 
				{
					// Piece is pinned. Piece can only move in the pinned direction or opposite of the pinned direction.
					for (Direction dir : SimpleBoard::StraightDirections)
					{
						if (dir == pin_dir || dir == direction::Reverse(pin_dir)) 
						{
							for (int i = 0; i < Connections::StraightConnections[dir_idx]; i++, idx++) 
							{
								const auto& next = Connections::StraightConnections[idx];
								auto p = board.GetPiece(next);
								if (p == Empty)
									moves.emplace_back(first->second, next);
								else 
								{
									if (color::get_color(p) != SideToMove)
										moves.emplace_back(first->second, next, MoveType::Capture);

									idx += (Connections::StraightConnections[dir_idx] - i - 1);
									break;
								}
							}
						}
						else 
						{
							idx += Connections::StraightConnections[dir_idx];
						}
						dir_idx++;
					}
				}
				else 
				{
					// Piece is not pinned. Piece is free to move in any direction.
					for (Direction dir : SimpleBoard::StraightDirections)
					{
						for (int i = 0; i < Connections::StraightConnections[dir_idx]; i++, idx++)
						{
							const auto& next = Connections::StraightConnections[idx];
							auto p = board.GetPiece(next);
							if (p == Empty)
								moves.emplace_back(first->second, next);
							else
							{
								if (color::get_color(p) != SideToMove)
									moves.emplace_back(first->second, next, MoveType::Capture);

								idx += (Connections::StraightConnections[dir_idx] - i - 1);
								break;
							}
						}
						dir_idx++;
					}
				}
			}
		}
	}

	template <>
	template <Color SideToMove>
	void MoveGenerator<SimpleBoard>::GenerateDiagonalMoves(const SimpleBoard& board, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;

		for (int i = 0; i < 2; i++)
		{
			auto [first, last] = board.GetPiecePositions<SideToMove>(ctraits::DiagonallyMovingPieces[i]);

			for (; first != last; first++)
			{
				auto dir_idx = Connections::DiagonalSquareIndexes[first->second];
				auto idx = dir_idx + 4;

				if (Direction pin_dir = _attackDetector.GetPinDirection(first->second); pin_dir != Empty)
				{
					// Piece is pinned. Piece can only move in the pinned direction or opposite of the pinned direction.
					for (Direction dir : SimpleBoard::DiagonalDirections)
					{
						if (dir == pin_dir || dir == direction::Reverse(pin_dir)) 
						{
							for (int i = 0; i < Connections::DiagonalDirections[dir_idx]; i++, idx++)
							{
								const auto& next = Connections::DiagonalDirections[idx];
								auto p = board.GetPiece(next);
								if (p == Empty)
									moves.emplace_back(first->second, next);
								else
								{
									if (color::get_color(p) != SideToMove)
										moves.emplace_back(first->second, next, MoveType::Capture);

									idx += (Connections::DiagonalDirections[dir_idx] - i - 1);
									break;
								}
							}
						}
						else
						{
							idx += Connections::DiagonalConnections[dir_idx];
						}
						dir_idx++;
					}
				}
				else
				{
					// Piece is not pinned. Piece is free to move in any direction.
					for (Direction dir : SimpleBoard::DiagonalDirections)
					{
						for (int i = 0; i < Connections::DiagonalDirections[dir_idx]; i++, idx++)
						{
							const auto& next = Connections::DiagonalDirections[idx];
							auto p = board.GetPiece(next);
							if (p == Empty)
								moves.emplace_back(first->second, next);
							else
							{
								if (color::get_color(p) != SideToMove)
									moves.emplace_back(first->second, next, MoveType::Capture);

								idx += (Connections::DiagonalDirections[dir_idx] - i - 1);
								break;
							}
						}
						dir_idx++;
					}
				}
			}
		}
	}

	template <>
	template <Color SideToMove>
	void MoveGenerator<SimpleBoard>::GeneratePawnMoves(const SimpleBoard& board, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;
		using octraits = traits::color_traits<ctraits::Opposite>;
		using bctraits = traits::board_color_traits<SimpleBoard, SideToMove>;

		auto [first, last] = board.GetPiecePositions<SideToMove>(ctraits::Pawn);
		//for (; first != last; first++)
		//{
		//	auto pin_dir = _attackDetector.GetPinDirection(first->second);

		//	auto idx = Connections::PawnSquareIndexes[sq];
		//	if constexpr (SideToMove == color::Black)
		//		idx += 4;

		//	// One square forward, two square forward moves
		//	if (const auto& next = Connections::PawnConnections[idx]; 
		//		board.GetPiece(next) == Empty &&
		//	    (pin_dir == Empty || pin_dir == bctraits::PawnMoveDirection || pin_dir == bctraits::PawnReverseMoveDirection))
		//	{
		//		const auto& nextnext = Connections::PawnConnections[idx + 1];
		//		if (nextnext == conn::PromotionFlag)
		//		{
		//			moves.emplace_back(first->second, next, MoveType::Queen_Promotion);
		//			moves.emplace_back(first->second, next, MoveType::Rook_Promotion);
		//			moves.emplace_back(first->second, next, MoveType::Knight_Promotion);
		//			moves.emplace_back(first->second, next, MoveType::Bishop_Promotion);
		//		}
		//		else
		//			moves.emplace_back(first->second, next);

		//		if (nextnext != Empty && board.GetPiece(nextnext) == Empty)
		//			moves.emplace_back(first->second, nextnext, MoveType::Double_Pawn_Push);
		//	}

		//	// Captures
		//	for (int i = 0; i < 2; i++)
		//	{
		//		if (const auto& next = Connections::PawnConnections[idx+2+i];
		//			board.GetPiece(next) != Empty &&
		//			color::get_color(board.GetPiece(next)) != SideToMove &&
		//			(pin_dir == Empty || pin_dir == bctraits::PawnAttackDirections[i]))
		//		{
		//			const auto& nextnext = Connections::PawnConnections[idx + 1];
		//			if (nextnext == conn::PromotionFlag)
		//			{
		//				moves.emplace_back(first->second, next, MoveType::Queen_Promotion_Capture);
		//				moves.emplace_back(first->second, next, MoveType::Rook_Promotion_Capture);
		//				moves.emplace_back(first->second, next, MoveType::Knight_Promotion_Capture);
		//				moves.emplace_back(first->second, next, MoveType::Bishop_Promotion_Capture);
		//			}
		//			else
		//				moves.emplace_back(first->second, next, MoveType::Capture);
		//		}
		//	}
		//}

		Square en_passant = board.GetEnPassantSquare();
		if (en_passant == Empty)
			return;

		//for (int i{ 0 }; i < 2; i++)
		//{
		//	Square pos{ en_passant + bctraits::PawnReverseAttackDirections[i] };
		//	if (!IsInside<SimpleBoard>(en_passant, pos) || board.GetPiece(pos) != ctraits::Pawn)
		//		continue;

		//	if (Direction pin_dir = _attackDetector.GetPinDirection(pos); pin_dir != Empty && pin_dir != bctraits::PawnAttackDirections[i])
		//		continue;

		//	Square king_pos = board.GetKingPosition<SideToMove>();
		//	if (BoardType::GetRank(king_pos) != BoardType::GetRank(pos))
		//		moves.emplace_back(pos, en_passant, MoveType::En_Passant_Capture);
		//	else
		//	{
		//		Square ppos{ en_passant + bctraits::PawnReverseMoveDirection };
		//		Direction dir = king_pos > pos ? BoardType::W : BoardType::E;
		//		bool make_move{ true };
		//		for (Square next{ king_pos + dir }; IsInside<BoardType>(next, next - dir); next += dir)
		//		{
		//			auto p = board.GetPiece(next);
		//			if (next == pos || next == ppos || p == Empty)
		//				continue;

		//			if (p == octraits::Rook || p == octraits::Queen)
		//				make_move = false;
		//			break;
		//		}

		//		if (make_move)
		//			moves.emplace_back(pos, en_passant, MoveType::En_Passant_Capture);
		//	}
		//}
	
	}
#pragma endregion
*/

}