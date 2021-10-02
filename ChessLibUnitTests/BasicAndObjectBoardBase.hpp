#pragma once

#include "CommonBoardTestBase.hpp"

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/BoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>

#include <array>

using namespace chesslib;
using namespace chesslib::squareset;
using namespace chesslib::pieceset;

class BasicAndObjectBoardBase : public CommonBoardTestBase
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

    BoardBaseWithPieces::PieceMap white_pieces_starting_position
    {
        {WhitePawn, a2},   {WhitePawn, b2},   {WhitePawn, c2},   {WhitePawn, d2},
        {WhitePawn, e2},   {WhitePawn, f2},   {WhitePawn, g2},   {WhitePawn, h2},
        {WhiteRook, a1},   {WhiteRook, h1},   {WhiteKnight, b1}, {WhiteKnight, g1},
        {WhiteBishop, c1}, {WhiteBishop, f1}, {WhiteQueen, d1},  {WhiteKing, e1}
    };

    BoardBaseWithPieces::PieceMap black_pieces_starting_position
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

    BoardBaseWithPieces::PieceMap white_pieces_fen1
    {
        {WhitePawn, b2}, {WhitePawn, f2}, {WhitePawn, g2}, {WhitePawn, d5}, {WhitePawn, e4}, {WhiteKing, g1}
    };

    BoardBaseWithPieces::PieceMap black_pieces_fen1
    {
        {BlackPawn, a7}, {BlackPawn, b7}, {BlackPawn, c5}, {BlackPawn, d6}, {BlackPawn, f7}, {BlackPawn, f4},
        {BlackPawn, g7}, {BlackPawn, g5}, {BlackKing, g8}
    };

    MoveType qm{ MoveType::Quite };
    MoveType cm{ MoveType::Capture };

    std::vector<MoveList> expected_king_moves
    {
        { Move{e4, d4, qm}, Move{e4, f4, qm}, Move{e4, d3, qm}, Move{e4, e3, qm}, Move{e4, f3, qm} },
        { Move{e6, d6, qm}, Move{e6, f6, qm}, Move{e6, d7, qm}, Move{e6, e7, qm}, Move{e6, f7, qm} },
        { Move{a1, a2, qm}, Move{a1, b1, qm}, Move{a1, b2, qm} },
        { Move{h8, h7, qm}, Move{h8, g8, qm}, Move{h8, g7, qm} },
        { Move{e3, f3, qm} },
        { Move{d6, c6, qm}, Move{d6, d7, qm} },
        { },
        { }
    };

    std::vector<MoveList> expected_check_eliminating_moves
    {
        { Move{b4, a2, qm}, Move{b4, a6, cm, pieceset::BlackQueen} },
        { Move{b4, a2, qm}, Move{b4, a6, cm, pieceset::WhiteQueen} },
        { 
            Move{c6, d7, cm, pieceset::BlackRook}, Move{f5, d7, cm, pieceset::BlackRook},
            Move{g7, d7, cm, pieceset::BlackRook}, Move{f5, d3, qm}, Move{g7, d4, qm}
        },
        { Move{f5, d7, cm, pieceset::WhiteRook}, Move{g7, d7, cm, pieceset::WhiteRook}, Move{f5, d3, qm}, Move{g7, d4, qm} },
        {
            Move{b3, a4, cm, pieceset::BlackRook}, Move{a7, a4, cm, pieceset::BlackRook},
            Move{b3, b4, qm}, Move{b7, b4, qm}, Move{f2, f4, MoveType::Double_Pawn_Push}
        },
        {
            Move{b1, e4, cm, pieceset::WhiteKnight}, Move{h4, e4, cm, pieceset::WhiteKnight},
            Move{f6, e4, cm, pieceset::WhiteKnight}, Move{e8, e4, cm, pieceset::WhiteKnight},
            Move{f5, e4, cm, pieceset::WhiteKnight}, Move{d5, e4, cm, pieceset::WhiteKnight}
        },
        {
            Move{d5, e6, MoveType::En_Passant_Capture, BlackPawn},
            Move{f5, e6, MoveType::En_Passant_Capture, BlackPawn}, Move{d4, e5, cm, BlackPawn}
        },
        {
            Move{f4, g3, MoveType::En_Passant_Capture, WhitePawn},
            Move{h4, g3, MoveType::En_Passant_Capture, WhitePawn}, Move{f5, g4, cm, WhitePawn}
        }
    };

    std::vector<MoveList> expected_moves
    {
        { },
        { Move{d5, e4, qm}, Move{d5, c4, cm, WhitePawn}, Move{b3, c4, cm, WhitePawn} },
        { Move{d2, c1, cm, BlackBishop}, Move{e2, e1, cm, BlackQueen}, Move{e4, e5, qm} },
        { Move{d6, d5, qm}, Move{d6, e7, qm}}
    };
};