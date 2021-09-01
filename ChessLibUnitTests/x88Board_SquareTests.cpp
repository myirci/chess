#include "pch.h"

#include <ChessLib/x88Board/Square.hpp>

using namespace chesslib::x88board::square;

TEST(SquareTest, square_file_rank)
{
    index i{ 0 };
    for (index rank = 0; rank < 8; rank++)
    {
        for (index file = 0; file < 16; file++)
        {
            index sq = get_square_from_file_and_rank(file, rank);
            EXPECT_EQ(sq, i);
            auto [f, r] = get_file_and_rank_from_square(i);
            EXPECT_EQ(f, file);
            EXPECT_EQ(r, rank);
            i++;
        }
    }
}

TEST(SquareTest, square_chars)
{
    char f{ 'a' }, r{ '1' };
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            index sq = get_square_from_chars(f + j, r + i);
            EXPECT_EQ(sq, j + i * 16);
            auto [fl, rk] = get_chars_from_square(sq);
            EXPECT_EQ(fl, f + j);
            EXPECT_EQ(rk, r + i);
        }
    }
}