#pragma once

#include <ChessLib/Board/PieceCentricBoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Chess/Definitions.hpp>

#include <array>
#include <string_view>
#include <memory>
#include <vector>

namespace chesslib::basic_board
{
	constexpr Direction N{ 8 }, S{ -8 }, E{ 1 }, W{ -1 }, NE{ 9 }, SW{ -9 }, NW{ 7 }, SE{ -7 };

	// One dimensional array of 64 squares to represent 8x8 board board.
	// Not optimal for move generation: off-board piece detection is expensive to compute.
	class BasicBoard : public PieceCentricBoardBase
	{
	public:

#pragma region Static Members

		static constexpr int BOARDSIZE = 64;
		
		static constexpr Square TopToBottomOrder[BOARDSIZE]
		{
			squareset::a8, squareset::b8, squareset::c8, squareset::d8, squareset::e8, squareset::f8, squareset::g8, squareset::h8,
			squareset::a7, squareset::b7, squareset::c7, squareset::d7, squareset::e7, squareset::f7, squareset::g7, squareset::h7,
			squareset::a6, squareset::b6, squareset::c6, squareset::d6, squareset::e6, squareset::f6, squareset::g6, squareset::h6,
			squareset::a5, squareset::b5, squareset::c5, squareset::d5, squareset::e5, squareset::f5, squareset::g5, squareset::h5,
			squareset::a4, squareset::b4, squareset::c4, squareset::d4, squareset::e4, squareset::f4, squareset::g4, squareset::h4,
			squareset::a3, squareset::b3, squareset::c3, squareset::d3, squareset::e3, squareset::f3, squareset::g3, squareset::h3,
			squareset::a2, squareset::b2, squareset::c2, squareset::d2, squareset::e2, squareset::f2, squareset::g2, squareset::h2,
			squareset::a1, squareset::b1, squareset::c1, squareset::d1, squareset::e1, squareset::f1, squareset::g1, squareset::h1
		};

		static constexpr Square BottomToTopOrder[BOARDSIZE]
		{
			squareset::a1, squareset::b1, squareset::c1, squareset::d1, squareset::e1, squareset::f1, squareset::g1, squareset::h1,
			squareset::a2, squareset::b2, squareset::c2, squareset::d2, squareset::e2, squareset::f2, squareset::g2, squareset::h2,
			squareset::a3, squareset::b3, squareset::c3, squareset::d3, squareset::e3, squareset::f3, squareset::g3, squareset::h3,
			squareset::a4, squareset::b4, squareset::c4, squareset::d4, squareset::e4, squareset::f4, squareset::g4, squareset::h4,
			squareset::a5, squareset::b5, squareset::c5, squareset::d5, squareset::e5, squareset::f5, squareset::g5, squareset::h5,
			squareset::a6, squareset::b6, squareset::c6, squareset::d6, squareset::e6, squareset::f6, squareset::g6, squareset::h6,
			squareset::a7, squareset::b7, squareset::c7, squareset::d7, squareset::e7, squareset::f7, squareset::g7, squareset::h7,
			squareset::a8, squareset::b8, squareset::c8, squareset::d8, squareset::e8, squareset::f8, squareset::g8, squareset::h8
		};

		static constexpr std::array<Direction, 8> AllDirections{ N, NE, E, SE, S, SW, W, NW };

		static constexpr std::array<Direction, 4> StraightDirections{ N, E, S, W };

		static constexpr std::array<Direction, 4> DiagonalDirections{ NE, SE, SW, NW };

		static constexpr std::array<Direction, 8> KnightJumps{ 17, 10, -6, -15, -17, -10, 6, 15 };

		static constexpr Square GetSquare(File f, Rank r) noexcept
		{
			return 8 * r + f;
		}

		static constexpr Rank GetRank(Square s) noexcept
		{
			return s >> 3;
		}

		static constexpr File GetFile(Square s) noexcept
		{
			return s % 8;
		}

		static constexpr std::pair<File, Rank> GetFileAndRank(Square s) noexcept
		{
			return { GetFile(s), GetRank(s) };
		}

		static constexpr Square GetSquareFromChars(char f, char r) noexcept
		{
			return GetSquare(f - 'a', r - '1');
		};

		static constexpr std::pair<char, char> GetCharPair(Square s) noexcept
		{
			auto [f, r] = GetFileAndRank(s);
			return { f + 'a', r + '1' };
		}

		static constexpr bool IsInside(Square curr, Square next) noexcept
		{
			return next < BOARDSIZE && next >= 0 && std::abs(GetFile(next) - GetFile(curr)) <= 2;
		}

#pragma endregion

		using BoardArray = std::array<Square, BOARDSIZE>;

		const BoardArray& GetBoard() const noexcept { return _board; }
		BoardArray& GetBoard() noexcept				{ return _board; }

		void MakeMove(const Move& move);
		void UnMakeMove();

	protected:

		BoardArray _board;

		BasicBoard() noexcept : PieceCentricBoardBase(), _board{} { }

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

		friend std::unique_ptr<BasicBoard> make_unique_board(std::string_view fen);
		friend std::shared_ptr<BasicBoard> make_shared_board(std::string_view fen);
	};

	std::unique_ptr<BasicBoard> make_unique_board(std::string_view fen);
	std::shared_ptr<BasicBoard> make_shared_board(std::string_view fen);
}