#include "Fen.hpp"

#include <ChessLib/Chess/Definitions.hpp>

#include <algorithm>

namespace chesslib 
{
    const std::unordered_set<char> Fen::ValidCharSet
    {
        charset::WhiteKing, charset::WhiteQueen, charset::WhiteRook, charset::WhiteBishop, 
        charset::WhiteKnight, charset::WhitePawn, charset::BlackKing, charset::BlackQueen, 
        charset::BlackRook, charset::BlackBishop, charset::BlackKnight, charset::BlackPawn,
        RankSeparator, '1', '2', '3', '4', '5', '6', '7', '8'
    };

    Fen::Fen(std::string_view fen_string) : fen_str{ "" }
    {
        auto [first, last] = regex::parse_string(fen_string, regex::not_white_space);
        auto diff = std::distance(first, last);
        if (diff != MinNumberOfFields && diff != MaxNumberOfFields) 
            throw std::logic_error("Invalid FEN string.");

        if (IsValid(first, (int)diff))
            Init(first, last);
        else
            throw std::logic_error("Invalid FEN string.");
    }

    bool Fen::IsValid(regex::csvregex_token_it it, int num_fields)
    {
        std::string_view pp{ it->first, it->second };
        if(!ValidatePiecePlacement(pp))
            return false;

        it++;
        std::string_view ac{ it->first, it->second };
        if (!ValidateActiveColor(ac))
            return false;
        
        it++;
        std::string_view cr{ it->first, it->second };
        if (!ValidateCastlingRights(cr))
            return false;

        it++;
        std::string_view ep{ it->first, it->second };
        if (!ValidateEnPassantTargetSquare(ep, ac[0]))
            return false;

        if (num_fields == MaxNumberOfFields) 
        {
            it++;
            std::string_view hmc{ it->first, it->second };
            if (!ValidateHalfMoveClock(hmc, ep))
                return false;

            it++;
            std::string_view fmc{ it->first, it->second };
            if (!ValidateFullMoveClock(fmc))
                return false;
        }

        return true;
    }

    bool Fen::ValidatePiecePlacement(std::string_view pp)
    {
        auto [first, last] = regex::parse_string(pp, regex::not_forward_slash);
        auto diff = std::distance(first, last);
        if (diff != 8)
            return false;

        std::unordered_map<char, int> chars;
        int rank{ 8 };
        while (first != last) 
            if (!CheckRank(rank--, first++, chars))
                return false;

        return ValidateNumberOfPieces(chars);
    }

    bool Fen::ValidateNumberOfPieces(std::unordered_map<char, int>& chars) 
    {
        int num_extra_white_pieces =
            std::max(0, chars[charset::WhiteQueen] - 1) + std::max(0, chars[charset::WhiteRook] - 2) +
            std::max(0, chars[charset::WhiteKnight] - 2) + std::max(0, chars[charset::WhiteBishop] - 2);

        int num_extra_black_pieces =
            std::max(0, chars[charset::BlackQueen] - 1) + std::max(0, chars[charset::BlackRook] - 2) +
            std::max(0, chars[charset::BlackKnight] - 2) + std::max(0, chars[charset::BlackBishop] - 2);

        return 
            chars[charset::WhiteKing] == 1 && chars[charset::BlackKing] == 1 &&
            chars[charset::WhitePawn] <= 8 && chars[charset::BlackPawn] <= 8 &&
            num_extra_white_pieces <= (8 - chars[charset::WhitePawn]) &&
            num_extra_black_pieces <= (8 - chars[charset::BlackPawn]);
    }

    bool Fen::CheckRank(int rank, regex::csvregex_token_it tit, std::unordered_map<char, int>& chars)
    {
        auto square_count{ 0 };
        bool digit{ false };

        auto it = tit->first;
        while (it != tit->second)
        {
            if (!ValidCharSet.contains(*it) || ((rank == 8 || rank == 1) && (*it == charset::WhitePawn || *it == charset::BlackPawn)))
                return false;

            digit = std::isdigit(*it);
            square_count += (digit ? (*it - '0') : 1);

            if (digit)
                if (auto next_it = std::next(it); next_it != tit->second && std::isdigit(*next_it))
                    return false;
            
            chars[*it++]++;
        }

        return square_count == 8;
    }

    bool Fen::ValidateActiveColor(std::string_view ac) 
    {
        return ac.size() == 1 && (ac[0] == charset::White || ac[0] == charset::Black);
    }
    
    bool Fen::ValidateCastlingRights(std::string_view cr) 
    {
        return 
            cr == "-"   || cr == "K"   || cr == "k"   || cr == "Q"   || cr == "q"    ||
            cr == "Kk"  || cr == "Kq"  || cr == "Qk"  || cr == "Qq"  || cr == "KQ"   || cr == "kq" ||
            cr == "Kkq" || cr == "Qkq" || cr == "KQk" || cr == "KQq" || cr == "KQkq";
    }

    bool Fen::ValidateEnPassantTargetSquare(std::string_view ep, char active_color) 
    {
        return
            ep == "-" ||
            ep.size() == 2 &&
            ep[0] >= 'a' && ep[0] <= 'h' &&
            ((active_color == charset::White && ep[1] == '6') || (active_color == charset::Black && ep[1] == '3'));
    }

    bool Fen::ValidateHalfMoveClock(std::string_view hmc, std::string_view ep)
    {
        return
            !hmc.empty() &&
            (hmc == "0" ||
             hmc.size() == 1 && std::isdigit(hmc[0]) && ep == "-" ||
             hmc.size() == 2 && hmc[0] > '0' && hmc[0] <= '9' && std::isdigit(hmc[1]) && ep == "-");
    }

    bool Fen::ValidateFullMoveClock(std::string_view fmc)
    {
        return
            !fmc.empty() &&
            (fmc.size() == 1 && std::isdigit(fmc[0]) ||
             fmc[0] != '0' && std::all_of(fmc.begin(), fmc.end(), [](char c) { return std::isdigit(c); }));
    }

    void Fen::Init(regex::csvregex_token_it first, regex::csvregex_token_it last)
    {
        while (first != last)
        {
            fen_str += *first;
            fen_str.push_back(FieldSeparator);
            fields.emplace_back(first->first, first->second);
            first++;
        }
        fen_str.pop_back();
    }

    std::vector<std::string_view> Fen::GetFlattenedFields(std::string_view fen) 
    {
        std::vector<std::string_view> flattened_fields;
        auto [fields_first, fields_last] = regex::parse_string(fen, regex::not_white_space);
        auto [ranks_first, ranks_last] = regex::parse_string(std::string_view{ fields_first->first, fields_first->second }, regex::not_forward_slash);
        
        while (ranks_first != ranks_last) 
        {
            flattened_fields.emplace_back(std::string_view{ ranks_first->first, ranks_first->second });
            ranks_first++;
        }
        std::reverse(flattened_fields.begin(), flattened_fields.end());
        
        fields_first++;
        while(fields_first != fields_last)
        {
            flattened_fields.emplace_back(std::string_view{ fields_first->first, fields_first->second });
            fields_first++;
        }
        return flattened_fields;
    }
}


