#include "pch.h"

#include "TestHelpers.hpp"

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/Board/SimpleBoard.hpp>
#include <ChessLib/Board/BoardFactory.hpp>
#include <ChessLib/Board/BoardFunctions.hpp>

using namespace chesslib;
using namespace chesslib::pieceset;

class SimpleBoardTests : public ::testing::Test
{
protected:

    static void GetBoardArrayAndPieces(std::string& line, SimpleBoard::BoardArray& bArray, SimpleBoard::PieceMap& whitePieces, SimpleBoard::PieceMap& blackPieces)
    {
        line.erase(std::remove_if(line.begin(), line.end(), [](char c) {return c == ','; }), line.end());
        if (line.size() != ChessBoard::BOARDSIZE)
            throw std::exception("Error in the number of squares");

        for (auto i{ 0 }; i < ChessBoard::BOARDSIZE; i++)
        {
            if (line[i] == 'e')
                bArray[i] = Empty;
            else
            {
                bArray[i] = char_to_piece.at(line[i]);
                if (bArray[i] > 0)
                    whitePieces.emplace(bArray[i], i);
                else
                    blackPieces.emplace(bArray[i], i);
            }
        }
    }
};

TEST_F(SimpleBoardTests, square_file_rank)
{
    Square i{ 0 };
    for (Rank rk = 0; rk < 8; rk++)
    {
        for (File fl = 0; fl < 8; fl++)
        {
            Square s = SimpleBoard::GetSquare(fl, rk);
            EXPECT_EQ(s, i);
            auto [f, r] = SimpleBoard::GetFileAndRank(i);
            EXPECT_EQ(f, fl);
            EXPECT_EQ(r, rk);
            i++;
        }
    }
}

TEST_F(SimpleBoardTests, square_from_char)
{
    char f{ 'a' }, r{ '1' };
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Square s = SimpleBoard::GetSquareFromChars(f + j, r + i);
            EXPECT_EQ(s, j + i * 8);
            auto [fl, rk] = SimpleBoard::GetCharPair(s);
            EXPECT_EQ(fl, f + j);
            EXPECT_EQ(rk, r + i);
        }
    }
}

TEST_F(SimpleBoardTests, setup_board)
{
    auto scoped_open = ScopedOpen(TestHelpers::BoardSetupTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-1");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);
    
    for (auto i{ 0 }; i < lines.size(); i+=3)
    {
        auto b = BoardFactory::make_unique_board<SimpleBoard>(lines[(size_t)(i+1)]);
        const auto& bArray = b->GetBoard();

        SimpleBoard::BoardArray bArrayExpected;
        SimpleBoard::PieceMap wPiecesExpected, bPiecesExpected;
        GetBoardArrayAndPieces(lines[(size_t)(i+2)], bArrayExpected, wPiecesExpected, bPiecesExpected);

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
            EXPECT_EQ(b->GetEnPassantSquare(), SimpleBoard::GetSquareFromChars(ep[0], ep[1]));

        auto hmc = fenStr.GetHalfMoveClock();
        EXPECT_EQ(b->GetHalfMoveClock(), utility::numeric::to_int(hmc.value_or("0")));

        auto fmc = fenStr.GetFullMoveClock();
        EXPECT_EQ(b->GetFullMoveClock(), utility::numeric::to_int(fmc.value_or("0")));

        EXPECT_EQ(lines[(size_t)(i + 1)], board_to_fen(*b));
    }
}

TEST_F(SimpleBoardTests, setup_board_and_board_to_fen)
{
    auto scoped_open = ScopedOpen(TestHelpers::BoardSetupTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-2");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<SimpleBoard>(lines[(size_t)(i + 1)]);
        EXPECT_EQ(lines[(size_t)(i + 1)], board_to_fen(*b));
    }
}

TEST_F(SimpleBoardTests, make_unmake_moves)
{
    auto scoped_open = ScopedOpen(TestHelpers::MakeUnMakeMovesTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-1");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 4 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 4)
    {
        const auto& fen1 = lines[(size_t)(i + 2)];
        const auto& fen2 = lines[(size_t)(i + 3)];

        auto mv = TestHelpers::GetMove<SimpleBoard>(lines[(size_t)(i + 1)]);
        auto b = BoardFactory::make_unique_board<SimpleBoard>(fen1);
        
        make_move(*b, mv);
        EXPECT_EQ(fen2, board_to_fen(*b));
        
        unmake_move(*b);
        EXPECT_EQ(fen1, board_to_fen(*b));
    }
}
