#include "Move.hpp"

namespace chesslib 
{
	bool operator == (const Move m1, const Move m2) 
	{
		return
			m1.from == m2.from &&
			m1.to == m2.to &&
			m1.mtype == m2.mtype;
	}

	bool operator < (const Move m1, const Move m2) 
	{
		return m1.from < m2.from || m1.from == m2.from && m1.to < m2.to;
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