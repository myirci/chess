#include "pch.h"

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/BasicBoard/BasicBoard.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

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

    BoardBase::PieceMap wp
    {
        {pieceset::WhitePawn, squareset::a2}, {pieceset::WhitePawn, squareset::b2}, {pieceset::WhitePawn, squareset::c2},
        {pieceset::WhitePawn, squareset::d2}, {pieceset::WhitePawn, squareset::e2}, {pieceset::WhitePawn, squareset::f2},
        {pieceset::WhitePawn, squareset::g2}, {pieceset::WhitePawn, squareset::h2},
        {pieceset::WhiteRook, squareset::a1}, {pieceset::WhiteRook, squareset::h1},
        {pieceset::WhiteKnight, squareset::b1}, {pieceset::WhiteKnight, squareset::g1},
        {pieceset::WhiteBishop, squareset::c1}, {pieceset::WhiteBishop, squareset::f1},
        {pieceset::WhiteQueen, squareset::d1}, {pieceset::WhiteKing, squareset::e1}
    };

    BoardBase::PieceMap bp
    {
        {pieceset::BlackPawn, squareset::a7}, {pieceset::BlackPawn, squareset::b7}, {pieceset::BlackPawn, squareset::c7},
        {pieceset::BlackPawn, squareset::d7}, {pieceset::BlackPawn, squareset::e7}, {pieceset::BlackPawn, squareset::f7},
        {pieceset::BlackPawn, squareset::g7}, {pieceset::BlackPawn, squareset::h7},
        {pieceset::BlackRook, squareset::a8}, {pieceset::BlackRook, squareset::h8},
        {pieceset::BlackKnight, squareset::b8}, {pieceset::BlackKnight, squareset::g8},
        {pieceset::BlackBishop, squareset::c8}, {pieceset::BlackBishop, squareset::f8},
        {pieceset::BlackQueen, squareset::d8}, {pieceset::BlackKing, squareset::e8}
    };

    auto b = basic_board::make_unique_basic_board(Fen::StartingPosition);
    const auto& boardArray2 = b->GetBoard();

    EXPECT_TRUE(std::equal(boardArray1.begin(), boardArray1.end(), boardArray2.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::White);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), squareset::Empty);
    EXPECT_EQ(b->GetHalfMoveClock(), 0);
    EXPECT_EQ(b->GetFullMoveClock(), 1);
    EXPECT_EQ(b->GetWhitePieces(), wp);
    EXPECT_EQ(b->GetBlackPieces(), bp);
    EXPECT_EQ(Fen::StartingPosition, utility::chess::board_to_fen(*b));
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

    BoardBase::PieceMap wp
    {
        {pieceset::WhitePawn, squareset::b2}, {pieceset::WhitePawn, squareset::f2}, {pieceset::WhitePawn, squareset::g2},
        {pieceset::WhitePawn, squareset::d5}, {pieceset::WhitePawn, squareset::e4}, {pieceset::WhiteKing, squareset::g1}
    };

    BoardBase::PieceMap bp
    {
        {pieceset::BlackPawn, squareset::a7}, {pieceset::BlackPawn, squareset::b7}, {pieceset::BlackPawn, squareset::c5},
        {pieceset::BlackPawn, squareset::d6}, {pieceset::BlackPawn, squareset::f7}, {pieceset::BlackPawn, squareset::f4},
        {pieceset::BlackPawn, squareset::g7}, {pieceset::BlackPawn, squareset::g5}, {pieceset::BlackKing, squareset::g8}
    };

    auto b = basic_board::make_unique_basic_board(fen);
    const auto& boardArray2 = b->GetBoard();

    EXPECT_TRUE(std::equal(boardArray1.begin(), boardArray1.end(), boardArray2.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::Black);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_FALSE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), squareset::c3);
    EXPECT_EQ(b->GetHalfMoveClock(), 1);
    EXPECT_EQ(b->GetFullMoveClock(), 2);
    EXPECT_EQ(b->GetWhitePieces(), wp);
    EXPECT_EQ(b->GetBlackPieces(), bp);
    EXPECT_EQ(fen, utility::chess::board_to_fen(*b));
}

TEST(BasicBoardTest, constructor_fen_compare) 
{
    std::vector<std::string_view> fens
    {
        "1k1r3r/pp2bp1p/1q5p/2NP4/1Ppp1PQ1/P6P/2P3P1/B4R1K b - - 0 1",
        "r1q3k1/2p2rpp/p1n5/P3p1N1/1pQ1n1b1/2P3N1/1P3KPP/R1B1R3 w - - 46 56",
        "r3k2r/pp1B1p1p/2p2P2/3p4/5p1P/q1N1Q1P1/P1P5/1K1R4 b kq - 5 16",
        "3q3r/5pk1/p2p2p1/4p1n1/2R2P2/1P5K/P3Q3/6R1 w - - 0 1",
        "r1b1r1k1/1q3pbp/2N2np1/1N1P4/2QB1P2/8/PP3nPP/R3KqR1 w Q - 0 1",
        "8/4q2k/3p2pb/PP1P4/2N1n1Pp/5Q1P/8/7K b - g3 0 1",
        "r1bqkn1r/pp2b2p/2n2p2/4p1pP/2Pp3N/3P2P1/PP3PB1/RNBQR1K1 w kq g6 0 1",
        "r4rk1/p2b2p1/3Np2p/3nPp2/1nBP4/2p2QPq/5P1P/R1BR2K1 w - f6 0 1",
        "4r1k1/1b5p/pp2r1p1/3pPp2/PPpQ1P1q/2P4P/2B3P1/R4RK1 w - f6 23 45",
        "4k2r/1b2b1p1/2q2n1p/4p3/1pP1n2P/p3R3/4QBB1/KN1R4 b k c3 0 59",
    };
    
    for (auto f : fens) 
    {
        auto b = basic_board::make_unique_basic_board(f);
        EXPECT_EQ(f, utility::chess::board_to_fen(*b));
    }
}