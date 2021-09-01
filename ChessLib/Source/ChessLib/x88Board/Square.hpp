#pragma once

#include <cstdint>
#include <utility>

namespace chesslib::x88board::square 
{
    using index = int8_t;
    
    constexpr index inside{ -120 }; // 0x88

    constexpr index 
        a1{ 0 }, a2{ 16 }, a3{ 32 }, a4{ 48 }, a5{ 64 }, a6{ 80 }, a7{ 96 }, a8{ 112 },
        b1{ 1 }, b2{ 17 }, b3{ 33 }, b4{ 49 }, b5{ 65 }, b6{ 81 }, b7{ 97 }, b8{ 113 },
        c1{ 2 }, c2{ 18 }, c3{ 34 }, c4{ 50 }, c5{ 66 }, c6{ 82 }, c7{ 98 }, c8{ 114 },
        d1{ 3 }, d2{ 19 }, d3{ 35 }, d4{ 51 }, d5{ 67 }, d6{ 83 }, d7{ 99 }, d8{ 115 },
        e1{ 4 }, e2{ 20 }, e3{ 36 }, e4{ 52 }, e5{ 68 }, e6{ 84 }, e7{ 100 }, e8{ 116 },
        f1{ 5 }, f2{ 21 }, f3{ 37 }, f4{ 53 }, f5{ 69 }, f6{ 85 }, f7{ 101 }, f8{ 117 },
        g1{ 6 }, g2{ 22 }, g3{ 38 }, g4{ 54 }, g5{ 70 }, g6{ 86 }, g7{ 102 }, g8{ 118 },
        h1{ 7 }, h2{ 23 }, h3{ 39 }, h4{ 55 }, h5{ 71 }, h6{ 87 }, h7{ 103 }, h8{ 119 };

    constexpr index top_to_bottom_order[64]
    {
        a8, b8, c8, d8, e8, f8, g8, h8,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a1, b1, c1, d1, e1, f1, g1, h1
    };

    constexpr index bottom_to_top_order[64]
    {
        a1, b1, c1, d1, e1, f1, g1, h1,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a8, b8, c8, d8, e8, f8, g8, h8
    };

    constexpr index twin_square(index idx) { return idx + 8; }

    constexpr index full_top_to_bottom_order[128]
    {
        a8, b8, c8, d8, e8, f8, g8, h8, twin_square(a8), twin_square(b8), twin_square(c8), twin_square(d8), twin_square(e8), twin_square(f8), twin_square(g8), twin_square(h8),
        a7, b7, c7, d7, e7, f7, g7, h7, twin_square(a7), twin_square(b7), twin_square(c7), twin_square(d7), twin_square(e7), twin_square(f7), twin_square(g7), twin_square(h7),
        a6, b6, c6, d6, e6, f6, g6, h6, twin_square(a6), twin_square(b6), twin_square(c6), twin_square(d6), twin_square(e6), twin_square(f6), twin_square(g6), twin_square(h6),
        a5, b5, c5, d5, e5, f5, g5, h5, twin_square(a5), twin_square(b5), twin_square(c5), twin_square(d5), twin_square(e5), twin_square(f5), twin_square(g5), twin_square(h5),
        a4, b4, c4, d4, e4, f4, g4, h4, twin_square(a4), twin_square(b4), twin_square(c4), twin_square(d4), twin_square(e4), twin_square(f4), twin_square(g4), twin_square(h4),
        a3, b3, c3, d3, e3, f3, g3, h3, twin_square(a3), twin_square(b3), twin_square(c3), twin_square(d3), twin_square(e3), twin_square(f3), twin_square(g3), twin_square(h3),
        a2, b2, c2, d2, e2, f2, g2, h2, twin_square(a2), twin_square(b2), twin_square(c2), twin_square(d2), twin_square(e2), twin_square(f2), twin_square(g2), twin_square(h2),
        a1, b1, c1, d1, e1, f1, g1, h1, twin_square(a1), twin_square(b1), twin_square(c1), twin_square(d1), twin_square(e1), twin_square(f1), twin_square(g1), twin_square(h1)
    };

    constexpr index full_bottom_to_top_order[128]
    {
        a1, b1, c1, d1, e1, f1, g1, h1, twin_square(a1), twin_square(b1), twin_square(c1), twin_square(d1), twin_square(e1), twin_square(f1), twin_square(g1), twin_square(h1),
        a2, b2, c2, d2, e2, f2, g2, h2, twin_square(a2), twin_square(b2), twin_square(c2), twin_square(d2), twin_square(e2), twin_square(f2), twin_square(g2), twin_square(h2),
        a3, b3, c3, d3, e3, f3, g3, h3, twin_square(a3), twin_square(b3), twin_square(c3), twin_square(d3), twin_square(e3), twin_square(f3), twin_square(g3), twin_square(h3),
        a4, b4, c4, d4, e4, f4, g4, h4, twin_square(a4), twin_square(b4), twin_square(c4), twin_square(d4), twin_square(e4), twin_square(f4), twin_square(g4), twin_square(h4),
        a5, b5, c5, d5, e5, f5, g5, h5, twin_square(a5), twin_square(b5), twin_square(c5), twin_square(d5), twin_square(e5), twin_square(f5), twin_square(g5), twin_square(h5),
        a6, b6, c6, d6, e6, f6, g6, h6, twin_square(a6), twin_square(b6), twin_square(c6), twin_square(d6), twin_square(e6), twin_square(f6), twin_square(g6), twin_square(h6),
        a7, b7, c7, d7, e7, f7, g7, h7, twin_square(a7), twin_square(b7), twin_square(c7), twin_square(d7), twin_square(e7), twin_square(f7), twin_square(g7), twin_square(h7),
        a8, b8, c8, d8, e8, f8, g8, h8, twin_square(a8), twin_square(b8), twin_square(c8), twin_square(d8), twin_square(e8), twin_square(f8), twin_square(g8), twin_square(h8)
    };

    constexpr index get_square_from_file_and_rank(index file, index rank) { return  rank * 16 + file; }
    
    constexpr index get_rank_from_square(index sq) { return sq >> 4; }

    constexpr index get_file_from_square(index sq) { return sq & 0x0F; }

    constexpr std::pair<index, index> get_file_and_rank_from_square(index sq) 
    { 
        return std::make_pair(get_file_from_square(sq), get_rank_from_square(sq)); 
    }

    constexpr index get_square_from_chars(char fl, char rk) 
    { 
        return get_square_from_file_and_rank(fl - 'a', rk - '1'); 
    };

    constexpr std::pair<index, index> get_chars_from_square(index sq) 
    {
        auto [file, rank] = get_file_and_rank_from_square(sq);
        return std::make_pair(static_cast<char>(file + 'a'), static_cast<char>(rank + '1'));
    }
}



