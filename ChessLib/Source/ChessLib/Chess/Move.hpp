#pragma once

#include <cstdint>
#include <vector>

#include <ChessLib/Chess/Definitions.hpp>

namespace chesslib 
{
    class Move
    {
    public:

        Move(Square loc_from, Square loc_to, MoveType move_type, Piece captured_piece = pieceset::None);

        Square GetFrom() const;
        Square GetTo() const;
        Piece GetCapturedPiece() const;
        MoveType GetMoveType() const;

        bool IsPromotion() const;
        bool IsCapture() const;
        bool IsCastle() const;
        bool IsEnPassantCapture() const;
    
    private:

        Square from;
        Square to;
        MoveType mtype;
        Piece captured;
        
        friend bool operator == (const Move m1, const Move m2);
        friend bool operator < (const Move m1, const Move m2);
    };

    using MoveList = std::vector<Move>;

    bool operator == (const Move m1, const Move m2);
    bool operator < (const Move m1, const Move m2);
}