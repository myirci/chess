#pragma once

#include <ChessLib/Chess/Move.hpp>

#include <string_view>
#include <vector>

class CommonBoardTestBase
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

    std::string_view fen_pos1{ "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Kkq c3 1 2" };

    std::vector<std::string_view> board_setup_fens
    {
        "1k1r3r/pp2bp1p/1q5p/2NP4/1Ppp1PQ1/P6P/2P3P1/B4R1K b - - 0 1",
        "r1q3k1/2p2rpp/p1n5/P3p1N1/1pQ1n1b1/2P3N1/1P3KPP/R1B1R3 w - - 46 56",
        "r3k2r/pp1B1p1p/2p2P2/3p4/5p1P/q1N1Q1P1/P1P5/1K1R4 b kq - 5 16",
        "3q3r/5pk1/p2p2p1/4p1n1/2R2P2/1P5K/P3Q3/6R1 w - - 0 1",
        "r1b1r1k1/1q3pbp/2N2np1/1N1P4/2QB1P2/8/PP3nPP/R3KqR1 w Q - 0 1",
        "8/4q2k/3p2pb/PP1P4/2N1n1Pp/5Q1P/8/7K b - g3 0 1",
        "r1bqkn1r/pp2b2p/2n2p2/4p1pP/2Pp3N/3P2P1/PP3PB1/RNBQR1K1 w kq g6 0 1",
        "r4rk1/p2b2p1/3Np2p/3nPp2/1nBP4/2p2QPq/5P1P/R1BR2K1 w - f6 0 1",
        "4r1k1/1b5p/pp2r1p1/3pPp2/PPpQ1P1q/2P4P/2B3P1/R4RK1 w - f6 23 45",
        "4k2r/1b2b1p1/2q2n1p/4p3/1pP1n2P/p3R3/4QBB1/KN1R4 b k c3 0 59",
    };

    std::vector<std::string_view> move_generator_king_move_fens
    {
        "8/8/4k3/8/4K3/8/8/8 w - - 0 1",
        "8/8/4k3/8/4K3/8/8/8 b - - 0 1",
        "7k/8/8/8/8/8/8/K7 w - - 0 1",
        "7k/8/8/8/8/8/8/K7 b - - 0 1",
        "3rk3/8/8/4q3/7b/4K3/8/8 w - - 0 1",
        "4R3/8/3k4/R7/8/6B1/8/3K4 b - - 0 1",
        "8/8/8/8/2p1n3/6n1/8/2k1K3 w - - 0 1",
        "7Q/3k4/1P5R/8/1B2B3/8/8/4K3 b - - 0 1",
        "8/b7/4k3/8/3nKn2/8/8/8 w - - 0 1",
    };

    std::vector<std::string_view> move_generator_check_eliminating_move_fens
    {
        "k7/8/q7/8/1N6/8/1P6/KR6 w - - 0 1",
        "K7/8/Q7/8/1n6/8/1p6/kr6 b - - 0 1",
        "k7/3r2Q1/2P5/5B2/8/8/3K4/8 w - - 0 1",
        "K7/3R2q1/8/5b2/8/8/3k4/8 b - - 0 1",
        "7k/RR6/8/2n5/r5K1/1P6/5P2/8 w - - 0 1",
        "4r3/8/3k1n2/3p1p2/4N2q/8/8/Kb6 b - - 0 1",
        "k7/8/8/3PpP2/3K4/8/8/8 w - e6 0 1",
        "8/8/8/5k2/5pPp/8/8/K7 b - g3 0 1",
        "k3B3/8/p7/1q3B2/P6R/N7/2PPK3/1R6 w - - 0 1",
        "3K4/8/8/8/8/8/n2bp1p1/1k5R b - - 0 1"
    };

    std::vector<std::string_view> move_generation_all_moves_fens
    {
        "3qk3/pp2pppb/b6n/2n2P2/2N5/r1PKB2r/P1PRN1PP/1q1r1b1R w - - 0 1",
        "Q2R2Q1/1p1N1b2/2Ppp3/RPnk1pqR/2Pn4/1q1r1p2/B2R4/K6B b - - 0 1",
        "4r3/q1k5/7b/8/3PPP2/r2NKN1r/3QRN2/2b1q1b1 w - - 0 1",
        "1B1R4/3n4/2rkn3/2p1p3/3pN3/Q7/6K1/3R4 b - - 0 1",
        "7k/q7/8/2Pp4/3K4/8/8/8 w - d6 0 1",
        "k7/8/8/8/3Pp3/8/8/4K2B b - d3 0 1",
        "rnbqkb1r/pppppppp/8/8/4n3/3P4/PPPKPPPP/RNBQ1BNR w kq - 0 1",
        "1rrr1nk1/5ppb/8/2R5/4Q3/r7/2K1Q1r1/1r6 w - - 0 1",
        "rnb1kbnr/pp1ppppp/2p5/q7/8/3P4/PPPKPPPP/RNBQ1BNR w kq - 0 1",
        "rnbqkb1r/ppp1ppp1/3N1n1p/3p4/Q7/2P5/PP1PPPPP/R1B1KBNR b KQkq - 0 1",
        "rnbqk2r/ppp1ppp1/3N1n1p/3p4/Q7/2P5/PP1PPPPP/R1B1KBNR b KQkq - 0 1",
    };

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
