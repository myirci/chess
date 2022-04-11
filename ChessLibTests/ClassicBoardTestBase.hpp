#pragma once

#include "BoardTestBase.hpp"

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Board/BoardBase.hpp>
#include <ChessLib/Board/ChessBoard.hpp>
#include <ChessLib/Board/PieceCentricBoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>

#include <array>

using namespace chesslib;
using namespace chesslib::pieceset;

class ClassicBoardTestBase : public BoardTestBase
{
protected:

    std::array<Square, 64> board_array_starting_position
    {
        WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook,
        WhitePawn, WhitePawn,   WhitePawn,   WhitePawn,  WhitePawn, WhitePawn,   WhitePawn,   WhitePawn,
        Empty,     Empty,       Empty,       Empty,      Empty,     Empty,       Empty,       Empty,
        Empty,     Empty,       Empty,       Empty,      Empty,     Empty,       Empty,       Empty,
        Empty,     Empty,       Empty,       Empty,      Empty,     Empty,       Empty,       Empty,
        Empty,     Empty,       Empty,       Empty,      Empty,     Empty,       Empty,       Empty,
        BlackPawn, BlackPawn,   BlackPawn,   BlackPawn,  BlackPawn, BlackPawn,   BlackPawn,   BlackPawn,
        BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook
    };

    PieceCentricBoardBase::PieceMap white_pieces_starting_position
    {
        {WhitePawn, ChessBoard::a2},   {WhitePawn, ChessBoard::b2},   {WhitePawn, ChessBoard::c2},   {WhitePawn, ChessBoard::d2},
        {WhitePawn, ChessBoard::e2},   {WhitePawn, ChessBoard::f2},   {WhitePawn, ChessBoard::g2},   {WhitePawn, ChessBoard::h2},
        {WhiteRook, ChessBoard::a1},   {WhiteRook, ChessBoard::h1},   {WhiteKnight, ChessBoard::b1}, {WhiteKnight, ChessBoard::g1},
        {WhiteBishop, ChessBoard::c1}, {WhiteBishop, ChessBoard::f1}, {WhiteQueen, ChessBoard::d1},  {WhiteKing, ChessBoard::e1}
    };

    PieceCentricBoardBase::PieceMap black_pieces_starting_position
    {
        {BlackPawn, ChessBoard::a7},   {BlackPawn, ChessBoard::b7},   {BlackPawn, ChessBoard::c7},   {BlackPawn, ChessBoard::d7},
        {BlackPawn, ChessBoard::e7},   {BlackPawn, ChessBoard::f7},   {BlackPawn, ChessBoard::g7},   {BlackPawn, ChessBoard::h7},
        {BlackRook, ChessBoard::a8},   {BlackRook, ChessBoard::h8},   {BlackKnight, ChessBoard::b8}, {BlackKnight, ChessBoard::g8},
        {BlackBishop, ChessBoard::c8}, {BlackBishop, ChessBoard::f8}, {BlackQueen, ChessBoard::d8},  {BlackKing, ChessBoard::e8}
    };

    std::array<Square, 64> board_array_fen1
    {
        Empty,     Empty,     Empty,     Empty,     Empty,     Empty,     WhiteKing, Empty,
        Empty,     WhitePawn, Empty,     Empty,     Empty,     WhitePawn, WhitePawn, Empty,
        Empty,     Empty,     Empty,     Empty,     Empty,     Empty,     Empty,     Empty,
        Empty,     Empty,     Empty,     Empty,     WhitePawn, BlackPawn, Empty,     Empty,
        Empty,     Empty,     BlackPawn, WhitePawn, Empty,     Empty,     BlackPawn, Empty,
        Empty,     Empty,     Empty,     BlackPawn, Empty,     Empty,     Empty,     Empty,
        BlackPawn, BlackPawn, Empty,     Empty,     Empty,     BlackPawn, BlackPawn, Empty,
        Empty,     Empty,     Empty,     Empty,     Empty,     Empty,     BlackKing, Empty
    };

    PieceCentricBoardBase::PieceMap white_pieces_fen1
    {
        {WhitePawn, ChessBoard::b2}, {WhitePawn, ChessBoard::f2}, {WhitePawn, ChessBoard::g2}, {WhitePawn, ChessBoard::d5}, {WhitePawn, ChessBoard::e4}, {WhiteKing, ChessBoard::g1}
    };

    PieceCentricBoardBase::PieceMap black_pieces_fen1
    {
        {BlackPawn, ChessBoard::a7}, {BlackPawn, ChessBoard::b7}, {BlackPawn, ChessBoard::c5}, {BlackPawn, ChessBoard::d6}, {BlackPawn, ChessBoard::f7}, {BlackPawn, ChessBoard::f4},
        {BlackPawn, ChessBoard::g7}, {BlackPawn, ChessBoard::g5}, {BlackKing, ChessBoard::g8}
    };

    MoveType cm{ MoveType::Capture };

    std::vector<MoveList> expected_king_moves
    {
        { Move{ChessBoard::e4, ChessBoard::d4}, Move{ChessBoard::e4, ChessBoard::f4}, Move{ChessBoard::e4, ChessBoard::d3}, Move{ChessBoard::e4, ChessBoard::e3}, Move{ChessBoard::e4, ChessBoard::f3} },
        { Move{ChessBoard::e6, ChessBoard::d6}, Move{ChessBoard::e6, ChessBoard::f6}, Move{ChessBoard::e6, ChessBoard::d7}, Move{ChessBoard::e6, ChessBoard::e7}, Move{ChessBoard::e6, ChessBoard::f7} },
        { Move{ChessBoard::a1, ChessBoard::a2}, Move{ChessBoard::a1, ChessBoard::b1}, Move{ChessBoard::a1, ChessBoard::b2} },
        { Move{ChessBoard::h8, ChessBoard::h7}, Move{ChessBoard::h8, ChessBoard::g8}, Move{ChessBoard::h8, ChessBoard::g7} },
        { Move{ChessBoard::e3, ChessBoard::f3} },
        { Move{ChessBoard::d6, ChessBoard::c6}, Move{ChessBoard::d6, ChessBoard::d7} },
        { },
        { },
        { Move{ChessBoard::e4, ChessBoard::f4, cm}, Move{ChessBoard::e4, ChessBoard::e3} }
    };

    std::vector<MoveList> expected_check_eliminating_moves
    {
        { Move{ChessBoard::b4, ChessBoard::a2}, Move{ChessBoard::b4, ChessBoard::a6, cm} },
        { Move{ChessBoard::b4, ChessBoard::a2}, Move{ChessBoard::b4, ChessBoard::a6, cm} },
        { 
            Move{ChessBoard::c6, ChessBoard::d7, cm}, Move{ChessBoard::f5, ChessBoard::d7, cm},
            Move{ChessBoard::g7, ChessBoard::d7, cm}, Move{ChessBoard::f5, ChessBoard::d3}, Move{ChessBoard::g7, ChessBoard::d4}
        },
        { Move{ChessBoard::f5, ChessBoard::d7, cm}, Move{ChessBoard::g7, ChessBoard::d7, cm}, Move{ChessBoard::f5, ChessBoard::d3}, Move{ChessBoard::g7, ChessBoard::d4} },
        {
            Move{ChessBoard::b3, ChessBoard::a4, cm}, Move{ChessBoard::a7, ChessBoard::a4, cm},
            Move{ChessBoard::b3, ChessBoard::b4}, Move{ChessBoard::b7, ChessBoard::b4}, Move{ChessBoard::f2, ChessBoard::f4, MoveType::Double_Pawn_Push}
        },
        {
            Move{ChessBoard::b1, ChessBoard::e4, cm}, Move{ChessBoard::h4, ChessBoard::e4, cm},
            Move{ChessBoard::f6, ChessBoard::e4, cm}, Move{ChessBoard::e8, ChessBoard::e4, cm},
            Move{ChessBoard::f5, ChessBoard::e4, cm}, Move{ChessBoard::d5, ChessBoard::e4, cm}
        },
        {
            Move{ChessBoard::d5, ChessBoard::e6, MoveType::En_Passant_Capture},
            Move{ChessBoard::f5, ChessBoard::e6, MoveType::En_Passant_Capture}, Move{ChessBoard::d4, ChessBoard::e5, cm}
        },
        {
            Move{ChessBoard::f4, ChessBoard::g3, MoveType::En_Passant_Capture},
            Move{ChessBoard::h4, ChessBoard::g3, MoveType::En_Passant_Capture}, Move{ChessBoard::f5,ChessBoard::g4, cm}
        },
        {
            Move{ChessBoard::f5, ChessBoard::d3}, Move{ChessBoard::d2, ChessBoard::d3}, Move{ChessBoard::h4, ChessBoard::c4}, Move{ChessBoard::a3, ChessBoard::c4},
            Move{ChessBoard::c2, ChessBoard::c4, MoveType::Double_Pawn_Push}, Move{ChessBoard::b1, ChessBoard::b5, cm},
            Move{ChessBoard::e8, ChessBoard::b5, cm}, Move{ChessBoard::a3, ChessBoard::b5, cm}, Move{ChessBoard::a4, ChessBoard::b5, cm}
        },
        {
            Move{ChessBoard::d2, ChessBoard::c1}, Move{ChessBoard::a2, ChessBoard::c1}, Move{ChessBoard::d2, ChessBoard::e1}, Move{ChessBoard::e2, ChessBoard::e1, MoveType::Queen_Promotion},
            Move{ChessBoard::e2, ChessBoard::e1, MoveType::Rook_Promotion}, Move{ChessBoard::e2, ChessBoard::e1, MoveType::Bishop_Promotion},
            Move{ChessBoard::e2, ChessBoard::e1, MoveType::Knight_Promotion}, Move{ChessBoard::g2, ChessBoard::g1, MoveType::Queen_Promotion},
            Move{ChessBoard::g2, ChessBoard::g1, MoveType::Rook_Promotion}, Move{ChessBoard::g2, ChessBoard::g1, MoveType::Bishop_Promotion},
            Move{ChessBoard::g2, ChessBoard::g1, MoveType::Knight_Promotion}, Move{ChessBoard::g2, ChessBoard::h1, MoveType::Queen_Promotion_Capture},
            Move{ChessBoard::g2, ChessBoard::h1, MoveType::Rook_Promotion_Capture},
            Move{ChessBoard::g2, ChessBoard::h1, MoveType::Bishop_Promotion_Capture},
            Move{ChessBoard::g2, ChessBoard::h1, MoveType::Knight_Promotion_Capture}
        }
    };

    std::vector<MoveList> expected_moves_all_moves
    {
        { },
        { Move{ChessBoard::d5, ChessBoard::e4}, Move{ChessBoard::d5, ChessBoard::c4, cm}, Move{ChessBoard::b3, ChessBoard::c4, cm} },
        { Move{ChessBoard::d2, ChessBoard::c1, cm}, Move{ChessBoard::e2, ChessBoard::e1, cm}, Move{ChessBoard::e4, ChessBoard::e5} },
        { Move{ChessBoard::d6, ChessBoard::d5}, Move{ChessBoard::d6, ChessBoard::e7}},
        {
            Move{ChessBoard::d4, ChessBoard::c3}, Move{ChessBoard::d4, ChessBoard::d3}, Move{ChessBoard::d4, ChessBoard::e3}, Move{ChessBoard::d4, ChessBoard::e5},
            Move{ChessBoard::d4, ChessBoard::d5, cm}
        },
        { Move{ChessBoard::a8, ChessBoard::a7}, Move{ChessBoard::a8, ChessBoard::b8}, Move{ChessBoard::a8, ChessBoard::b7} },
        { Move{ChessBoard::d2, ChessBoard::e3}, Move{ChessBoard::d2, ChessBoard::e1}, Move{ChessBoard::d3, ChessBoard::e4, cm} },
        {
            Move{ChessBoard::c5, ChessBoard::c6}, Move{ChessBoard::c5, ChessBoard::c7}, Move{ChessBoard::c5, ChessBoard::c8, cm}, Move{ChessBoard::c5, ChessBoard::c4},
            Move{ChessBoard::c5, ChessBoard::c3}, Move{ChessBoard::e4, ChessBoard::f5}, Move{ChessBoard::e4, ChessBoard::g6}, Move{ChessBoard::e4, ChessBoard::h7, cm},
            Move{ChessBoard::e4, ChessBoard::d3}, Move{ChessBoard::e2, ChessBoard::f2}, Move{ChessBoard::e2, ChessBoard::g2, cm}, Move{ChessBoard::e2, ChessBoard::d2}
        },
        {
            Move{ChessBoard::d2, ChessBoard::e3}, Move{ChessBoard::b1, ChessBoard::c3}, Move{ChessBoard::c2, ChessBoard::c3},
            Move{ChessBoard::b2, ChessBoard::b4, MoveType::Double_Pawn_Push}
        },
        { },
        { Move{ChessBoard::e8, ChessBoard::f8} }
    };

    std::vector<MoveList> expected_moves_subset
    {
        {
            Move{ChessBoard::e1, ChessBoard::e2}, Move{ChessBoard::e1, ChessBoard::f1}, Move{ChessBoard::e1, ChessBoard::d1}, Move{ChessBoard::e1, ChessBoard::f2},
            Move{ChessBoard::e1, ChessBoard::g1, MoveType::King_Side_Castle}, Move{ChessBoard::e1,ChessBoard::c1, MoveType::Queen_Side_Castle},
        },
        {
            Move{ChessBoard::e8, ChessBoard::f8}, Move{ChessBoard::e8, ChessBoard::e7}, Move{ChessBoard::e8, ChessBoard::d8}, Move{ChessBoard::e8, ChessBoard::f7},
            Move{ChessBoard::e8, ChessBoard::d7}, Move{ChessBoard::e8, ChessBoard::g8, MoveType::King_Side_Castle}
        },
        {
            Move{ChessBoard::e1, ChessBoard::e2}, Move{ChessBoard::e1, ChessBoard::d1}, Move{ChessBoard::e1, ChessBoard::f2}, Move{ChessBoard::e1, ChessBoard::d2, cm},
            Move{ChessBoard::e1, ChessBoard::c1, MoveType::Queen_Side_Castle}
        },
        {
            Move{ChessBoard::h2, ChessBoard::e2}, Move{ChessBoard::f1, ChessBoard::e2}, Move{ChessBoard::d1, ChessBoard::e2}, Move{ChessBoard::g1, ChessBoard::e2}, Move{ChessBoard::c3, ChessBoard::e2}
        },
        {
            Move{ChessBoard::d8, ChessBoard::d5}, Move{ChessBoard::h5, ChessBoard::d5}, Move{ChessBoard::d1, ChessBoard::d5}, Move{ChessBoard::h1, ChessBoard::d5}
        },
        {
            Move{ChessBoard::c4, ChessBoard::c8, cm}, Move{ChessBoard::h3, ChessBoard::c8, cm}, Move{ChessBoard::b6, ChessBoard::c8, cm},
            Move{ChessBoard::b7, ChessBoard::c8, MoveType::Queen_Promotion_Capture},
            Move{ChessBoard::b7, ChessBoard::c8, MoveType::Rook_Promotion_Capture},
            Move{ChessBoard::b7, ChessBoard::c8, MoveType::Bishop_Promotion_Capture},
            Move{ChessBoard::b7, ChessBoard::c8, MoveType::Knight_Promotion_Capture}
        },
        {
            Move{ChessBoard::h4, ChessBoard::e1}, Move{ChessBoard::d3, ChessBoard::e1}, Move{ChessBoard::e2, ChessBoard::e1, MoveType::Queen_Promotion},
            Move{ChessBoard::e2, ChessBoard::e1, MoveType::Rook_Promotion}, Move{ChessBoard::e2, ChessBoard::e1, MoveType::Bishop_Promotion},
            Move{ChessBoard::e2, ChessBoard::e1, MoveType::Knight_Promotion}
        },
        {
            Move{ChessBoard::h8, ChessBoard::g6}, Move{ChessBoard::f4, ChessBoard::e6, cm}, Move{ChessBoard::f4, ChessBoard::g6}, Move{ChessBoard::f4, ChessBoard::h3},
            Move{ChessBoard::f4, ChessBoard::g2}, Move{ChessBoard::f4, ChessBoard::d3}, Move{ChessBoard::f4, ChessBoard::d5}, Move{ChessBoard::h1, ChessBoard::g3}, Move{ChessBoard::h1, ChessBoard::f2}
        },
        {
            Move{ChessBoard::a8, ChessBoard::c7, cm}, Move{ChessBoard::a8, ChessBoard::b6}, Move{ChessBoard::b5, ChessBoard::a7}, Move{ChessBoard::b5, ChessBoard::a3},
            Move{ChessBoard::b5, ChessBoard::c7, cm}, Move{ChessBoard::b5, ChessBoard::d6}, Move{ChessBoard::b5, ChessBoard::d4}
        },
        {
            Move{ChessBoard::d5, ChessBoard::e5}, Move{ChessBoard::d5, ChessBoard::f5}, Move{ChessBoard::d5, ChessBoard::g5}, Move{ChessBoard::d5, ChessBoard::h5},
            Move{ChessBoard::d5, ChessBoard::d4}, Move{ChessBoard::d5, ChessBoard::d3}, Move{ChessBoard::d5, ChessBoard::c5, cm},
            Move{ChessBoard::d5, ChessBoard::e6}, Move{ChessBoard::d5, ChessBoard::f7, cm}, Move{ChessBoard::d5, ChessBoard::e4},
            Move{ChessBoard::d5, ChessBoard::f3, cm}, Move{ChessBoard::d5, ChessBoard::c4}, Move{ChessBoard::d5, ChessBoard::c6},
            Move{ChessBoard::b4, ChessBoard::b5}, Move{ChessBoard::b4, ChessBoard::b6, cm}, Move{ChessBoard::b4, ChessBoard::c4},
            Move{ChessBoard::b4, ChessBoard::d4}, Move{ChessBoard::b4, ChessBoard::e4}, Move{ChessBoard::b4, ChessBoard::f4, cm},
            Move{ChessBoard::b4, ChessBoard::a4}, Move{ChessBoard::b4, ChessBoard::c5, cm}, Move{ChessBoard::b4, ChessBoard::c3},
            Move{ChessBoard::b4, ChessBoard::a5}, Move{ChessBoard::a2, ChessBoard::b2}, Move{ChessBoard::a2, ChessBoard::a1, cm},
            Move{ChessBoard::d2, ChessBoard::d3}, Move{ChessBoard::d2, ChessBoard::d4}, Move{ChessBoard::d2, ChessBoard::e2}, Move{ChessBoard::d2, ChessBoard::d1},
            Move{ChessBoard::b1, ChessBoard::b2}, Move{ChessBoard::b1, ChessBoard::a1, cm}, Move{ChessBoard::h8, ChessBoard::g7, cm},
            Move{ChessBoard::b7, ChessBoard::c8}, Move{ChessBoard::b7, ChessBoard::c6}, Move{ChessBoard::b7, ChessBoard::a6}, Move{ChessBoard::b7, ChessBoard::a8},
            Move{ChessBoard::f2, ChessBoard::g3}, Move{ChessBoard::f2, ChessBoard::h4}, Move{ChessBoard::f2, ChessBoard::g1},
        },
        { Move{ChessBoard::c7, ChessBoard::c6}, Move{ChessBoard::a7, ChessBoard::b6, cm}, Move{ChessBoard::b7, ChessBoard::c6}, Move{ChessBoard::d7, ChessBoard::c6} },
        { Move{ChessBoard::c5, ChessBoard::c6}, Move{ChessBoard::c5, ChessBoard::b6, MoveType::En_Passant_Capture} },
        { Move{ChessBoard::c5, ChessBoard::c6}, Move{ChessBoard::c5, ChessBoard::d6, MoveType::En_Passant_Capture} },
        { Move{ChessBoard::e4, ChessBoard::e3}, Move{ChessBoard::e4, ChessBoard::f3, MoveType::En_Passant_Capture} },
        { Move{ChessBoard::e4, ChessBoard::e3}, Move{ChessBoard::e4, ChessBoard::d3, MoveType::En_Passant_Capture} },
        { Move{ChessBoard::c5, ChessBoard::b6, MoveType::En_Passant_Capture} },
        { Move{ChessBoard::e4, ChessBoard::d3, MoveType::En_Passant_Capture} },
        { 
            Move{ChessBoard::a7, ChessBoard::a8, MoveType::Queen_Promotion},
            Move{ChessBoard::a7, ChessBoard::a8, MoveType::Knight_Promotion},
            Move{ChessBoard::a7, ChessBoard::a8, MoveType::Bishop_Promotion},
            Move{ChessBoard::a7, ChessBoard::a8, MoveType::Rook_Promotion},
            Move{ChessBoard::a7, ChessBoard::b8, MoveType::Queen_Promotion_Capture},
            Move{ChessBoard::a7, ChessBoard::b8, MoveType::Knight_Promotion_Capture},
            Move{ChessBoard::a7, ChessBoard::b8, MoveType::Bishop_Promotion_Capture},
            Move{ChessBoard::a7, ChessBoard::b8, MoveType::Rook_Promotion_Capture},
            Move{ChessBoard::c7, ChessBoard::c8, MoveType::Queen_Promotion},
            Move{ChessBoard::c7, ChessBoard::c8, MoveType::Knight_Promotion},
            Move{ChessBoard::c7, ChessBoard::c8, MoveType::Bishop_Promotion},
            Move{ChessBoard::c7, ChessBoard::c8, MoveType::Rook_Promotion},
            Move{ChessBoard::c7, ChessBoard::d8, MoveType::Queen_Promotion_Capture},
            Move{ChessBoard::c7, ChessBoard::d8, MoveType::Knight_Promotion_Capture},
            Move{ChessBoard::c7, ChessBoard::d8, MoveType::Bishop_Promotion_Capture},
            Move{ChessBoard::c7, ChessBoard::d8, MoveType::Rook_Promotion_Capture},
            Move{ChessBoard::c7, ChessBoard::b8, MoveType::Queen_Promotion_Capture},
            Move{ChessBoard::c7, ChessBoard::b8, MoveType::Knight_Promotion_Capture},
            Move{ChessBoard::c7, ChessBoard::b8, MoveType::Bishop_Promotion_Capture},
            Move{ChessBoard::c7, ChessBoard::b8, MoveType::Rook_Promotion_Capture},
            Move{ChessBoard::h7, ChessBoard::h8, MoveType::Queen_Promotion},
            Move{ChessBoard::h7, ChessBoard::h8, MoveType::Knight_Promotion},
            Move{ChessBoard::h7, ChessBoard::h8, MoveType::Bishop_Promotion},
            Move{ChessBoard::h7, ChessBoard::h8, MoveType::Rook_Promotion},
            Move{ChessBoard::d3, ChessBoard::c4, cm}, Move{ChessBoard::e3, ChessBoard::e4}, Move{ChessBoard::b2, ChessBoard::b3},
            Move{ChessBoard::b2,ChessBoard::b4, MoveType::Double_Pawn_Push},
        },
        {
            Move{ChessBoard::c7, ChessBoard::c6}, Move{ChessBoard::c7, ChessBoard::c5, MoveType::Double_Pawn_Push},
            Move{ChessBoard::e5, ChessBoard::d4, cm}, Move{ChessBoard::e5, ChessBoard::f4, cm},
            Move{ChessBoard::a2, ChessBoard::b1, MoveType::Queen_Promotion_Capture},
            Move{ChessBoard::a2, ChessBoard::b1, MoveType::Knight_Promotion_Capture},
            Move{ChessBoard::a2, ChessBoard::b1, MoveType::Bishop_Promotion_Capture},
            Move{ChessBoard::a2, ChessBoard::b1, MoveType::Rook_Promotion_Capture},
            Move{ChessBoard::d2, ChessBoard::d1, MoveType::Queen_Promotion},
            Move{ChessBoard::d2, ChessBoard::d1, MoveType::Knight_Promotion},
            Move{ChessBoard::d2, ChessBoard::d1, MoveType::Bishop_Promotion},
            Move{ChessBoard::d2, ChessBoard::d1, MoveType::Rook_Promotion},
            Move{ChessBoard::d2, ChessBoard::c1, MoveType::Queen_Promotion_Capture},
            Move{ChessBoard::d2, ChessBoard::c1, MoveType::Knight_Promotion_Capture},
            Move{ChessBoard::d2, ChessBoard::c1, MoveType::Bishop_Promotion_Capture},
            Move{ChessBoard::d2, ChessBoard::c1, MoveType::Rook_Promotion_Capture},
            Move{ChessBoard::d2, ChessBoard::e1, MoveType::Queen_Promotion_Capture},
            Move{ChessBoard::d2, ChessBoard::e1, MoveType::Knight_Promotion_Capture},
            Move{ChessBoard::d2, ChessBoard::e1, MoveType::Bishop_Promotion_Capture},
            Move{ChessBoard::d2, ChessBoard::e1, MoveType::Rook_Promotion_Capture}
        }
    };

    std::vector<std::tuple<Move, std::string_view, std::string_view>> make_unmake_move_fens
    {
        {
            Move{ChessBoard::h4, ChessBoard::f2},
            "8/3k4/2r5/8/7B/8/1K4P1/8 w - - 0 1",
            "8/3k4/2r5/8/8/8/1K3BP1/8 b - - 1 1"
        },
        {
        Move{ChessBoard::g2, ChessBoard::g4, MoveType::Double_Pawn_Push},
            "8/3k1p2/2r5/8/7B/8/1K4P1/8 w - - 0 1",
            "8/3k1p2/2r5/8/6PB/8/1K6/8 b - g3 0 1"
        },
        {
            Move {ChessBoard::f7, ChessBoard::f5, MoveType::Double_Pawn_Push},
            "8/3k1p2/2r5/8/6PB/8/1K6/8 b - g3 0 1",
            "8/3k4/2r5/5p2/6PB/8/1K6/8 w - f6 0 2"
        },
        {
            Move{ChessBoard::h7, ChessBoard::h4, cm},
            "8/3k3q/2r5/8/6PB/8/1K2Q3/8 b - - 0 1",
            "8/3k4/2r5/8/6Pq/8/1K2Q3/8 w - - 0 2"
        },
        {
            Move{ChessBoard::c5, ChessBoard::d6, MoveType::En_Passant_Capture},
            "4k3/8/8/2PpP3/6p1/8/5P1P/4K3 w - d6 0 1",
            "4k3/8/3P4/4P3/6p1/8/5P1P/4K3 b - - 0 1"
        },
        {
            Move{ChessBoard::e5, ChessBoard::d6, MoveType::En_Passant_Capture},
            "4k3/8/8/2PpP3/6p1/8/5P1P/4K3 w - d6 0 1",
            "4k3/8/3P4/2P5/6p1/8/5P1P/4K3 b - - 0 1"
        },
        {
            Move{ChessBoard::g4, ChessBoard::h3, MoveType::En_Passant_Capture},
            "4k3/8/8/2PpP3/6pP/8/5P2/4K3 b - h3 0 1",
            "4k3/8/8/2PpP3/8/7p/5P2/4K3 w - - 0 2"
        },
        {
            Move{ChessBoard::g4, ChessBoard::f3, MoveType::En_Passant_Capture},
            "4k3/8/8/2PpP3/5PpP/8/8/4K3 b - f3 0 1",
            "4k3/8/8/2PpP3/7P/5p2/8/4K3 w - - 0 2"
        },
        {
            Move{ChessBoard::e1, ChessBoard::g1, MoveType::King_Side_Castle},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 b kq - 1 1"
        },
        {
            Move{ChessBoard::e1, ChessBoard::g1, MoveType::King_Side_Castle},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 b kq - 1 1"
        },
        {
            Move{ChessBoard::e8, ChessBoard::g8, MoveType::King_Side_Castle},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 b kq - 1 1",
            "r4rk1/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 w - - 2 2"
        },
        {
            Move{ChessBoard::e1, ChessBoard::c1, MoveType::Queen_Side_Castle},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/2KR3R b kq - 1 1"
        },
        {
            Move{ChessBoard::e8, ChessBoard::c8, MoveType::Queen_Side_Castle},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/2KR3R b kq - 1 1",
            "2kr3r/pppppppp/8/8/8/8/PPPPPPPP/2KR3R w - - 2 2"
        },
        {
            Move{ChessBoard::b7,ChessBoard::b8, MoveType::Knight_Promotion},
            "8/1P4P1/8/4k3/8/2K5/p4p2/8 w - - 0 1",
            "1N6/6P1/8/4k3/8/2K5/p4p2/8 b - - 0 1"
        },
        {
            Move{ChessBoard::a2, ChessBoard::a1, MoveType::Rook_Promotion},
            "1N6/6P1/8/4k3/8/2K5/p4p2/8 b - - 0 1",
            "1N6/6P1/8/4k3/8/2K5/5p2/r7 w - - 0 2"
        },
        {
            Move{ChessBoard::g7, ChessBoard::g8, MoveType::Queen_Promotion},
            "1N6/6P1/8/4k3/8/2K5/5p2/r7 w - - 0 2",
            "1N4Q1/8/8/4k3/8/2K5/5p2/r7 b - - 0 2"
        },
        {
            Move{ChessBoard::f2, ChessBoard::f1, MoveType::Bishop_Promotion},
            "1N4Q1/8/8/4k3/8/2K5/5p2/r7 b - - 0 2",
            "1N4Q1/8/8/4k3/8/2K5/8/r4b2 w - - 0 3"
        },
        {
            Move{ChessBoard::e7, ChessBoard::d8, MoveType::Queen_Promotion_Capture},
            "1n1b4/2P1P3/8/8/1K4k1/8/4p1p1/5N1B w - - 0 1",
            "1n1Q4/2P5/8/8/1K4k1/8/4p1p1/5N1B b - - 0 1"
        },
        {
            Move{ChessBoard::g2, ChessBoard::h1, MoveType::Rook_Promotion_Capture},
            "1n1Q4/2P5/8/8/1K4k1/8/4p1p1/5N1B b - - 0 1",
            "1n1Q4/2P5/8/8/1K4k1/8/4p3/5N1r w - - 0 2"
        },
        {
            Move{ChessBoard::c7, ChessBoard::b8, MoveType::Knight_Promotion_Capture},
            "1n1Q4/2P5/8/8/1K4k1/8/4p3/5N1r w - - 0 2",
            "1N1Q4/8/8/8/1K4k1/8/4p3/5N1r b - - 0 2"
        },
        {
            Move{ChessBoard::e2, ChessBoard::f1, MoveType::Bishop_Promotion_Capture},
            "1N1Q4/8/8/8/1K4k1/8/4p3/5N1r b - - 0 2",
            "1N1Q4/8/8/8/1K4k1/8/8/5b1r w - - 0 3"
        },
        {
            Move{ChessBoard::e1, ChessBoard::d1, MoveType::Quite},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R2K3R b kq - 1 1"
        },
        {
            Move{ChessBoard::e8, ChessBoard::f8, MoveType::Quite},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R2K3R b kq - 1 1",
            "r4k1r/pppppppp/8/8/8/8/PPPPPPPP/R2K3R w - - 2 2"
        },
        {
            Move{ChessBoard::a1, ChessBoard::b1, MoveType::Quite},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2K2R b Kkq - 1 1"
        }, 
        {
            Move{ChessBoard::a8, ChessBoard::c8, MoveType::Quite},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2K2R b Kkq - 1 1",
            "2r1k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2K2R w Kk - 2 2"
        },
        {
            Move{ChessBoard::h1, ChessBoard::f1, MoveType::Quite},
            "2r1k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2K2R w Kk - 2 2",
            "2r1k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2KR2 b k - 3 2"
        },
        {
            Move{ChessBoard::h8, ChessBoard::g8, MoveType::Quite},
            "2r1k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2KR2 b k - 3 2",
            "2r1k1r1/pppppppp/8/8/8/8/PPPPPPPP/1R2KR2 w - - 4 3"
        }, 
        {
            Move{ChessBoard::d5, ChessBoard::a8, cm},
            "r3k2r/p1pppp1p/8/3BB3/3qb3/8/P1PPPP1P/R3K2R w KQkq - 0 1",
            "B3k2r/p1pppp1p/8/4B3/3qb3/8/P1PPPP1P/R3K2R b KQk - 0 1"
        },
        {
            Move{ChessBoard::e4, ChessBoard::h1, cm},
            "B3k2r/p1pppp1p/8/4B3/3qb3/8/P1PPPP1P/R3K2R b KQk - 0 1",
            "B3k2r/p1pppp1p/8/4B3/3q4/8/P1PPPP1P/R3K2b w Qk - 0 2"
        },
        {
            Move{ChessBoard::e5, ChessBoard::h8, cm},
            "B3k2r/p1pppp1p/8/4B3/3q4/8/P1PPPP1P/R3K2b w Qk - 0 2",
            "B3k2B/p1pppp1p/8/8/3q4/8/P1PPPP1P/R3K2b b Q - 0 2"
        },
        {
            Move{ChessBoard::d4, ChessBoard::a1, cm},
            "B3k2B/p1pppp1p/8/8/3q4/8/P1PPPP1P/R3K2b b Q - 0 2",
            "B3k2B/p1pppp1p/8/8/8/8/P1PPPP1P/q3K2b w - - 0 3"
        },
        {
            Move{ChessBoard::b7, ChessBoard::b5, MoveType::Double_Pawn_Push},
            "r1bqkbnr/pppppppp/2n5/P7/8/8/1PPPPPPP/RNBQKBNR b KQkq - 0 1",
            "r1bqkbnr/p1pppppp/2n5/Pp6/8/8/1PPPPPPP/RNBQKBNR w KQkq b6 0 2"
        },
        {
            Move{ChessBoard::a5, ChessBoard::b6, MoveType::En_Passant_Capture},
            "r1bqkbnr/p1pppppp/2n5/Pp6/8/8/1PPPPPPP/RNBQKBNR w KQkq b6 0 2",
            "r1bqkbnr/p1pppppp/1Pn5/8/8/8/1PPPPPPP/RNBQKBNR b KQkq - 0 2"
        }
    };
};