#include "pch.h"

#include <ChessLib/Chess/Move.hpp>

using namespace chesslib;

TEST(MoveTest, Promotion)
{
    Move m1{0, 1, MoveType::Knight_Promotion};
    EXPECT_TRUE(m1.IsPromotion());

    Move m2{ 0, 1, MoveType::Knight_Promotion_Capture};
    EXPECT_TRUE(m2.IsPromotion());

    Move m3{ 0, 1, MoveType::Bishop_Promotion};
    EXPECT_TRUE(m3.IsPromotion());

    Move m4{ 0, 1, MoveType::Bishop_Promotion_Capture};
    EXPECT_TRUE(m4.IsPromotion());

    Move m5{ 0, 1, MoveType::Rook_Promotion};
    EXPECT_TRUE(m5.IsPromotion());

    Move m6{ 0, 1, MoveType::Rook_Promotion_Capture};
    EXPECT_TRUE(m6.IsPromotion());

    Move m7{ 0, 1, MoveType::Queen_Promotion};
    EXPECT_TRUE(m7.IsPromotion());

    Move m8{ 0, 1, MoveType::Queen_Promotion_Capture};
    EXPECT_TRUE(m8.IsPromotion());

    Move m9{ 0, 1, MoveType::Quite};
    EXPECT_FALSE(m9.IsPromotion());

    Move m10{ 0, 1, MoveType::Capture};
    EXPECT_FALSE(m10.IsPromotion());

    Move m11{ 0, 1, MoveType::Double_Pawn_Push};
    EXPECT_FALSE(m11.IsPromotion());

    Move m12{ 0, 1, MoveType::En_Passant_Capture};
    EXPECT_FALSE(m12.IsPromotion());

    Move m13{ 0, 1, MoveType::King_Side_Castle};
    EXPECT_FALSE(m13.IsPromotion());

    Move m14{ 0, 1, MoveType::Queen_Side_Castle};
    EXPECT_FALSE(m14.IsPromotion());
}

TEST(MoveTest, Capture)
{
    Move m1{ 0, 1, MoveType::Capture};
    EXPECT_TRUE(m1.IsCapture());

    Move m2{ 0, 1, MoveType::Knight_Promotion_Capture};
    EXPECT_TRUE(m2.IsCapture());

    Move m3{ 0, 1, MoveType::Bishop_Promotion_Capture};
    EXPECT_TRUE(m3.IsCapture());

    Move m4{ 0, 1, MoveType::Rook_Promotion_Capture};
    EXPECT_TRUE(m4.IsCapture());

    Move m5{ 0, 1, MoveType::Queen_Promotion_Capture};
    EXPECT_TRUE(m5.IsCapture());

    Move m6{ 0, 1, MoveType::En_Passant_Capture};
    EXPECT_TRUE(m6.IsCapture());

    Move m7{ 0, 1, MoveType::Quite};
    EXPECT_FALSE(m7.IsCapture());

    Move m8{ 0, 1, MoveType::Double_Pawn_Push};
    EXPECT_FALSE(m8.IsCapture());

    Move m9{ 0, 1, MoveType::King_Side_Castle};
    EXPECT_FALSE(m9.IsCapture());

    Move m10{ 0, 1, MoveType::Queen_Side_Castle};
    EXPECT_FALSE(m10.IsCapture());

    Move m11{ 0, 1, MoveType::Bishop_Promotion};
    EXPECT_FALSE(m11.IsCapture());

    Move m12{ 0, 1, MoveType::Rook_Promotion};
    EXPECT_FALSE(m12.IsCapture());

    Move m13{ 0, 1, MoveType::Knight_Promotion};
    EXPECT_FALSE(m13.IsCapture());

    Move m14{ 0, 1, MoveType::Queen_Promotion};
    EXPECT_FALSE(m14.IsCapture());
}

TEST(MoveTest, Castle)
{
    Move m1{ 0, 1, MoveType::King_Side_Castle};
    EXPECT_TRUE(m1.IsCastle());

    Move m2{ 0, 1, MoveType::Queen_Side_Castle};
    EXPECT_TRUE(m2.IsCastle());

    Move m3{ 0, 1, MoveType::Quite};
    EXPECT_FALSE(m3.IsCastle());

    Move m4{ 0, 1, MoveType::Capture};
    EXPECT_FALSE(m4.IsCastle());
}

TEST(MoveTest, EnPassant)
{
    Move m1{ 0, 1, MoveType::En_Passant_Capture};
    EXPECT_TRUE(m1.IsEnPassantCapture());

    Move m2{ 0, 1, MoveType::Quite};
    EXPECT_FALSE(m2.IsEnPassantCapture());

    Move m3{ 0, 1, MoveType::Capture};
    EXPECT_FALSE(m3.IsEnPassantCapture());
}
