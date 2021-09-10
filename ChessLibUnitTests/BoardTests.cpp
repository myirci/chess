#include "pch.h"

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/BasicBoard/BasicBoard.hpp>
#include <ChessLib/Chess/Utility.hpp>

using namespace chesslib;
using namespace chesslib::basic_board;
TEST(BasicBoardTest, constructor_starting_pos)
{
    std::array<Square, 64> boardArray1
    {
        pieceset::WhiteRook, pieceset::WhiteKnight, pieceset::WhiteBishop, pieceset::WhiteQueen, pieceset::WhiteKing, pieceset::WhiteBishop, pieceset::WhiteKnight, pieceset::WhiteRook,
        pieceset::WhitePawn, pieceset::WhitePawn, pieceset::WhitePawn, pieceset::WhitePawn, pieceset::WhitePawn, pieceset::WhitePawn, pieceset::WhitePawn, pieceset::WhitePawn,
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty,
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty,
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty,
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty,
        pieceset::BlackPawn, pieceset::BlackPawn, pieceset::BlackPawn, pieceset::BlackPawn, pieceset::BlackPawn, pieceset::BlackPawn, pieceset::BlackPawn, pieceset::BlackPawn,
        pieceset::BlackRook, pieceset::BlackKnight, pieceset::BlackBishop, pieceset::BlackQueen, pieceset::BlackKing, pieceset::BlackBishop, pieceset::BlackKnight, pieceset::BlackRook
    };

    BasicBoard b{ Fen::StartingPosition };
    const auto& boardArray2 = b.GetBoard();

    EXPECT_TRUE(std::equal(boardArray1.begin(), boardArray1.end(), boardArray2.begin()));
    EXPECT_EQ(b.GetActiveColor(), color::White);
    EXPECT_TRUE(b.QueryCastling(Castling::WHITE_KS));
    EXPECT_TRUE(b.QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b.QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b.QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b.GetEnPassantSquare(), squareset::Empty);
    EXPECT_EQ(b.GetHalfMoveClock(), 0);
    EXPECT_EQ(b.GetFullMoveClock(), 1);
    //EXPECT_EQ(Fen::StartingPosition, utility::chess::board_to_fen(b, [](int i) { return squareset::top_to_bottom_order[i]; }));
}

TEST(BasicBoardTest, constructor_pos1)
{
    std::string_view fen{ "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Kkq c3 1 2" };

    std::array<Square, 64> boardArray1
    {
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, pieceset::WhiteKing, squareset::Empty,
        squareset::Empty, pieceset::WhitePawn, squareset::Empty, squareset::Empty, squareset::Empty, pieceset::WhitePawn, pieceset::WhitePawn, squareset::Empty,
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty,
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, pieceset::WhitePawn, pieceset::BlackPawn, squareset::Empty, squareset::Empty,
        squareset::Empty, squareset::Empty, pieceset::BlackPawn, pieceset::WhitePawn, squareset::Empty, squareset::Empty, pieceset::BlackPawn, squareset::Empty,
        squareset::Empty, squareset::Empty, squareset::Empty, pieceset::BlackPawn, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty,
        pieceset::BlackPawn, pieceset::BlackPawn, squareset::Empty, squareset::Empty, squareset::Empty, pieceset::BlackPawn, pieceset::BlackPawn, squareset::Empty,
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, pieceset::BlackKing, squareset::Empty
    };

    BasicBoard b{ fen };
    const auto& boardArray2 = b.GetBoard();

    EXPECT_TRUE(std::equal(boardArray1.begin(), boardArray1.end(), boardArray2.begin()));
    EXPECT_EQ(b.GetActiveColor(), color::Black);
    EXPECT_TRUE(b.QueryCastling(Castling::WHITE_KS));
    EXPECT_FALSE(b.QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b.QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b.QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b.GetEnPassantSquare(), squareset::c3);
    EXPECT_EQ(b.GetHalfMoveClock(), 1);
    EXPECT_EQ(b.GetFullMoveClock(), 2);
}