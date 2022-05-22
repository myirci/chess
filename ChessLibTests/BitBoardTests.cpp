#include "pch.h"

#include "TestHelpers.hpp"

#include <ChessLib/Board/BitBoard.hpp>
#include <ChessLib/Board/BoardFactory.hpp>
#include <ChessLib/Chess/Fen.hpp>

using namespace chesslib;
using namespace chesslib::bitboard;

class BitBoardTests : public ::testing::Test
{
protected:

    static constexpr Index
        wPIdx = 0, wRIdx = 1, wNIdx = 2, wBIdx = 3,  wQIdx = 4,  wKIdx = 5,  wIdx = 6,
        bPIdx = 7, bRIdx = 8, bNIdx = 9, bBIdx = 10, bQIdx = 11, bKIdx = 12, bIdx = 13;

    std::array<Bitset, 14> starting_position_bitsets
    {
        0x000000000000FF00, 0x0000000000000081, 0x0000000000000042, 0x0000000000000024, 0x0000000000000008, 0x0000000000000010, 0x000000000000FFFF,
        0x00FF000000000000, 0x8100000000000000, 0x4200000000000000, 0x2400000000000000, 0x0800000000000000, 0x1000000000000000, 0xFFFF000000000000
    };

    std::array<Bitset, 14> fen1_bitsets
    {
        0x0000000810006200, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000040, 0x0000000810006240,
        0x0063084420000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x4000000000000000, 0x4063084420000000
    };
};

TEST_F(BitBoardTests, square_from_bitmask) 
{
    for (int i = 0; i < BitBoard::BOARDSIZE; i++) 
    {
        Bitset mask = BitBoard::SquareMask[BitBoard::BottomToTopOrder[i]];
        auto hash = (std::size_t)std::log2(mask);
        EXPECT_EQ(BitBoard::BottomToTopOrder[hash], BitBoard::BottomToTopOrder[i]);
    }
}

TEST_F(BitBoardTests, set_board_starting_pos)
{
    auto b = BoardFactory::make_unique_board<BitBoard>(Fen::StartingPosition);

    const auto& wpieces = b->GetWhitePieceSet();  
    EXPECT_EQ(wpieces[BitBoard::Pawn],   starting_position_bitsets[wPIdx]);
    EXPECT_EQ(wpieces[BitBoard::Rook],   starting_position_bitsets[wRIdx]);
    EXPECT_EQ(wpieces[BitBoard::Knight], starting_position_bitsets[wNIdx]);
    EXPECT_EQ(wpieces[BitBoard::Bishop], starting_position_bitsets[wBIdx]);
    EXPECT_EQ(wpieces[BitBoard::Queen],  starting_position_bitsets[wQIdx]);
    EXPECT_EQ(wpieces[BitBoard::King],   starting_position_bitsets[wKIdx]);
    EXPECT_EQ(wpieces[BitBoard::All],    starting_position_bitsets[wIdx]);

    const auto& bpieces = b->GetBlackPieceSet();
    EXPECT_EQ(bpieces[BitBoard::Pawn],   starting_position_bitsets[bPIdx]);
    EXPECT_EQ(bpieces[BitBoard::Rook],   starting_position_bitsets[bRIdx]);
    EXPECT_EQ(bpieces[BitBoard::Knight], starting_position_bitsets[bNIdx]);
    EXPECT_EQ(bpieces[BitBoard::Bishop], starting_position_bitsets[bBIdx]);
    EXPECT_EQ(bpieces[BitBoard::Queen],  starting_position_bitsets[bQIdx]);
    EXPECT_EQ(bpieces[BitBoard::King],   starting_position_bitsets[bKIdx]);
    EXPECT_EQ(bpieces[BitBoard::All],    starting_position_bitsets[bIdx]);

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

TEST_F(BitBoardTests, set_board_fen1)
{
    constexpr std::string_view fenStr = "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Kkq c3 0 2";
    auto b = BoardFactory::make_unique_board<BitBoard>(fenStr);
    
    const auto& wpieces = b->GetWhitePieceSet();
    EXPECT_EQ(wpieces[BitBoard::Pawn],   fen1_bitsets[wPIdx]);
    EXPECT_EQ(wpieces[BitBoard::Rook],   fen1_bitsets[wRIdx]);
    EXPECT_EQ(wpieces[BitBoard::Knight], fen1_bitsets[wNIdx]);
    EXPECT_EQ(wpieces[BitBoard::Bishop], fen1_bitsets[wBIdx]);
    EXPECT_EQ(wpieces[BitBoard::Queen],  fen1_bitsets[wQIdx]);
    EXPECT_EQ(wpieces[BitBoard::King],   fen1_bitsets[wKIdx]);
    EXPECT_EQ(wpieces[BitBoard::All],    fen1_bitsets[wIdx]);

    const auto& bpieces = b->GetBlackPieceSet();
    EXPECT_EQ(bpieces[BitBoard::Pawn],   fen1_bitsets[bPIdx]);
    EXPECT_EQ(bpieces[BitBoard::Rook],   fen1_bitsets[bRIdx]);
    EXPECT_EQ(bpieces[BitBoard::Knight], fen1_bitsets[bNIdx]);
    EXPECT_EQ(bpieces[BitBoard::Bishop], fen1_bitsets[bBIdx]);
    EXPECT_EQ(bpieces[BitBoard::Queen],  fen1_bitsets[bQIdx]);
    EXPECT_EQ(bpieces[BitBoard::King],   fen1_bitsets[bKIdx]);
    EXPECT_EQ(bpieces[BitBoard::All],    fen1_bitsets[bIdx]);

    EXPECT_EQ(b->GetActiveColor(), color::Black);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_FALSE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), ChessBoard::c3);
    EXPECT_EQ(b->GetHalfMoveClock(), 0);
    EXPECT_EQ(b->GetFullMoveClock(), 2);

    EXPECT_EQ(fenStr, board_to_fen(*b));
}

TEST_F(BitBoardTests, setup_board_and_board_to_fen)
{
    auto scoped_open = ScopedOpen(TestHelpers::BoardSetupTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-2");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<BitBoard>(lines[(size_t)(i + 1)]);
        EXPECT_EQ(lines[(size_t)(i + 1)], board_to_fen(*b));
    }
}

TEST_F(BitBoardTests, make_unmake_moves)
{
    auto scoped_open = ScopedOpen(TestHelpers::MakeUnMakeMovesTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-1");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 4 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 4)
    {
        const auto& fen1 = lines[(size_t)(i + 2)];
        const auto& fen2 = lines[(size_t)(i + 3)];

        auto mv = TestHelpers::GetMove<BitBoard>(lines[(size_t)(i + 1)]);
        auto b = BoardFactory::make_unique_board<BitBoard>(fen1);

        make_move(*b, mv);
        EXPECT_EQ(fen2, board_to_fen(*b));

        unmake_move(*b);
        EXPECT_EQ(fen1, board_to_fen(*b));
    }
}