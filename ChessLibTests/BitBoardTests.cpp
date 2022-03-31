#include "pch.h"

#include <ChessLib/Board/BitBoard.hpp>
#include <ChessLib/Board/BoardFactory.hpp>

using namespace chesslib;

class BitBoardTests :
    public ::testing::Test
{

};

TEST_F(BitBoardTests, constructor_starting_pos)
{
    auto b = BoardFactory::make_unique_board<bitboard::BitBoard>(Fen::StartingPosition);

    // const auto& board_array = b->GetBoard();
    // EXPECT_TRUE(std::equal(board_array.begin(), board_array.end(), board_array_starting_position.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::White);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), squareset::None);
    EXPECT_EQ(b->GetHalfMoveClock(), 0);
    EXPECT_EQ(b->GetFullMoveClock(), 1);
    // EXPECT_EQ(b->GetWhitePieces(), white_pieces_starting_position);
    // EXPECT_EQ(b->GetBlackPieces(), black_pieces_starting_position);
    // EXPECT_EQ(Fen::StartingPosition, utility::chess::board_to_fen(*b));
}
