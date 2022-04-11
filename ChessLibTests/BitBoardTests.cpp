#include "pch.h"

#include "BoardTestBase.hpp"

#include <ChessLib/Board/BitBoard.hpp>
#include <ChessLib/Board/BoardFactory.hpp>
#include <ChessLib/Chess/Fen.hpp>

using namespace chesslib;

class BitBoardTests :
    public ::testing::Test,
    public BoardTestBase
{
protected:

    static constexpr Index
        wPIdx = 0, wRIdx = 1, wNIdx = 2, wBIdx = 3,  wQIdx = 4,  wKIdx = 5,  wIdx = 6,
        bPIdx = 7, bRIdx = 8, bNIdx = 9, bBIdx = 10, bQIdx = 11, bKIdx = 12, bIdx = 13;

    std::array<bitboard::Bitset, 14> starting_position_bitsets
    {
        0x000000000000FF00, 0x0000000000000081, 0x0000000000000042, 0x0000000000000024, 0x0000000000000008, 0x0000000000000010, 0x000000000000FFFF,
        0x00FF000000000000, 0x8100000000000000, 0x4200000000000000, 0x2400000000000000, 0x0800000000000000, 0x1000000000000000, 0xFFFF000000000000
    };

    std::array<bitboard::Bitset, 14> fen1_bitsets
    {
        0x0000000810006200, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000040, 0x0000000810006240,
        0x0063084420000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x4000000000000000, 0x4063084420000000
    };
};

TEST_F(BitBoardTests, constructor_starting_pos)
{
    auto b = BoardFactory::make_unique_board<bitboard::BitBoard>(Fen::StartingPosition);

    const auto& wpieces = b->GetWhitePieceSet();  
    EXPECT_EQ(wpieces[bitboard::BitBoard::Pawn],   starting_position_bitsets[wPIdx]);
    EXPECT_EQ(wpieces[bitboard::BitBoard::Rook],   starting_position_bitsets[wRIdx]);
    EXPECT_EQ(wpieces[bitboard::BitBoard::Knight], starting_position_bitsets[wNIdx]);
    EXPECT_EQ(wpieces[bitboard::BitBoard::Bishop], starting_position_bitsets[wBIdx]);
    EXPECT_EQ(wpieces[bitboard::BitBoard::Queen],  starting_position_bitsets[wQIdx]);
    EXPECT_EQ(wpieces[bitboard::BitBoard::King],   starting_position_bitsets[wKIdx]);
    EXPECT_EQ(wpieces[bitboard::BitBoard::All],    starting_position_bitsets[wIdx]);

    const auto& bpieces = b->GetBlackPieceSet();
    EXPECT_EQ(bpieces[bitboard::BitBoard::Pawn],   starting_position_bitsets[bPIdx]);
    EXPECT_EQ(bpieces[bitboard::BitBoard::Rook],   starting_position_bitsets[bRIdx]);
    EXPECT_EQ(bpieces[bitboard::BitBoard::Knight], starting_position_bitsets[bNIdx]);
    EXPECT_EQ(bpieces[bitboard::BitBoard::Bishop], starting_position_bitsets[bBIdx]);
    EXPECT_EQ(bpieces[bitboard::BitBoard::Queen],  starting_position_bitsets[bQIdx]);
    EXPECT_EQ(bpieces[bitboard::BitBoard::King],   starting_position_bitsets[bKIdx]);
    EXPECT_EQ(bpieces[bitboard::BitBoard::All],    starting_position_bitsets[bIdx]);

    EXPECT_EQ(b->GetActiveColor(), color::White);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), Empty);
    EXPECT_EQ(b->GetHalfMoveClock(), 0);
    EXPECT_EQ(b->GetFullMoveClock(), 1);

    EXPECT_EQ(Fen::StartingPosition, board_to_fen(*b));
}

TEST_F(BitBoardTests, constructor_fen1)
{
    auto b = BoardFactory::make_unique_board<bitboard::BitBoard>(fen_pos1);
    
    const auto& wpieces = b->GetWhitePieceSet();
    EXPECT_EQ(wpieces[bitboard::BitBoard::Pawn],   fen1_bitsets[wPIdx]);
    EXPECT_EQ(wpieces[bitboard::BitBoard::Rook],   fen1_bitsets[wRIdx]);
    EXPECT_EQ(wpieces[bitboard::BitBoard::Knight], fen1_bitsets[wNIdx]);
    EXPECT_EQ(wpieces[bitboard::BitBoard::Bishop], fen1_bitsets[wBIdx]);
    EXPECT_EQ(wpieces[bitboard::BitBoard::Queen],  fen1_bitsets[wQIdx]);
    EXPECT_EQ(wpieces[bitboard::BitBoard::King],   fen1_bitsets[wKIdx]);
    EXPECT_EQ(wpieces[bitboard::BitBoard::All],    fen1_bitsets[wIdx]);

    const auto& bpieces = b->GetBlackPieceSet();
    EXPECT_EQ(bpieces[bitboard::BitBoard::Pawn],   fen1_bitsets[bPIdx]);
    EXPECT_EQ(bpieces[bitboard::BitBoard::Rook],   fen1_bitsets[bRIdx]);
    EXPECT_EQ(bpieces[bitboard::BitBoard::Knight], fen1_bitsets[bNIdx]);
    EXPECT_EQ(bpieces[bitboard::BitBoard::Bishop], fen1_bitsets[bBIdx]);
    EXPECT_EQ(bpieces[bitboard::BitBoard::Queen],  fen1_bitsets[bQIdx]);
    EXPECT_EQ(bpieces[bitboard::BitBoard::King],   fen1_bitsets[bKIdx]);
    EXPECT_EQ(bpieces[bitboard::BitBoard::All],    fen1_bitsets[bIdx]);

    EXPECT_EQ(b->GetActiveColor(), color::Black);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_FALSE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), ChessBoard::c3);
    EXPECT_EQ(b->GetHalfMoveClock(), 1);
    EXPECT_EQ(b->GetFullMoveClock(), 2);

    EXPECT_EQ(fen_pos1, board_to_fen(*b));
}

TEST_F(BitBoardTests, constructor_fen_compare)
{
    for (auto f : board_setup_fens)
    {
        auto b = BoardFactory::make_unique_board<bitboard::BitBoard>(f);
        EXPECT_EQ(f, board_to_fen(*b));
    }
}