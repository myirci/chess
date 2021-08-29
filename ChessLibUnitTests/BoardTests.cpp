#include "pch.h"

#include <Chess/Fen.hpp>
#include <8x8Board/Board.hpp>

using namespace chesslib;

TEST(BoardTest, constructor_starting_pos)
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

    Board b{ Fen::StartingPosition };
    const auto& boardArray2 = b.GetBoard();

    EXPECT_TRUE(std::equal(boardArray1.begin(), boardArray1.end(), boardArray2.begin()));
}

TEST(BoardTest, constructor_pos1)
{
    std::string_view fen{ "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq c3 0 2" };

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

    Board b{ fen };
    const auto& boardArray2 = b.GetBoard();

    EXPECT_TRUE(std::equal(boardArray1.begin(), boardArray1.end(), boardArray2.begin()));
}