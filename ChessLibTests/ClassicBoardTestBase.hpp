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

    MoveType cm{ MoveType::Capture };

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
};