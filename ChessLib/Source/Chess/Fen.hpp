#pragma once

#include <Chess/Utility.hpp>

#include <string>
#include <string_view>
#include <optional>
#include <unordered_set>
#include <unordered_map>

namespace chesslib 
{
    /*
    FEN: ForsythEdwards Notation
    Field-1: Piece Placement
    Field-2: Active Color
    Field-3: Castling Availability
    Field-4: En Passant
    Field-5: Half Move Clock
    Field-6: Full Move Nummber

    Fields are separated by space char
    */

    class Fen
    {
    public:

        const static std::unordered_set<char> ValidCharSet;

        static constexpr std::string_view StartingPosition{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };

        Fen(std::string_view fen_string = StartingPosition);
       
        std::string_view Get_Fen() const { return fen_str; }
        
        std::string_view Get_PiecePlacement() const { return fields[PiecePlacement]; }
        std::string_view Get_ActiveColor() const { return fields[ActiveColor]; }
        std::string_view Get_CastlingAvailability() const { return fields[CastlingAvailability]; }
        std::string_view Get_EnPassantTargetSquare() const { return fields[EnPassantTargetSquare]; }
        std::optional<std::string_view> Get_HalfMoveClock() const { return fields.size() == MaxNumberOfFields ? std::optional<std::string_view>(fields[HalfMoveClock]) : std::nullopt; }
        std::optional<std::string_view> Get_FullMoveClock() const { return fields.size() == MaxNumberOfFields ? std::optional<std::string_view>(fields[FullMoveClock]) : std::nullopt; }

        static std::vector<std::string_view> GetFlattenedFields(std::string_view fen);

    private:
        
        static constexpr char FieldSeparator{ ' ' };
        static constexpr char RankSeparator{ '/' };

        static constexpr uint8_t MinNumberOfFields = 4;
        static constexpr uint8_t MaxNumberOfFields = 6;

        static constexpr uint8_t PiecePlacement = 0;
        static constexpr uint8_t ActiveColor = 1;
        static constexpr uint8_t CastlingAvailability = 2;
        static constexpr uint8_t EnPassantTargetSquare = 3;
        static constexpr uint8_t HalfMoveClock = 4;
        static constexpr uint8_t FullMoveClock = 5;

        static bool IsValid(regex::csvregex_token_it it, int num_fields);
        static bool ValidatePiecePlacement(std::string_view pp);
        static bool ValidateNumberOfPieces(std::unordered_map<char, int>& chars);
        inline static bool ValidateActiveColor(std::string_view ac);
        inline static bool ValidateCastlingRights(std::string_view cr);
        inline static bool ValidateEnPassantTargetSquare(std::string_view ep, char active_color);
        static bool ValidateHalfMoveClock(std::string_view hmc, std::string_view ep);
        static bool ValidateFullMoveClock(std::string_view fmc);
        inline static bool CheckRank(int rank, regex::csvregex_token_it tit, std::unordered_map<char, int>& chars);

        void Init(regex::csvregex_token_it first, regex::csvregex_token_it last);

        std::string fen_str;
        std::vector<std::string_view> fields;
    };
}



