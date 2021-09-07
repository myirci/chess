#pragma once

#include<ChessLib/Chess/Definitions.hpp>

namespace chesslib::basic_board
{
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



