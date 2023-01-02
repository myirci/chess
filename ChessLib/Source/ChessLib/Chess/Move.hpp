#pragma once

#include <cstdint>
#include <vector>
#include <string_view>

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/ColorTraits.hpp>

namespace chesslib 
{
    class Move
    {
    public:

        constexpr Move() : from{ Empty }, to{ Empty }, mtype{ MoveType::Quite } { }

        constexpr Move(Square loc_from, Square loc_to, MoveType move_type = MoveType::Quite) : from{ loc_from }, to{ loc_to }, mtype{ move_type } { }

        constexpr Square GetFrom() const          { return from; }
        constexpr Square GetTo() const            { return to; }
        constexpr MoveType GetMoveType() const    { return mtype; }
        constexpr bool IsPromotion() const        { return (static_cast<int8_t>(mtype) & 0x08) > 0; }
        constexpr bool IsCapture() const          { return (static_cast<int8_t>(mtype) & 0x04) > 0; }
        constexpr bool IsCastle() const           { return mtype == MoveType::King_Side_Castle || mtype == MoveType::Queen_Side_Castle; }
        constexpr bool IsEnPassantCapture() const { return mtype == MoveType::En_Passant_Capture; }

        constexpr void SetFrom(Square f)          { from = f; }
        constexpr void SetTo(Square t)            { to = t; }
        constexpr void SetMoveType(MoveType mt)   { mtype = mt; }
    private:

        Square from;
        Square to;
        MoveType mtype;
        
        friend constexpr bool operator == (const Move& m1, const Move& m2)
        {
            return m1.from == m2.from && m1.to == m2.to && m1.mtype == m2.mtype;
        }

        friend constexpr bool operator < (const Move& m1, const Move& m2)
        {
            return m1.from < m2.from || m1.from == m2.from && m1.to < m2.to;
        }
    };

    using MoveList = std::vector<Move>;

    std::string_view to_string(MoveType mtype);

    template <Color PieceColor>
    Piece get_promoted_piece(MoveType mtype) 
    {
        using ctraits = traits::color_traits<PieceColor>;

        switch (mtype)
        {
            case MoveType::Queen_Promotion_Capture:
            case MoveType::Queen_Promotion:
                return ctraits::Queen;

            case MoveType::Knight_Promotion_Capture:
            case MoveType::Knight_Promotion:
                return ctraits::Knight;

            case MoveType::Rook_Promotion_Capture:
            case MoveType::Rook_Promotion:
                return ctraits::Rook;

            case MoveType::Bishop_Promotion_Capture:
            case MoveType::Bishop_Promotion:
                return ctraits::Bishop;

            default: 
                return Empty;
        }
    }
} 