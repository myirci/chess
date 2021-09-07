
#include <ChessLib/BasicBoard/Board.hpp>
#include <ChessLib/BasicBoard/Square.hpp>
#include <ChessLib/Chess/Fen.hpp>

#include <cctype>

namespace chesslib::basic_board
{
	Board::Board(std::string_view fen) 
	{
		auto flattened_fields = Fen::GetFlattenedFields(fen);
		if (flattened_fields.size() != 13 && flattened_fields.size() != 11)
			throw std::logic_error("Fen parse error - field error.");

		auto last = flattened_fields.cbegin();
		std::advance(last, 8);
		SetBoard(flattened_fields.cbegin(), last);

		SetActiveColor(flattened_fields[8][0]);
		SetCastlingRights(flattened_fields[9]);

		if (flattened_fields[10] != "-")
			SetEnPassantSquare(get_square_from_chars(flattened_fields[10][0], flattened_fields[10][1]));
		
		if (flattened_fields.size() == 13) 
		{
			SetHalfMoveClock(flattened_fields[11]);
			SetFullMoveClock(flattened_fields[12]);
		}
	}

	const Board::BoardArray& Board::GetBoard() const { return board; }

	void Board::SetBoard(const_rank_iterator first, const_rank_iterator last)
	{
		int idx = 0;

		while (first != last) 
		{
			for (char c : *first)
			{
				if (std::isdigit(c))
				{
					for (int i = 0; i < static_cast<int>(c - '0'); i++)
						board[idx++] = squareset::Empty;
				}
				else
				{
					Piece p = char_to_piece.at(c);
					if (color::get_color(p) == color::White)
						white_pieces.emplace(p, idx);
					else
						black_pieces.emplace(p, idx);
					board[idx++] = p;
				}
			}
			first++;
		}
	}
}