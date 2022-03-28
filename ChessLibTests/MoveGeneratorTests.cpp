#include "pch.h"

class MoveGeneratorTests : public ::testing::Test
{

};

/*
TEST_F(MoveGeneratorTests, move_generation_king_only_moves)
{
    for (auto i{0}; i < move_generator_king_move_fens.size(); i++)
    {
        auto b = basic_board::make_unique_board(move_generator_king_move_fens[i]);
        auto moves = b->GenerateMoves();
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expected_king_moves[i].begin()));
    }
}

TEST_F(MoveGeneratorTests, move_generation_check_eliminating_moves)
{
    for (auto i{ 0 }; i < move_generator_check_eliminating_move_fens.size(); i++)
    {
        auto b = basic_board::make_unique_board(move_generator_check_eliminating_move_fens[i]);
        auto moves = b->GenerateMoves();
        EXPECT_TRUE(IsSubset(moves, expected_check_eliminating_moves[i]));
    }
}

TEST_F(MoveGeneratorTests, move_generation_compare_all_moves)
{
    for (auto i{ 0 }; i < move_generation_all_moves_fens.size(); i++)
    {
        auto b = basic_board::make_unique_board(move_generation_all_moves_fens[i]);
        auto moves = b->GenerateMoves();
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expected_moves_all_moves[i].begin()));
    }
}

TEST_F(MoveGeneratorTests, move_generation_compare_move_subset)
{
    for (auto i{ 0 }; i < move_generation_subset.size(); i++)
    {
        auto b = basic_board::make_unique_board(move_generation_subset[i]);
        auto moves = b->GenerateMoves();
        EXPECT_TRUE(IsSubset(moves, expected_moves_subset[i]));
    }
}

TEST_F(MoveGeneratorTests, move_generation_number_of_moves)
{
    for (const auto& p : move_generation_number_of_moves)
    {
        auto b = basic_board::make_unique_board(p.first);
        auto moves = b->GenerateMoves();
        EXPECT_EQ(p.second, moves.size());
    }
}

*/