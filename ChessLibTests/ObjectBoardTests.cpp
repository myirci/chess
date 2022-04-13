#include "pch.h"

#include "TestHelpers.hpp"

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/Board/ObjBoard.hpp>
#include <ChessLib/Board/BoardFactory.hpp>

using namespace chesslib;
using namespace chesslib::pieceset;
using namespace chesslib::objboard;

class ObjectBoardTests : public ::testing::Test
{
protected:

    static void GetBoardArrayAndPieces(std::string& line, ObjBoard::BoardArray& bArray, ObjBoard::PieceMap& whitePieces, ObjBoard::PieceMap& blackPieces)
    {
        line.erase(std::remove_if(line.begin(), line.end(), [](char c) {return c == ','; }), line.end());
        if (line.size() != ChessBoard::BOARDSIZE)
            throw std::exception("Error in the number of squares");

        for (auto i{ 0 }; i < ChessBoard::BOARDSIZE; i++)
        {
            bArray[i]._code = i;

            if (line[i] == 'e')
                continue;

            Piece p = char_to_piece.at(line[i]);
            auto pobj = make_piece(p, i);
            bArray[i]._piece = pobj.get();
            if (p > 0) 
                whitePieces.emplace(p, std::move(pobj));
            else 
                blackPieces.emplace(p, std::move(pobj));
        }
    }

    static bool CheckObjboardPieces(
        const std::array<Piece, 6>& pieceArray, 
        const ObjBoard::PieceMap& pieces1,
        const ObjBoard::PieceMap& pieces2)
    {
        for (Piece p : pieceArray)
        {
            auto itp1 = pieces1.equal_range(p);
            auto itp2 = pieces2.equal_range(p);
            size_t numPiece1 = std::distance(itp1.first, itp1.second);
            size_t numPiece2 = std::distance(itp2.first, itp2.second);
            if (numPiece1 != numPiece2)
                return false;

            std::vector<Square> locations1;
            while (itp1.first != itp1.second)
            {
                locations1.push_back(itp1.first->second->_loc);
                itp1.first++;
            }

            std::vector<Square> locations2;
            while (itp2.first != itp2.second)
            {
                locations2.push_back(itp2.first->second->_loc);
                itp2.first++;
            }

            if (!std::is_permutation(locations1.begin(), locations1.end(), locations2.begin()))
                return false;
        }

        return true;
    }

    static constexpr std::array<Piece, 6> white_pieces_array
    {
        WhitePawn, WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing
    };

    static constexpr std::array<Piece, 6> black_pieces_array
    {
        BlackPawn, BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing
    };
};

TEST_F(ObjectBoardTests, setup_board)
{
    auto scoped_open = ScopedOpen(TestHelpers::BoardSetupTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-1");
    EXPECT_TRUE(lines.size() % 3 == 0);

    auto numTestCases = lines.size() / 3;
    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<objboard::ObjBoard>(lines[(size_t)(i + 1)]);
        const auto& bArray = b->GetBoard();

        ObjBoard::BoardArray bArrayExpected;
        ObjBoard::PieceMap wPiecesExpected, bPiecesExpected;
        GetBoardArrayAndPieces(lines[(size_t)(i + 2)], bArrayExpected, wPiecesExpected, bPiecesExpected);

        for (int i = 0; i < bArrayExpected.size(); i++)
        {
            EXPECT_EQ(bArray[i]._code, bArrayExpected[i]._code);
            if (bArrayExpected[i]._piece)
                EXPECT_EQ(bArray[i]._piece->_code, bArrayExpected[i]._piece->_code);
            else 
                EXPECT_EQ(bArray[i]._piece, bArrayExpected[i]._piece);
        }
        
        EXPECT_TRUE(CheckObjboardPieces(white_pieces_array, wPiecesExpected, b->GetWhitePieces()));
        EXPECT_TRUE(CheckObjboardPieces(black_pieces_array, bPiecesExpected, b->GetBlackPieces()));

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
            EXPECT_EQ(b->GetEnPassantSquare(), ObjBoard::GetSquareFromChars(ep[0], ep[1]));

        auto hmc = fenStr.GetHalfMoveClock();
        EXPECT_EQ(b->GetHalfMoveClock(), utility::numeric::to_int(hmc.value_or("0")));

        auto fmc = fenStr.GetFullMoveClock();
        EXPECT_EQ(b->GetFullMoveClock(), utility::numeric::to_int(fmc.value_or("0")));

        EXPECT_EQ(lines[(size_t)(i + 1)], board_to_fen(*b));
    }
}

TEST_F(ObjectBoardTests, setup_board_and_board_to_fen)
{
    auto scoped_open = ScopedOpen(TestHelpers::BoardSetupTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-2");
    
    EXPECT_TRUE(lines.size() % 3 == 0);

    auto numTestCases = lines.size() / 3;
    for (auto i{ 0 }; i < numTestCases; i += 3)
    {
        auto b = BoardFactory::make_unique_board<objboard::ObjBoard>(lines[(size_t)(i + 1)]);
        EXPECT_EQ(lines[(size_t)(i + 1)], board_to_fen(*b));
    }
}