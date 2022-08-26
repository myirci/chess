#include "pch.h"

#include "TestHelpers.hpp"

#include <algorithm>

#include <ChessLib/Board/BoardFactory.hpp>
#include <ChessLib/Board/BasicBoard.hpp>
#include <ChessLib/Board/x88Board.hpp>
#include <ChessLib/MoveGenerator/MoveGenerator.hpp>
#include <ChessLib/MoveGenerator/MoveGeneratorConn.hpp>
#include <ChessLib/MoveGenerator/Connectivity.hpp>
#include <ChessLib/MoveGenerator/Connections.hpp>
#include <ChessLib/MoveGenerator/PrecomputedMoves.hpp>

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

    template <typename T, size_t N>
    bool IsEqual(const std::array<T, N>& arr, const std::vector<T>& vec) 
    {
        if (arr.size() != vec.size())
            return false;

        for (int i = 0; i < arr.size(); i++) 
            if (arr[i] != vec[i])
                return false;
        
        return true;
    }
};

#pragma region BasicBoardTests

TEST_F(MoveGeneratorTests, basic_board_test_1_king_moves)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-1");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<BasicBoard>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<BasicBoard>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<BasicBoard>(*b);
        EXPECT_EQ(expectedMoves.size(), moves.size());
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    }
}

TEST_F(MoveGeneratorTests, basic_board_test_2_check_eliminating_moves)
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

TEST_F(MoveGeneratorTests, basic_board_test_3_random_positions_1)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-3");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<BasicBoard>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<BasicBoard>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<BasicBoard>(*b);
        EXPECT_EQ(expectedMoves.size(), moves.size());
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    }
}

TEST_F(MoveGeneratorTests, basic_board_test_4_random_positions_2)
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

TEST_F(MoveGeneratorTests, basic_board_test_5_number_of_moves)
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

TEST_F(MoveGeneratorTests, basic_board_test_6_single_piece_moves)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-6");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<BasicBoard>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<BasicBoard>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<BasicBoard>(*b);
        EXPECT_EQ(expectedMoves.size(), moves.size());
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    }
}

TEST_F(MoveGeneratorTests, basic_board_test_7_en_passant_capture)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-7");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        std::cout << lines[i] << std::endl;
        auto b = BoardFactory::make_unique_board<BasicBoard>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<BasicBoard>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<BasicBoard>(*b);
        EXPECT_EQ(expectedMoves.size(), moves.size());
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    }
}
#pragma endregion

#pragma region x88BoardTests
TEST_F(MoveGeneratorTests, x88_board_test_1_king_moves)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-1");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<x88Board>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<x88Board>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<x88Board>(*b);
        EXPECT_EQ(expectedMoves.size(), moves.size());
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    }
}

TEST_F(MoveGeneratorTests, x88_board_test_2_check_eliminating_moves)
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

TEST_F(MoveGeneratorTests, x88_board_test_3_random_positions_1)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-3");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<x88Board>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<x88Board>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<x88Board>(*b);
        EXPECT_EQ(expectedMoves.size(), moves.size());
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    }
}

TEST_F(MoveGeneratorTests, x88_board_test_4_random_positions_2)
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

TEST_F(MoveGeneratorTests, x88_board_test_5_number_of_moves)
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

TEST_F(MoveGeneratorTests, x88_board_test_6_single_piece_moves)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-6");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<x88Board>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<x88Board>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<x88Board>(*b);
        EXPECT_EQ(expectedMoves.size(), moves.size());
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    }
}

TEST_F(MoveGeneratorTests, x88_board_test_7_en_passant_capture)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-7");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<x88Board>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<x88Board>(lines[(size_t)(i + 2)]);
        auto moves = GenerateMoves<x88Board>(*b);
        EXPECT_EQ(expectedMoves.size(), moves.size());
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    }
}
#pragma endregion

#pragma region SimpleBoard_PrecomputedConnectionsTests
TEST_F(MoveGeneratorTests, precomputed_connections_test)
{
    precompute::Connectivity con{};
    const auto& conVec = con.GetConnections(); 
    const auto& idxVec = con.GetIndexes();

    constexpr std::size_t size = conn::ConnectionsAndIndices.first.size();
    auto isequal = IsEqual<Square, size>(conn::ConnectionsAndIndices.first, conVec);
    EXPECT_TRUE(isequal);

    int idx1{ 0 }, idx2{ 1 };
    for (int i = 0; i < 10; i++)
    {
        EXPECT_EQ(conn::ConnectionsAndIndices.second[idx1], idxVec[i]._indexes_start);
        EXPECT_EQ(conn::ConnectionsAndIndices.second[idx2], idxVec[i]._indexes_end);
        idx1 += 2;
        idx2 = idx1 + 1;
    }

    EXPECT_EQ(conn::ConnectionsAndIndices.second[20], idxVec[10]._indexes_start);
    EXPECT_EQ(conn::ConnectionsAndIndices.second[21], idxVec[11]._indexes_start);
}

TEST_F(MoveGeneratorTests, simple_board_precomputed_connections_test_1_king_moves)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-1");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<BasicBoard>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<BasicBoard>(lines[(size_t)(i + 2)]);
        MoveGeneratorConn<SimpleBoard> mgen{};
        auto moves = mgen.GenerateMoves(*b);
        EXPECT_EQ(expectedMoves.size(), moves.size());
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    }
}

TEST_F(MoveGeneratorTests, simple_board_precomputed_connections_test_2_check_eliminating_moves)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-2");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<BasicBoard>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<BasicBoard>(lines[(size_t)(i + 2)]);
        MoveGeneratorConn<SimpleBoard> mgen{};
        auto moves = mgen.GenerateMoves(*b);
        EXPECT_TRUE(TestHelpers::IsSubset(moves, expectedMoves));
    }
}

TEST_F(MoveGeneratorTests, simple_board_precomputed_connections_test_3_random_positions_1)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-3");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<BasicBoard>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<BasicBoard>(lines[(size_t)(i + 2)]);
        MoveGeneratorConn<SimpleBoard> mgen{};
        auto moves = mgen.GenerateMoves(*b);
        EXPECT_EQ(expectedMoves.size(), moves.size());
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    }
}

TEST_F(MoveGeneratorTests, simple_board_precomputed_connections_test_4_random_positions_2)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-4");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        std::cout << lines[(size_t)(i)] << std::endl;
        auto b = BoardFactory::make_unique_board<BasicBoard>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<BasicBoard>(lines[(size_t)(i + 2)]);
        MoveGeneratorConn<SimpleBoard> mgen{};
        auto moves = mgen.GenerateMoves(*b);
        EXPECT_TRUE(TestHelpers::IsSubset(moves, expectedMoves));
    }
}

TEST_F(MoveGeneratorTests, simple_board_precomputed_connections_test_5_number_of_moves)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-5");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<SimpleBoard>(lines[(size_t)(i + 1)]);
        MoveGeneratorConn<SimpleBoard> mgen{};
        auto moves = mgen.GenerateMoves(*b);
        EXPECT_EQ(moves.size(), (size_t)std::stoi(lines[(size_t)(i + 2)]));
    }
}

TEST_F(MoveGeneratorTests, simple_board_precomputed_connections_test_6_single_piece_moves)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-6");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<SimpleBoard>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<SimpleBoard>(lines[(size_t)(i + 2)]);
        MoveGeneratorConn<SimpleBoard> mgen{};
        auto moves = mgen.GenerateMoves(*b);
        EXPECT_EQ(expectedMoves.size(), moves.size());
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    }
}

TEST_F(MoveGeneratorTests, simple_board_precomputed_connections_test_7_en_passant_capture)
{
    auto scoped_open = ScopedOpen(TestHelpers::MoveGeneratorTestCases);
    auto lines = TestHelpers::GetCleanLines(scoped_open.GetFile(), "Group-7");

    EXPECT_TRUE(lines.size() != 0 && lines.size() % 3 == 0);

    for (auto i{ 0 }; i < lines.size(); i += 3)
    {
        auto b = BoardFactory::make_unique_board<SimpleBoard>(lines[(size_t)(i + 1)]);
        auto expectedMoves = TestHelpers::GetMoves<SimpleBoard>(lines[(size_t)(i + 2)]);
        MoveGeneratorConn<SimpleBoard> mgen{};
        auto moves = mgen.GenerateMoves(*b);
        EXPECT_EQ(expectedMoves.size(), moves.size());
        EXPECT_TRUE(std::is_permutation(moves.begin(), moves.end(), expectedMoves.begin()));
    } 
}
#pragma endregion
