#pragma once

#include <cstdint>
#include <unordered_map>
#include <limits.h>

namespace chesslib 
{
    // typedefs for int8_t (equivalent to a char or byte), gives semantics for different usages.
    using Square    = int8_t;
    using Piece     = int8_t;
    using Color     = int8_t;
    using Rank      = int8_t;
    using File      = int8_t;
    using Index     = int8_t;
    using Direction = int8_t;
    using Distance  = int8_t;

    inline constexpr int8_t Empty{ -128 };

    enum class MoveType : int8_t
    {
        Quite = 0,                  // 0        0000
        Double_Pawn_Push,           // 1        0001
        King_Side_Castle,           // 2        0010
        Queen_Side_Castle,          // 3        0011
        Capture,                    // 4        0100
        En_Passant_Capture,         // 5        0101
                                    // 6
                                    // 7
        Knight_Promotion = 8,       // 8        1000
        Bishop_Promotion,           // 9        1001
        Rook_Promotion,             // 10       1010
        Queen_Promotion,            // 11       1011
        Knight_Promotion_Capture,   // 12       1100
        Bishop_Promotion_Capture,   // 13       1101
        Rook_Promotion_Capture,     // 14       1110
        Queen_Promotion_Capture     // 15       1111
    };

    enum class Castling : int8_t
    {
        WHITE_KS = 1,
        WHITE_QS = 2,
        BLACK_KS = 4,
        BLACK_QS = 8
    };

    namespace pieceset 
    {
        inline constexpr double
            PawnValue{ 1.0 }, 
            KnightValue{ 3.0 }, 
            BishopValue{ 3.0 }, 
            RookValue{ 5.0 },
            QueenValue{ 10.0 }, 
            KingValue{ std::numeric_limits<double>::max() };

        inline constexpr Piece 
            WhitePawn{ 1 },  WhiteRook{ 2 },  WhiteKnight{ 3 },  WhiteBishop{ 4 },  WhiteQueen{ 5 },  WhiteKing{ 6 },  
            BlackPawn{ -1 }, BlackRook{ -2 }, BlackKnight{ -3 }, BlackBishop{ -4 }, BlackQueen{ -5 }, BlackKing{ -6 };

        constexpr Piece Opposite(Piece p) noexcept { return -p; }
    }

    namespace color 
    {
        inline constexpr Color White{ 1 }, Black{ -1 };

        inline constexpr Color get_color(Piece p) noexcept { return p > 0 ? White : Black; }

        inline constexpr Color get_opposite_color(Piece p) noexcept { return p > 0 ? Black : White; }
    }

    namespace charset
    {
        inline constexpr char White{ 'w' }, Black{ 'b' };

        inline constexpr char
            WhiteKing{ 'K' }, WhiteQueen{ 'Q' }, WhiteRook{ 'R' }, WhiteBishop{ 'B' }, WhiteKnight{ 'N' }, WhitePawn{ 'P' },
            BlackKing{ 'k' }, BlackQueen{ 'q' }, BlackRook{ 'r' }, BlackBishop{ 'b' }, BlackKnight{ 'n' }, BlackPawn{ 'p' };
        
        inline constexpr char a{ 'a' }, b{ 'b' }, c{ 'c' }, d{ 'd' }, e{ 'e' }, f{ 'f' }, g{ 'g' }, h{ 'h' };
        inline constexpr char A{ 'A' }, B{ 'B' }, C{ 'C' }, D{ 'D' }, E{ 'D' }, F{ 'G' }, G{ 'G' }, H{ 'H' };
    }

    namespace direction 
    {
        inline constexpr Direction Reverse(Direction dir) { return -dir; }
    }

    inline const std::unordered_map<char, Piece> char_to_piece =
    {
        { charset::WhitePawn,   pieceset::WhitePawn   }, 
        { charset::BlackPawn,   pieceset::BlackPawn   },
        { charset::WhiteRook,   pieceset::WhiteRook   }, 
        { charset::BlackRook,   pieceset::BlackRook   },
        { charset::WhiteKnight, pieceset::WhiteKnight }, 
        { charset::BlackKnight, pieceset::BlackKnight },
        { charset::WhiteBishop, pieceset::WhiteBishop }, 
        { charset::BlackBishop, pieceset::BlackBishop },
        { charset::WhiteQueen,  pieceset::WhiteQueen  }, 
        { charset::BlackQueen,  pieceset::BlackQueen  },
        { charset::WhiteKing,   pieceset::WhiteKing   }, 
        { charset::BlackKing,   pieceset::BlackKing   }
    };

    inline const std::unordered_map<Piece, char> piece_to_char =
    {
        { pieceset::WhitePawn,   charset::WhitePawn   }, 
        { pieceset::BlackPawn,   charset::BlackPawn   },
        { pieceset::WhiteRook,   charset::WhiteRook   }, 
        { pieceset::BlackRook,   charset::BlackRook   },
        { pieceset::WhiteKnight, charset::WhiteKnight }, 
        { pieceset::BlackKnight, charset::BlackKnight },
        { pieceset::WhiteBishop, charset::WhiteBishop }, 
        { pieceset::BlackBishop, charset::BlackBishop },
        { pieceset::WhiteQueen,  charset::WhiteQueen  }, 
        { pieceset::BlackQueen,  charset::BlackQueen  },
        { pieceset::WhiteKing,   charset::WhiteKing   }, 
        { pieceset::BlackKing,   charset::BlackKing   }
    };
}