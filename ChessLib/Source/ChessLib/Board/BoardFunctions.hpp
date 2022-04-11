#pragma once

#include <string>
#include <sstream>

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Chess/ColorTraits.hpp>
#include <ChessLib/Chess/BoardColorTraits.hpp>
#include <ChessLib/Board/BasicBoard.hpp>
#include <ChessLib/Board/x88Board.hpp>
#include <ChessLib/Board/ObjBoard.hpp>
#include <ChessLib/Board/BitBoard.hpp>

namespace chesslib
{
	// Preconditions: brd is cleared or newly created. 
	template<typename BoardType>
	void set_board(BoardType& brd, std::string_view fen)
	{
		auto flattened_fields = Fen::GetFlattenedFields(fen);
		if (flattened_fields.size() != 13 && flattened_fields.size() != 11)
			throw std::logic_error("Fen parse error - field error.");

		Square sq{ 0 };
		for (Rank r = 0; r < 8; r++)
		{
			for (char c : flattened_fields[r])
			{
				if (std::isdigit(c))
					sq += (c - '0');
				else
				{
					if constexpr (std::is_same_v<BoardType, x88Board>)
						brd.SetPiece(char_to_piece.at(c), x88Board::BottomToTopOrder[sq++]);
					else 
						brd.SetPiece(char_to_piece.at(c), sq++);
				}
			}
		}

		brd.SetActiveColor(get_color_from_char(flattened_fields[8][0]));

		set_castling_rights(brd, flattened_fields[9]);

		if (flattened_fields[10] != "-")
			brd.SetEnPassantSquare(BoardType::GetSquareFromChars(flattened_fields[10][0], flattened_fields[10][1]));

		if (flattened_fields.size() == 13)
		{
			set_half_move_clock(brd, flattened_fields[11]);
			set_full_move_clock(brd, flattened_fields[12]);
		}
	}

	template<typename BoardType>
	std::string board_to_fen(BoardType const& brd)
	{
		int empty_count{ 0 };
		std::stringstream ss;

		Index idx{ 0 };
		for (Rank r = 0; r < 8; r++)
		{
			for (File f = 0; f < 8; f++)
			{
				auto p = brd.GetPiece(BoardType::TopToBottomOrder[idx++]);
				if (p == Empty) 
				{
					empty_count++;
				}
				else 
				{
					if (empty_count != 0)
					{
						ss << empty_count;
						empty_count = 0;
					}
					auto it = piece_to_char.find(p);
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
		if (ep == Empty) ss << '-';
		else
		{
			auto c = typename BoardType::GetCharPair(ep);
			ss << c.first << c.second;
		}

		ss << ' ' << brd.GetHalfMoveClock() << ' ' << brd.GetFullMoveClock();

		return ss.str();
	}	

	template<typename BoardType, Color SideToMove>
	void make_move(BoardType const& brd, const Move& move)
	{
		using ctraits   = traits::color_traits<SideToMove>;
		using octraits  = traits::color_traits<ctraits::Opposite>;
		using bctraits  = traits::board_color_traits<BoardType, SideToMove>;
		using boctraits = traits::board_color_traits<BoardType, ctraits::Opposite>;
		
		Square from{ move.GetFrom() }, to{ move.GetTo() };
		Piece movingPiece{ brd.GetPiece(from) };
		
		Piece captured{ Empty };
		if (move.IsCapture())
			captured = move.IsEnPassantCapture() ? octraits::Pawn : brd.GetPiece(to);

		// push move to the stack		
		brd.PushToMoveStack(move, captured);

		// update half move clock
		if (movingPiece == ctraits::Pawn || captured != Empty)
			brd.ClearHalfMoveClock();
		else
			brd.IncrementHalfMoveClock();

		// update full move clock 
		if constexpr (SideToMove == color::Black)
			brd.IncrementFullMoveClock();

		// update castling rights
		if (brd.IsCastlingAvailable())
		{
			if (movingPiece == ctraits::King)
			{
				brd.SetCastling(ctraits::KingSideCastling, false);
				brd.SetCastling(ctraits::QueenSideCastling, false);
			}
			else if (movingPiece == ctraits::Rook)
			{
				if (from == bctraits::KingSideRookInitialPosition)
					brd.SetCastling(ctraits::KingSideCastling, false);
				else if (from == bctraits::QueenSideRookInitialPosition)
					brd.SetCastling(ctraits::QueenSideCastling, false);
			}

			if (captured == octraits::Rook)
			{
				if (to == boctraits::KingSideRookInitialPosition)
					SetCastling(octraits::KingSideCastling, false);
				else if (to == boctraits::QueenSideRookInitialPosition)
					SetCastling(octraits::QueenSideCastling, false);
			}
		}

		// update piece locations
		MoveType mtype{ move.GetMoveType() };

		if (move.IsPromotion())
		{
			brd.MakePromotionMove<SideToMove>(from, to, captured, get_promoted_piece<SideToMove>(mtype));
		}
		else if (move.IsCapture())
		{
			if (mtype == MoveType::En_Passant_Capture)
			{
				
			}

			/*
			if (mtype == MoveType::En_Passant_Capture)
			{
				Square removed_pawn_pos{ _enpassant_target + bptraits::ReverseMoveDirection };
				brd.RemovePiece<ctraits::Opposite>(captured, removed_pawn_pos);
				// _board[removed_pawn_pos] = Empty;
			}
			else
				RemovePiece<ctraits::Opposite>(captured, to);

			brd.MakeQuiteMove<SideToMove>(from, to);
			*/
		}
		else 
		{
			// Move types: quite, double pawn push, castling 
			brd.MakeQuiteMove<SideToMove>(from, to);

			if (mtype == MoveType::King_Side_Castle)
				brd.MakeQuiteMove<SideToMove>(bctraits::KingSideRookInitialPosition, bctraits::KingSideRookPositionAfterCastling);
			else if (mtype == MoveType::Queen_Side_Castle)
				brd.MakeQuiteMove<SideToMove>(bctraits::QueenSideRookInitialPosition, bctraits::QueenSideRookPositionAfterCastling);
		}

		// update enpassant target square
		/*auto enpassant_target_square = mtype == MoveType::Double_Pawn_Push ? from + bptraits::MoveDirection : Empty;
		brd.SetEnPassantSquare(enpassant_target_square);*/

		// update side to move
		brd.SetActiveColor(ctraits::Opposite);
	}

	// GetCharPair()
	template<typename BoardType, typename MoveType>
	std::string to_string(const MoveType& move)
	{
		std::string move_string{ "__-__" };
		std::tie(move_string[0], move_string[1]) = typename BoardType::GetCharPair(move.GetFrom());
		std::tie(move_string[3], move_string[4]) = typename BoardType::GetCharPair(move.GetTo());
		return move_string;
	}

	// GetCharPair()
	template<typename BoardType, typename MV>
	std::string to_string_long(const MV& move)
	{
		std::string move_string{ "__-__ " };
		move_string += to_string(move.GetMoveType());
		std::tie(move_string[0], move_string[1]) = typename BoardType::GetCharPair(move.GetFrom());
		std::tie(move_string[3], move_string[4]) = typename BoardType::GetCharPair(move.GetTo());
		return move_string;
	}

	template<typename BoardType>
	std::string to_string(const utility::IterableStack<BoardState>& move_stack) 
	{
		auto [first, last] = move_stack.bottom_to_top();
		std::stringstream ss{""};
		if (first != last)
			ss << to_string<BoardType>(first->move);
		for (first++; first != last; first++) 
			ss << ", " << to_string<BoardType>(first->move);
		return ss.str();
	}
}