#pragma once

#include <cstdint>
#include <vector>

#include <ChessLib/Chess/Definitions.hpp>

namespace chesslib 
{
    class Move
    {
    public:

        Move(Square loc_from, Square loc_to, MoveType move_type = MoveType::Quite) :
            from{ loc_from }, to{ loc_to }, mtype{ move_type } { }

        Square GetFrom() const          { return from; }
        Square GetTo() const            { return to; }
        MoveType GetMoveType() const    { return mtype; }
        bool IsPromotion() const        { return (static_cast<int8_t>(mtype) & 0x08) > 0; }
        bool IsCapture() const          { return (static_cast<int8_t>(mtype) & 0x04) > 0; } 
        bool IsCastle() const           { return mtype == MoveType::King_Side_Castle || mtype == MoveType::Queen_Side_Castle; }
        bool IsEnPassantCapture() const { return mtype == MoveType::En_Passant_Capture; }
    
    private:

        Square from;
        Square to;
        MoveType mtype;
        
        friend bool operator == (const Move m1, const Move m2);
        friend bool operator < (const Move m1, const Move m2);
    };

    using MoveList = std::vector<Move>;

    bool operator == (const Move m1, const Move m2);

    bool operator < (const Move m1, const Move m2);
}