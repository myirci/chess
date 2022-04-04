#pragma once

#include <string>
#include <sstream>

#include <ChessLib/Utility/Utility.hpp>
#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/Board/BasicBoard.hpp>
#include <ChessLib/Board/x88Board.hpp>
#include <ChessLib/Board/ObjBoard.hpp>
#include <ChessLib/Board/BitBoard.hpp>

namespace chesslib::utility::chess
{
	std::string_view to_string(MoveType mtype);

	namespace fen
	{
		Color GetColorFromChar(char c);
		
		template<typename BoardType>
		void SetCastlingRights(BoardType& brd, std::string_view castling_availability)
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
		void SetHalfMoveClock(BoardType& brd, std::string_view hmc)
		{
			auto h = utility::numeric::to_int(hmc);
			if (!h.has_value())
				throw std::logic_error("Fen parse error - invalid half move clock.");
			brd.SetHalfMoveClock(static_cast<uint16_t>(h.value()));
		}

		template<typename BoardType>
		void SetFullMoveClock(BoardType& brd, std::string_view fmc)
		{
			auto f = utility::numeric::to_int(fmc);
			if (!f.has_value())
				throw std::logic_error("Fen parse error - invalid full move clock.");
			brd.SetFullMoveClock(static_cast<uint16_t>(f.value()));
		}
	}

	// Preconditions: brd is cleared or newly created. 
	template<typename BoardType>
	void set_board(BoardType& brd, std::string_view fen)
	{
		auto flattened_fields = Fen::GetFlattenedFields(fen);
		if (flattened_fields.size() != 13 && flattened_fields.size() != 11)
			throw std::logic_error("Fen parse error - field error.");

		Square sq{ 0 };
		for (Rank r = 0; r < 8; r++)
		{
			for (char c : flattened_fields[r])
			{
				if (std::isdigit(c))
					sq += (c - '0');
				else
				{
					if constexpr (std::is_same_v<BoardType, x88Board>)
						brd.SetPiece(char_to_piece.at(c), x88Board::BottomToTopOrder[sq++]);
					else 
						brd.SetPiece(char_to_piece.at(c), sq++);
				}
			}
		}

		brd.SetActiveColor(fen::GetColorFromChar(flattened_fields[8][0]));

		fen::SetCastlingRights(brd, flattened_fields[9]);

		if (flattened_fields[10] != "-")
			brd.SetEnPassantSquare(BoardType::GetSquareFromChars(flattened_fields[10][0], flattened_fields[10][1]));

		if (flattened_fields.size() == 13)
		{
			fen::SetHalfMoveClock(brd, flattened_fields[11]);
			fen::SetFullMoveClock(brd, flattened_fields[12]);
		}
	}

	template<typename BoardType>
	std::string board_to_fen(BoardType const& brd)
	{
		int empty_count{ 0 };
		std::stringstream ss;

		Index idx{ 0 };
		for (Rank r = 0; r < 8; r++)
		{
			for (File f = 0; f < 8; f++)
			{
				auto p = brd.GetPiece(BoardType::TopToBottomOrder[idx++]);
				if (p == Empty) 
				{
					empty_count++;
				}
				else 
				{
					if (empty_count != 0)
					{
						ss << empty_count;
						empty_count = 0;
					}
					auto it = piece_to_char.find(p);
					ss << it->second;
				}
			}

			if (empty_count != 0)
			{
				ss << empty_count;
				empty_count = 0;
			}

			if (r != 7) ss << '/';
		}

		auto ac = brd.GetActiveColor() == color::White ? charset::White : charset::Black;
		ss << ' ' << ac;

		ss << ' ';
		if (brd.IsCastlingAvailable())
		{
			if (brd.QueryCastling(Castling::WHITE_KS)) ss << charset::WhiteKing;
			if (brd.QueryCastling(Castling::WHITE_QS)) ss << charset::WhiteQueen;
			if (brd.QueryCastling(Castling::BLACK_KS)) ss << charset::BlackKing;
			if (brd.QueryCastling(Castling::BLACK_QS)) ss << charset::BlackQueen;
		}
		else ss << '-';

		ss << ' ';
		auto ep = brd.GetEnPassantSquare();
		if (ep == Empty) ss << '-';
		else
		{
			auto c = typename BoardType::GetCharPair(ep);
			ss << c.first << c.second;
		}

		ss << ' ' << brd.GetHalfMoveClock() << ' ' << brd.GetFullMoveClock();

		return ss.str();
	}	

	// GetCharPair()
	template<typename BoardType, typename MoveType>
	std::string to_string(const MoveType& move)
	{
		std::string move_string{ "__-__" };
		std::tie(move_string[0], move_string[1]) = typename BoardType::GetCharPair(move.GetFrom());
		std::tie(move_string[3], move_string[4]) = typename BoardType::GetCharPair(move.GetTo());
		return move_string;
	}

	// GetCharPair()
	template<typename BoardType, typename MV>
	std::string to_string_long(const MV& move)
	{
		std::string move_string{ "__-__ " };
		move_string += to_string(move.GetMoveType());
		std::tie(move_string[0], move_string[1]) = typename BoardType::GetCharPair(move.GetFrom());
		std::tie(move_string[3], move_string[4]) = typename BoardType::GetCharPair(move.GetTo());
		return move_string;
	}

	template<typename BoardType>
	std::string to_string(const utility::IterableStack<BoardState>& move_stack) 
	{
		auto [first, last] = move_stack.bottom_to_top();
		std::stringstream ss{""};
		if (first != last)
			ss << to_string<BoardType>(first->move);
		for (first++; first != last; first++) 
			ss << ", " << to_string<BoardType>(first->move);
		return ss.str();
	}
}