#pragma once

#include <ChessLib/BasicBoard/BasicBoard.hpp>
#include <ChessLib/BasicBoard/Square.hpp>

#include <ChessLib/X88Board/x88Board.hpp>
#include <ChessLib/X88Board/Square.hpp>

namespace chesslib::traits 
{
	template<typename T>
	struct board_traits {};

	template <>
	struct board_traits<basic_board::BasicBoard>
	{
		static Square TopToBottom(Square s) { return basic_board::top_to_bottom_order[s]; }
		static std::pair<char, char> ToCharPair(Square s) { return basic_board::get_chars(s); }
	};

	template <>
	struct board_traits<x88board::Board>
	{
		static Square TopToBottom(Square s) { return x88board::top_to_bottom_order[s]; }
		static std::pair<char, char> ToCharPair(Square s) { return x88board::get_chars(s); }
	};
}