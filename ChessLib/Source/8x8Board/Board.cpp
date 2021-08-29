#include "Board.hpp"

#include <Chess/Definitions.hpp>
#include <Chess/FenHelper.hpp>

#include <cctype>

namespace chesslib
{
	Board::Board(std::string_view fen) : board{ { } }
	{
		int start_idx{ 56 }, end_idx{ 64 };
		auto it = fen.begin();
		while (start_idx >= 0)
		{
			int i = start_idx;

			while (*it != '/' && *it != ' ')
			{
				if (std::isdigit(*it)) 
				{
					for (int j = 0; j < static_cast<int>(*it - '0'); j++)
						board[i++] = squareset::Empty;
				}
				else 
				{
					Piece p = char_to_piece.at(*it);
					board[i] = p;
					if (color::get_color(p) == color::Color::White)
						white_pieces.emplace(p, i);
					else
						black_pieces.emplace(p, i);
					i++;
				}
				it++;
			}

			it++;
			start_idx -= 8;
			end_idx -= 8;
		}
	}

	const Board::BoardArray& Board::GetBoard() const { return board; }
	const Board::PieceMap& Board::GetWhitePieces() const { return white_pieces; }
	const Board::PieceMap& Board::GetBlackPieces() const { return black_pieces; }
}