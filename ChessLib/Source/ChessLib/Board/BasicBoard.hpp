#pragma once

#include <ChessLib/Board/ChessBoard.hpp>
#include <ChessLib/Board/PieceCentricBoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/ColorTraits.hpp>
#include <ChessLib/Chess/BoardColorTraits.hpp>

#include <array>
#include <string_view>
#include <memory>
#include <vector>

namespace chesslib
{
	class BoardFactory;

	// One dimensional array of 64 squares to represent 8x8 board board.
	// Not optimal for move generation: off-board piece detection is expensive to compute.
	class BasicBoard :
		public ChessBoard,
		public PieceCentricBoardBase
	{
	public:

		using BoardArray = std::array<Square, BOARDSIZE>;

		static constexpr bool IsInside(Square curr, Square next) noexcept
		{
			return next < BOARDSIZE&& next >= 0 && std::abs(GetFile(next) - GetFile(curr)) <= 2;
		}
		const BoardArray& GetBoard() const noexcept { return _board; }
		BoardArray& GetBoard() noexcept { return _board; }

		void SetPiece(Piece p, Square s);
		Piece GetPiece(Square s) const { return _board[s]; }

		template<Color SideToMove>
		void MakeQuiteMove(Square from, Square to)
		{
			UpdatePiecePosition<SideToMove>(_board[from], from, to);
			_board[to] = _board[from];
			_board[from] = Empty;
		}

		template<Color SideToMove>
		void MakeCaptureMove(Square from, Square to, Piece captured)
		{
			using ctraits = traits::color_traits<SideToMove>;

			RemovePiece<ctraits::Opposite>(captured, to);
			MakeQuiteMove<SideToMove>(from, to);
		}

		template<Color SideToMove>
		void MakeEnpassantCaptureMove(Square from, Square to)
		{
			using ctraits = traits::color_traits<SideToMove>;
			using octraits = traits::color_traits<ctraits::Opposite>;
			using bctraits = traits::board_color_traits<BasicBoard, SideToMove>;

			Square removed_pawn_pos{ _enpassant_target + bctraits::PawnReverseMoveDirection };
			RemovePiece<ctraits::Opposite>(octraits::Pawn, removed_pawn_pos);
			_board[removed_pawn_pos] = Empty;
			MakeQuiteMove<SideToMove>(from, to);
		}

		template<Color SideToMove>
		void MakePromotionMove(Square from, Square to, Piece captured, Piece promoted)
		{
			using ctraits = traits::color_traits<SideToMove>;

			// Remove the captured piece from the board
			if (captured != Empty) 
				RemovePiece<ctraits::Opposite>(captured, to);
			
			RemovePiece<SideToMove>(_board[from], from);
			AddNewPiece<SideToMove>(promoted, to);

			_board[to] = promoted;
			_board[from] = Empty;
		}

		template<Color SideToMove>
		void UnMakePromotionMove(Square from, Square to, Piece captured)
		{
			using ctraits = traits::color_traits<SideToMove>;
			using octraits = traits::color_traits<ctraits::Opposite>;

			RemovePiece<ctraits::Opposite>(_board[to], to);
			AddNewPiece<ctraits::Opposite>(octraits::Pawn, from);
			_board[from] = octraits::Pawn;

			if (captured != Empty)
			{
				_board[to] = captured;
				AddNewPiece<SideToMove>(captured, to);
			}
			else
				_board[to] = Empty;
		}

		template<Color SideToMove>
		void UnMakeEnpassantCaptureMove(Square from, Square to, Piece captured) 
		{
			using ctraits = traits::color_traits<SideToMove>;
			using bctraits = traits::board_color_traits<BasicBoard, SideToMove>;

			MakeQuiteMove<SideToMove>(to, from);

			Square removed_pawn_pos{ _enpassant_target + bctraits::PawnReverseMoveDirection };
			AddNewPiece<ctraits::Opposite>(captured, removed_pawn_pos);
			_board[removed_pawn_pos] = captured;
		}

		template<Color SideToMove>
		void UnMakeCaptureMove(Square from, Square to, Piece captured) 
		{
			using ctraits = traits::color_traits<SideToMove>;

			// Move types: capture, enpassant capture
			MakeQuiteMove<SideToMove>(to, from);
			_board[to] = captured;
			AddNewPiece<ctraits::Opposite>(captured, to);
		}

		void MakeMove(const Move& move);
		void UnMakeMove();

	protected:

		BoardArray _board;

		BasicBoard() noexcept : PieceCentricBoardBase() { _board.fill(Empty); }

		/*
		template<Color Clr>
		void MakeMoveImplementation(const Move& move);

		template<Color Clr>
		inline void MakeQuiteMove(Square from, Square to);

		template<Color Clr>
		void UnMakeMove(const Move& move, Piece captured);

		template<Color Clr>
		void GenerateMovesImplementation(MoveList& moves) const;

		template<Color Attacker>
		void ComputeChecksAndPins(Square king_pos) const;

		template<Color Attacker, bool IsStraightMovingPiece>
		void ComputeChecksAndPins(Square king_pos, const std::array<Direction, 4>& attack_directions) const;

		template<Color Clr>
		void GenerateKingMoves(Square king_pos, MoveList& moves) const;

		template<Color Attacker>
		inline bool IsDirectionInvalidForKing(Direction dir) const;

		template<Color Attacker>
		bool IsUnderAttack(Square sq) const;

		template<Color Attacker, bool IsStraightMovingPiece>
		bool IsUnderAttack(Square sq, const std::array<Direction, 4>& attack_directions) const;

		template<Color Attacker>
		bool IsUnderAttackByAKnight(Square sq) const;

		template<Color Clr>
		inline bool IsKingSideCastlingPossible() const;

		template<Color Clr>
		inline bool IsQueenSideCastlingPossible() const;

		// Generates check blocking and checker capturing moves
		template<Color Clr>
		void GenerateCheckEliminatingMoves(Square king_pos, MoveList& moves) const;

		template<Color Clr>
		void GenerateCheckEliminatingEnPassantCaptureMoves(Square attacker_loc, MoveList& moves) const;

		// Generates non-king moves to the given square
		template<Color Clr>
		void ToSquareMoves(Square sq, MoveList& moves) const;

		template<Color Clr>
		void GenerateKnightMoves(MoveList& moves) const;

		// Generates non-king piece moves that move straight (horizontal/vertical)
		template<Color Clr>
		void GenerateStraightMoves(MoveList& moves) const;

		// Generates non-king piece moves that move diagonally
		template<Color Clr>
		void GenerateDiagonalMoves(MoveList& moves) const;

		template<Color Clr>
		void GeneratePawnMoves(MoveList& moves) const;

		template<Color Clr>
		void GenerateEnPassantCaptureMoves(Square king_pos, MoveList& moves) const;
		*/

		friend BoardFactory;
	};
}