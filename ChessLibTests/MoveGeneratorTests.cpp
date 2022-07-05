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

TEST_F(MoveGeneratorTests, knight_connections_test) 
{
    Connectivity dynamic_con(false);
    dynamic_con.ComputeKnightConnections();
    auto& knight_indexes = dynamic_con.GetKnightIndexes();
    auto& knight_connections = dynamic_con.GetKnightConnections();
    EXPECT_EQ(Connections::KnightSquareIndexes, knight_indexes);
    EXPECT_TRUE(IsEqual<Connections::KnightConnections.size()>(Connections::KnightConnections, knight_connections));
}

TEST_F(MoveGeneratorTests, king_connections_test)
{
    Connectivity dynamic_con(false);
    dynamic_con.ComputeKingConnections();
    auto& king_indexes = dynamic_con.GetKingIndexes();
    auto& king_connections = dynamic_con.GetKingConnections();
    EXPECT_EQ(Connections::KingSquareIndexes, king_indexes);
    EXPECT_TRUE(IsEqual<Connections::KingConnections.size()>(Connections::KingConnections, king_connections));
}

TEST_F(MoveGeneratorTests, straight_connections_test)
{
    Connectivity dynamic_con(false);
    dynamic_con.ComputeStraightConnections();
    auto& straight_indexes = dynamic_con.GetStraightIndexes();
    auto& straight_connections = dynamic_con.GetStraightConnections();
    EXPECT_EQ(Connections::StraightSquareIndexes, straight_indexes);
    EXPECT_TRUE(IsEqual<Connections::StraightConnections.size()>(Connections::StraightConnections, straight_connections));
}

TEST_F(MoveGeneratorTests, diagonal_connections_test)
{
    Connectivity dynamic_con(false);
    dynamic_con.ComputeDiagonalConnections();
    auto& diagonal_indexes = dynamic_con.GetDiagonalIndexes();
    auto& diagonal_connections = dynamic_con.GetDiagonalConnections();
    EXPECT_EQ(Connections::DiagonalSquareIndexes, diagonal_indexes);
    EXPECT_TRUE(IsEqual<Connections::DiagonalConnections.size()>(Connections::DiagonalConnections, diagonal_connections));
}

TEST_F(MoveGeneratorTests, pawn_connections_test)
{
    Connectivity dynamic_con(false);
    dynamic_con.ComputePawnConnections();
    auto& pawn_indexes = dynamic_con.GetPawnIndexes();
    auto& pawn_connections = dynamic_con.GetPawnConnections();
    EXPECT_EQ(Connections::PawnSquareIndexes, pawn_indexes);
    EXPECT_TRUE(IsEqual<Connections::PawnConnections.size()>(Connections::PawnConnections, pawn_connections));
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