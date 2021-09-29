#pragma once

#include <ChessLib/Chess/BoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>

#include <array>
#include <string_view>
#include <memory>
#include <vector>

namespace chesslib::basic_board
{
	// On dimensional array of 64 squares to represent 8x8 board board.
	// Not suitable for move generation: off-board piece detection is expensive to compute.
	class BasicBoard : public BoardBaseWithPieces
	{
	public:

		constexpr static int BOARDSIZE = 64;
		using BoardArray = std::array<Square, BOARDSIZE>;

		const BoardArray& GetBoard() const;
		BoardArray& GetBoard();

		template<Color Attacker>
		void ComputeChecksAndPins(Square king_pos);

		template<Color Attacker>
		bool IsUnderAttack(Square sq) const;

		template<Color Clr>
		void GenerateKingMoves(Square king_pos, MoveList& moves) const;

	protected:

		BasicBoard();
		BoardArray board;

		template<Color Attacker, bool IsStraightMovingPiece>
		bool IsUnderAttack(Square sq, const std::array<Direction, 4>& attack_directions) const;

		template<Color Attacker, bool IsStraightMovingPiece>
		void ComputeChecksAndPins(Square king_pos, const std::array<Direction, 4>& attack_directions);

		template<Color Clr>
		void ToSquareNonKingMoves(Square sq, MoveList& moves) const;

		template<Color Attacker>
		bool IsUnderAttackByAKnight(Square sq) const;

		static bool IsInside(Square curr, Square next);

		template<Color Attacker>
		inline bool IsStraightAttack(Piece p, Distance dist) const;

		template<Color Attacker>
		inline bool IsNonKingStraightAttack(Piece p) const;

		template<Color Attacker>
		inline bool IsDiagonalAttack(Piece p, Direction dir, Distance dist) const;

		template<Color Attacker>
		inline bool IsNonKingDiagonalAttack(Piece p, Direction dir, Distance dist) const;

		template<Color Attacker>
		inline bool IsDirectionInvalidForKing(Direction dir) const;

		template<Color Clr>
		inline bool IsKingSideCastlingPossible() const;

		template<Color Clr>
		inline bool IsQueenSideCastlingPossible() const;

		friend std::unique_ptr<BasicBoard> make_unique_board(std::string_view fen);
		friend std::shared_ptr<BasicBoard> make_shared_board(std::string_view fen);
	};

	std::unique_ptr<BasicBoard> make_unique_board(std::string_view fen);
	std::shared_ptr<BasicBoard> make_shared_board(std::string_view fen);
}


