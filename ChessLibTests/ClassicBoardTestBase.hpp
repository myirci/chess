#pragma once

#include "BoardTestBase.hpp"

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Board/BoardBase.hpp>
#include <ChessLib/Board/PieceCentricBoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>

#include <array>

using namespace chesslib;
using namespace chesslib::squareset;
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
        {WhitePawn, a2},   {WhitePawn, b2},   {WhitePawn, c2},   {WhitePawn, d2},
        {WhitePawn, e2},   {WhitePawn, f2},   {WhitePawn, g2},   {WhitePawn, h2},
        {WhiteRook, a1},   {WhiteRook, h1},   {WhiteKnight, b1}, {WhiteKnight, g1},
        {WhiteBishop, c1}, {WhiteBishop, f1}, {WhiteQueen, d1},  {WhiteKing, e1}
    };

    PieceCentricBoardBase::PieceMap black_pieces_starting_position
    {
        {BlackPawn, a7},   {BlackPawn, b7},   {BlackPawn, c7},   {BlackPawn, d7},
        {BlackPawn, e7},   {BlackPawn, f7},   {BlackPawn, g7},   {BlackPawn, h7},
        {BlackRook, a8},   {BlackRook, h8},   {BlackKnight, b8}, {BlackKnight, g8},
        {BlackBishop, c8}, {BlackBishop, f8}, {BlackQueen, d8},  {BlackKing, e8}
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
        {WhitePawn, b2}, {WhitePawn, f2}, {WhitePawn, g2}, {WhitePawn, d5}, {WhitePawn, e4}, {WhiteKing, g1}
    };

    PieceCentricBoardBase::PieceMap black_pieces_fen1
    {
        {BlackPawn, a7}, {BlackPawn, b7}, {BlackPawn, c5}, {BlackPawn, d6}, {BlackPawn, f7}, {BlackPawn, f4},
        {BlackPawn, g7}, {BlackPawn, g5}, {BlackKing, g8}
    };

    MoveType cm{ MoveType::Capture };

    std::vector<MoveList> expected_king_moves
    {
        { Move{e4, d4}, Move{e4, f4}, Move{e4, d3}, Move{e4, e3}, Move{e4, f3} },
        { Move{e6, d6}, Move{e6, f6}, Move{e6, d7}, Move{e6, e7}, Move{e6, f7} },
        { Move{a1, a2}, Move{a1, b1}, Move{a1, b2} },
        { Move{h8, h7}, Move{h8, g8}, Move{h8, g7} },
        { Move{e3, f3} },
        { Move{d6, c6}, Move{d6, d7} },
        { },
        { },
        { Move{e4, f4, cm}, Move{e4, e3} }
    };

    std::vector<MoveList> expected_check_eliminating_moves
    {
        { Move{b4, a2}, Move{b4, a6, cm} },
        { Move{b4, a2}, Move{b4, a6, cm} },
        { 
            Move{c6, d7, cm}, Move{f5, d7, cm},
            Move{g7, d7, cm}, Move{f5, d3}, Move{g7, d4}
        },
        { Move{f5, d7, cm}, Move{g7, d7, cm}, Move{f5, d3}, Move{g7, d4} },
        {
            Move{b3, a4, cm}, Move{a7, a4, cm},
            Move{b3, b4}, Move{b7, b4}, Move{f2, f4, MoveType::Double_Pawn_Push}
        },
        {
            Move{b1, e4, cm}, Move{h4, e4, cm},
            Move{f6, e4, cm}, Move{e8, e4, cm},
            Move{f5, e4, cm}, Move{d5, e4, cm}
        },
        {
            Move{d5, e6, MoveType::En_Passant_Capture},
            Move{f5, e6, MoveType::En_Passant_Capture}, Move{d4, e5, cm}
        },
        {
            Move{f4, g3, MoveType::En_Passant_Capture},
            Move{h4, g3, MoveType::En_Passant_Capture}, Move{f5, g4, cm}
        },
        {
            Move{f5, d3}, Move{d2, d3}, Move{h4, c4}, Move{a3, c4},
            Move{c2, c4, MoveType::Double_Pawn_Push}, Move{b1, b5, cm},
            Move{e8, b5, cm}, Move{a3, b5, cm}, Move{a4, b5, cm}
        },
        {
            Move{d2, c1}, Move{a2, c1}, Move{d2, e1}, Move{e2, e1, MoveType::Queen_Promotion},
            Move{e2, e1, MoveType::Rook_Promotion}, Move{e2, e1, MoveType::Bishop_Promotion},
            Move{e2, e1, MoveType::Knight_Promotion}, Move{g2, g1, MoveType::Queen_Promotion},
            Move{g2, g1, MoveType::Rook_Promotion}, Move{g2, g1, MoveType::Bishop_Promotion},
            Move{g2, g1, MoveType::Knight_Promotion}, Move{g2, h1, MoveType::Queen_Promotion_Capture},
            Move{g2, h1, MoveType::Rook_Promotion_Capture},
            Move{g2, h1, MoveType::Bishop_Promotion_Capture},
            Move{g2, h1, MoveType::Knight_Promotion_Capture}
        }
    };

    std::vector<MoveList> expected_moves_all_moves
    {
        { },
        { Move{d5, e4}, Move{d5, c4, cm}, Move{b3, c4, cm} },
        { Move{d2, c1, cm}, Move{e2, e1, cm}, Move{e4, e5} },
        { Move{d6, d5}, Move{d6, e7}},
        {
            Move{d4, c3}, Move{d4, d3}, Move{d4, e3}, Move{d4, e5}, 
            Move{d4, d5, cm}
        },
        { Move{a8, a7}, Move{a8, b8}, Move{a8, b7} },
        { Move{d2, e3}, Move{d2, e1}, Move{d3, e4, cm} },
        {
            Move{c5, c6}, Move{c5, c7}, Move{c5, c8, cm}, Move{c5, c4},
            Move{c5, c3}, Move{e4, f5}, Move{e4, g6}, Move{e4, h7, cm},
            Move{e4, d3}, Move{e2, f2}, Move{e2, g2, cm}, Move{e2, d2}
        },
        {
            Move{d2, e3}, Move{b1, c3}, Move{c2, c3},
            Move{b2, b4, MoveType::Double_Pawn_Push}
        },
        { },
        { Move{e8, f8} }
    };

    std::vector<MoveList> expected_moves_subset
    {
        {
            Move{e1, e2}, Move{e1, f1}, Move{e1, d1}, Move{e1, f2},
            Move{e1, g1, MoveType::King_Side_Castle}, Move{e1, c1, MoveType::Queen_Side_Castle},
        },
        {
            Move{e8, f8}, Move{e8, e7}, Move{e8, d8}, Move{e8, f7},
            Move{e8, d7}, Move{e8, g8, MoveType::King_Side_Castle}
        },
        {
            Move{e1, e2}, Move{e1, d1}, Move{e1, f2}, Move{e1, d2, cm},
            Move{e1, c1, MoveType::Queen_Side_Castle}
        },
        {
            Move{h2, e2}, Move{f1, e2}, Move{d1, e2}, Move{g1, e2}, Move{c3, e2}
        },
        {
            Move{d8, d5}, Move{h5, d5}, Move{d1, d5}, Move{h1, d5}
        },
        {
            Move{c4, c8, cm}, Move{h3, c8, cm}, Move{b6, c8, cm},
            Move{b7, c8, MoveType::Queen_Promotion_Capture},
            Move{b7, c8, MoveType::Rook_Promotion_Capture},
            Move{b7, c8, MoveType::Bishop_Promotion_Capture},
            Move{b7, c8, MoveType::Knight_Promotion_Capture}
        },
        {
            Move{h4, e1}, Move{d3, e1}, Move{e2, e1, MoveType::Queen_Promotion},
            Move{e2, e1, MoveType::Rook_Promotion}, Move{e2, e1, MoveType::Bishop_Promotion},
            Move{e2, e1, MoveType::Knight_Promotion}
        },
        {
            Move{h8, g6}, Move{f4, e6, cm}, Move{f4, g6}, Move{f4, h3},
            Move{f4, g2}, Move{f4, d3}, Move{f4, d5}, Move{h1, g3}, Move{h1, f2}
        },
        {
            Move{a8, c7, cm}, Move{a8, b6}, Move{b5, a7}, Move{b5, a3},
            Move{b5, c7, cm}, Move{b5, d6}, Move{b5, d4}
        },
        {
            Move{d5, e5}, Move{d5, f5}, Move{d5, g5}, Move{d5, h5},
            Move{d5, d4}, Move{d5, d3}, Move{d5, c5, cm},
            Move{d5, e6}, Move{d5, f7, cm}, Move{d5, e4},
            Move{d5, f3, cm}, Move{d5, c4}, Move{d5, c6},
            Move{b4, b5}, Move{b4, b6, cm}, Move{b4, c4}, 
            Move{b4, d4}, Move{b4, e4}, Move{b4, f4, cm},
            Move{b4, a4}, Move{b4, c5, cm}, Move{b4, c3}, 
            Move{b4, a5}, Move{a2, b2}, Move{a2, a1, cm},
            Move{d2, d3}, Move{d2, d4}, Move{d2, e2}, Move{d2, d1},
            Move{b1, b2}, Move{b1, a1, cm}, Move{h8, g7, cm},
            Move{b7, c8}, Move{b7, c6}, Move{b7, a6}, Move{b7, a8},
            Move{f2, g3}, Move{f2, h4}, Move{f2, g1},
        },
        { Move{c7, c6}, Move{a7, b6, cm}, Move{b7, c6}, Move{d7, c6} },
        { Move{c5, c6}, Move{c5, b6, MoveType::En_Passant_Capture} },
        { Move{c5, c6}, Move{c5, d6, MoveType::En_Passant_Capture} },
        { Move{e4, e3}, Move{e4, f3, MoveType::En_Passant_Capture} },
        { Move{e4, e3}, Move{e4, d3, MoveType::En_Passant_Capture} },
        { Move{c5, b6, MoveType::En_Passant_Capture} },
        { Move{e4, d3, MoveType::En_Passant_Capture} },
        { 
            Move{a7, a8, MoveType::Queen_Promotion},
            Move{a7, a8, MoveType::Knight_Promotion},
            Move{a7, a8, MoveType::Bishop_Promotion}, 
            Move{a7, a8, MoveType::Rook_Promotion},
            Move{a7, b8, MoveType::Queen_Promotion_Capture},
            Move{a7, b8, MoveType::Knight_Promotion_Capture},
            Move{a7, b8, MoveType::Bishop_Promotion_Capture},
            Move{a7, b8, MoveType::Rook_Promotion_Capture},
            Move{c7, c8, MoveType::Queen_Promotion}, 
            Move{c7, c8, MoveType::Knight_Promotion},
            Move{c7, c8, MoveType::Bishop_Promotion}, 
            Move{c7, c8, MoveType::Rook_Promotion},
            Move{c7, d8, MoveType::Queen_Promotion_Capture},
            Move{c7, d8, MoveType::Knight_Promotion_Capture},
            Move{c7, d8, MoveType::Bishop_Promotion_Capture},
            Move{c7, d8, MoveType::Rook_Promotion_Capture},
            Move{c7, b8, MoveType::Queen_Promotion_Capture},
            Move{c7, b8, MoveType::Knight_Promotion_Capture},
            Move{c7, b8, MoveType::Bishop_Promotion_Capture},
            Move{c7, b8, MoveType::Rook_Promotion_Capture},
            Move{h7, h8, MoveType::Queen_Promotion}, 
            Move{h7, h8, MoveType::Knight_Promotion},
            Move{h7, h8, MoveType::Bishop_Promotion}, 
            Move{h7, h8, MoveType::Rook_Promotion},
            Move{d3, c4, cm}, Move{e3, e4}, Move{b2, b3},
            Move{b2, b4, MoveType::Double_Pawn_Push},
        },
        {
            Move{c7, c6}, Move{c7, c5, MoveType::Double_Pawn_Push},
            Move{e5, d4, cm}, Move{e5, f4, cm},
            Move{a2, b1, MoveType::Queen_Promotion_Capture},
            Move{a2, b1, MoveType::Knight_Promotion_Capture},
            Move{a2, b1, MoveType::Bishop_Promotion_Capture},
            Move{a2, b1, MoveType::Rook_Promotion_Capture},
            Move{d2, d1, MoveType::Queen_Promotion}, 
            Move{d2, d1, MoveType::Knight_Promotion},
            Move{d2, d1, MoveType::Bishop_Promotion}, 
            Move{d2, d1, MoveType::Rook_Promotion},
            Move{d2, c1, MoveType::Queen_Promotion_Capture},
            Move{d2, c1, MoveType::Knight_Promotion_Capture},
            Move{d2, c1, MoveType::Bishop_Promotion_Capture},
            Move{d2, c1, MoveType::Rook_Promotion_Capture},
            Move{d2, e1, MoveType::Queen_Promotion_Capture},
            Move{d2, e1, MoveType::Knight_Promotion_Capture},
            Move{d2, e1, MoveType::Bishop_Promotion_Capture},
            Move{d2, e1, MoveType::Rook_Promotion_Capture}
        }
    };

    std::vector<std::tuple<Move, std::string_view, std::string_view>> make_unmake_move_fens
    {
        {
            Move{h4, f2},
            "8/3k4/2r5/8/7B/8/1K4P1/8 w - - 0 1",
            "8/3k4/2r5/8/8/8/1K3BP1/8 b - - 1 1"
        },
        {
        Move{ g2, g4, MoveType::Double_Pawn_Push },
            "8/3k1p2/2r5/8/7B/8/1K4P1/8 w - - 0 1",
            "8/3k1p2/2r5/8/6PB/8/1K6/8 b - g3 0 1"
        },
        {
            Move {f7, f5, MoveType::Double_Pawn_Push},
            "8/3k1p2/2r5/8/6PB/8/1K6/8 b - g3 0 1",
            "8/3k4/2r5/5p2/6PB/8/1K6/8 w - f6 0 2"
        },
        {
            Move{h7, h4, cm},
            "8/3k3q/2r5/8/6PB/8/1K2Q3/8 b - - 0 1",
            "8/3k4/2r5/8/6Pq/8/1K2Q3/8 w - - 0 2"
        },
        {
            Move{c5, d6, MoveType::En_Passant_Capture},
            "4k3/8/8/2PpP3/6p1/8/5P1P/4K3 w - d6 0 1",
            "4k3/8/3P4/4P3/6p1/8/5P1P/4K3 b - - 0 1"
        },
        {
            Move{e5, d6, MoveType::En_Passant_Capture},
            "4k3/8/8/2PpP3/6p1/8/5P1P/4K3 w - d6 0 1",
            "4k3/8/3P4/2P5/6p1/8/5P1P/4K3 b - - 0 1"
        },
        {
            Move{g4, h3, MoveType::En_Passant_Capture},
            "4k3/8/8/2PpP3/6pP/8/5P2/4K3 b - h3 0 1",
            "4k3/8/8/2PpP3/8/7p/5P2/4K3 w - - 0 2"
        },
        {
            Move{g4, f3, MoveType::En_Passant_Capture},
            "4k3/8/8/2PpP3/5PpP/8/8/4K3 b - f3 0 1",
            "4k3/8/8/2PpP3/7P/5p2/8/4K3 w - - 0 2"
        },
        {
            Move{e1, g1, MoveType::King_Side_Castle},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 b kq - 1 1"
        },
        {
            Move{e1, g1, MoveType::King_Side_Castle},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 b kq - 1 1"
        },
        {
            Move{e8, g8, MoveType::King_Side_Castle},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 b kq - 1 1",
            "r4rk1/pppppppp/8/8/8/8/PPPPPPPP/R4RK1 w - - 2 2"
        },
        {
            Move{e1, c1, MoveType::Queen_Side_Castle},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/2KR3R b kq - 1 1"
        },
        {
            Move{e8, c8, MoveType::Queen_Side_Castle},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/2KR3R b kq - 1 1",
            "2kr3r/pppppppp/8/8/8/8/PPPPPPPP/2KR3R w - - 2 2"
        },
        {
            Move{b7, b8, MoveType::Knight_Promotion},
            "8/1P4P1/8/4k3/8/2K5/p4p2/8 w - - 0 1",
            "1N6/6P1/8/4k3/8/2K5/p4p2/8 b - - 0 1"
        },
        {
            Move{a2, a1, MoveType::Rook_Promotion},
            "1N6/6P1/8/4k3/8/2K5/p4p2/8 b - - 0 1",
            "1N6/6P1/8/4k3/8/2K5/5p2/r7 w - - 0 2"
        },
        {
            Move{g7, g8, MoveType::Queen_Promotion},
            "1N6/6P1/8/4k3/8/2K5/5p2/r7 w - - 0 2",
            "1N4Q1/8/8/4k3/8/2K5/5p2/r7 b - - 0 2"
        },
        {
            Move{f2, f1, MoveType::Bishop_Promotion},
            "1N4Q1/8/8/4k3/8/2K5/5p2/r7 b - - 0 2",
            "1N4Q1/8/8/4k3/8/2K5/8/r4b2 w - - 0 3"
        },
        {
            Move{e7, d8, MoveType::Queen_Promotion_Capture},
            "1n1b4/2P1P3/8/8/1K4k1/8/4p1p1/5N1B w - - 0 1",
            "1n1Q4/2P5/8/8/1K4k1/8/4p1p1/5N1B b - - 0 1"
        },
        {
            Move{g2, h1, MoveType::Rook_Promotion_Capture},
            "1n1Q4/2P5/8/8/1K4k1/8/4p1p1/5N1B b - - 0 1",
            "1n1Q4/2P5/8/8/1K4k1/8/4p3/5N1r w - - 0 2"
        },
        {
            Move{c7, b8, MoveType::Knight_Promotion_Capture},
            "1n1Q4/2P5/8/8/1K4k1/8/4p3/5N1r w - - 0 2",
            "1N1Q4/8/8/8/1K4k1/8/4p3/5N1r b - - 0 2"
        },
        {
            Move{e2, f1, MoveType::Bishop_Promotion_Capture},
            "1N1Q4/8/8/8/1K4k1/8/4p3/5N1r b - - 0 2",
            "1N1Q4/8/8/8/1K4k1/8/8/5b1r w - - 0 3"
        },
        {
            Move{e1, d1, MoveType::Quite},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R2K3R b kq - 1 1"
        },
        {
            Move{e8, f8, MoveType::Quite},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R2K3R b kq - 1 1",
            "r4k1r/pppppppp/8/8/8/8/PPPPPPPP/R2K3R w - - 2 2"
        },
        {
            Move{a1, b1, MoveType::Quite},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/R3K2R w KQkq - 0 1",
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2K2R b Kkq - 1 1"
        }, 
        {
            Move{a8, c8, MoveType::Quite},
            "r3k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2K2R b Kkq - 1 1",
            "2r1k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2K2R w Kk - 2 2"
        },
        {
            Move{h1, f1, MoveType::Quite},
            "2r1k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2K2R w Kk - 2 2",
            "2r1k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2KR2 b k - 3 2"
        },
        {
            Move{h8, g8, MoveType::Quite},
            "2r1k2r/pppppppp/8/8/8/8/PPPPPPPP/1R2KR2 b k - 3 2",
            "2r1k1r1/pppppppp/8/8/8/8/PPPPPPPP/1R2KR2 w - - 4 3"
        }, 
        {
            Move{d5, a8, cm},
            "r3k2r/p1pppp1p/8/3BB3/3qb3/8/P1PPPP1P/R3K2R w KQkq - 0 1",
            "B3k2r/p1pppp1p/8/4B3/3qb3/8/P1PPPP1P/R3K2R b KQk - 0 1"
        },
        {
            Move{e4, h1, cm},
            "B3k2r/p1pppp1p/8/4B3/3qb3/8/P1PPPP1P/R3K2R b KQk - 0 1",
            "B3k2r/p1pppp1p/8/4B3/3q4/8/P1PPPP1P/R3K2b w Qk - 0 2"
        },
        {
            Move{e5, h8, cm},
            "B3k2r/p1pppp1p/8/4B3/3q4/8/P1PPPP1P/R3K2b w Qk - 0 2",
            "B3k2B/p1pppp1p/8/8/3q4/8/P1PPPP1P/R3K2b b Q - 0 2"
        },
        {
            Move{d4, a1, cm},
            "B3k2B/p1pppp1p/8/8/3q4/8/P1PPPP1P/R3K2b b Q - 0 2",
            "B3k2B/p1pppp1p/8/8/8/8/P1PPPP1P/q3K2b w - - 0 3"
        },
        {
            Move{b7, b5, MoveType::Double_Pawn_Push},
            "r1bqkbnr/pppppppp/2n5/P7/8/8/1PPPPPPP/RNBQKBNR b KQkq - 0 1",
            "r1bqkbnr/p1pppppp/2n5/Pp6/8/8/1PPPPPPP/RNBQKBNR w KQkq b6 0 2"
        },
        {
            Move{a5, b6, MoveType::En_Passant_Capture},
            "r1bqkbnr/p1pppppp/2n5/Pp6/8/8/1PPPPPPP/RNBQKBNR w KQkq b6 0 2",
            "r1bqkbnr/p1pppppp/1Pn5/8/8/8/1PPPPPPP/RNBQKBNR b KQkq - 0 2"
        }
    };
};