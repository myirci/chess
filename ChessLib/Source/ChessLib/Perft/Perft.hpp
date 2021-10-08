#pragma once

#include <ChessLib/Chess/Move.hpp>

#include <vector>

namespace chesslib::perft 
{
    struct Stats
    {
        Stats() { }

        uint64_t num_nodes{ 0 };
        uint64_t num_captures{ 0 };
        uint64_t num_promotions{ 0 };
        uint64_t num_enpassant_captures{ 0 };
        uint64_t num_castles{ 0 };
        uint64_t num_checks{ 0 };
        uint64_t num_double_checks{ 0 };
        uint64_t num_discovery_checks{ 0 };
        uint64_t num_check_mates{ 0 };
        uint64_t num_stale_mates{ 0 };
    };

    template <typename Board>
    uint64_t perft(Board& board, int depth) 
    {
        if (depth <= 1)
            return board.GenerateMoves().size();

        uint64_t sum{ 0 };
        auto moves = board.GenerateMoves();
        for (auto const& mv : moves)
        {
            board.MakeMove(mv);
            sum += perft(board, depth - 1);
            board.UnMakeMove();
        }
        return sum;
    }

    template <typename Board>
    std::vector<std::pair<Move, uint64_t>> perft_divide(Board& board, int depth)
    {
        std::vector<std::pair<Move, uint64_t>> perft_results;

        if (depth <= 1) 
        {
            auto moves = board.GenerateMoves();
            std::transform(moves.begin(), moves.end(), std::back_inserter(perft_results),
                [](const auto& move) { return std::make_pair(move, 1); });
            return perft_results;
        }
         
        auto moves = board.GenerateMoves();
        for (auto const& mv : moves)
        {
            board.MakeMove(mv);
            perft_results.emplace_back(mv, perft(board, depth - 1));
            board.UnMakeMove();
        }

        return perft_results;
    }

    template <typename Board>
    Stats perft_statistics(Board& board, int depth)
    {
    }

    template <typename Board>
    std::vector<Stats> perft_divide_statistics(Board& board, int depth)
    {

    }
}