#include "pch.h"

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/BasicBoard/BasicBoard.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

using namespace chesslib;
using namespace chesslib::basic_board;

class BoardTests : public ::testing::Test 
{
public:

    BoardTests() { }

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

    BoardBase::PieceMap whitePieces1
    {
        {pieceset::WhitePawn, squareset::a2}, {pieceset::WhitePawn, squareset::b2}, {pieceset::WhitePawn, squareset::c2},
        {pieceset::WhitePawn, squareset::d2}, {pieceset::WhitePawn, squareset::e2}, {pieceset::WhitePawn, squareset::f2},
        {pieceset::WhitePawn, squareset::g2}, {pieceset::WhitePawn, squareset::h2},
        {pieceset::WhiteRook, squareset::a1}, {pieceset::WhiteRook, squareset::h1},
        {pieceset::WhiteKnight, squareset::b1}, {pieceset::WhiteKnight, squareset::g1},
        {pieceset::WhiteBishop, squareset::c1}, {pieceset::WhiteBishop, squareset::f1},
        {pieceset::WhiteQueen, squareset::d1}, {pieceset::WhiteKing, squareset::e1}
    };

    BoardBase::PieceMap blackPieces1
    {
        {pieceset::BlackPawn, squareset::a7}, {pieceset::BlackPawn, squareset::b7}, {pieceset::BlackPawn, squareset::c7},
        {pieceset::BlackPawn, squareset::d7}, {pieceset::BlackPawn, squareset::e7}, {pieceset::BlackPawn, squareset::f7},
        {pieceset::BlackPawn, squareset::g7}, {pieceset::BlackPawn, squareset::h7},
        {pieceset::BlackRook, squareset::a8}, {pieceset::BlackRook, squareset::h8},
        {pieceset::BlackKnight, squareset::b8}, {pieceset::BlackKnight, squareset::g8},
        {pieceset::BlackBishop, squareset::c8}, {pieceset::BlackBishop, squareset::f8},
        {pieceset::BlackQueen, squareset::d8}, {pieceset::BlackKing, squareset::e8}
    };

    std::array<Square, 64> boardArray2
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

    BoardBase::PieceMap whitePieces2
    {
        {pieceset::WhitePawn, squareset::b2}, {pieceset::WhitePawn, squareset::f2}, {pieceset::WhitePawn, squareset::g2},
        {pieceset::WhitePawn, squareset::d5}, {pieceset::WhitePawn, squareset::e4}, {pieceset::WhiteKing, squareset::g1}
    };

    BoardBase::PieceMap blackPieces2
    {
        {pieceset::BlackPawn, squareset::a7}, {pieceset::BlackPawn, squareset::b7}, {pieceset::BlackPawn, squareset::c5},
        {pieceset::BlackPawn, squareset::d6}, {pieceset::BlackPawn, squareset::f7}, {pieceset::BlackPawn, squareset::f4},
        {pieceset::BlackPawn, squareset::g7}, {pieceset::BlackPawn, squareset::g5}, {pieceset::BlackKing, squareset::g8}
    };

    std::array<Square, 128> boardArray3
    {
        pieceset::WhiteRook, pieceset::WhiteKnight, pieceset::WhiteBishop, pieceset::WhiteQueen, pieceset::WhiteKing, pieceset::WhiteBishop, pieceset::WhiteKnight, pieceset::WhiteRook, 0, 0, 0, 0, 0, 0, 0, 0,
        pieceset::WhitePawn, pieceset::WhitePawn, pieceset::WhitePawn, pieceset::WhitePawn, pieceset::WhitePawn, pieceset::WhitePawn, pieceset::WhitePawn, pieceset::WhitePawn, 0, 0, 0, 0, 0, 0, 0, 0,
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        pieceset::BlackPawn, pieceset::BlackPawn, pieceset::BlackPawn, pieceset::BlackPawn, pieceset::BlackPawn, pieceset::BlackPawn, pieceset::BlackPawn, pieceset::BlackPawn, 0, 0, 0, 0, 0, 0, 0, 0,
        pieceset::BlackRook, pieceset::BlackKnight, pieceset::BlackBishop, pieceset::BlackQueen, pieceset::BlackKing, pieceset::BlackBishop, pieceset::BlackKnight, pieceset::BlackRook, 0, 0, 0, 0, 0, 0, 0, 0
    };

    BoardBase::PieceMap whitePieces3
    {
        {pieceset::WhitePawn, x88board::a2}, {pieceset::WhitePawn, x88board::b2}, {pieceset::WhitePawn, x88board::c2},
        {pieceset::WhitePawn, x88board::d2}, {pieceset::WhitePawn, x88board::e2}, {pieceset::WhitePawn, x88board::f2},
        {pieceset::WhitePawn, x88board::g2}, {pieceset::WhitePawn, x88board::h2},
        {pieceset::WhiteRook, x88board::a1}, {pieceset::WhiteRook, x88board::h1},
        {pieceset::WhiteKnight, x88board::b1}, {pieceset::WhiteKnight, x88board::g1},
        {pieceset::WhiteBishop, x88board::c1}, {pieceset::WhiteBishop, x88board::f1},
        {pieceset::WhiteQueen, x88board::d1}, {pieceset::WhiteKing, x88board::e1}
    };

    BoardBase::PieceMap blackPieces3
    {
        {pieceset::BlackPawn, x88board::a7}, {pieceset::BlackPawn, x88board::b7}, {pieceset::BlackPawn, x88board::c7},
        {pieceset::BlackPawn, x88board::d7}, {pieceset::BlackPawn, x88board::e7}, {pieceset::BlackPawn, x88board::f7},
        {pieceset::BlackPawn, x88board::g7}, {pieceset::BlackPawn, x88board::h7},
        {pieceset::BlackRook, x88board::a8}, {pieceset::BlackRook, x88board::h8},
        {pieceset::BlackKnight, x88board::b8}, {pieceset::BlackKnight, x88board::g8},
        {pieceset::BlackBishop, x88board::c8}, {pieceset::BlackBishop, x88board::f8},
        {pieceset::BlackQueen, x88board::d8}, {pieceset::BlackKing, x88board::e8}
    };

    std::array<Square, 128> boardArray4
    {
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, pieceset::WhiteKing, squareset::Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        squareset::Empty, pieceset::WhitePawn, squareset::Empty, squareset::Empty, squareset::Empty, pieceset::WhitePawn, pieceset::WhitePawn, squareset::Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, pieceset::WhitePawn, pieceset::BlackPawn, squareset::Empty, squareset::Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        squareset::Empty, squareset::Empty, pieceset::BlackPawn, pieceset::WhitePawn, squareset::Empty, squareset::Empty, pieceset::BlackPawn, squareset::Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        squareset::Empty, squareset::Empty, squareset::Empty, pieceset::BlackPawn, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        pieceset::BlackPawn, pieceset::BlackPawn, squareset::Empty, squareset::Empty, squareset::Empty, pieceset::BlackPawn, pieceset::BlackPawn, squareset::Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, squareset::Empty, pieceset::BlackKing, squareset::Empty, 0, 0, 0, 0, 0, 0, 0, 0
    };

    BoardBase::PieceMap whitePieces4
    {
        {pieceset::WhitePawn, x88board::b2}, {pieceset::WhitePawn, x88board::f2}, {pieceset::WhitePawn, x88board::g2},
        {pieceset::WhitePawn, x88board::d5}, {pieceset::WhitePawn, x88board::e4}, {pieceset::WhiteKing, x88board::g1}
    };

    BoardBase::PieceMap blackPieces4
    {
        {pieceset::BlackPawn, x88board::a7}, {pieceset::BlackPawn, x88board::b7}, {pieceset::BlackPawn, x88board::c5},
        {pieceset::BlackPawn, x88board::d6}, {pieceset::BlackPawn, x88board::f7}, {pieceset::BlackPawn, x88board::f4},
        {pieceset::BlackPawn, x88board::g7}, {pieceset::BlackPawn, x88board::g5}, {pieceset::BlackKing, x88board::g8}
    };

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

    std::string_view fen1{ "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Kkq c3 1 2" };
};

TEST_F(BoardTests, basic_board_constructor_starting_pos)
{
    auto b = basic_board::make_unique_board(Fen::StartingPosition);
    const auto& boardArray = b->GetBoard();

    EXPECT_TRUE(std::equal(boardArray.begin(), boardArray.end(), boardArray1.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::White);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), squareset::Empty);
    EXPECT_EQ(b->GetHalfMoveClock(), 0);
    EXPECT_EQ(b->GetFullMoveClock(), 1);
    EXPECT_EQ(b->GetWhitePieces(), whitePieces1);
    EXPECT_EQ(b->GetBlackPieces(), blackPieces1);
    EXPECT_EQ(Fen::StartingPosition, utility::chess::board_to_fen(*b));
}

TEST_F(BoardTests, basic_baord_constructor_pos1)
{
    auto b = basic_board::make_unique_board(fen1);
    const auto& boardArray = b->GetBoard();

    EXPECT_TRUE(std::equal(boardArray.begin(), boardArray.end(), boardArray2.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::Black);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_FALSE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), squareset::c3);
    EXPECT_EQ(b->GetHalfMoveClock(), 1);
    EXPECT_EQ(b->GetFullMoveClock(), 2);
    EXPECT_EQ(b->GetWhitePieces(), whitePieces2);
    EXPECT_EQ(b->GetBlackPieces(), blackPieces2);
    EXPECT_EQ(fen1, utility::chess::board_to_fen(*b));
}

TEST_F(BoardTests, basic_board_constructor_fen_compare) 
{  
    for (auto f : fens) 
    {
        auto b = basic_board::make_unique_board(f);
        EXPECT_EQ(f, utility::chess::board_to_fen(*b));
    }
}

TEST_F(BoardTests, x88board_constructor_starting_pos)
{
    auto b = x88board::make_unique_board(Fen::StartingPosition);
    const auto& boardArray = b->GetBoard();

    EXPECT_TRUE(std::equal(boardArray.begin(), boardArray.end(), boardArray3.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::White);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), squareset::Empty);
    EXPECT_EQ(b->GetHalfMoveClock(), 0);
    EXPECT_EQ(b->GetFullMoveClock(), 1);
    EXPECT_EQ(b->GetWhitePieces(), whitePieces3);
    EXPECT_EQ(b->GetBlackPieces(), blackPieces3);
    EXPECT_EQ(Fen::StartingPosition, utility::chess::board_to_fen(*b));
}

TEST_F(BoardTests, x88baord_constructor_pos1)
{
    auto b = x88board::make_unique_board(fen1);
    const auto& boardArray = b->GetBoard();

    EXPECT_TRUE(std::equal(boardArray.begin(), boardArray.end(), boardArray4.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::Black);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_FALSE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), x88board::c3);
    EXPECT_EQ(b->GetHalfMoveClock(), 1);
    EXPECT_EQ(b->GetFullMoveClock(), 2);
    EXPECT_EQ(b->GetWhitePieces(), whitePieces4);
    EXPECT_EQ(b->GetBlackPieces(), blackPieces4);
    EXPECT_EQ(fen1, utility::chess::board_to_fen(*b));
}

TEST_F(BoardTests, x88board_constructor_fen_compare)
{
    for (auto f : fens)
    {
        auto b = x88board::make_unique_board(f);
        EXPECT_EQ(f, utility::chess::board_to_fen(*b));
    }
}