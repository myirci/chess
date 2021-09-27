#include "pch.h"

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/BasicBoard/BasicBoard.hpp>
#include <ChessLib/X88Board/x88Board.hpp>
#include <ChessLib/ObjBoard/ObjBoard.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

#include <bitset>

using namespace chesslib;
using namespace chesslib::squareset;
using namespace chesslib::pieceset;

class BoardTests : public ::testing::Test 
{
public:

    BoardTests() { }

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

    std::array<Square, 64> board_array_1
    {
        WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook,
        WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn,
        Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty,
        Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty,
        Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty,
        Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty,
        BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn,
        BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook
    };

    BoardBaseWithPieces::PieceMap white_pieces_1
    {
        {WhitePawn, a2}, {WhitePawn, b2}, {WhitePawn, c2}, {WhitePawn, d2}, {WhitePawn, e2}, {WhitePawn, f2}, {WhitePawn, g2}, {WhitePawn, h2},
        {WhiteRook, a1}, {WhiteRook, h1}, {WhiteKnight, b1}, {WhiteKnight, g1}, {WhiteBishop, c1}, {WhiteBishop, f1}, {WhiteQueen, d1}, {WhiteKing, e1}
    };

    BoardBaseWithPieces::PieceMap black_pieces_1
    {
        {BlackPawn, a7}, {BlackPawn, b7}, {BlackPawn, c7}, {BlackPawn, d7}, {BlackPawn, e7}, {BlackPawn, f7}, {BlackPawn, g7}, {BlackPawn, h7},
        {BlackRook, a8}, {BlackRook, h8}, {BlackKnight, b8}, {BlackKnight, g8}, {BlackBishop, c8}, {BlackBishop, f8}, {BlackQueen, d8}, {BlackKing, e8}
    };

    std::array<Square, 64> board_Array_2
    {
        Empty, Empty, Empty, Empty, Empty, Empty, WhiteKing, Empty,
        Empty, WhitePawn, Empty, Empty, Empty, WhitePawn, WhitePawn, Empty,
        Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty,
        Empty, Empty, Empty, Empty, WhitePawn, BlackPawn, Empty, Empty,
        Empty, Empty, BlackPawn, WhitePawn, Empty, Empty, BlackPawn, Empty,
        Empty, Empty, Empty, BlackPawn, Empty, Empty, Empty, Empty,
        BlackPawn, BlackPawn, Empty, Empty, Empty, BlackPawn, BlackPawn, Empty,
        Empty, Empty, Empty, Empty, Empty, Empty, BlackKing, Empty
    };

    BoardBaseWithPieces::PieceMap white_pieces_2
    {
        {WhitePawn, b2}, {WhitePawn, f2}, {WhitePawn, g2}, {WhitePawn, d5}, {WhitePawn, e4}, {WhiteKing, g1}
    };

    BoardBaseWithPieces::PieceMap black_pieces_2
    {
        {BlackPawn, a7}, {BlackPawn, b7}, {BlackPawn, c5}, {BlackPawn, d6}, {BlackPawn, f7}, {BlackPawn, f4}, {BlackPawn, g7}, {BlackPawn, g5}, {BlackKing, g8}
    };

    std::array<Square, 128> board_array_x88board_1
    {
        WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook, 0, 0, 0, 0, 0, 0, 0, 0,
        WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, WhitePawn, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, BlackPawn, 0, 0, 0, 0, 0, 0, 0, 0,
        BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook, 0, 0, 0, 0, 0, 0, 0, 0
    };

    BoardBaseWithPieces::PieceMap white_pieces_x88board_1
    {
        {WhitePawn, x88board::a2}, {WhitePawn, x88board::b2}, {WhitePawn, x88board::c2},
        {WhitePawn, x88board::d2}, {WhitePawn, x88board::e2}, {WhitePawn, x88board::f2},
        {WhitePawn, x88board::g2}, {WhitePawn, x88board::h2}, {WhiteRook, x88board::a1}, {WhiteRook, x88board::h1},
        {WhiteKnight, x88board::b1}, {WhiteKnight, x88board::g1}, {WhiteBishop, x88board::c1}, {WhiteBishop, x88board::f1},
        {WhiteQueen, x88board::d1}, {WhiteKing, x88board::e1}
    };

    BoardBaseWithPieces::PieceMap black_pieces_x88board_1
    {
        {BlackPawn, x88board::a7}, {BlackPawn, x88board::b7}, {BlackPawn, x88board::c7},
        {BlackPawn, x88board::d7}, {BlackPawn, x88board::e7}, {BlackPawn, x88board::f7},
        {BlackPawn, x88board::g7}, {BlackPawn, x88board::h7}, {BlackRook, x88board::a8}, {BlackRook, x88board::h8},
        {BlackKnight, x88board::b8}, {BlackKnight, x88board::g8}, {BlackBishop, x88board::c8}, {BlackBishop, x88board::f8},
        {BlackQueen, x88board::d8}, {BlackKing, x88board::e8}
    };

    std::array<Square, 128> board_array_x88board_2
    {
        Empty, Empty, Empty, Empty, Empty, Empty, WhiteKing, Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty, WhitePawn, Empty, Empty, Empty, WhitePawn, WhitePawn, Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty, Empty, Empty, Empty, Empty, Empty, Empty, Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty, Empty, Empty, Empty, WhitePawn, BlackPawn, Empty, Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty, Empty, BlackPawn, WhitePawn, Empty, Empty, BlackPawn, Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty, Empty, Empty, BlackPawn, Empty, Empty, Empty, Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        BlackPawn, BlackPawn, Empty, Empty, Empty, BlackPawn, BlackPawn, Empty, 0, 0, 0, 0, 0, 0, 0, 0,
        Empty, Empty, Empty, Empty, Empty, Empty, BlackKing, Empty, 0, 0, 0, 0, 0, 0, 0, 0
    };

    BoardBaseWithPieces::PieceMap white_pieces_x88board_2
    {
        {WhitePawn, x88board::b2}, {WhitePawn, x88board::f2}, {WhitePawn, x88board::g2},
        {WhitePawn, x88board::d5}, {WhitePawn, x88board::e4}, {WhiteKing, x88board::g1}
    };

    BoardBaseWithPieces::PieceMap black_pieces_x88board_2
    {
        {BlackPawn, x88board::a7}, {BlackPawn, x88board::b7}, {BlackPawn, x88board::c5},
        {BlackPawn, x88board::d6}, {BlackPawn, x88board::f7}, {BlackPawn, x88board::f4},
        {BlackPawn, x88board::g7}, {BlackPawn, x88board::g5}, {BlackKing, x88board::g8}
    };

    std::array<Piece, 6> whitePieceArray
    {
        WhitePawn, WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing
    };

    std::array<Piece, 6> blackPieceArray
    {
        BlackPawn, BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing
    };

    std::vector<std::string_view> fens
    {
        "1k1r3r/pp2bp1p/1q5p/2NP4/1Ppp1PQ1/P6P/2P3P1/B4R1K b - - 0 1",
        "r1q3k1/2p2rpp/p1n5/P3p1N1/1pQ1n1b1/2P3N1/1P3KPP/R1B1R3 w - - 46 56",
        "r3k2r/pp1B1p1p/2p2P2/3p4/5p1P/q1N1Q1P1/P1P5/1K1R4 b kq - 5 16",
        "3q3r/5pk1/p2p2p1/4p1n1/2R2P2/1P5K/P3Q3/6R1 w - - 0 1",
        "r1b1r1k1/1q3pbp/2N2np1/1N1P4/2QB1P2/8/PP3nPP/R3KqR1 w Q - 0 1",
        "8/4q2k/3p2pb/PP1P4/2N1n1Pp/5Q1P/8/7K b - g3 0 1",
        "r1bqkn1r/pp2b2p/2n2p2/4p1pP/2Pp3N/3P2P1/PP3PB1/RNBQR1K1 w kq g6 0 1",
        "r4rk1/p2b2p1/3Np2p/3nPp2/1nBP4/2p2QPq/5P1P/R1BR2K1 w - f6 0 1",
        "4r1k1/1b5p/pp2r1p1/3pPp2/PPpQ1P1q/2P4P/2B3P1/R4RK1 w - f6 23 45",
        "4k2r/1b2b1p1/2q2n1p/4p3/1pP1n2P/p3R3/4QBB1/KN1R4 b k c3 0 59",
    };

    std::string_view fen1{ "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Kkq c3 1 2" };

    std::vector<std::bitset<64>> white_attacks
    {
        { 0x444A55F5FBEEF2DF }, { 0x00A017EF3BFF5BFE }, { 0x1451167EF9EE3FFF }, { 0x040404D4FF7EEFBF },
        { 0x0A153E5F0BFF7FBE }, { 0x20203FB170DFFAE8 }, { 0x000044FAF5FFFFFE }, { 0x1423A9FFFD5FFBFE },
        { 0x00002A7F7FBFE9FF }, { 0x080808CADCFF3FFE }
    };

    std::vector<std::bitset<64>> black_attacks
    {
        { 0xFF8FFF4F821E0000 }, { 0xFFFFFBF76AE53808 }, { 0x7EB95F0F17560304 }, { 0xFFBCFAF7B8A00000 },
        { 0xFFFFF7BB559971D8 }, { 0xF8EFF8F4B0542804 }, { 0x7EBF7F7DEA950000 }, { 0xFFF5B7EAF35CCF20 },
        { 0xFFF0FFFF74CA2010 }, { 0x7DBEBFCE7F452A00 }
    };

    std::bitset<64> white_attacks_startpos{ 0x0000000000FFFF7E };
    std::bitset<64> black_attacks_startpos{ 0x7EFFFF0000000000 };

    std::vector<std::string_view> checks_and_pins_fens
    {
        "3qk3/pp2pppb/b6n/2n2P2/2N5/r1PKB2r/P1PRN1PP/1q1r1b1R w - - 0 1",
        "Q2R2Q1/1p1N1b2/2Ppp3/RPnk1pqR/2Pn4/1q1r1p2/B2R4/K6B b - - 0 1",
        "rnbqkbnr/p1pppppp/1p6/8/8/5P2/PPPPPKPP/RNBQ1BNR w kq - 0 1",
        "4r3/q1k5/7b/8/3PPP2/r2NKN1r/3QRN2/2b1q1b1 w - - 0 1",
        "1B1R4/3n4/2rkn3/2p1p3/3pN3/Q7/6K1/3R4 b - - 0 1"
    };
    
    std::vector<BoardBase::PinMap> expected_pins_basic_board 
    {
        { 
            {e3, {h3, basic_board::direction::E}}, {d2, {d1, basic_board::direction::S}},
            {c3, {a3, basic_board::direction::W}}, {f5, {h7, basic_board::direction::NE}},
            {e2, {f1, basic_board::direction::SE}}, {c2, {b1, basic_board::direction::SW}},
            {c4, {a6, basic_board::direction::NW}} 
        },
        { 
            {f3, {h1, basic_board::direction::SE}} 
        },
        { },
        { 
            {d2, {c1, basic_board::direction::SW}}, {e2, {e1, basic_board::direction::S}},
            {f2, {g1, basic_board::direction::SE}}, {f3, {h3, basic_board::direction::E}},
            {f4, {h6, basic_board::direction::NE}}, {e4, {e8, basic_board::direction::N}},
            {d4, {a7, basic_board::direction::NW}}, {d3, {a3, basic_board::direction::W}} 
        },
        {
            {c5, {a3, basic_board::direction::SW}}, {d4, {d1, basic_board::direction::S}},
            {d7, {d8, basic_board::direction::N}}
        }
    };

    std::vector<BoardBase::CheckList> expected_checks_basic_board
    {
        {{d8, basic_board::direction::N}, {c5, basic_board::direction::None}},
        {{c4, basic_board::direction::SW}},
        { },
        { },
        {{b8, basic_board::direction::NW}, {e4, basic_board::direction::None}}
    };

    std::vector<std::string_view> move_generator_king_moves_test_fens
    {
        "r3k2r/p3p3/8/8/1q6/8/P2P3P/R3K2R w KQkq - 0 1", 
        "r3k2r/1P6/8/8/8/8/3n4/R3K2R b KQkq - 0 1", 
        "r3k2r/1P6/8/8/8/8/3n4/R3K2R w KQkq - 0 1",
        "8/b7/4k3/8/3nKn2/8/8/8 w - - 0 1"
    };

    std::vector<std::vector<Move>> expected_king_moves
    {
        { 
            Move(e1, e2, MoveType::Quite), Move(e1, f1, MoveType::Quite), Move(e1, d1, MoveType::Quite),
            Move(e1, f2, MoveType::Quite), Move(e1, g1, MoveType::King_Side_Castle), Move(e1, c1, MoveType::Queen_Side_Castle)
        },
        {
            Move(e8, f8, MoveType::Quite), Move(e8, e7, MoveType::Quite), Move(e8, d8, MoveType::Quite),
            Move(e8, f7, MoveType::Quite), Move(e8, d7, MoveType::Quite), Move(e8, g8, MoveType::King_Side_Castle)
        },
        {
            Move(e1, e2, MoveType::Quite), Move(e1, d1, MoveType::Quite), Move(e1, f2, MoveType::Quite),
            Move(e1, d2, MoveType::Capture, BlackKnight), Move(e1, c1, MoveType::Queen_Side_Castle)
        },
        {
            Move(e4, f4, MoveType::Capture, BlackKnight), Move(e4, e3, MoveType::Quite)
        }
    };
};

TEST_F(BoardTests, basic_board_constructor_starting_pos)
{
    auto b = basic_board::make_unique_board(Fen::StartingPosition);
    const auto& boardArray = b->GetBoard();

    EXPECT_TRUE(std::equal(boardArray.begin(), boardArray.end(), board_array_1.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::White);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), Empty);
    EXPECT_EQ(b->GetHalfMoveClock(), 0);
    EXPECT_EQ(b->GetFullMoveClock(), 1);
    EXPECT_EQ(b->GetWhitePieces(), white_pieces_1);
    EXPECT_EQ(b->GetBlackPieces(), black_pieces_1);
    EXPECT_EQ(Fen::StartingPosition, utility::chess::board_to_fen(*b));
}

TEST_F(BoardTests, basic_baord_constructor_pos1)
{
    auto b = basic_board::make_unique_board(fen1);
    const auto& boardArray = b->GetBoard();

    EXPECT_TRUE(std::equal(boardArray.begin(), boardArray.end(), board_Array_2.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::Black);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_FALSE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), c3);
    EXPECT_EQ(b->GetHalfMoveClock(), 1);
    EXPECT_EQ(b->GetFullMoveClock(), 2);
    EXPECT_EQ(b->GetWhitePieces(), white_pieces_2);
    EXPECT_EQ(b->GetBlackPieces(), black_pieces_2);
    EXPECT_EQ(fen1, utility::chess::board_to_fen(*b));
}

TEST_F(BoardTests, basic_board_constructor_fen_compare) 
{  
    for (auto f : fens) 
    {
        auto b = basic_board::make_unique_board(f);
        EXPECT_EQ(f, utility::chess::board_to_fen(*b));
    }
}

TEST_F(BoardTests, basic_board_is_under_attack) 
{
    // starting position
    auto b = basic_board::make_unique_board(Fen::StartingPosition);
    for (Square s{ 0 }; s < 64; s++)
        EXPECT_EQ(b->IsUnderAttack<color::White>(s), white_attacks_startpos[s]);

    for (Square s{ 0 }; s < 64; s++)
        EXPECT_EQ(b->IsUnderAttack<color::Black>(s), black_attacks_startpos[s]);

    for (int i{0}; i < fens.size(); i++)
    {
        auto brd = basic_board::make_unique_board(fens[i]);
        
        for (Square s{ 0 }; s < 64; s++) 
            EXPECT_EQ(brd->IsUnderAttack<color::White>(s), white_attacks[i][s]);
       
        for (Square s{ 0 }; s < 64; s++) 
            EXPECT_EQ(brd->IsUnderAttack<color::Black>(s), black_attacks[i][s]);    
    }
}

TEST_F(BoardTests, basic_board_checks_and_pins) 
{
    for (int i{ 0 }; i < checks_and_pins_fens.size(); i++)
    {
        auto brd = basic_board::make_unique_board(checks_and_pins_fens[i]);
        auto stm = brd->GetActiveColor();
        if (stm == color::White)
        {
            auto piece_loc = brd->GetWhitePieces().find(WhiteKing)->second;
            brd->ComputeChecksAndPins<color::Black>(piece_loc);
        }   
        else
        {
            auto piece_loc = brd->GetBlackPieces().find(BlackKing)->second;
            brd->ComputeChecksAndPins<color::White>(piece_loc);
        }

        const auto& checks = brd->GetChecks();
        EXPECT_TRUE(std::is_permutation(checks.begin(), checks.end(), expected_checks_basic_board[i].begin()));

        const auto& pins = brd->GetPins();
        EXPECT_EQ(pins, expected_pins_basic_board[i]);
    }
}

TEST_F(BoardTests, x88board_constructor_starting_pos)
{
    auto b = x88board::make_unique_board(Fen::StartingPosition);
    const auto& boardArray = b->GetBoard();

    EXPECT_TRUE(std::equal(boardArray.begin(), boardArray.end(), board_array_x88board_1.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::White);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), Empty);
    EXPECT_EQ(b->GetHalfMoveClock(), 0);
    EXPECT_EQ(b->GetFullMoveClock(), 1);
    EXPECT_EQ(b->GetWhitePieces(), white_pieces_x88board_1);
    EXPECT_EQ(b->GetBlackPieces(), black_pieces_x88board_1);
    EXPECT_EQ(Fen::StartingPosition, utility::chess::board_to_fen(*b));
}

TEST_F(BoardTests, x88baord_constructor_pos1)
{
    auto b = x88board::make_unique_board(fen1);
    const auto& boardArray = b->GetBoard();

    EXPECT_TRUE(std::equal(boardArray.begin(), boardArray.end(), board_array_x88board_2.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::Black);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_FALSE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), x88board::c3);
    EXPECT_EQ(b->GetHalfMoveClock(), 1);
    EXPECT_EQ(b->GetFullMoveClock(), 2);
    EXPECT_EQ(b->GetWhitePieces(), white_pieces_x88board_2);
    EXPECT_EQ(b->GetBlackPieces(), black_pieces_x88board_2);
    EXPECT_EQ(fen1, utility::chess::board_to_fen(*b));
}

TEST_F(BoardTests, x88board_constructor_fen_compare)
{
    for (auto f : fens)
    {
        auto b = x88board::make_unique_board(f);
        EXPECT_EQ(f, utility::chess::board_to_fen(*b));
    }
}
TEST_F(BoardTests, objboard_constructor_starting_pos)
{
    auto b = objboard::make_unique_board(Fen::StartingPosition);
    const auto& squareArray = b->GetBoard();

    for (int i = 0; i < board_array_1.size(); i++)
    {
        EXPECT_EQ(squareArray[i]._code, i);
        if (board_array_1[i] == Empty)
            EXPECT_EQ(squareArray[i]._piece, nullptr);
        else
            EXPECT_EQ(squareArray[i]._piece->_code, board_array_1[i]);
    }

    EXPECT_TRUE(check_objboard_pieces(whitePieceArray, white_pieces_1, b->GetWhitePieces()));
    EXPECT_TRUE(check_objboard_pieces(blackPieceArray, black_pieces_1, b->GetBlackPieces()));
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

TEST_F(BoardTests, objbaord_constructor_pos1)
{
    auto b = objboard::make_unique_board(fen1);
    const auto& squareArray = b->GetBoard();
    for (int i = 0; i < board_Array_2.size(); i++)
    {
        if (board_Array_2[i] == Empty)
            EXPECT_EQ(squareArray[i]._piece, nullptr);
        else
            EXPECT_EQ(squareArray[i]._piece->_code, board_Array_2[i]);
    }

    EXPECT_TRUE(check_objboard_pieces(whitePieceArray, white_pieces_2, b->GetWhitePieces()));
    EXPECT_TRUE(check_objboard_pieces(blackPieceArray, black_pieces_2, b->GetBlackPieces()));    
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
TEST_F(BoardTests, objboard_constructor_fen_compare)
{
    for (auto f : fens)
    {
        auto b = objboard::make_unique_board(f);
        EXPECT_EQ(f, utility::chess::board_to_fen(*b));
    }
}