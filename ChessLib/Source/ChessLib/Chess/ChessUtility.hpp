#pragma once

#include <string>

#include <ChessLib/Chess/TypeTraits.hpp>

namespace chesslib::utility::chess 
{
	// GetBoard()
	// GetActiveColor()
	// IsCastlingAvailable()
	// QueryCastling(Castling)
	// GetEnPassantSquare()
	// GetHalfMoveClock()
	// GetFullMoveClock()
	template<typename Board>
	std::string board_to_fen(Board const& brd)
	{
		auto b = brd.GetBoard();

		int empty_count{ 0 };
		std::stringstream ss;

		Index i{ 0 };
		for (Rank r = 0; r < 8; r++)
		{
			for (File f = 0; f < 8; f++)
			{
				auto idx = traits::board_traits<Board>::TopToBottom(i++);

				if (b[idx] == squareset::Empty)
					empty_count++;
				else
				{
					if (empty_count != 0)
					{
						ss << empty_count;
						empty_count = 0;
					}
					auto it = piece_to_char.find(b[idx]);
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
		if (ep == squareset::Empty) ss << '-';
		else
		{
			auto c = traits::board_traits<Board>::ToCharPair(ep);
			ss << c.first << c.second;
		}

		ss << ' ' << brd.GetHalfMoveClock() << ' ' << brd.GetFullMoveClock();

		return ss.str();
	}
}