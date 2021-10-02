#include "pch.h"

#include "BasicAndObjectBoardBase.hpp"

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/BasicBoard/BasicBoard.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

#include <bitset>

using namespace chesslib;
using namespace chesslib::squareset;
using namespace chesslib::pieceset;

class BasicBoardTests : public ::testing::Test, public BasicAndObjectBoardBase
{
public:
    BasicBoardTests() { }

protected:
};

TEST_F(BasicBoardTests, constructor_starting_pos)
{
    auto b = basic_board::make_unique_board(Fen::StartingPosition);
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
    auto b = basic_board::make_unique_board(fen_pos1);
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
        auto b = basic_board::make_unique_board(f);
        EXPECT_EQ(f, utility::chess::board_to_fen(*b));
    }
}

TEST_F(BasicBoardTests, move_generation_king_only_moves) 
{
    for (auto i{0}; i < move_generator_king_move_fens.size(); i++)
    {
        auto b = basic_board::make_unique_board(move_generator_king_move_fens[i]);
        auto moves = b->GenerateMoves();
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expected_king_moves[i].begin()));
    }
}

TEST_F(BasicBoardTests, move_generation_check_eliminating_moves) 
{
    for (auto i{ 0 }; i < move_generator_check_eliminating_move_fens.size(); i++)
    {
        auto b = basic_board::make_unique_board(move_generator_check_eliminating_move_fens[i]);
        auto moves = b->GenerateMoves();
        EXPECT_TRUE(IsSubset(moves, expected_check_eliminating_moves[i]));
    }
}

TEST_F(BasicBoardTests, move_generation)
{
    for (auto i{ 0 }; i < move_generation_fens.size(); i++)
    {
        auto b = basic_board::make_unique_board(move_generation_fens[i]);
        auto moves = b->GenerateMoves();
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expected_moves[i].begin()));
    }
}