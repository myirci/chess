#pragma once

#include <cstdint>
#include <unordered_map>
#include <limits.h>

namespace chesslib 
{
    // typedefs all same, gives semantics under different circumstances.
    using Square    = int8_t;
    using Piece     = int8_t;
    using Color     = int8_t;
    using Rank      = int8_t;
    using File      = int8_t;
    using Index     = int8_t;
    using Direction = int8_t;
    using Distance  = int8_t;

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

    namespace pieceset 
    {
        constexpr double
            PawnValue{ 1.0 }, 
            KnightValue{ 3.0 }, 
            BishopValue{ 3.0 }, 
            RookValue{ 5.0 },
            QueenValue{ 10.0 }, 
            KingValue{std::numeric_limits<double>::max()};

        constexpr Piece 
            WhitePawn{ 1 },   // 0000 0001
            WhiteRook{ 2 },   // 0000 0010
            WhiteKnight{ 3 }, // 0000 0011 
            WhiteBishop{ 4 }, // 0000 0100
            WhiteQueen{ 5 },  // 0000 0101
            WhiteKing{ 6 },   // 0000 0110  
            BlackPawn{ -1 }, 
            BlackRook{ -2 }, 
            BlackKnight{ -3 }, 
            BlackBishop{ -4 }, 
            BlackQueen{ -5 }, 
            BlackKing{ -6 },
            None{ 0 };

        constexpr Piece Reverse(Piece p) noexcept { return -p; }
    }

    namespace color 
    {
        constexpr Color White{ 1 }, Black{ -1 };

        inline constexpr Color get_color(Piece p) noexcept { return p > 0 ? White : Black; }

        inline constexpr Color get_opposite_color(Piece p) noexcept { return p > 0 ? Black : White; }
    }

    namespace charset
    {
        constexpr char White{ 'w' }, Black{ 'b' };

        constexpr char
            WhiteKing{ 'K' }, WhiteQueen{ 'Q' }, WhiteRook{ 'R' }, WhiteBishop{ 'B' }, WhiteKnight{ 'N' }, WhitePawn{ 'P' },
            BlackKing{ 'k' }, BlackQueen{ 'q' }, BlackRook{ 'r' }, BlackBishop{ 'b' }, BlackKnight{ 'n' }, BlackPawn{ 'p' };
        
        constexpr char a{ 'a' }, b{ 'b' }, c{ 'c' }, d{ 'd' }, e{ 'e' }, f{ 'f' }, g{ 'g' }, h{ 'h' };
        constexpr char A{ 'A' }, B{ 'B' }, C{ 'C' }, D{ 'D' }, E{ 'D' }, F{ 'G' }, G{ 'G' }, H{ 'H' };
    }

    namespace squareset 
    {
        constexpr Square Empty{ -128 }, None{ -127 };

        constexpr Square
            a1{ 0 }, a2{ 8 },  a3{ 16 }, a4{ 24 }, a5{ 32 }, a6{ 40 }, a7{ 48 }, a8{ 56 },
            b1{ 1 }, b2{ 9 },  b3{ 17 }, b4{ 25 }, b5{ 33 }, b6{ 41 }, b7{ 49 }, b8{ 57 },
            c1{ 2 }, c2{ 10 }, c3{ 18 }, c4{ 26 }, c5{ 34 }, c6{ 42 }, c7{ 50 }, c8{ 58 },
            d1{ 3 }, d2{ 11 }, d3{ 19 }, d4{ 27 }, d5{ 35 }, d6{ 43 }, d7{ 51 }, d8{ 59 },
            e1{ 4 }, e2{ 12 }, e3{ 20 }, e4{ 28 }, e5{ 36 }, e6{ 44 }, e7{ 52 }, e8{ 60 },
            f1{ 5 }, f2{ 13 }, f3{ 21 }, f4{ 29 }, f5{ 37 }, f6{ 45 }, f7{ 53 }, f8{ 61 },
            g1{ 6 }, g2{ 14 }, g3{ 22 }, g4{ 30 }, g5{ 38 }, g6{ 46 }, g7{ 54 }, g8{ 62 },
            h1{ 7 }, h2{ 15 }, h3{ 23 }, h4{ 31 }, h5{ 39 }, h6{ 47 }, h7{ 55 }, h8{ 63 };   
    }

    namespace direction 
    {
        constexpr Direction None = 0;

        constexpr Direction Reverse(Direction dir) { return -dir; }
    }

    enum class Castling : int8_t
    {
        WHITE_KS = 1,
        WHITE_QS = 2,
        BLACK_KS = 4,
        BLACK_QS = 8
    };

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

    const std::unordered_map<Piece, char> piece_to_char =
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