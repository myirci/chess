#include <pch.h>

#include <ChessLib/X88Board/Square.hpp>
#include <ChessLib/BasicBoard/Square.hpp>

using namespace chesslib;
TEST(SquareTest_Basic_Board, square_file_rank)
{
    Square i{ 0 };
    for (Rank rk = 0; rk < 8; rk++)
    {
        for (File fl = 0; fl < 8; fl++)
        {
            Square s = basic_board::get_square(fl, rk);
            EXPECT_EQ(s, i);
            auto [f, r] = basic_board::get_file_and_rank(i);
            EXPECT_EQ(f, fl);
            EXPECT_EQ(r, rk);
            i++;
        }
    }
}

TEST(SquareTest_Basic_Board, square_chars)
{
    char f{ 'a' }, r{ '1' };
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Square s = basic_board::get_square_from_chars(f + j, r + i);
            EXPECT_EQ(s, j + i * 8);
            auto [fl, rk] = basic_board::get_chars(s);
            EXPECT_EQ(fl, f + j);
            EXPECT_EQ(rk, r + i);
        }
    }
}

TEST(SquareTest_0x88_Board, square_file_rank)
{
    Square i{ 0 };
    for (Rank rk = 0; rk < 8; rk++)
    {
        for (File fl = 0; fl < 16; fl++)
        {
            Square s = x88board::get_square(fl, rk);
            EXPECT_EQ(s, i);
            auto [f, r] = x88board::get_file_and_rank(i);
            EXPECT_EQ(f, fl);
            EXPECT_EQ(r, rk);
            i++;
        }
    }
}

TEST(SquareTest_0x88_Board, square_chars)
{
    char f{ 'a' }, r{ '1' };
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Square s = x88board::get_square_from_chars(f + j, r + i);
            EXPECT_EQ(s, j + i * 16);
            auto [fl, rk] = x88board::get_chars(s);
            EXPECT_EQ(fl, f + j);
            EXPECT_EQ(rk, r + i);
        }
    }
}