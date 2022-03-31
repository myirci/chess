#include "pch.h"

#include "ClassicBoardTestBase.hpp"

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/Board/BasicBoard.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>
#include <ChessLib/Board/BoardFactory.hpp>

#include <bitset>

using namespace chesslib;
using namespace chesslib::squareset;
using namespace chesslib::pieceset;
using namespace chesslib::utility::chess;

class BasicBoardTests : 
    public ::testing::Test, 
    public ClassicBoardTestBase
{

};

TEST_F(BasicBoardTests, constructor_starting_pos)
{
    auto b = BoardFactory::make_unique_board<BasicBoard>(Fen::StartingPosition);
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

TEST_F(BasicBoardTests, constructor_fen1)
{
    auto b = BoardFactory::make_unique_board<BasicBoard>(fen_pos1);
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

TEST_F(BasicBoardTests, constructor_fen_compare)
{  
    for (auto f : board_setup_fens) 
    {
        auto b = BoardFactory::make_unique_board<BasicBoard>(f);
        EXPECT_EQ(f, utility::chess::board_to_fen(*b));
    }
}

/*

TEST_F(BasicBoardTests, make_unmake_moves)
{
    for (const auto& [move, fen1, fen2] : make_unmake_move_fens)
    {
        auto b = basic_board::make_unique_board(fen1);
        b->MakeMove(move);
        EXPECT_EQ(fen2, utility::chess::board_to_fen(*b));
        b->UnMakeMove();
        EXPECT_EQ(fen1, utility::chess::board_to_fen(*b));
    }
}

*/