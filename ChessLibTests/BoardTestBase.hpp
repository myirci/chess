#pragma once

#include <ChessLib/Chess/Move.hpp>

#include <string_view>
#include <vector>

class BoardTestBase
{
public:

    static bool IsSubset(const chesslib::MoveList& larger, const chesslib::MoveList& smaller)
    {
        if (smaller.size() > larger.size())
            return false;

        for (auto it = smaller.cbegin(); it != smaller.cend(); it++) 
        {
            auto find_it = std::find(larger.begin(), larger.end(), *it);
            if (find_it == larger.end())
                return false;
        }
        return true;
    };

protected:

    std::vector<std::string_view> move_generation_subset
    {
        "r3k2r/p3p3/8/8/1q6/8/P2P3P/R3K2R w KQkq - 0 1",
        "r3k2r/1P6/8/8/8/8/3n4/R3K2R b KQkq - 0 1",
        "r3k2r/1P6/8/8/8/8/3n4/R3K2R w KQkq - 0 1",
        "rnb1kbnr/ppp1qppp/2pP1p2/8/1P1P2P1/2N2P2/PPP4R/R1BQKBN1 w Qkq - 0 1",
        "3q4/4pk2/2p2n2/7r/8/8/B7/KN1r1R1b b - - 0 1",
        "2q3K1/1P6/1N6/8/2R5/7B/8/k7 w - - 0 1",
        "2R4K/8/8/q7/7b/2bn4/4p3/2k4R b - - 0 1",
        "n3k2N/2R2R2/4p3/1n5P/5N2/1pp5/2p1K3/n6N w - - 0 1",
        "n3k2N/2R2R2/4p3/1n5P/5N2/1pp5/2p1K3/n6N b - - 0 1",
        "6kB/1B3ppp/1n1P4/2qQ4/1Q3p2/PP2Pn2/R1PR1B2/nRK1N3 w - - 0 1",
        "brbrbbbb/bbrbpppp/kN2p3/1ppp4/2p5/8/8/4K3 b - - 0 1",
        "4k3/8/8/1pP5/8/8/8/4K3 w - b6 0 1",
        "4k3/8/8/2Pp4/8/8/8/4K3 w - d6 0 1",
        "4k3/8/8/8/4pP2/8/8/4K3 b - f3 0 1",
        "4k3/8/8/8/3Pp3/8/8/4K3 b - d3 0 1",
        "7k/q7/8/1pP5/3K4/8/8/8 w - b6 0 1",
        "8/7k/8/8/3Pp3/8/8/1B2K3 b - d3 0 1",
        "1r1n1n2/P1P2P1P/4r3/2k4b/2b5/3PPP2/1P2K3/8 w - - 0 1",
        "2k5/2p5/8/4p3/3PPP2/8/pK1p4/NNQ1B1R1 b - - 0 1"
    };

    std::vector<std::pair<std::string_view, int>> move_generation_number_of_moves
    {
        {"8/8/1p6/p6q/PP6/2Qp4/6P1/k5K1 b - - 0 1", 2},
        {"3k4/8/2r5/2Pp4/2K5/8/8/8 w - d3 0 1", 7},
        {"8/2p5/3p4/KP5r/1R3pPk/8/4P3/8 b - g3 0 1", 17},
        {"8/2p5/3p4/KP4r1/5pPk/8/4P3/1R6 b - g3 0 1", 18},
        {"8/8/3p4/1Pp4r/1KR2pk1/8/4P1P1/8 w - c6 0 1", 7},
        {"8/2p5/3p4/KP3R1r/5p2/8/4PkP1/8 b - - 0 1", 20},
        {"8/2p5/3p4/KP4kr/5pP1/8/4P3/6R1 b - g3 0 1", 16},
        {"K7/8/8/8/1R2pP1k/8/8/8 b - f3 0 1", 5},
        {"K7/8/8/8/1R1pPp1k/8/8/8 b - e3 0 1", 9},
        {"n1r1r1b1/1P1P1P1P/1Q6/3NBNK1/R7/4p1p1/3PBPkP/2R5 w - - 0 1", 144},
        {"3Q4/1Q4Q1/4Q3/2Q4R/Q4Q2/3Q4/1Q4Rp/1KbBBNNk w - - 0 1", 218}
    };
};
