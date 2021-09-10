#pragma once

#include<ChessLib/Chess/Definitions.hpp>

namespace chesslib::basic_board
{
    using namespace squareset;

    constexpr Square top_to_bottom_order[64]
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

    constexpr Square bottom_to_top_order[64]
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

    constexpr Rank rank1{ 0 }, rank2{ 1 }, rank3{ 2 }, rank4{ 3 }, rank5{ 4 }, rank6{ 5 }, rank7{ 6 }, rank8{ 7 };

    constexpr File fileA{ 0 }, fileB{ 1 }, fileC{ 2 }, fileD{ 3 }, fileE{ 4 }, fileF{ 5 }, fileG{ 6 }, fileH{ 7 };

    constexpr Square get_square(File f, Rank r) { return 8*r + f; }
    
    constexpr Rank get_rank(Square s) { return s >> 3; }

    constexpr File get_file(Square s) { return s % 8; }

    constexpr std::pair<File, Rank> get_file_and_rank(Square s) { return { get_file(s), get_rank(s) }; }
    
    constexpr Square get_square_from_chars(char f, char r) { return get_square(f - 'a', r - '1'); };

    constexpr std::pair<char, char> get_chars(Square s)
    {
        auto [f, r] = get_file_and_rank(s);
        return { f + 'a', r + '1' };
    }
}



