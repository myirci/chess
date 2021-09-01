#pragma once

#include <cstdint>

#include <ChessLib/Chess/Definitions.hpp>

namespace chesslib 
{
    enum class MoveType : int8_t
    {
        Quite = 0,                  // 0        0000
        Double_Pawn_Push,           // 1        0001
        King_Side_Castle,           // 2        0010
        Queen_Side_Castle,          // 3        0011
        Capture,                    // 4        0100
        En_Passant_Capture,         // 5        0101
        Knight_Promotion = 8,       //          1000
        Bishop_Promotion,           // 9        1001
        Rook_Promotion,             // 10       1010
        Queen_Promotion,            // 11       1011
        Knight_Promotion_Capture,   // 12       1100
        Bishop_Promotion_Capture,   // 13       1101
        Rook_Promotion_Capture,     // 14       1110
        Queen_Promotion_Capture     // 15       1111
    };

    class Move
    {
    private:

        Square from;
        Square to;
        MoveType mtype;
        Piece captured;

    public:

        Move(Square loc_from, Square loc_to, MoveType move_type, Piece captured_piece);

        Square GetFrom() const;
        Square GetTo() const;
        Piece GetCapturedPiece() const;
        MoveType GetMoveType() const;

        bool IsPromotion() const;
        bool IsCapture() const;
        bool IsCastle() const;
        bool IsEnPassantCapture() const;
    };
}