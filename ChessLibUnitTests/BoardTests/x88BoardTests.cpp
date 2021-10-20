#include <pch.h>

#include "CommonBoardTestBase.hpp"

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/Board/X88Board/x88Board.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

using namespace chesslib;
using namespace chesslib::pieceset;
using namespace chesslib::x88board;

class x88BoardTests : public ::testing::Test, public CommonBoardTestBase
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

    MailboxBoardBase::PieceMap white_pieces_starting_position
    {
        {WhitePawn, a2}, {WhitePawn, b2}, {WhitePawn, c2}, {WhitePawn, d2}, {WhitePawn, e2}, 
        {WhitePawn, f2}, {WhitePawn, g2}, {WhitePawn, h2}, {WhiteRook, a1}, {WhiteRook, h1},
        {WhiteKnight, b1}, {WhiteKnight, g1}, {WhiteBishop, c1}, {WhiteBishop, f1}, {WhiteQueen, d1}, 
        {WhiteKing, e1}
    };

    MailboxBoardBase::PieceMap black_pieces_starting_position
    {
        {BlackPawn, a7}, {BlackPawn, b7}, {BlackPawn, c7}, {BlackPawn, d7}, {BlackPawn, e7}, 
        {BlackPawn, f7}, {BlackPawn, g7}, {BlackPawn, h7}, {BlackRook, a8}, {BlackRook, h8},
        {BlackKnight, b8}, {BlackKnight, g8}, {BlackBishop, c8}, {BlackBishop, f8}, {BlackQueen, d8}, 
        {BlackKing, e8}
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

    MailboxBoardBase::PieceMap white_pieces_fen1
    {
        {WhitePawn, b2}, {WhitePawn, f2}, {WhitePawn, g2}, {WhitePawn, d5}, {WhitePawn, e4}, {WhiteKing, g1}
    };

    MailboxBoardBase::PieceMap black_pieces_fen1
    {
        {BlackPawn, a7}, {BlackPawn, b7}, {BlackPawn, c5}, {BlackPawn, d6}, {BlackPawn, f7}, {BlackPawn, f4},
        {BlackPawn, g7}, {BlackPawn, g5}, {BlackKing, g8}
    };
};

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