#pragma once

#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/MoveGenerator/AttackDetectorConn.hpp>
#include <ChessLib/MoveGenerator/Connections.hpp>

namespace chesslib
{
	template <typename BoardType>
	class MoveGeneratorConn
	{
	public:
		MoveList GenerateMoves(const BoardType& board);

	private:

		AttackDetectorConn<BoardType> _attackDetector;

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

		template<Color SideToMove, Piece MovingPiece>
		void GenerateSlidingPieceMoves(const BoardType& board, MoveList& moves) const;

		template<Color SideToMove, Direction Dir>
		void GenerateSlidingPieceMoves(const BoardType& board, Square pos, MoveList& moves) const;

		template<Color SideToMove>
		void GenerateKnightMoves(const BoardType& board, MoveList& moves) const;

		template<Color SideToMove>
		void GeneratePawnMoves(const BoardType& board, MoveList& moves) const;
	};

	template <typename BoardType>
	MoveList MoveGeneratorConn<BoardType>::GenerateMoves(const BoardType& board)
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
	void MoveGeneratorConn<BoardType>::GenerateMoves(const BoardType& board, MoveList& moves)
	{
		using ctraits = traits::color_traits<SideToMove>;

		auto king_pos = board.GetKingPosition<SideToMove>();
		_attackDetector.ComputeChecksAndPins<ctraits::Opposite>(board, king_pos);

		auto num_checks = _attackDetector.GetNumberOfChecks();
		if (num_checks == 0)
		{
			GenerateKingMoves<SideToMove, false>(board, king_pos, moves);
			GenerateSlidingPieceMoves<SideToMove, ctraits::Rook>(board, moves);
			GenerateSlidingPieceMoves<SideToMove, ctraits::Queen>(board, moves);
			GenerateSlidingPieceMoves<SideToMove, ctraits::Bishop>(board, moves);
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
	void MoveGeneratorConn<BoardType>::GenerateKingMoves(const BoardType& board, Square king_pos, MoveList& moves)
	{
		using ctraits = traits::color_traits<SideToMove>;
		using bctraits = traits::board_color_traits<BoardType, SideToMove>;

		Square next{ Empty };
		conn::View<conn::AllDir> view{ king_pos };
		for (int8_t i = 0; i < view.GetSize(); i++)
		{
			next = view.Next();
			if constexpr (Check)
			{
				if (!_attackDetector.VerifyKingMovingDirection<ctraits::Opposite>(board, king_pos - next))
					continue;
			}

			if (!_attackDetector.IsUnderAttack<ctraits::Opposite>(board, next))
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
	void MoveGeneratorConn<BoardType>::GenerateCheckEliminatingMoves(const BoardType& board, MoveList& moves)
	{
		// Checker cannot be captured/blocked in check - direction. 
		// In the check direction, only checked king can capture the checking piece. This is covered in king moves.
		#define Capture(Dir, s, mt) \
		if (Dir != check.attack_dir) \
			GenerateToSquareSlidingPieceMoves<SideToMove, Dir>(board, s, mt, moves) 

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
	void MoveGeneratorConn<BoardType>::GenerateToSquareSlidingPieceMoves(const BoardType& board, Square sq, MoveType mt, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;

		Square next{ Empty };
		Piece p{ Empty };
		conn::View<Dir> view{ sq };
		for (int8_t i = 0; i < view.GetSize(); i++)
		{
			next = view.Next();
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
	void MoveGeneratorConn<BoardType>::GenerateToSquareKnightMoves(const BoardType& board, Square sq, MoveType mt, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;

		Square next{ Empty };
		conn::View<conn::KnightJumps> view{ sq };
		for (int8_t i = 0; i < view.GetSize(); i++)
		{
			next = view.Next();
			if (board.GetPiece(next) == ctraits::Knight && !_attackDetector.IsPinned(next))
				moves.emplace_back(next, sq, mt);
		}
	}

	template <typename BoardType>
	template<Color SideToMove, bool IsCapture>
	void MoveGeneratorConn<BoardType>::GenerateToSquarePawnMoves(const BoardType& board, Square sq, MoveList& moves) const
	{
		using bctraits = traits::board_color_traits<BoardType, SideToMove>;
		if (sq < bctraits::ValidPawnMoveSquares[0] || sq > bctraits::ValidPawnMoveSquares[1])
			return;

		using ctraits = traits::color_traits<SideToMove>;

		conn::View<conn::PawnDirectionTrait<ctraits::Opposite>::Dir> view{ sq };
		Rank r = view.Next();
		if constexpr (IsCapture)
		{
			Square next[2]{ view.Next(2), view.Next(1) };
			for (int i = 0; i < 2; i++) 
			{
				if (next[i] != Empty && board.GetPiece(next[i]) == ctraits::Pawn && !_attackDetector.IsPinned(next[i]))
				{
					if (r == bctraits::PromotedPawnRank)
					{
						moves.emplace_back(next[i], sq, MoveType::Queen_Promotion_Capture);
						moves.emplace_back(next[i], sq, MoveType::Rook_Promotion_Capture);
						moves.emplace_back(next[i], sq, MoveType::Bishop_Promotion_Capture);
						moves.emplace_back(next[i], sq, MoveType::Knight_Promotion_Capture);
					}
					else
					{
						moves.emplace_back(next[i], sq, MoveType::Capture);
					}
				}
			}

			using octraits = traits::color_traits<ctraits::Opposite>;
			Square enpassant_sq{ board.GetEnPassantSquare() };
			if (enpassant_sq != Empty && board.GetPiece(sq) == octraits::Pawn)
			{
				conn::View<conn::PawnDirectionTrait<ctraits::Opposite>::Dir> ep_view{ enpassant_sq };
				next[0] = ep_view.Next(3);
				next[1] = ep_view.Next(1);
				for (int i = 0; i < 2; i++)
					if (next[i] != Empty && board.GetPiece(next[i]) == ctraits::Pawn && !_attackDetector.IsPinned(next[i]))
						moves.emplace_back(next[i], enpassant_sq, MoveType::En_Passant_Capture);
			}
		}
		else
		{
			Square next = view.Next();
			Piece p = board.GetPiece(next);
			if (p == ctraits::Pawn && !_attackDetector.IsPinned(next))
			{
				if (r == bctraits::PromotedPawnRank)
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
			else if (p == Empty && r == bctraits::DoublePushedPawnRank)
			{
				next = view.Next();
				if (board.GetPiece(next) == ctraits::Pawn && !_attackDetector.IsPinned(next))
					moves.emplace_back(next, sq, MoveType::Double_Pawn_Push);
			}
		}
	}

	template <typename BoardType>
	template<Color SideToMove, Piece MovingPiece>
	void MoveGeneratorConn<BoardType>::GenerateSlidingPieceMoves(const BoardType& board, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;

		// Piece is pinned. Piece can only move in the pinned direction or opposite of the pinned direction.
		#define G1(Dir1, Dir2)\
		if (Dir1 == pin_dir || Dir2 == pin_dir) \
		{\
			GenerateSlidingPieceMoves<SideToMove, Dir1>(board, first->second, moves);\
			GenerateSlidingPieceMoves<SideToMove, Dir2>(board, first->second, moves);\
		}\

		#define H(Dir) GenerateSlidingPieceMoves<SideToMove, Dir>(board, first->second, moves)

		auto [first, last] = board.GetPiecePositions<SideToMove>(MovingPiece);
		for (; first != last; first++)
		{
			if (Direction pin_dir = _attackDetector.GetPinDirection(first->second); pin_dir != Empty)
			{
				if constexpr (MovingPiece == ctraits::Rook) 
				{
					G1(BoardType::N, BoardType::S)
					G1(BoardType::E, BoardType::W)
				}
				else if constexpr (MovingPiece == ctraits::Bishop)
				{
					G1(BoardType::NE, BoardType::SW)
					G1(BoardType::SE, BoardType::NW)
				}
				else 
				{
					G1(BoardType::N, BoardType::S)
					G1(BoardType::E, BoardType::W)
					G1(BoardType::NE, BoardType::SW)
					G1(BoardType::SE, BoardType::NW)
				}
			}
			else
			{
				// Piece is not pinned. Piece is free to move in any direction.
				if constexpr (MovingPiece == ctraits::Rook) 
				{
					H(BoardType::N); H(BoardType::E); H(BoardType::S); H(BoardType::W);
				}
				else if constexpr (MovingPiece == ctraits::Bishop)
				{
					H(BoardType::NE); H(BoardType::SE); H(BoardType::SW); H(BoardType::NW);
				}
				else 
				{
					H(BoardType::N); H(BoardType::E); H(BoardType::S); H(BoardType::W);
					H(BoardType::NE); H(BoardType::SE); H(BoardType::SW); H(BoardType::NW);
				}
			}
		}
	}

	template <typename BoardType>
	template<Color SideToMove, Direction Dir>
	void MoveGeneratorConn<BoardType>::GenerateSlidingPieceMoves(const BoardType& board, Square pos, MoveList& moves) const
	{
		Square next{ Empty };
		Piece p{ Empty };

		conn::View<Dir> view{ pos };
		for (int8_t i = 0; i < view.GetSize(); i++)
		{
			next = view.Next();
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
	void MoveGeneratorConn<BoardType>::GenerateKnightMoves(const BoardType& board, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;
		auto [first, last] = board.GetPiecePositions<SideToMove>(ctraits::Knight);

		Square next{ Empty };
		Piece p{ Empty };

		for (; first != last; first++)
		{
			if (_attackDetector.IsPinned(first->second))
				continue;

			conn::View<conn::KnightJumps> view{ first->second };
			for (int8_t i = 0; i < view.GetSize(); i++) 
			{
				next = view.Next();
				p = board.GetPiece(next);
				if (p == Empty)
					moves.emplace_back(first->second, next);
				else if (color::get_color(p) != SideToMove)
					moves.emplace_back(first->second, next, MoveType::Capture);
			}
		}
	}

	template <typename BoardType>
	template <Color SideToMove>
	void MoveGeneratorConn<BoardType>::GeneratePawnMoves(const BoardType& board, MoveList& moves) const
	{
		using ctraits = traits::color_traits<SideToMove>;
		using octraits = traits::color_traits<ctraits::Opposite>;
		using bctraits = traits::board_color_traits<BoardType, SideToMove>;

		Rank r{ Empty };
		Square next{ Empty };
		Piece p{ Empty };

		auto [first, last] = board.GetPiecePositions<SideToMove>(ctraits::Pawn);
		for (; first != last; first++)
		{
			auto pin_dir = _attackDetector.GetPinDirection(first->second);

			conn::View<conn::PawnDirectionTrait<SideToMove>::Dir> view{ first->second };
			
			r = view.Next();
			next = view.Next();

		    // One square forward, two square forward
			if (board.GetPiece(next) == Empty &&
				(pin_dir == Empty || pin_dir == bctraits::PawnMoveDirection || pin_dir == bctraits::PawnReverseMoveDirection))
			{
				if (r == bctraits::PawnPromotionRank)
				{
					moves.emplace_back(first->second, next, MoveType::Queen_Promotion);
					moves.emplace_back(first->second, next, MoveType::Rook_Promotion);
					moves.emplace_back(first->second, next, MoveType::Knight_Promotion);
					moves.emplace_back(first->second, next, MoveType::Bishop_Promotion);
				}
				else 
				{
					moves.emplace_back(first->second, next);
				}
				
				next = view.Next();
				if (r == bctraits::PawnDoublePushRank && board.GetPiece(next) == Empty)
					moves.emplace_back(first->second, next, MoveType::Double_Pawn_Push);
			}
			else
				next = view.Next();

			// Captures
			for (int i = 0; i < 2; i++) 
			{
				next = view.Next();
				if (next == Empty)
					continue;

				p = board.GetPiece(next);
				if (p != Empty && color::get_color(p) != SideToMove &&
					(pin_dir == Empty || pin_dir == bctraits::PawnReverseAttackDirections[i]))
				{
					if (r == bctraits::PawnPromotionRank)
					{
						moves.emplace_back(first->second, next, MoveType::Queen_Promotion_Capture);
						moves.emplace_back(first->second, next, MoveType::Rook_Promotion_Capture);
						moves.emplace_back(first->second, next, MoveType::Knight_Promotion_Capture);
						moves.emplace_back(first->second, next, MoveType::Bishop_Promotion_Capture);
					}
					else
					{
						moves.emplace_back(first->second, next, MoveType::Capture);
					}
				}
			}
		}

		Square en_passant = board.GetEnPassantSquare();
		if (en_passant == Empty)
			return;

		conn::View<conn::PawnDirectionTrait<ctraits::Opposite>::Dir> ep_view{ en_passant };
		r = ep_view.Next();
		Square next_arr[2] = { ep_view.Next(2), ep_view.Next(1) };

		for (int i = 0; i < 2; i++)
		{
			if (next_arr[i] == Empty)
				continue;

			p = board.GetPiece(next_arr[i]);
			if (p != ctraits::Pawn)
				continue;

			Direction pin_dir = _attackDetector.GetPinDirection(next_arr[i]);
			if (pin_dir != Empty && pin_dir != bctraits::PawnReverseAttackDirections[i])
				continue;

			Square king_pos = board.GetKingPosition<SideToMove>();
			if (BoardType::GetRank(king_pos) != BoardType::GetRank(next_arr[i]))
				moves.emplace_back(next_arr[i], en_passant, MoveType::En_Passant_Capture);
			else
			{
				/*Square ppos{ en_passant + bctraits::PawnReverseMoveDirection };
				Direction dir = king_pos > next_arr[i] ? BoardType::W : BoardType::E;
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
					moves.emplace_back(pos, en_passant, MoveType::En_Passant_Capture);*/
			}
		}
	}
}