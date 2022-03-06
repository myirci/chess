#pragma once

#include <ChessLib/Chess/Definitions.hpp>
#include <array>

namespace chesslib 
{
	class ChessBoard
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

		static constexpr Direction N{ 8 }, S{ -8 }, E{ 1 }, W{ -1 }, NE{ 9 }, SW{ -9 }, NW{ 7 }, SE{ -7 };

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
			return next < BOARDSIZE&& next >= 0 && std::abs(GetFile(next) - GetFile(curr)) <= 2;
		}

#pragma endregion

	};
}