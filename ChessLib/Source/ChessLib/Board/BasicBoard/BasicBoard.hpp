#pragma once

#include <ChessLib/Board/MailboxBoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>

#include <array>
#include <string_view>
#include <memory>
#include <vector>

namespace chesslib::basic_board
{
	// On dimensional array of 64 squares to represent 8x8 board board.
	// Not suitable for move generation: off-board piece detection is expensive to compute.
	class BasicBoard : public MailboxBoardBase
	{
	public:

#pragma region static_methods

		static inline bool IsInside(Square curr, Square next);

#pragma endregion

		constexpr static int BOARDSIZE = 64;
		using BoardArray = std::array<Square, BOARDSIZE>;

		const BoardArray& GetBoard() const;
		BoardArray& GetBoard();

		MoveList GenerateMoves() const;

		void MakeMove(const Move& move);

		void UnMakeMove();

		void ComputeChecksAndPins() const;

	protected:

		BasicBoard();
		BoardArray board;

		template<Color Clr>
		void MakeMoveImplementation(const Move& move);

		template<Color Clr>
		inline void MakeQuiteMove(Square from, Square to);

		template<Color Clr>
		void UnMakeMove(const Move& move);

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

		friend std::unique_ptr<BasicBoard> make_unique_board(std::string_view fen);
		friend std::shared_ptr<BasicBoard> make_shared_board(std::string_view fen);
	};

	std::unique_ptr<BasicBoard> make_unique_board(std::string_view fen);
	std::shared_ptr<BasicBoard> make_shared_board(std::string_view fen);
}


