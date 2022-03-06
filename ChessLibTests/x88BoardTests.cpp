#include "pch.h"

#include "BoardTestBase.hpp"

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/Board/x88Board.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

using namespace chesslib;
using namespace chesslib::pieceset;

Square Empty = chesslib::squareset::Empty;

class x88BoardTests : public ::testing::Test, public BoardTestBase
{
public:

    x88BoardTests() { }

protected:

    std::array<Square, 128> board_array_starting_position
    {
        WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook, 0, 0, 0, 0, 0, 0, 0, 0,
        WhitePawn, WhitePawn,   WhitePawn,   WhitePawn,  WhitePawn, WhitePawn,   WhitePawn,   WhitePawn, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty,     Empty,       Empty,       Empty,      Empty,     Empty,       Empty,       Empty,     0, 0, 0, 0, 0, 0, 0, 0,
        Empty,     Empty,       Empty,       Empty,      Empty,     Empty,       Empty,       Empty,     0, 0, 0, 0, 0, 0, 0, 0,
        Empty,     Empty,       Empty,       Empty,      Empty,     Empty,       Empty,       Empty,     0, 0, 0, 0, 0, 0, 0, 0,
        Empty,     Empty,       Empty,       Empty,      Empty,     Empty,       Empty,       Empty,     0, 0, 0, 0, 0, 0, 0, 0,
        BlackPawn, BlackPawn,   BlackPawn,   BlackPawn,  BlackPawn, BlackPawn,   BlackPawn,   BlackPawn, 0, 0, 0, 0, 0, 0, 0, 0,
        BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook, 0, 0, 0, 0, 0, 0, 0, 0
    };

    PieceCentricBoardBase::PieceMap white_pieces_starting_position
    {
        {WhitePawn, x88Board::a2}, {WhitePawn, x88Board::b2}, {WhitePawn, x88Board::c2}, {WhitePawn, x88Board::d2}, 
        {WhitePawn, x88Board::e2}, {WhitePawn, x88Board::f2}, {WhitePawn, x88Board::g2}, {WhitePawn, x88Board::h2}, 
        {WhiteRook, x88Board::a1}, {WhiteRook, x88Board::h1}, {WhiteKnight, x88Board::b1}, {WhiteKnight, x88Board::g1}, 
        {WhiteBishop, x88Board::c1}, {WhiteBishop, x88Board::f1}, {WhiteQueen, x88Board::d1}, {WhiteKing, x88Board::e1}
    };

    PieceCentricBoardBase::PieceMap black_pieces_starting_position
    {
        {BlackPawn, x88Board::a7}, {BlackPawn, x88Board::b7}, {BlackPawn, x88Board::c7}, {BlackPawn, x88Board::d7}, 
        {BlackPawn, x88Board::e7}, {BlackPawn, x88Board::f7}, {BlackPawn, x88Board::g7}, {BlackPawn, x88Board::h7}, 
        {BlackRook, x88Board::a8}, {BlackRook, x88Board::h8}, {BlackKnight, x88Board::b8}, {BlackKnight, x88Board::g8},
        {BlackBishop, x88Board::c8}, {BlackBishop, x88Board::f8}, {BlackQueen, x88Board::d8}, {BlackKing, x88Board::e8}
    };

    std::array<Square, 128> board_array_fen1
    {
        Empty,     Empty,     Empty,     Empty,     Empty,     Empty,     WhiteKing, Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty,     WhitePawn, Empty,     Empty,     Empty,     WhitePawn, WhitePawn, Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty,     Empty,     Empty,     Empty,     Empty,     Empty,     Empty,     Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty,     Empty,     Empty,     Empty,     WhitePawn, BlackPawn, Empty,     Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty,     Empty,     BlackPawn, WhitePawn, Empty,     Empty,     BlackPawn, Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty,     Empty,     Empty,     BlackPawn, Empty,     Empty,     Empty,     Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        BlackPawn, BlackPawn, Empty,     Empty,     Empty,     BlackPawn, BlackPawn, Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty,     Empty,     Empty,     Empty,     Empty,     Empty,     BlackKing, Empty, 0, 0, 0, 0, 0, 0, 0, 0
    };

    PieceCentricBoardBase::PieceMap white_pieces_fen1
    {
        {WhitePawn, x88Board::b2}, {WhitePawn, x88Board::f2}, {WhitePawn, x88Board::g2}, {WhitePawn, x88Board::d5}, 
        {WhitePawn, x88Board::e4}, {WhiteKing, x88Board::g1}
    };

    PieceCentricBoardBase::PieceMap black_pieces_fen1
    {
        {BlackPawn, x88Board::a7}, {BlackPawn, x88Board::b7}, {BlackPawn, x88Board::c5}, {BlackPawn, x88Board::d6}, 
        {BlackPawn, x88Board::f7}, {BlackPawn, x88Board::f4}, {BlackPawn, x88Board::g7}, {BlackPawn, x88Board::g5}, 
        {BlackKing, x88Board::g8}
    };
};

/*
TEST_F(x88BoardTests, constructor_starting_pos)
{
    auto b = make_unique_board(Fen::StartingPosition);
    const auto& board_array = b->GetBoard();

    EXPECT_TRUE(std::equal(board_array.begin(), board_array.end(), board_array_starting_position.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::White);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), squareset::None);
    EXPECT_EQ(b->GetHalfMoveClock(), 0);
    EXPECT_EQ(b->GetFullMoveClock(), 1);
    EXPECT_EQ(b->GetWhitePieces(), white_pieces_starting_position);
    EXPECT_EQ(b->GetBlackPieces(), black_pieces_starting_position);
    EXPECT_EQ(Fen::StartingPosition, utility::chess::board_to_fen(*b));
}

TEST_F(x88BoardTests, constructor_fen1)
{
    auto b = make_unique_board(fen_pos1);
    const auto& board_array = b->GetBoard();

    EXPECT_TRUE(std::equal(board_array.begin(), board_array.end(), board_array_fen1.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::Black);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_FALSE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), c3);
    EXPECT_EQ(b->GetHalfMoveClock(), 1);
    EXPECT_EQ(b->GetFullMoveClock(), 2);
    EXPECT_EQ(b->GetWhitePieces(), white_pieces_fen1);
    EXPECT_EQ(b->GetBlackPieces(), black_pieces_fen1);
    EXPECT_EQ(fen_pos1, utility::chess::board_to_fen(*b));
}

TEST_F(x88BoardTests, constructor_fen_compare)
{
    for (auto f : board_setup_fens)
    {
        auto b = make_unique_board(f);
        EXPECT_EQ(f, utility::chess::board_to_fen(*b));
    }
}

*/