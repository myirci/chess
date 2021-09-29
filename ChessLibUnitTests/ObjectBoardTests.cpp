#include "pch.h"

#include "BasicAndObjectBoardBase.hpp"

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/ObjBoard/ObjBoard.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

#include <bitset>

using namespace chesslib;
using namespace chesslib::squareset;
using namespace chesslib::pieceset;

class ObjectBoardTests : 
    public ::testing::Test, 
    public BasicAndObjectBoardBase
{
public:

    ObjectBoardTests() { }

protected:

    bool check_objboard_pieces(
        const std::array<Piece, 6>& pieceArray,
        const BoardBaseWithPieces::PieceMap& pieces1,
        const objboard::ObjBoard::PieceMap& pieces2)
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
                locations1.push_back(itp1.first->second);
                itp1.first++;
            }

            std::vector<Square> locations2;
            while (itp2.first != itp2.second)
            {
                locations2.push_back(itp2.first->second->_loc);
                itp2.first++;
            }

            return std::is_permutation(locations1.begin(), locations1.end(), locations2.begin());
        }
    }

    std::array<Piece, 6> white_pieces_array
    {
        WhitePawn, WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing
    };

    std::array<Piece, 6> black_pieces_array
    {
        BlackPawn, BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing
    };
};

TEST_F(ObjectBoardTests, constructor_starting_pos)
{
    auto b = objboard::make_unique_board(Fen::StartingPosition);
    const auto& squareArray = b->GetBoard();

    for (int i = 0; i < board_array_starting_position.size(); i++)
    {
        EXPECT_EQ(squareArray[i]._code, i);
        if (board_array_starting_position[i] == Empty)
            EXPECT_EQ(squareArray[i]._piece, nullptr);
        else
            EXPECT_EQ(squareArray[i]._piece->_code, board_array_starting_position[i]);
    }

    EXPECT_TRUE(check_objboard_pieces(white_pieces_array, white_pieces_starting_position, b->GetWhitePieces()));
    EXPECT_TRUE(check_objboard_pieces(black_pieces_array, black_pieces_starting_position, b->GetBlackPieces()));
    EXPECT_EQ(b->GetActiveColor(), color::White);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), Empty);
    EXPECT_EQ(b->GetHalfMoveClock(), 0);
    EXPECT_EQ(b->GetFullMoveClock(), 1);
    EXPECT_EQ(Fen::StartingPosition, utility::chess::board_to_fen(*b));
}

TEST_F(ObjectBoardTests, constructor_fen1)
{
    auto b = objboard::make_unique_board(fen1);
    const auto& squareArray = b->GetBoard();
    for (int i = 0; i < board_array_fen1.size(); i++)
    {
        if (board_array_fen1[i] == Empty)
            EXPECT_EQ(squareArray[i]._piece, nullptr);
        else
            EXPECT_EQ(squareArray[i]._piece->_code, board_array_fen1[i]);
    }

    EXPECT_TRUE(check_objboard_pieces(white_pieces_array, white_pieces_fen1, b->GetWhitePieces()));
    EXPECT_TRUE(check_objboard_pieces(black_pieces_array, black_pieces_fen1, b->GetBlackPieces()));
    EXPECT_EQ(b->GetActiveColor(), color::Black);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_FALSE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), c3);
    EXPECT_EQ(b->GetHalfMoveClock(), 1);
    EXPECT_EQ(b->GetFullMoveClock(), 2);
    EXPECT_EQ(fen1, utility::chess::board_to_fen(*b));
}
TEST_F(ObjectBoardTests, constructor_fen_compare)
{
    for (auto f : fens)
    {
        auto b = objboard::make_unique_board(f);
        EXPECT_EQ(f, utility::chess::board_to_fen(*b));
    }
}