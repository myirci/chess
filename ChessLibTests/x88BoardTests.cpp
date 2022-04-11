#include "pch.h"

#include "BoardTestBase.hpp"

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/Board/x88Board.hpp>
#include <ChessLib/Board/BoardFactory.hpp>
#include <ChessLib/Board/BoardFunctions.hpp>

using namespace chesslib;
using namespace chesslib::pieceset;

class x88BoardTests : 
    public ::testing::Test, 
    public BoardTestBase
{
};

TEST_F(x88BoardTests, SquareFileRank)
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

TEST_F(x88BoardTests, SquareFromChars)
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

TEST_F(x88BoardTests, constructor_starting_pos)
{
    auto b = BoardFactory::make_unique_board<x88Board>(Fen::StartingPosition);
    const auto& board_array = b->GetBoard();

    EXPECT_TRUE(std::equal(board_array.begin(), board_array.end(), board_array_starting_position.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::White);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), Empty);
    EXPECT_EQ(b->GetHalfMoveClock(), 0);
    EXPECT_EQ(b->GetFullMoveClock(), 1);
    EXPECT_EQ(b->GetWhitePieces(), white_pieces_starting_position);
    EXPECT_EQ(b->GetBlackPieces(), black_pieces_starting_position);
    EXPECT_EQ(Fen::StartingPosition, board_to_fen(*b));
}

TEST_F(x88BoardTests, constructor_fen1)
{
    auto b = BoardFactory::make_unique_board<x88Board>(fen_pos1);
    const auto& board_array = b->GetBoard();

    EXPECT_TRUE(std::equal(board_array.begin(), board_array.end(), board_array_fen1.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::Black);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_FALSE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), x88Board::c3);
    EXPECT_EQ(b->GetHalfMoveClock(), 1);
    EXPECT_EQ(b->GetFullMoveClock(), 2);
    EXPECT_EQ(b->GetWhitePieces(), white_pieces_fen1);
    EXPECT_EQ(b->GetBlackPieces(), black_pieces_fen1);
    EXPECT_EQ(fen_pos1, board_to_fen(*b));
}

TEST_F(x88BoardTests, constructor_fen_compare)
{
    for (auto f : board_setup_fens)
    {
        auto b = BoardFactory::make_unique_board<x88Board>(f);
        EXPECT_EQ(f, board_to_fen(*b));
    }
}