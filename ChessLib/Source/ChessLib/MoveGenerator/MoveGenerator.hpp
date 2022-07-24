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
	
		template<Color SideToMove, bool Check>
		void GenerateKingMoves(const BoardType& board, Square king_pos, MoveList& moves);
	
		template<Color SideToMove>
		void GenerateCheckEliminatingMoves(const BoardType& board, MoveList& moves);

		template<Color SideToMove, Direction Dir>
		void GenerateToSquareSlidingPieceMoves(const BoardType& board, Square sq, MoveType mt, MoveList& moves) const;

		template<Color SideToMove>
		void GenerateToSquareKnightMoves(const BoardType& board, Square sq, MoveType mt, MoveList& moves) const;

		template<Color SideToMove, bool IsCapture>
		void GenerateToSquarePawnMoves(const BoardType& board, Square sq, MoveList& moves) const;

		template<Color SideToMove, typename DirectionArray>
		void GenerateSlidingPieceMoves(const BoardType& board, Piece moving_piece, const DirectionArray& dir_array, MoveList& moves) const;

		template<Color SideToMove>
		void GenerateSlidingPieceMoves(const BoardType& board, Square pos, Direction dir, MoveList& moves) const;

		template<Color SideToMove>
		void GenerateKnightMoves(const BoardType& board, MoveList& moves) const;
		
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

		auto num_checks = _attackDetector.GetNumberOfChecks();
		if (num_checks == 0)
		{
			GenerateKingMoves<SideToMove, false>(board, king_pos, moves);
			GenerateSlidingPieceMoves<SideToMove>(board, ctraits::Rook, BoardType::StraightDirections, moves);
			GenerateSlidingPieceMoves<SideToMove>(board, ctraits::Queen, BoardType::AllDirections, moves);
			GenerateSlidingPieceMoves<SideToMove>(board, ctraits::Bishop, BoardType::DiagonalDirections, moves);
			GenerateKnightMoves<SideToMove>(board, moves);
			GeneratePawnMoves<SideToMove>(board, moves);
		}
		else 
		{
			GenerateKingMoves<SideToMove, true>(board, king_pos, moves);
			if (num_checks == 1) 
				GenerateCheckEliminatingMoves<SideToMove>(board, moves);
		}	
	}

	template <typename BoardType>
	template <Color SideToMove, bool Check>
	void MoveGenerator<BoardType>::GenerateKingMoves(const BoardType& board, Square king_pos, MoveList& moves)
	{
		using ctraits = traits::color_traits<SideToMove>;
		using bctraits = traits::board_color_traits<BoardType, SideToMove>;

		Square next{ Empty };
		for (Direction dir : BoardType::AllDirections)
		{
			if constexpr (Check)
			{
				if (!_attackDetector.VerifyKingMovingDirection<ctraits::Opposite>(board, dir))
					continue;
			}

			next = king_pos + dir;
			if (IsInside<BoardType>(next, king_pos) && !_attackDetector.IsUnderAttack<ctraits::Opposite>(board, next))
			{
				auto p = board.GetPiece(next);
				if (p == Empty)
					moves.emplace_back(king_pos, next);
				else if (color::get_color(p) != SideToMove)
					moves.emplace_back(king_pos, next, MoveType::Capture);
			}
		}

		if constexpr (Check)
			return;
		else 
		{
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
	}

	template <typename BoardType>
	template<Color SideToMove>
	void MoveGenerator<BoardType>::GenerateCheckEliminatingMoves(const BoardType& board, MoveList& moves)
	{
		// Checker can only be captured in the reverse check direction by the king itself. 
		// Check direction is from the checking piece to the king.
		// Note that we are reversing the direction here because the direction is also inverted in the ToSquare methods.
		#define Capture(Dir, s, mt) \
		if (Dir != check.attack_dir) \
			GenerateToSquareSlidingPieceMoves<SideToMove, Dir>(board, s, mt, moves) 

		// Check cannot be blocked by a piece that comes from the checking or reverse checking direction.
		#define Block(Dir, s, mt) \
		if (Dir != check.attack_dir && Dir != direction::Reverse(check.attack_dir)) \
			GenerateToSquareSlidingPieceMoves<SideToMove, Dir>(board, s, mt, moves) 

		// Caputure the checking piece
		const auto& check = _attackDetector.GetFirstCheck();
		Capture(BoardType::N, check.attacker, MoveType::Capture);
		Capture(BoardType::NE, check.attacker, MoveType::Capture);
		Capture(BoardType::E, check.attacker, MoveType::Capture);
		Capture(BoardType::SE, check.attacker, MoveType::Capture);
		Capture(BoardType::S, check.attacker, MoveType::Capture);
		Capture(BoardType::SW, check.attacker, MoveType::Capture);
		Capture(BoardType::W, check.attacker, MoveType::Capture);
		Capture(BoardType::NW, check.attacker, MoveType::Capture);
		GenerateToSquareKnightMoves<SideToMove>(board, check.attacker, MoveType::Capture, moves);
		GenerateToSquarePawnMoves<SideToMove, true>(board, check.attacker, moves);

		using ctraits = traits::color_traits<SideToMove>;
		using octraits = traits::color_traits<ctraits::Opposite>;

		// A check with a knight or pawn cannot be blocked.
		auto checkerPiece = board.GetPiece(check.attacker);
		if (checkerPiece == octraits::Knight || checkerPiece == octraits::Pawn)
			return;

		Square sq{ check.attacker + check.attack_dir };
		while (sq != check.attacked)
		{
			Block(BoardType::N, sq, MoveType::Quite);
			Block(BoardType::NE, sq, MoveType::Quite);
			Block(BoardType::E, sq, MoveType::Quite);
			Block(BoardType::SE, sq, MoveType::Quite);
			Block(BoardType::S, sq, MoveType::Quite);
			Block(BoardType::SW, sq, MoveType::Quite);
			Block(BoardType::W, sq, MoveType::Quite);
			Block(BoardType::NW, sq, MoveType::Quite);
			GenerateToSquareKnightMoves<SideToMove>(board, sq, MoveType::Quite, moves);
			GenerateToSquarePawnMoves<SideToMove, false>(board, sq, moves);
			sq += check.attack_dir;
		}
	}

	template <typename BoardType>
	template <Color SideToMove, Direction Dir>
	void MoveGenerator<BoardType>::GenerateToSquareSlidingPieceMoves(const BoardType& board, Square sq, MoveType mt, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;

		Piece p{ Empty };
		for (Square next{ sq + Dir }; IsInside<BoardType>(next, next - Dir); next += Dir)
		{
			p = board.GetPiece(next);
			if (p == Empty)
				continue;

			if constexpr (Dir == BoardType::N || Dir == BoardType::E || Dir == BoardType::S || Dir == BoardType::W)
			{
				if ((p == ctraits::Rook || p == ctraits::Queen) && !_attackDetector.IsPinned(next))
					moves.emplace_back(next, sq, mt);
				return;
			}
			else if constexpr (Dir == BoardType::NE || Dir == BoardType::SE || Dir == BoardType::SW || Dir == BoardType::NW)
			{
				if ((p == ctraits::Bishop || p == ctraits::Queen) && !_attackDetector.IsPinned(next))
					moves.emplace_back(next, sq, mt);
				break;
			}
		}
	}

	template <typename BoardType>
	template<Color SideToMove>
	void MoveGenerator<BoardType>::GenerateToSquareKnightMoves(const BoardType& board, Square sq, MoveType mt, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;
		for (Direction dir : BoardType::KnightJumps)
			if (Square next{ sq + dir }; IsInside<BoardType>(next, sq) && board.GetPiece(next) == ctraits::Knight && !_attackDetector.IsPinned(next))
				moves.emplace_back(next, sq, mt);
	}

	template <typename BoardType>
	template<Color SideToMove, bool IsCapture>
	void MoveGenerator<BoardType>::GenerateToSquarePawnMoves(const BoardType& board, Square sq, MoveList& moves) const
	{
		using bctraits = traits::board_color_traits<BoardType, SideToMove>;
		if (sq < bctraits::ValidPawnMoveSquares[0] || sq > bctraits::ValidPawnMoveSquares[1])
			return;

		using ctraits = traits::color_traits<SideToMove>;
		if constexpr (IsCapture) 
		{
			for (Direction dir : bctraits::PawnReverseAttackDirections)
			{
				if (Square next{ sq + dir }; IsInside<BoardType>(next, sq) && board.GetPiece(next) == ctraits::Pawn && !_attackDetector.IsPinned(next))
				{
					if (BoardType::GetRank(sq) == bctraits::PromotedPawnRank)
					{
						moves.emplace_back(next, sq, MoveType::Queen_Promotion_Capture);
						moves.emplace_back(next, sq, MoveType::Rook_Promotion_Capture);
						moves.emplace_back(next, sq, MoveType::Bishop_Promotion_Capture);
						moves.emplace_back(next, sq, MoveType::Knight_Promotion_Capture);
					}
					else
					{
						moves.emplace_back(next, sq, MoveType::Capture);
					}
				}
			}

			using octraits = traits::color_traits<ctraits::Opposite>;
			if (Square enpassant_sq{ board.GetEnPassantSquare() }; enpassant_sq != Empty && board.GetPiece(sq) == octraits::Pawn)
			{
				for (Direction dir : bctraits::PawnReverseAttackDirections)
					if (Square next{ enpassant_sq + dir }; IsInside<BoardType>(enpassant_sq, next) && board.GetPiece(next) == ctraits::Pawn && !_attackDetector.IsPinned(next))
						moves.emplace_back(next, enpassant_sq, MoveType::En_Passant_Capture);
			}
		}
		else 
		{
			Square next{ sq + bctraits::PawnReverseMoveDirection };

			if (board.GetPiece(next) == ctraits::Pawn && !_attackDetector.IsPinned(next))
			{
				if (BoardType::GetRank(sq) == bctraits::PromotedPawnRank)
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
				if (BoardType::GetRank(next) == bctraits::PawnDoublePushRank && board.GetPiece(next) == ctraits::Pawn && !_attackDetector.IsPinned(next))
					moves.emplace_back(next, sq, MoveType::Double_Pawn_Push);
			}
		}
	}

	template <typename BoardType>
	template<Color SideToMove, typename DirectionArray>
	void MoveGenerator<BoardType>::GenerateSlidingPieceMoves(const BoardType& board, Piece moving_piece, const DirectionArray& dir_array, MoveList& moves) const
	{
		auto [first, last] = board.GetPiecePositions<SideToMove>(moving_piece);
		for (; first != last; first++)
		{
			if (Direction pin_dir = _attackDetector.GetPinDirection(first->second); pin_dir != Empty)
			{
				// Piece is pinned. Piece can only move in the pinned direction or opposite of the pinned direction.
				for (Direction dir : dir_array)
					if (dir == pin_dir || dir == direction::Reverse(pin_dir))
						GenerateSlidingPieceMoves<SideToMove>(board, first->second, dir, moves);
			}
			else
			{
				// Piece is not pinned. Piece is free to move in any direction.
				for (Direction dir : dir_array)
					GenerateSlidingPieceMoves<SideToMove>(board, first->second, dir, moves);
			}
		}
	}

	template <typename BoardType>
	template<Color SideToMove>
	void MoveGenerator<BoardType>::GenerateSlidingPieceMoves(const BoardType& board, Square pos, Direction dir, MoveList& moves) const
	{
		Piece p{ Empty };
		for (Square next{ pos + dir }; IsInside<BoardType>(next, next - dir); next += dir)
		{
			p = board.GetPiece(next);
			if (p == Empty)
				moves.emplace_back(pos, next);
			else
			{
				if (color::get_color(p) != SideToMove)
					moves.emplace_back(pos, next, MoveType::Capture);
				return;
			}
		}
	}

	template <typename BoardType>
	template <Color SideToMove>
	void MoveGenerator<BoardType>::GenerateKnightMoves(const BoardType& board, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;
		auto [first, last] = board.GetPiecePositions<SideToMove>(ctraits::Knight);

		Square next{ Empty };
		Piece p{ Empty };

		for (; first != last; first++)
		{
			if (_attackDetector.IsPinned(first->second))
				continue;

			for (Direction dir : BoardType::KnightJumps)
			{
				next = first->second + dir;
				if (IsInside<BoardType>(next, first->second))
				{
					p = board.GetPiece(next);
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
				if (Rank r = BoardType::GetRank(next); r == bctraits::PromotedPawnRank)
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
					(pin_dir == Empty || pin_dir == bctraits::PawnReverseAttackDirections[i]))
				{
					if (BoardType::GetRank(next) == bctraits::PromotedPawnRank)
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
			// Check if there is a pawn to do en-passant capture.
			Square pos{ en_passant + bctraits::PawnReverseAttackDirections[i] };
			if (!IsInside<BoardType>(en_passant, pos) || board.GetPiece(pos) != ctraits::Pawn)
				continue;

			// Check that the pawn is not pinned or it is moving in the pin direction.
			Direction pin_dir = _attackDetector.GetPinDirection(pos);
			if (pin_dir != Empty && pin_dir != bctraits::PawnReverseAttackDirections[i])
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
}