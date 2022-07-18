#include "pch.h"

#include "TestHelpers.hpp"

#include <algorithm>

#include <ChessLib/Board/BoardFactory.hpp>
#include <ChessLib/Board/BasicBoard.hpp>
#include <ChessLib/Board/x88Board.hpp>
#include <ChessLib/MoveGenerator/MoveGenerator.hpp>
#include <ChessLib/MoveGenerator/Connectivity.hpp>
#include <ChessLib/MoveGenerator/Connections.hpp>

using namespace chesslib;

class MoveGeneratorTests : public ::testing::Test
{
protected:

    template<typename BoardType>
    MoveList GenerateMoves(const BoardType& board)
    {
        MoveGenerator<BoardType> mgen;
        return mgen.GenerateMoves(board);
    }

    template <int N>
    bool IsEqual(const std::array<Square, N>& arr, const std::vector<Square>& vec) 
    {
        if (arr.size() != vec.size())
            return false;

        for (int i = 0; i < arr.size(); i++) 
            if (arr[i] != vec[i])
                return false;
        
        return true;
    }
};

TEST_F(MoveGeneratorTests, connections_test) 
{
    Connectivity con{};
    const auto& conVec = con.GetConnections();
    const auto& idxVec = con.GetIndexes();
    EXPECT_TRUE(IsEqual<conn::ConnectionsAndIndices.first.size()>(conn::ConnectionsAndIndices.first, conVec));

    int idx1{ 0 }, idx2{ 1 };
    for (int i = 0; i < 9; i++) 
    {
        EXPECT_EQ(conn::ConnectionsAndIndices.second[idx1], idxVec[i]._indexes_start);
        EXPECT_EQ(conn::ConnectionsAndIndices.second[idx2], idxVec[i]._indexes_end);
        idx1 += 2;
        idx2 = idx1 + 1;
    }
    
    EXPECT_EQ(conn::ConnectionsAndIndices.second[18], idxVec[9]._indexes_start);
    EXPECT_EQ(conn::ConnectionsAndIndices.second[19], idxVec[10]._indexes_start);
    EXPECT_EQ(conn::ConnectionsAndIndices.second[20], idxVec[11]._indexes_start);
}

TEST_F(MoveGeneratorTests, basic_board_king_moves)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-1");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<BasicBoard>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<BasicBoard>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<BasicBoard>(*b);
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    }
}

TEST_F(MoveGeneratorTests, basic_board_check_eliminating_moves)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-2");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<BasicBoard>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<BasicBoard>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<BasicBoard>(*b);
        EXPECT_TRUE(TestHelpers::IsSubset(moves, expectedMoves));
    }
}

TEST_F(MoveGeneratorTests, basic_board_generate_moves_1)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-3");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<BasicBoard>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<BasicBoard>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<BasicBoard>(*b);
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    }
}

TEST_F(MoveGeneratorTests, basic_board_generate_moves_2)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-4");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<BasicBoard>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<BasicBoard>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<BasicBoard>(*b);
        EXPECT_TRUE(TestHelpers::IsSubset(moves, expectedMoves));
    }
}

TEST_F(MoveGeneratorTests, basic_board_generate_moves_3)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-5");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<BasicBoard>(lines[(size_t)(i + 1)]);
        auto moves = GenerateMoves<BasicBoard>(*b);
        EXPECT_EQ(moves.size(), (size_t)std::stoi(lines[(size_t)(i + 2)]));
    }
}

TEST_F(MoveGeneratorTests, x88_board_king_moves)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-1");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<x88Board>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<x88Board>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<x88Board>(*b);
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    }
}

TEST_F(MoveGeneratorTests, x88_board_check_eliminating_moves)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-2");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<x88Board>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<x88Board>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<x88Board>(*b);
        EXPECT_TRUE(TestHelpers::IsSubset(moves, expectedMoves));
    }
}

TEST_F(MoveGeneratorTests, x88_board_generate_moves_1)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-3");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<x88Board>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<x88Board>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<x88Board>(*b);
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    }
}

TEST_F(MoveGeneratorTests, x88_board_generate_moves_2)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-4");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<x88Board>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<x88Board>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<x88Board>(*b);
        EXPECT_TRUE(TestHelpers::IsSubset(moves, expectedMoves));
    }
}

TEST_F(MoveGeneratorTests, x88_board_generate_moves_3)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-5");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<x88Board>(lines[(size_t)(i + 1)]);
        auto moves = GenerateMoves<x88Board>(*b);
        EXPECT_EQ(moves.size(), (size_t)std::stoi(lines[(size_t)(i + 2)]));
    }
}