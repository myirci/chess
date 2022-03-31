
#include <ChessLib/Chess/ChessUtility.hpp>

namespace chesslib::utility::chess
{

	namespace fen
	{
		Color GetColorFromChar(char c)
		{
			if (c == charset::White)
				return color::White;

			if (c == charset::Black)
				return color::Black;

			throw std::logic_error("Color char error.");
		}
	}

	template<>
	void set_board(bitboard::BitBoard& brd, std::string_view fen) 
	{
		auto flattened_fields = Fen::GetFlattenedFields(fen);
		if (flattened_fields.size() != 13 && flattened_fields.size() != 11)
			throw std::logic_error("Fen parse error - field error.");

		Index idx{ 0 }, mapped_square{ 0 };
		for (Rank r = 0; r < 8; r++)
		{
			for (char c : flattened_fields[r])
			{
				if (std::isdigit(c))
					for (Index i{ 0 }; i < c - '0'; i++)
						brd.SetSquare(bitboard::BitBoard::BottomToTopOrder[idx++], pieceset::None);
				else
					brd.SetSquare(bitboard::BitBoard::BottomToTopOrder[idx++], char_to_piece.at(c));
			}
		}

		brd.SetActiveColor(fen::GetColorFromChar(flattened_fields[8][0]));

		fen::SetCastlingRights(brd, flattened_fields[9]);

		if (flattened_fields[10] != "-")
			brd.SetEnPassantSquare(bitboard::BitBoard::GetSquareFromChars(flattened_fields[10][0], flattened_fields[10][1]));

		if (flattened_fields.size() == 13)
		{
			fen::SetHalfMoveClock(brd, flattened_fields[11]);
			fen::SetFullMoveClock(brd, flattened_fields[12]);
		}
	}

	std::string_view to_string(MoveType mtype) 
	{
		const static std::unordered_map<MoveType, std::string_view> mt_str
		{
			{ MoveType::Quite, "Quite" },
			{ MoveType::Capture, "Capture" },
			{ MoveType::Double_Pawn_Push, "Double Pawn Push" },
			{ MoveType::Bishop_Promotion, "Bishop Promotion" },
			{ MoveType::Bishop_Promotion_Capture, "Bishop Promotion Capture" },
			{ MoveType::En_Passant_Capture, "En-passant Capture" },
			{ MoveType::King_Side_Castle, "King Side Castle" },
			{ MoveType::Knight_Promotion, "Knight Promotion" },
			{ MoveType::Knight_Promotion_Capture, "Knight Promotion Capture" },
			{ MoveType::Queen_Promotion, "Queen Promotion" },
			{ MoveType::Queen_Promotion_Capture, "Queen Promotion Capture" },
			{ MoveType::Queen_Side_Castle, "Queen Side Castle" },
			{ MoveType::Rook_Promotion, "Rook Promotion" },
			{ MoveType::Rook_Promotion_Capture, "Rook Promotion Capture" }
		};

		return mt_str.at(mtype);
	}
}