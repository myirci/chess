#include "pch.h"

#include <ChessLib/Board/BasicBoard.hpp>
#include <ChessLib/Board/x88Board.hpp>

using namespace chesslib;

TEST(SquareTest, BasicBoard_SquareFileRank)
{
    Square i{ 0 };
    for (Rank rk = 0; rk < 8; rk++)
    {
        for (File fl = 0; fl < 8; fl++)
        {
            Square s = BasicBoard::GetSquare(fl, rk);
            EXPECT_EQ(s, i);
            auto [f, r] = BasicBoard::GetFileAndRank(i);
            EXPECT_EQ(f, fl);
            EXPECT_EQ(r, rk);
            i++;
        }
    }
}

TEST(SquareTest, BasicBoard_SquareFromChars)
{
    char f{ 'a' }, r{ '1' };
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Square s = BasicBoard::GetSquareFromChars(f + j, r + i);
            EXPECT_EQ(s, j + i * 8);
            auto [fl, rk] = BasicBoard::GetCharPair(s);
            EXPECT_EQ(fl, f + j);
            EXPECT_EQ(rk, r + i);
        }
    }
}

TEST(SquareTest, 0x88Board_SquareFileRank)
{
    Square i{ 0 };
    for (Rank rk = 0; rk < 8; rk++)
    {
        for (File fl = 0; fl < 16; fl++)
        {
            Square s = x88Board::GetSquare(fl, rk);
            EXPECT_EQ(s, i);
            auto [f, r] = x88Board::GetFileAndRank(i);
            EXPECT_EQ(f, fl);
            EXPECT_EQ(r, rk);
            i++;
        }
    }
}

TEST(SquareTest, 0x88Board_SquareFromChars)
{
    char f{ 'a' }, r{ '1' };
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Square s = x88Board::GetSquareFromChars(f + j, r + i);
            EXPECT_EQ(s, j + i * 16);
            auto [fl, rk] = x88Board::GetCharPair(s);
            EXPECT_EQ(fl, f + j);
            EXPECT_EQ(rk, r + i);
        }
    }
}
