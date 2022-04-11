#pragma once

#include <ChessLib/Utility/Utility.hpp>

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
       
        std::string_view GetFen() const                          { return fen_str; }
        std::string_view GetPiecePlacement() const               { return fields[PiecePlacement]; }
        std::string_view GetActiveColor() const                  { return fields[ActiveColor]; }
        std::string_view GetCastlingAvailability() const         { return fields[CastlingAvailability]; }
        std::string_view GetEnPassantTargetSquare() const        { return fields[EnPassantTargetSquare]; }
        std::optional<std::string_view> GetHalfMoveClock() const { return fields.size() == MaxNumberOfFields ? std::optional<std::string_view>(fields[HalfMoveClock]) : std::nullopt; }
        std::optional<std::string_view> GetFullMoveClock() const { return fields.size() == MaxNumberOfFields ? std::optional<std::string_view>(fields[FullMoveClock]) : std::nullopt; }

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

        static bool IsValid(utility::regex::csvregex_token_it it, int num_fields);
        static bool ValidatePiecePlacement(std::string_view pp);
        static bool ValidateNumberOfPieces(std::unordered_map<char, int>& chars);
        inline static bool ValidateActiveColor(std::string_view ac);
        inline static bool ValidateCastlingRights(std::string_view cr);
        inline static bool ValidateEnPassantTargetSquare(std::string_view ep, char active_color);
        static bool ValidateHalfMoveClock(std::string_view hmc, std::string_view ep);
        static bool ValidateFullMoveClock(std::string_view fmc);
        inline static bool CheckRank(int rank, utility::regex::csvregex_token_it tit, std::unordered_map<char, int>& chars);

        void Init(utility::regex::csvregex_token_it first, utility::regex::csvregex_token_it last);

        std::string fen_str;
        std::vector<std::string_view> fields;
    };

    template<typename BoardType>
    void set_castling_rights(BoardType& brd, std::string_view castling_availability)
    {
        if (castling_availability != "-")
        {
            for (char c : castling_availability)
            {
                switch (c)
                {
                case charset::WhiteKing: brd.SetCastling(Castling::WHITE_KS, true); break;
                case charset::WhiteQueen: brd.SetCastling(Castling::WHITE_QS, true); break;
                case charset::BlackKing: brd.SetCastling(Castling::BLACK_KS, true); break;
                case charset::BlackQueen: brd.SetCastling(Castling::BLACK_QS, true); break;
                default:
                    throw std::logic_error("Fen parse error - invalid castling rights.");
                }
            }
        }
    }

    template<typename BoardType>
    void set_half_move_clock(BoardType& brd, std::string_view hmc)
    {
        auto h = utility::numeric::to_int(hmc);
        if (!h.has_value())
            throw std::logic_error("Fen parse error - invalid half move clock.");
        brd.SetHalfMoveClock(static_cast<uint16_t>(h.value()));
    }

    template<typename BoardType>
    void set_full_move_clock(BoardType& brd, std::string_view fmc)
    {
        auto f = utility::numeric::to_int(fmc);
        if (!f.has_value())
            throw std::logic_error("Fen parse error - invalid full move clock.");
        brd.SetFullMoveClock(static_cast<uint16_t>(f.value()));
    }

    Color get_color_from_char(char c);
}