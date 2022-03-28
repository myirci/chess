#pragma once

#include <ChessLib/Board/PieceCentricBoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>

#include <array>
#include <unordered_map>
#include <memory>
#include <vector>

namespace chesslib
{
	class BoardFactory;

	class x88Board : public PieceCentricBoardBase
	{
	public:

#pragma region Static Members

		static constexpr int BOARDSIZE = 128;

		static constexpr Square
			a1{ 0 }, a2{ 16 }, a3{ 32 }, a4{ 48 }, a5{ 64 }, a6{ 80 }, a7{ 96 }, a8{ 112 },
			b1{ 1 }, b2{ 17 }, b3{ 33 }, b4{ 49 }, b5{ 65 }, b6{ 81 }, b7{ 97 }, b8{ 113 },
			c1{ 2 }, c2{ 18 }, c3{ 34 }, c4{ 50 }, c5{ 66 }, c6{ 82 }, c7{ 98 }, c8{ 114 },
			d1{ 3 }, d2{ 19 }, d3{ 35 }, d4{ 51 }, d5{ 67 }, d6{ 83 }, d7{ 99 }, d8{ 115 },
			e1{ 4 }, e2{ 20 }, e3{ 36 }, e4{ 52 }, e5{ 68 }, e6{ 84 }, e7{ 100 }, e8{ 116 },
			f1{ 5 }, f2{ 21 }, f3{ 37 }, f4{ 53 }, f5{ 69 }, f6{ 85 }, f7{ 101 }, f8{ 117 },
			g1{ 6 }, g2{ 22 }, g3{ 38 }, g4{ 54 }, g5{ 70 }, g6{ 86 }, g7{ 102 }, g8{ 118 },
			h1{ 7 }, h2{ 23 }, h3{ 39 }, h4{ 55 }, h5{ 71 }, h6{ 87 }, h7{ 103 }, h8{ 119 };

		static constexpr Square TopToBottomOrder[64]
		{
			a8, b8, c8, d8, e8, f8, g8, h8,
			a7, b7, c7, d7, e7, f7, g7, h7,
			a6, b6, c6, d6, e6, f6, g6, h6,
			a5, b5, c5, d5, e5, f5, g5, h5,
			a4, b4, c4, d4, e4, f4, g4, h4,
			a3, b3, c3, d3, e3, f3, g3, h3,
			a2, b2, c2, d2, e2, f2, g2, h2,
			a1, b1, c1, d1, e1, f1, g1, h1
		};

		static constexpr Square BottomToTopOrder[64]
		{
			a1, b1, c1, d1, e1, f1, g1, h1,
			a2, b2, c2, d2, e2, f2, g2, h2,
			a3, b3, c3, d3, e3, f3, g3, h3,
			a4, b4, c4, d4, e4, f4, g4, h4,
			a5, b5, c5, d5, e5, f5, g5, h5,
			a6, b6, c6, d6, e6, f6, g6, h6,
			a7, b7, c7, d7, e7, f7, g7, h7,
			a8, b8, c8, d8, e8, f8, g8, h8
		};

		static constexpr Direction N{ 16 }, S{ -16 }, E{ 1 }, W{ -1 }, NE{ 17 }, SW{ -17 }, NW{ 15 }, SE{ -15 };

		static constexpr std::array<Direction, 8> AllDirections{ N, NE, E, SE, S, SW, W, NW };

		static constexpr std::array<Direction, 4> StraightDirections{ N, E, S, W };

		static constexpr std::array<Direction, 4> DiagonalDirections{ NE, SE, SW, NW };

		static constexpr std::array<Direction, 8> KnightJumps{ 33, 18, -14, -31, -33, -18, 14, 31 };

		static constexpr Square Mirror(Square s) noexcept 
		{ 
			return s + 8; 
		}

		static constexpr Square GetSquare(File f, Rank r) noexcept
		{ 
			return 16 * r + f; 
		}

		static constexpr Rank GetRank(Square s) noexcept 
		{ 
			return s >> 4; 
		}

		static constexpr File GetFile(Square s) noexcept 
		{ 
			return s & 0x0F; 
		}

		static constexpr std::pair<File, Rank> GetFileAndRank(Square s) noexcept
		{ 
			return { GetFile(s), GetRank(s) }; 
		}

		static constexpr Square GetSquareFromChars(char f, char r) noexcept 
		{ 
			return GetSquare(f - 'a', r - '1'); 
		}

		static constexpr std::pair<char, char> GetCharPair(Square s) noexcept
		{
			auto [f, r] = GetFileAndRank(s);
			return { f + 'a', r + '1' };
		}

		static constexpr bool IsInside(Square sq) noexcept 
		{ 
			return !(0x88 & sq); 
		}

#pragma endregion
	
		using BoardArray = std::array<Square, BOARDSIZE>;

		const BoardArray& GetBoard() const noexcept { return _board; }
		BoardArray& GetBoard() noexcept             { return _board; }

		// void MakeMove(const Move& move);
		// void UnMakeMove();

	protected:
		
		BoardArray _board;

		x88Board() : PieceCentricBoardBase(), _board{} { }
		
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
