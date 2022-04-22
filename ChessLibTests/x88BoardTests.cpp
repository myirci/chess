#include "pch.h"

#include "TestHelpers.hpp"

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/Board/x88Board.hpp>
#include <ChessLib/Board/BoardFactory.hpp>
#include <ChessLib/Board/BoardFunctions.hpp>
#include <ChessLib/Board/ChessBoard.hpp>

using namespace chesslib;
using namespace chesslib::pieceset;

class x88BoardTests : public ::testing::Test
{
protected:

    static void GetBoardArrayAndPieces(std::string& line, x88Board::BoardArray& bArray, x88Board::PieceMap& whitePieces, x88Board::PieceMap& blackPieces)
    {
        line.erase(std::remove_if(line.begin(), line.end(), [](char c) {return c == ','; }), line.end());
        if (line.size() != ChessBoard::BOARDSIZE)
            throw std::exception("Error in the number of squares");

        std::fill(bArray.begin(), bArray.end(), Empty);

        for (auto i{ 0 }; i < ChessBoard::BOARDSIZE; i++)
        {
            if (line[i] == 'e')
                continue;

            auto idx = x88Board::BottomToTopOrder[i];
            bArray[idx] = char_to_piece.at(line[i]);
            if (bArray[idx] > 0)
                whitePieces.emplace(bArray[idx], idx);
            else
                blackPieces.emplace(bArray[idx], idx);
        }
    }
};

TEST_F(x88BoardTests, SquareFileRank)
{
    Square i{ 0 };
    for (Rank rk = 0; rk < 8; rk++)
    {
        for (File fl = 0; fl < 16; fl++)
        {
            Square s = x88Board::GetSquare(fl, rk);
            EXPECT_EQ(s, i);
            auto [f, r] = x88Board::GetFileAndRank(i);
            EXPECT_EQ(f, fl);
            EXPECT_EQ(r, rk);
            i++;
        }
    }
}

TEST_F(x88BoardTests, SquareFromChars)
{
    char f{ 'a' }, r{ '1' };
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Square s = x88Board::GetSquareFromChars(f + j, r + i);
            EXPECT_EQ(s, j + i * 16);
            auto [fl, rk] = x88Board::GetCharPair(s);
            EXPECT_EQ(fl, f + j);
            EXPECT_EQ(rk, r + i);
        }
    }
}

TEST_F(x88BoardTests, setup_board)
{
    auto scoped_open = ScopedOpen(TestHelpers::BoardSetupTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-1");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<x88Board>(lines[(size_t)(i + 1)]);
        const auto& bArray = b->GetBoard();

        x88Board::BoardArray bArrayExpected;
        x88Board::PieceMap wPiecesExpected, bPiecesExpected;
        GetBoardArrayAndPieces(lines[(size_t)(i + 2)], bArrayExpected, wPiecesExpected, bPiecesExpected);

        EXPECT_TRUE(std::equal(bArray.begin(), bArray.end(), bArrayExpected.begin()));
        EXPECT_EQ(b->GetWhitePieces(), wPiecesExpected);
        EXPECT_EQ(b->GetBlackPieces(), bPiecesExpected);
        
        Fen fenStr(lines[(size_t)(i + 1)]);
        EXPECT_EQ(b->GetActiveColor(), get_color_from_char(fenStr.GetActiveColor()[0]));

        auto castling = fenStr.GetCastlingAvailability();
        EXPECT_EQ(b->QueryCastling(Castling::WHITE_KS), castling.find(charset::WhiteKing) != std::string_view::npos);
        EXPECT_EQ(b->QueryCastling(Castling::WHITE_QS), castling.find(charset::WhiteQueen) != std::string_view::npos);
        EXPECT_EQ(b->QueryCastling(Castling::BLACK_KS), castling.find(charset::BlackKing) != std::string_view::npos);
        EXPECT_EQ(b->QueryCastling(Castling::BLACK_QS), castling.find(charset::BlackQueen) != std::string_view::npos);

        auto ep = fenStr.GetEnPassantTargetSquare();
        if (ep == "-")
            EXPECT_EQ(b->GetEnPassantSquare(), Empty);
        else
            EXPECT_EQ(b->GetEnPassantSquare(), x88Board::GetSquareFromChars(ep[0], ep[1]));

        auto hmc = fenStr.GetHalfMoveClock();
        EXPECT_EQ(b->GetHalfMoveClock(), utility::numeric::to_int(hmc.value_or("0")));

        auto fmc = fenStr.GetFullMoveClock();
        EXPECT_EQ(b->GetFullMoveClock(), utility::numeric::to_int(fmc.value_or("0")));

        EXPECT_EQ(lines[(size_t)(i + 1)], board_to_fen(*b));
    }
}

TEST_F(x88BoardTests, setup_board_and_board_to_fen)
{
    auto scoped_open = ScopedOpen(TestHelpers::BoardSetupTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-2");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<x88Board>(lines[(size_t)(i + 1)]);
        EXPECT_EQ(lines[(size_t)(i + 1)], board_to_fen(*b));
    }
}