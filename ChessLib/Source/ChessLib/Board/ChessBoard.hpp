#pragma once

#include <ChessLib/Chess/Definitions.hpp>
#include <array>

namespace chesslib 
{
	class ChessBoard
	{
	public:

		static constexpr int BOARDSIZE = 64;

		static constexpr Square
			a1{ 0 }, a2{ 8 }, a3{ 16 }, a4{ 24 }, a5{ 32 }, a6{ 40 }, a7{ 48 }, a8{ 56 },
			b1{ 1 }, b2{ 9 }, b3{ 17 }, b4{ 25 }, b5{ 33 }, b6{ 41 }, b7{ 49 }, b8{ 57 },
			c1{ 2 }, c2{ 10 }, c3{ 18 }, c4{ 26 }, c5{ 34 }, c6{ 42 }, c7{ 50 }, c8{ 58 },
			d1{ 3 }, d2{ 11 }, d3{ 19 }, d4{ 27 }, d5{ 35 }, d6{ 43 }, d7{ 51 }, d8{ 59 },
			e1{ 4 }, e2{ 12 }, e3{ 20 }, e4{ 28 }, e5{ 36 }, e6{ 44 }, e7{ 52 }, e8{ 60 },
			f1{ 5 }, f2{ 13 }, f3{ 21 }, f4{ 29 }, f5{ 37 }, f6{ 45 }, f7{ 53 }, f8{ 61 },
			g1{ 6 }, g2{ 14 }, g3{ 22 }, g4{ 30 }, g5{ 38 }, g6{ 46 }, g7{ 54 }, g8{ 62 },
			h1{ 7 }, h2{ 15 }, h3{ 23 }, h4{ 31 }, h5{ 39 }, h6{ 47 }, h7{ 55 }, h8{ 63 };

		static constexpr Square TopToBottomOrder[BOARDSIZE]
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

		static constexpr Square BottomToTopOrder[BOARDSIZE]
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

		static constexpr Direction N{ 8 }, S{ -8 }, E{ 1 }, W{ -1 }, NE{ 9 }, SW{ -9 }, NW{ 7 }, SE{ -7 };

		static constexpr std::array<Direction, 8> AllDirections{ N, NE, E, SE, S, SW, W, NW };
		static constexpr std::array<Direction, 4> StraightDirections{ N, E, S, W };
		static constexpr std::array<Direction, 4> DiagonalDirections{ NE, SE, SW, NW };
		static constexpr std::array<Direction, 8> KnightJumps{ 17, 10, -6, -15, -17, -10, 6, 15 };

		static constexpr Square GetSquare(File f, Rank r) noexcept { return 8 * r + f; }
		static constexpr Rank GetRank(Square s) noexcept { return s >> 3; }
		static constexpr File GetFile(Square s) noexcept { return s % 8; }
		static constexpr std::pair<File, Rank> GetFileAndRank(Square s) noexcept { return { GetFile(s), GetRank(s) }; }
		static constexpr Square GetSquareFromChars(char f, char r) noexcept { return GetSquare(f - 'a', r - '1'); };

		static constexpr std::pair<char, char> GetCharPair(Square s) noexcept
		{
			auto [f, r] = GetFileAndRank(s);
			return { f + 'a', r + '1' };
		}
	};
}