#pragma once

#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Utility/Utility.hpp>

#include <iostream>
#include <fstream>
#include <string_view>
#include <vector>

class TestHelpers
{
public:

    static constexpr std::string_view BoardSetupTestCases = "../../ChessLibTests/data/board_setup_tests.txt";
    static constexpr std::string_view MakeUnMakeMovesTestCases = "../../ChessLibTests/data/make_unmake_moves.txt";
    static constexpr std::string_view MoveGeneratorTestCases = "../../ChessLibTests/data/move_generator_tests.txt";

    static std::vector<std::string> GetCleanLines(std::ifstream& iFile, std::string_view groupName)
    {
        std::vector<std::string> cleanLines;
        bool groupFound{ false };
        std::string line{ "" };
        while (std::getline(iFile, line))
        {
            chesslib::utility::string_processing::trim(line);
            if (line.empty())
                continue;

            if (line == groupName)
            {
                groupFound = true;
                continue;
            }

            if (!groupFound)
                continue;

            if (line == "{" || line[0] == '#')
                continue;

            if (line == "}")
                break;

            cleanLines.push_back(line);
        }

        return cleanLines;
    }

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

    template<typename BoardType>
    static chesslib::Move GetMove(std::string_view moveStr) 
    {
        chesslib::Square from    = BoardType::GetSquareFromChars(moveStr[0], moveStr[1]);
        chesslib::Square to      = BoardType::GetSquareFromChars(moveStr[3], moveStr[4]);
        chesslib::MoveType mType = chesslib::MoveType::Quite;
        if (moveStr.size() > 5) 
            mType = GetMoveType(moveStr.substr(6));

        return chesslib::Move(from, to, mType);
    }

    template<typename BoardType>
    static std::vector<chesslib::Move> GetMoves(std::string_view moveStr) 
    {
        std::vector<chesslib::Move> moves;

        std::string str{ moveStr };
        str.erase(std::remove_if(str.begin(), str.end(), [](char x) { return std::isspace(x); }), str.end());

        std::stringstream ss(str);
        std::string mv;
        while (std::getline(ss, mv, ','))
            moves.push_back(GetMove<BoardType>(mv));

        return moves;
    }

    static chesslib::MoveType GetMoveType(std::string_view mt) 
    {
        if (mt == "cm")
            return chesslib::MoveType::Capture;
        else if (mt == "ep")
            return chesslib::MoveType::En_Passant_Capture;
        else if (mt == "dp")
            return chesslib::MoveType::Double_Pawn_Push;
        else if (mt == "ksc")
            return chesslib::MoveType::King_Side_Castle;
        else if (mt == "qsc")
            return chesslib::MoveType::Queen_Side_Castle;
        else if (mt == "np")
            return chesslib::MoveType::Knight_Promotion;
        else if (mt == "bp")
            return chesslib::MoveType::Bishop_Promotion;
        else if (mt == "qp")
            return chesslib::MoveType::Queen_Promotion;
        else if (mt == "rp")
            return chesslib::MoveType::Rook_Promotion;
        else if (mt == "npc")
            return chesslib::MoveType::Knight_Promotion_Capture;
        else if (mt == "bpc")
            return chesslib::MoveType::Bishop_Promotion_Capture;
        else if (mt == "qpc")
            return chesslib::MoveType::Queen_Promotion_Capture;
        else if (mt == "rpc")
            return chesslib::MoveType::Rook_Promotion_Capture;
        
        throw std::logic_error("Move type error");
    }
};

class ScopedOpen 
{
public:
    ScopedOpen(std::string_view fName)
    {
        m_file.open(fName.data(), std::ios::in);
        if (!m_file.is_open())
            throw std::exception("File could not be opened.");
    }

    std::ifstream& GetFile() { return m_file; }

    ~ScopedOpen() 
    {
        if (m_file.is_open())
            m_file.close();
    }
private:
    std::ifstream m_file;
};
