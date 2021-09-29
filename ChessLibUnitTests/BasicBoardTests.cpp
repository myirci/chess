#include "pch.h"

#include "BasicAndObjectBoardBase.hpp"

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/BasicBoard/BasicBoard.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

#include <bitset>

using namespace chesslib;
using namespace chesslib::squareset;
using namespace chesslib::pieceset;

class BasicBoardTests : 
    public ::testing::Test, 
    public BasicAndObjectBoardBase
{
public:

    BasicBoardTests() { }

protected:

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
    
    std::vector<BoardBase::PinMap> expected_pins 
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

    std::vector<BoardBase::CheckList> expected_checks
    {
        {{d8, basic_board::direction::N, 5}, {c5, basic_board::direction::None, 0}},
        {{c4, basic_board::direction::SW, 1}},
        { },
        { },
        {{b8, basic_board::direction::NW, 2}, {e4, basic_board::direction::None, 0}}
    };

    std::vector<MoveList> expected_king_moves
    {
        { Move(e1, e2, MoveType::Quite), Move(e1, f1, MoveType::Quite), Move(e1, d1, MoveType::Quite), Move(e1, f2, MoveType::Quite), Move(e1, g1, MoveType::King_Side_Castle), Move(e1, c1, MoveType::Queen_Side_Castle) },
        { Move(e8, f8, MoveType::Quite), Move(e8, e7, MoveType::Quite), Move(e8, d8, MoveType::Quite), Move(e8, f7, MoveType::Quite), Move(e8, d7, MoveType::Quite), Move(e8, g8, MoveType::King_Side_Castle) },
        { Move(e1, e2, MoveType::Quite), Move(e1, d1, MoveType::Quite), Move(e1, f2, MoveType::Quite), Move(e1, d2, MoveType::Capture, BlackKnight), Move(e1, c1, MoveType::Queen_Side_Castle) },
        { Move(e4, f4, MoveType::Capture, BlackKnight), Move(e4, e3, MoveType::Quite) },
        { Move(e4, d4, MoveType::Quite), Move(e4, d3, MoveType::Quite), Move(e4, e3, MoveType::Quite), Move(e4, f3, MoveType::Quite), Move(e4, f4, MoveType::Quite) },
        { Move(e6, d6, MoveType::Quite), Move(e6, d7, MoveType::Quite), Move(e6, e7, MoveType::Quite), Move(e6, f7, MoveType::Quite), Move(e6, f6, MoveType::Quite) },
        { Move(h1, g2, MoveType::Capture, pieceset::BlackQueen) },
        { }, 
        { },
        { },
        { Move(d6, c7, MoveType::Quite), Move(d6, e5, MoveType::Capture, pieceset::WhitePawn) },
        { Move(e4, d4, MoveType::Capture, pieceset::BlackQueen) },
        { Move(e4, f4, MoveType::Capture, pieceset::BlackQueen) }
    };
};

TEST_F(BasicBoardTests, constructor_starting_pos)
{
    auto b = basic_board::make_unique_board(Fen::StartingPosition);
    const auto& board_array = b->GetBoard();

    EXPECT_TRUE(std::equal(board_array.begin(), board_array.end(), board_array_starting_position.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::White);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), Empty);
    EXPECT_EQ(b->GetHalfMoveClock(), 0);
    EXPECT_EQ(b->GetFullMoveClock(), 1);
    EXPECT_EQ(b->GetWhitePieces(), white_pieces_starting_position);
    EXPECT_EQ(b->GetBlackPieces(), black_pieces_starting_position);
    EXPECT_EQ(Fen::StartingPosition, utility::chess::board_to_fen(*b));
}

TEST_F(BasicBoardTests, constructor_fen1)
{
    auto b = basic_board::make_unique_board(fen1);
    const auto& board_array = b->GetBoard();

    EXPECT_TRUE(std::equal(board_array.begin(), board_array.end(), board_array_fen1.begin()));
    EXPECT_EQ(b->GetActiveColor(), color::Black);
    EXPECT_TRUE(b->QueryCastling(Castling::WHITE_KS));
    EXPECT_FALSE(b->QueryCastling(Castling::WHITE_QS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_KS));
    EXPECT_TRUE(b->QueryCastling(Castling::BLACK_QS));
    EXPECT_EQ(b->GetEnPassantSquare(), c3);
    EXPECT_EQ(b->GetHalfMoveClock(), 1);
    EXPECT_EQ(b->GetFullMoveClock(), 2);
    EXPECT_EQ(b->GetWhitePieces(), white_pieces_fen1);
    EXPECT_EQ(b->GetBlackPieces(), black_pieces_fen1);
    EXPECT_EQ(fen1, utility::chess::board_to_fen(*b));
}

TEST_F(BasicBoardTests, constructor_fen_compare)
{  
    for (auto f : fens) 
    {
        auto b = basic_board::make_unique_board(f);
        EXPECT_EQ(f, utility::chess::board_to_fen(*b));
    }
}

TEST_F(BasicBoardTests, is_under_attack)
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

TEST_F(BasicBoardTests, checks_and_pins)
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
        EXPECT_TRUE(std::is_permutation(checks.begin(), checks.end(), expected_checks[i].begin()));

        const auto& pins = brd->GetPins();
        EXPECT_EQ(pins, expected_pins[i]);
    }
}
TEST_F(BasicBoardTests, king_moves)
{
    for (int i{ 0 }; i < move_generator_king_moves_test_fens.size(); i++)
    {
        MoveList moves;
        auto brd = basic_board::make_unique_board(move_generator_king_moves_test_fens[i]);
        auto stm = brd->GetActiveColor();
        if (stm == color::White)
        {
            auto piece_loc = brd->GetWhitePieces().find(WhiteKing)->second;
            brd->ComputeChecksAndPins<color::Black>(piece_loc);
            brd->GenerateKingMoves<color::White>(piece_loc, moves);
        }
        else
        {
            auto piece_loc = brd->GetBlackPieces().find(BlackKing)->second;
            brd->ComputeChecksAndPins<color::White>(piece_loc);
            brd->GenerateKingMoves<color::Black>(piece_loc, moves);
        }

        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expected_king_moves[i].begin()));
    }
}