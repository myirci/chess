#include <pch.h>

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/Perft/Perft.hpp>
#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Board/BasicBoard/BasicBoard.hpp>
/*
using namespace chesslib;

class PerftTest : public ::testing::Test 
{
public:
	PerftTest() { }
protected:
	std::vector<std::tuple<std::string_view, int, unsigned long>> perft_positions
	{
		{Fen::StartingPosition, 4, 197281},
		{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4, 4085603},
		{"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 4, 43238},
		{"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 5, 674624},
		{"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4, 422333},
		{"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 4, 2103487},
		{"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 4, 3894594}
	};
};

TEST_F(PerftTest, basic_board_perft)
{
	for (auto [fen, depth, expected_sum] : perft_positions) 
	{
		auto board = basic_board::make_unique_board(fen);
		auto sum = perft::perft(*board, depth);
		EXPECT_EQ(sum, expected_sum);
	}
}

TEST_F(PerftTest, x88_board_perft) 
{
	for (auto [fen, depth, expected_sum] : perft_positions)
	{
		auto board = x88board::make_unique_board(fen);
		auto sum = perft::perft(*board, depth);
		EXPECT_EQ(sum, expected_sum);
	}
}

*/