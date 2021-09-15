#include "BoardBase.hpp"

#include <ChessLib/Chess/Utility.hpp>

#include <stdexcept>

namespace chesslib
{
	BoardBase::BoardBase() :
		active_color{ 0 },
		castling_rights{ 0 },
		en_passant_target{ squareset::Empty },
		half_move_clock{ 0 },
		full_move_clock{ 1 }	
	{ }

	Color BoardBase::GetActiveColor() const { return active_color; }

	Square BoardBase::GetEnPassantSquare() const { return en_passant_target; };

	uint16_t BoardBase::GetHalfMoveClock() const { return half_move_clock; };

	uint16_t BoardBase::GetFullMoveClock() const { return full_move_clock; };

	bool BoardBase::IsCastlingAvailable() const { return castling_rights != 0; }

	bool BoardBase::QueryCastling(Castling c) const { return castling_rights & static_cast<int8_t>(c); }

	void BoardBase::SetCastling(Castling c, bool flag)
	{
		castling_rights = flag
			? castling_rights | static_cast<int8_t>(c)
			: castling_rights & ~static_cast<int8_t>(c);
	}

	void BoardBase::SetActiveColor(char side_to_move) 
	{
		active_color = side_to_move == charset::White ? color::White : color::Black;
	}

	void BoardBase::SetCastlingRights(std::string_view castling_availability)
	{
		if (castling_availability != "-")
		{
			for (char c : castling_availability)
			{
				switch (c)
				{
					case charset::WhiteKing: SetCastling(Castling::WHITE_KS, true); break;
					case charset::WhiteQueen: SetCastling(Castling::WHITE_QS, true); break;
					case charset::BlackKing: SetCastling(Castling::BLACK_KS, true); break;
					case charset::BlackQueen: SetCastling(Castling::BLACK_QS, true); break;
					default:
						throw std::logic_error("Fen parse error - invalid castling rights.");
				}
			}
		}
	}

	void BoardBase::SetEnPassantSquare(Square ep) { en_passant_target = ep; }

	void BoardBase::SetHalfMoveClock(std::string_view hmc) 
	{
		auto h = utility::numeric::to_int(hmc);
		if (!h.has_value())
			throw std::logic_error("Fen parse error - invalid half move clock.");
		half_move_clock = static_cast<uint16_t>(h.value());
	}

	void BoardBase::SetFullMoveClock(std::string_view fmc) 
	{
		auto f = utility::numeric::to_int(fmc);
		if (!f.has_value())
			throw std::logic_error("Fen parse error - invalid full move clock.");
		full_move_clock = static_cast<uint16_t>(f.value());
	}

	BoardBaseWithPieces::BoardBaseWithPieces() { }

	const BoardBaseWithPieces::PieceMap& BoardBaseWithPieces::GetWhitePieces() const { return white_pieces; }

	BoardBaseWithPieces::PieceMap& BoardBaseWithPieces::GetWhitePieces() { return white_pieces; }

	const BoardBaseWithPieces::PieceMap& BoardBaseWithPieces::GetBlackPieces() const { return black_pieces; }

	BoardBaseWithPieces::PieceMap& BoardBaseWithPieces::GetBlackPieces() { return black_pieces; }
}