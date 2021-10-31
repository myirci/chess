
#include <ChessLib/Chess/ChessUtility.hpp>

namespace chesslib::utility::chess
{

	namespace fenhelpers
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