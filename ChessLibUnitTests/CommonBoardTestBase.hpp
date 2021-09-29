#pragma once

#include <string_view>
#include <vector>

class CommonBoardTestBase
{
protected:

    std::string_view fen1{ "6k1/pp3pp1/3p4/2pP2p1/4Pp2/8/1P3PP1/6K1 b Kkq c3 1 2" };

    std::vector<std::string_view> fens
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

    std::vector<std::string_view> checks_and_pins_fens
    {
        "3qk3/pp2pppb/b6n/2n2P2/2N5/r1PKB2r/P1PRN1PP/1q1r1b1R w - - 0 1",
        "Q2R2Q1/1p1N1b2/2Ppp3/RPnk1pqR/2Pn4/1q1r1p2/B2R4/K6B b - - 0 1",
        "rnbqkbnr/p1pppppp/1p6/8/8/5P2/PPPPPKPP/RNBQ1BNR w kq - 0 1",
        "4r3/q1k5/7b/8/3PPP2/r2NKN1r/3QRN2/2b1q1b1 w - - 0 1",
        "1B1R4/3n4/2rkn3/2p1p3/3pN3/Q7/6K1/3R4 b - - 0 1"
    };

    std::vector<std::string_view> move_generator_king_moves_test_fens
    {
        "r3k2r/p3p3/8/8/1q6/8/P2P3P/R3K2R w KQkq - 0 1",
        "r3k2r/1P6/8/8/8/8/3n4/R3K2R b KQkq - 0 1",
        "r3k2r/1P6/8/8/8/8/3n4/R3K2R w KQkq - 0 1",
        "8/b7/4k3/8/3nKn2/8/8/8 w - - 0 1",
        "8/8/4k3/8/4K3/8/8/8 w - - 0 1",
        "8/8/4k3/8/4K3/8/8/8 b - - 0 1",
        "6k1/8/8/8/8/8/6q1/7K w - - 0 1",
        "6k1/8/8/8/5n2/8/6q1/7K w - - 0 1",
        "8/3pp3/2pkp3/2pp4/8/6B1/8/6K1 b - - 0 1",
        "1B6/3pp3/2pkp3/2pp4/8/8/8/6K1 b - - 0 1",
        "8/3pp3/2pkp3/2ppP3/8/8/8/6K1 b - - 0 1",
        "1k6/8/8/3PPP2/3qK3/3PPP2/8/8 w - - 0 1",
        "1k6/8/8/3PPP2/4Kq2/3PPP2/8/8 w - - 0 1"
    };
};
