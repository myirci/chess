#pragma once

#include <string_view>
#include <regex>
#include <optional>

#include <ChessLib/Chess/Definitions.hpp>

namespace chesslib::utility 
{
	namespace regex 
	{
		using svit = std::string_view::iterator;
		using svcit = std::string_view::const_iterator;
		using csvregex_token_it = std::regex_token_iterator<svcit>;

		const std::regex not_white_space("[^\\s]+");
		const std::regex not_forward_slash("[^/]+");

		std::pair<csvregex_token_it, csvregex_token_it> parse_string(std::string_view str, const std::regex& pattern);
	}

	namespace numeric 
	{
		std::optional<int> to_int(std::string_view sv);
	}

	namespace chess 
	{
		/*
		template<typename Board, typename IndexTransformer>
		std::string board_to_fen(Board const& board, IndexTransformer tr) 
		{
			int count_empty{ 0 };
			std::stringstream ss;

			auto b = board.GetBoard();

			for (int i{ 0 }; i < 64; i++)
			{
				if (b[tr(i)] == squareset::Empty)
					count_empty++;
				else
				{
					if (count_empty != 0)
					{
						ss << count_empty;
						count_empty = 0;
					}
					auto it = piece_to_char.find(b[tr(i)]);
					ss << it->second;
				}

				if ((i + 1) % 8 == 0)
				{
					if (count_empty != 0)
					{
						ss << count_empty;
						count_empty = 0;
					}

					if (i != 63) ss << "/";
				}
			}

			auto ac = board.GetActiveColor() == color::White ? " w " : " b ";
			ss << ac;

			std::string castling{ "" };
			if (board.QueryCastling(Castling::WHITE_KS)) castling.append("K");
			if (board.QueryCastling(Castling::WHITE_QS)) castling.append("Q");
			if (board.QueryCastling(Castling::BLACK_KS)) castling.append("k");
			if (board.QueryCastling(Castling::BLACK_QS)) castling.append("q");
			if (castling == "") ss << "- ";
			else ss << castling;

			auto ep = board.GetEnPassantSquare();
			if (ep == squareset::Empty) ss << " -";
			// else
			
			ss << " " << board.GetHalfMoveClock() << " " << b.GetFullMoveClock();

			return ss.str();
		}
		*/
	}
}