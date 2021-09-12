#pragma once

#include <ChessLib/BasicBoard/BasicBoard.hpp>
#include <ChessLib/BasicBoard/Square.hpp>

#include <ChessLib/X88Board/x88Board.hpp>
#include <ChessLib/X88Board/Square.hpp>

namespace chesslib::traits 
{
	template<typename T>
	struct board_traits { };

	template <>
	struct board_traits<basic_board::BasicBoard>
	{
		static constexpr bool IsBasicBoard = true;

		static constexpr Square TopToBottom(Square s) { return basic_board::top_to_bottom_order[s]; }

		static constexpr Square BottomToTop(Square s) { return basic_board::bottom_to_top_order[s]; }
		
		static constexpr std::pair<char, char> ToCharPair(Square s) { return basic_board::get_chars(s); }
	};

	template <>
	struct board_traits<x88board::x88Board>
	{
		static constexpr bool IsBasicBoard = false;
		
		static constexpr Square TopToBottom(Square s) { return x88board::top_to_bottom_order[s]; }

		static constexpr Square BottomToTop(Square s) { return x88board::bottom_to_top_order[s]; }
		
		static constexpr std::pair<char, char> ToCharPair(Square s) { return x88board::get_chars(s); }
	};
}