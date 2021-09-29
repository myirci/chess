#pragma once

#include "CommonBoardTestBase.hpp"

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/BoardBase.hpp>

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
        {WhitePawn, b2}, {WhitePawn, f2}, {WhitePawn, g2},
        {WhitePawn, d5}, {WhitePawn, e4}, {WhiteKing, g1}
    };

    BoardBaseWithPieces::PieceMap black_pieces_fen1
    {
        {BlackPawn, a7}, {BlackPawn, b7}, {BlackPawn, c5},
        {BlackPawn, d6}, {BlackPawn, f7}, {BlackPawn, f4},
        {BlackPawn, g7}, {BlackPawn, g5}, {BlackKing, g8}
    };
};