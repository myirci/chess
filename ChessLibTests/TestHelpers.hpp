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
