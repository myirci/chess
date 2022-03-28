#pragma once

#include <ChessLib/Board/ChessBoard.hpp>
#include <ChessLib/Board/PieceCentricBoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Chess/Definitions.hpp>

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
		BoardArray& GetBoard() noexcept				{ return _board; }

		// void MakeMove(const Move& move);
		// void UnMakeMove();

	protected:

		BoardArray _board;

		BasicBoard() noexcept : PieceCentricBoardBase(), _board{} { }

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