
#include <ChessLib/8x8Board/Board.hpp>
#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/Fen.hpp>

#include <cctype>

namespace chesslib::basic_board
{
	Board::Board(std::string_view fen) : 
		board{ { } },
		active_color{ 0 },
		castling_rights{ 0 },
		en_passant_target{ squareset::Empty },
		half_move_clock{ 0 },
		full_move_clock{ 1 }
	{
		auto flattened_fields = Fen::GetFlattenedFields(fen);
		if (flattened_fields.size() != 13 && flattened_fields.size() != 11)
			throw std::logic_error("Fen parse error - field error.");

		// piece placement
		int idx = 0;
		for(int rank_idx = 0; rank_idx < 8; rank_idx++)
		{
			for (char c : flattened_fields[rank_idx]) 
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
		}

		// Active color
		active_color = flattened_fields[8][0] == charset::White ? color::White : color::Black;
		
		// Castling rights
		if (flattened_fields[9] != "-") 
		{
			for (char c : flattened_fields[9]) 
			{
				switch (c) 
				{
					case charset::WhiteKing	: SetCastling(Castling::WHITE_KS, true); break;
					case charset::WhiteQueen: SetCastling(Castling::WHITE_QS, true); break;
					case charset::BlackKing	: SetCastling(Castling::BLACK_KS, true); break;
					case charset::BlackQueen: SetCastling(Castling::BLACK_QS, true); break;
					default:
						throw std::logic_error("Fen parse error - invalid castling rights.");
				}
			}
		}

		// En passant
		if (flattened_fields[10] != "-") 
		{

		}

		if (flattened_fields.size() == 13) 
		{
			auto hmc = numeric::to_int(flattened_fields[11]);
			if (!hmc.has_value())
				throw std::logic_error("Fen parse error - invalid half move clock.");
			half_move_clock = static_cast<uint16_t>(hmc.value());

			auto fmc = numeric::to_int(flattened_fields[12]);
			if (!fmc.has_value())
				throw std::logic_error("Fen parse error - invalid full move clock.");
			full_move_clock = static_cast<uint16_t>(fmc.value());
		}
	}

	const Board::BoardArray& Board::GetBoard() const { return board; }
	
	const Board::PieceMap& Board::GetWhitePieces() const { return white_pieces; }
	
	const Board::PieceMap& Board::GetBlackPieces() const { return black_pieces; }

	Color Board::GetActiveColor() const { return active_color; }
	
	Square Board::GetEnPassantSquare() const { return en_passant_target; };
	
	uint16_t Board::GetHalfMoveClock() const { return half_move_clock; };
	
	uint16_t Board::GetFullMoveClock() const { return full_move_clock; };

	bool Board::QueryCastling(Castling c) const { return castling_rights & static_cast<int8_t>(c); }

	void Board::SetCastling(Castling c, bool flag) 
	{ 
		castling_rights = flag 
			? castling_rights | static_cast<int8_t>(c) 
			: castling_rights & ~static_cast<int8_t>(c); 
	}
}