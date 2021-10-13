#pragma once

#include <ChessLib/Chess/Move.hpp>

#include <ChessLib/Chess/ChessUtility.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

namespace chesslib::perft 
{
    struct Stats
    {
        Stats(unsigned int num_depth) : depth{num_depth} 
        {
            if (depth < 0)
                throw std::logic_error("Depth cannot be negative.");


            size_t size{ (size_t)depth + 1 };
            
            num_nodes.resize(size);
            num_captures.resize(size);
            num_promotions.resize(size);
            num_enpassant_captures.resize(size);
            num_castles.resize(size);
            num_checks.resize(size);
            num_double_checks.resize(size);
            num_discovery_checks.resize(size);
            num_check_mates.resize(size);
            num_stale_mates.resize(size);
        }

        void Reverse() 
        {
            std::reverse(num_nodes.begin(), num_nodes.end());
            std::reverse(num_captures.begin(), num_captures.end());
            std::reverse(num_promotions.begin(), num_promotions.end());
            std::reverse(num_enpassant_captures.begin(), num_enpassant_captures.end());
            std::reverse(num_castles.begin(), num_castles.end());
            std::reverse(num_checks.begin(), num_checks.end());
            std::reverse(num_double_checks.begin(), num_double_checks.end());
            std::reverse(num_discovery_checks.begin(), num_discovery_checks.end());
            std::reverse(num_check_mates.begin(), num_check_mates.end());
            std::reverse(num_stale_mates.begin(), num_stale_mates.end());
        }
        
        unsigned int depth;

        std::vector<uint64_t> num_nodes;
        std::vector<uint64_t> num_captures;
        std::vector<uint64_t> num_promotions;
        std::vector<uint64_t> num_enpassant_captures;
        std::vector<uint64_t> num_castles;
        std::vector<uint64_t> num_checks;
        std::vector<uint64_t> num_double_checks;
        std::vector<uint64_t> num_discovery_checks;
        std::vector<uint64_t> num_check_mates;
        std::vector<uint64_t> num_stale_mates;
    };

    std::ostream& operator << (std::ostream& os, const Stats& stats)
    {
        
        for (unsigned int i{ 0 }; i <= stats.depth; i++) 
        {
            os << "\tDepth: " << i << std::endl;
            os << "\t\tNumber of nodes                : " << stats.num_nodes[i] << std::endl;
            os << "\t\tNumber of captures             : " << stats.num_captures[i] << std::endl;
            os << "\t\tNumber of en-passsant captures : " << stats.num_enpassant_captures[i] << std::endl;
            os << "\t\tNumber of promotions           : " << stats.num_promotions[i] << std::endl;
            os << "\t\tNumber of castles              : " << stats.num_castles[i] << std::endl;
            os << "\t\tNumber of checks               : " << stats.num_checks[i] << std::endl;
            os << "\t\tNumber of discovery checks     : " << stats.num_discovery_checks[i] << std::endl;
            os << "\t\tNumber of double checks        : " << stats.num_double_checks[i] << std::endl;
            os << "\t\tNumber of check mates          : " << stats.num_check_mates[i] << std::endl;
            os << "\t\tNumber of stale mates          : " << stats.num_stale_mates[i] << std::endl;
        }
        return os;
    }

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

    namespace implementation 
    {
        template <typename Board>
        void perft_statistics(Board& board, Stats& stat, int depth)
        {
            stat.num_nodes[depth]++;
            
            const auto& move_stack = board.GetMoveStack();
            if (!move_stack.empty()) 
            {
                const auto& prev_move = move_stack.top();
                if (prev_move.move.IsCapture())
                    stat.num_captures[depth]++;

                if (prev_move.move.IsPromotion())
                    stat.num_promotions[depth]++;

                if (prev_move.move.IsCastle())
                    stat.num_castles[depth]++;

                if (prev_move.move.IsEnPassantCapture())
                    stat.num_enpassant_captures[depth]++;
            }
            
            auto moves = board.GenerateMoves();

            const auto& checks = board.GetChecks();
            if (!checks.empty())
            {
                stat.num_checks[depth] += checks.size();

                if (checks.size() == 2)
                {
                    stat.num_double_checks[depth]++;
                    /*if (depth == 0) 
                    {
                        auto fen_str = utility::chess::board_to_fen(board);
                        std::cout << "\"" << fen_str << "\" " << utility::chess::to_string<Board>(move_stack) << std::endl;
                    }*/
                }
                    
                if (!move_stack.empty()) 
                {
                    auto to = move_stack.top().move.GetTo();
                    for (const auto& check : checks)
                        if (std::get<0>(check) != to)
                            stat.num_discovery_checks[depth]++;
                }
            }

            if (moves.empty())
            {
                if (checks.empty())
                    stat.num_stale_mates[depth]++;
                else 
                {
                    stat.num_check_mates[depth]++;
                    /*
                    if (depth == 0) 
                    {
                        auto fen_str = utility::chess::board_to_fen(board);
                        std::cout << "\"" << fen_str << "\" " << utility::chess::to_string<Board>(move_stack) << std::endl;
                    }*/
                }
                    
            }

            if (depth > 0)
            {
                for (const auto& move : moves)
                {
                    board.MakeMove(move);
                    perft_statistics(board, stat, depth - 1);
                    board.UnMakeMove();
                }
            }            
        }
    }

    template <typename Board>
    std::unique_ptr<Stats> perft_statistics(Board& board, int depth)
    {
        auto stat = std::make_unique<Stats>(depth);
        implementation::perft_statistics(board, *stat, depth);
        stat->Reverse();
        return stat;
    }

    template <typename Board>
    std::vector<std::pair<std::optional<Move>, std::unique_ptr<Stats>>> perft_divide_statistics(Board& board, int depth)
    {
        std::vector<std::pair<std::optional<Move>, std::unique_ptr<Stats>>> perft_results;
        
        if (depth == 0) 
            perft_results.emplace_back(std::nullopt, perft_statistics(board, 0));
        else
        {
            auto moves = board.GenerateMoves();
            for (auto const& move : moves)
            {
                board.MakeMove(move);
                perft_results.emplace_back(move, perft_statistics(board, depth - 1));
                board.UnMakeMove();
            }
        }
        return perft_results;
    }
}