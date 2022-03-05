#include "pch.h"

#include <vector>
#include <ChessLib/Chess/Fen.hpp>

using namespace chesslib;

TEST(FenTests, DefaultConstructor)
{
    Fen f;
    EXPECT_EQ(f.Get_PiecePlacement(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    EXPECT_EQ(f.Get_ActiveColor(), "w");
    EXPECT_EQ(f.Get_CastlingAvailability(), "KQkq");
    EXPECT_EQ(f.Get_EnPassantTargetSquare(), "-");
    EXPECT_EQ(f.Get_HalfMoveClock(), "0");
    EXPECT_EQ(f.Get_FullMoveClock(), "1");
    EXPECT_EQ(f.Get_Fen(), Fen::StartingPosition);
}

TEST(FenTests, Constructor_FenWithSixFields)
{
    std::string_view fs{ "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq c3 0 2" };
    Fen f(fs);
    EXPECT_EQ(f.Get_PiecePlacement(), "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1");
    EXPECT_EQ(f.Get_ActiveColor(), "b");
    EXPECT_EQ(f.Get_CastlingAvailability(), "KQkq");
    EXPECT_EQ(f.Get_EnPassantTargetSquare(), "c3");
    EXPECT_EQ(f.Get_HalfMoveClock(), "0");
    EXPECT_EQ(f.Get_FullMoveClock(), "2");
    EXPECT_EQ(f.Get_Fen(), fs);
}

TEST(FenTests, Constructor_FenWithFourFields)
{
    std::string_view fs{ "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w KQkq c6" };
    Fen f(fs);
    EXPECT_EQ(f.Get_PiecePlacement(), "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1");
    EXPECT_EQ(f.Get_ActiveColor(), "w");
    EXPECT_EQ(f.Get_CastlingAvailability(), "KQkq");
    EXPECT_EQ(f.Get_EnPassantTargetSquare(), "c6");
    EXPECT_EQ(f.Get_HalfMoveClock(), std::nullopt);
    EXPECT_EQ(f.Get_FullMoveClock(), std::nullopt);
    EXPECT_EQ(f.Get_Fen(), fs);
}

TEST(FenTests, Constructor_FenWithSpaces)
{
    std::string_view fs{ "    6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1   b      Kkq       c3      0    2      " };
    std::string_view fs_clean{ "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Kkq c3 0 2" };
    Fen f(fs);
    EXPECT_EQ(f.Get_PiecePlacement(), "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1");
    EXPECT_EQ(f.Get_ActiveColor(), "b");
    EXPECT_EQ(f.Get_CastlingAvailability(), "Kkq");
    EXPECT_EQ(f.Get_EnPassantTargetSquare(), "c3");
    EXPECT_EQ(f.Get_HalfMoveClock(), "0");
    EXPECT_EQ(f.Get_FullMoveClock(), "2");
    EXPECT_EQ(f.Get_Fen(), fs_clean);
}

TEST(FenTests, Constructor_FenWithWhiteSpaces)
{
    std::string_view fs{ "    6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1  \t w   \t\t  \n -  \t  \v   c6  \f  \r  0    2     \t " };
    std::string_view fs_clean{ "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w - c6 0 2" };
    Fen f(fs);
    EXPECT_EQ(f.Get_PiecePlacement(), "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1");
    EXPECT_EQ(f.Get_ActiveColor(), "w");
    EXPECT_EQ(f.Get_CastlingAvailability(), "-");
    EXPECT_EQ(f.Get_EnPassantTargetSquare(), "c6");
    EXPECT_EQ(f.Get_HalfMoveClock(), "0");
    EXPECT_EQ(f.Get_FullMoveClock(), "2");
    EXPECT_EQ(f.Get_Fen(), fs_clean);
}

TEST(FenTests, Constructor_InvalidNumberOfFields)
{
    EXPECT_THROW(Fen(""), std::logic_error);
    EXPECT_THROW(Fen(" "), std::logic_error);
    EXPECT_THROW(Fen("                                                   "), std::logic_error);
    EXPECT_THROW(Fen("\t \n \f \r \v \t"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 - c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/ 4Pp2/8/1P3PP1/6K1 b - c6 1 2"), std::logic_error);
}

TEST(FenTests, Constructor_InvalidPiecePlacement)
{
    // Nonsense 
    EXPECT_THROW(Fen("x x x x x x"), std::logic_error);

    // Wrong number of ranks
    EXPECT_THROW(Fen("6k1/pp3pp1/2pP2p1/4Pp2/8/1P3PP1/6K1 b - c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/8/2pP2p1/4Pp2/8/1P3PP1/6K1 b - c6 1 2"), std::logic_error);

    // pawn at 8th rank
    EXPECT_THROW(Fen("5pk1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Kkq c6 1 2"), std::logic_error);

    // pawn at 1st rank
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/p5K1 b Kkq c6 1 2"), std::logic_error);

    // Wrong number of squares in a rank
    EXPECT_THROW(Fen("6k2/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Kkq c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Kkq c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3pp4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Kkq c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2p1P2p1/4Pp2/8/1P3PP1/6K1 b Kkq c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp4/8/1P3PP1/6K1 b Kkq c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/6/1P3PP1/6K1 b Kkq c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/9/1P3PP1/6K1 b Kkq c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K3 b Kkq c6 1 2"), std::logic_error);

    // Two numbers next to each other
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/44/1P3PP1/6K1 b - c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3p11/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p211/2pP2p1/4Pp2/8/1P3PP1/6K1 b - c6 1 2"), std::logic_error);

    // Invalid char
    EXPECT_THROW(Fen("6k1/pp3p0p1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3po1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3p-1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pa2/8/1P3PP1/6K1 b - c6 1 2"), std::logic_error);

    // More/Less than one white king
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PPK/6K1 b - c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6N1 b - c6 1 2"), std::logic_error);

    // More/Less than one black king
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/k7/1P3PP1/6K1 b - c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6n1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - c6 1 2"), std::logic_error);

    // More than 8 white pawn
    EXPECT_THROW(Fen("6k1/pp3PP1/3PP3/2pP2pP/4Pp2/8/1P3PP1/6K1 b - c6 1 2"), std::logic_error);

    // More than 8 black pawn
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PPp/6K1 b - c6 1 2"), std::logic_error);

    // More extra white pieces than missing pawns
    EXPECT_THROW(Fen("6k1/pp3ppP/3p4/2pP2p1/RR2Pp2/QQQ4R/1P3PP1/6K1 b - c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/NN1NN1NN/1P3PP1/6K1 b - c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3pRRRR/2pP2p1/BBBPp2/8/1P3PP1/NNN3K1 b - c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/N3Pp2/BBBNQQQQ/1P3NN1/6K1 b - c6 1 2"), std::logic_error);

    // More extra black pieces than missing pawns
    EXPECT_THROW(Fen("6k1/pp3pp1/3prr1r/2pP2p1/4Pp2/8/1P3PP1/6K1 b - c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/ppnnnpp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pN3pN1/3pN2N/2pP2p1/NNPp2/8/1P3PP1/6K1 b - c6 1 2"), std::logic_error);
    EXPECT_THROW(Fen("5qk1/pp3pqq/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - c6 1 2"), std::logic_error);
}

TEST(FenTests, InvalidActiveColor)
{
    // Invalid characters
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 a KQkq c6"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 k KQkq c6"), std::logic_error);
}

TEST(FenTests, CastlingRights)
{
    auto fen_vec = std::vector<std::pair<Fen, std::string_view>>
    { 
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - -"}, "-" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b K -"}, "K" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b k -"}, "k" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Q -"}, "Q" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b q -"}, "q" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Kk -"}, "Kk" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Kq -"}, "Kq" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Qk -"}, "Qk" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Qq -"}, "Qq" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQ -"}, "KQ" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b kq -"}, "kq" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Kkq -"}, "Kkq" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Qkq -"}, "Qkq" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQk -"}, "KQk" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQq -"}, "KQq" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b KQkq -"}, "KQkq" }
    };

    for (const auto& fp : fen_vec) 
        EXPECT_EQ(fp.first.Get_CastlingAvailability(), fp.second);
}

TEST(FenTests, InvalidCastlingRights)
{
    // Invalid characters
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w KQkqk -"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w KQqkq -"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w Kka -"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w w -"), std::logic_error);

    // Reverse order
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w kqKQ -"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w QK -"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w kK -"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w qK -"), std::logic_error);
}

TEST(FenTests, EnpassantTargetSquare)
{
    auto fen_vec = std::vector<std::pair<Fen, std::string_view>>
    { 
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - -"}, "-" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - a3"}, "a3" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - b3"}, "b3" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - c3"}, "c3" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - d3"}, "d3" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - e3"}, "e3" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - f3"}, "f3" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - g3"}, "g3" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - h3"}, "h3" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w - a6"}, "a6" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w - b6"}, "b6" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w - c6"}, "c6" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w - d6"}, "d6" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w - e6"}, "e6" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w - f6"}, "f6" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w - g6"}, "g6" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w - h6"}, "h6" }
    };

    for (const auto& fp : fen_vec)
        EXPECT_EQ(fp.first.Get_EnPassantTargetSquare(), fp.second);
}

TEST(FenTests, InvalidEnpassantTargetSquare)
{
    // Invalid characters
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - a"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - b"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - h"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - i"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - 6"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - 3"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - 4"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - --"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - a2"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - d4"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - f7"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - k"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - a3b"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - d6c"), std::logic_error);

    // Color mismatch
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w - h3"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - a6"), std::logic_error);
}

TEST(FenTests, HalfMoveClock)
{
    auto fen_vec = std::vector<std::pair<Fen, std::string_view>>
    {
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 0 1"}, "0" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - a3 0 13"}, "0" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 19 23"}, "19" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 1 8"}, "1" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 50 85"}, "50" }
    };

    for (const auto& fp : fen_vec)
        EXPECT_EQ(fp.first.Get_HalfMoveClock(), fp.second);
}

TEST(FenTests, InvalidHalfMoveClock)
{
    // two digit - starts with zer0
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 00 1"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 01 1"), std::logic_error);

    // non-zero but en-passant-target square is set
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - d3 4 12"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 w - f6 16 12"), std::logic_error);

    // invalid chars
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - a 1"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 1k 1"), std::logic_error);
}

TEST(FenTests, FullMoveClock) 
{
    auto fen_vec = std::vector<std::pair<Fen, std::string_view>>
    {
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 0 1"}, "1" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - a3 0 13"}, "13" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 19 45"}, "45" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 1 89"}, "89" },
        { Fen{"6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 50 345"}, "345" }
    };

    for (const auto& fp : fen_vec)
        EXPECT_EQ(fp.first.Get_FullMoveClock(), fp.second);
}

TEST(FenTests, InvalidFullMoveClock)
{
    // more than one digit - starts with zer0
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 0 01"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 0 021"), std::logic_error);

    // invalid chars
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 0 q"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 0 as"), std::logic_error);
    EXPECT_THROW(Fen("6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b - - 0 12s"), std::logic_error);
}