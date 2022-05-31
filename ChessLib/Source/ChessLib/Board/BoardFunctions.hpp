#pragma once

#include <string>
#include <sstream>
#include <type_traits>

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Chess/ColorTraits.hpp>
#include <ChessLib/Chess/BoardColorTraits.hpp>
#include <ChessLib/Board/BasicBoard.hpp>

namespace chesslib
{
	class x88Board;

	namespace impl
	{
		template<typename BoardType, Color SideToMove>
		void make_move(BoardType& brd, const Move& move)
		{
			using ctraits = traits::color_traits<SideToMove>;
			using octraits = traits::color_traits<ctraits::Opposite>;
			using bctraits = traits::board_color_traits<BoardType, SideToMove>;
			using boctraits = traits::board_color_traits<BoardType, ctraits::Opposite>;

			// update full move clock 
			if constexpr (SideToMove == color::Black)
				brd.IncrementFullMoveClock();

			Square from{ move.GetFrom() }, to{ move.GetTo() }, capturedPieceLoc{ Empty };
			Piece captured{ Empty };

			// captured piece, captured piece loc and removed captured piece from the board
			if (move.IsCapture())
			{
				if (move.IsEnPassantCapture())
				{
					captured = octraits::Pawn;
					capturedPieceLoc = brd.GetEnPassantSquare() + bctraits::PawnReverseMoveDirection;
				}
				else
				{
					captured = brd.GetPiece(to);
					capturedPieceLoc = to;
				}

				brd.RemovePiece<ctraits::Opposite>(captured, capturedPieceLoc);
			}

			// push move to the stack		
			brd.PushToMoveStack(move, captured);

			// update half move clock
			Piece movingPiece{ brd.GetPiece(from) };
			if (movingPiece == ctraits::Pawn || captured != Empty)
				brd.ClearHalfMoveClock();
			else
				brd.IncrementHalfMoveClock();

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
						brd.SetCastling(octraits::KingSideCastling, false);
					else if (to == boctraits::QueenSideRookInitialPosition)
						brd.SetCastling(octraits::QueenSideCastling, false);
				}
			}

			// update board
			MoveType mtype{ move.GetMoveType() };
			if (move.IsPromotion())
			{
				brd.RemovePiece<SideToMove>(movingPiece, from);
				brd.PutPiece<SideToMove>(get_promoted_piece<SideToMove>(mtype), to);
			}
			else
			{
				brd.UpdatePiece<SideToMove>(movingPiece, from, to);

				if (mtype == MoveType::King_Side_Castle)
					brd.UpdatePiece<SideToMove>(ctraits::Rook, bctraits::KingSideRookInitialPosition, bctraits::KingSideRookPositionAfterCastling);
				else if (mtype == MoveType::Queen_Side_Castle)
					brd.UpdatePiece<SideToMove>(ctraits::Rook, bctraits::QueenSideRookInitialPosition, bctraits::QueenSideRookPositionAfterCastling);
			}

			// update enpassant target square
			auto epts = mtype == MoveType::Double_Pawn_Push ? from + bctraits::PawnMoveDirection : Empty;
			brd.SetEnPassantSquare(epts);

			// update side to move
			brd.SetActiveColor(ctraits::Opposite);
		}

		template<typename BoardType, Color SideToMove>
		void unmake_move(BoardType& brd)
		{
			using ctraits = traits::color_traits<SideToMove>;
			using octraits = traits::color_traits<ctraits::Opposite>;
			using boctraits = traits::board_color_traits<BoardType, ctraits::Opposite>;

			// Get the last move and board state before the move was made.
			const auto& statePrev = brd.GetMoveStackTop();

			// restore side to move
			brd.SetActiveColor(ctraits::Opposite);

			// restore castling rights, en-passant location and half move clock
			brd.SetCastlingRights(statePrev.castling_rights);
			brd.SetEnPassantSquare(statePrev.enpassant_target);
			brd.SetHalfMoveClock(statePrev.halfmove_clock);

			// restore the full move clock
			if constexpr (SideToMove == color::White)
				brd.DecrementFullMoveClock();

			// restore the board and piece positions
			Move const& move = statePrev.move;
			Square from{ move.GetFrom() }, to{ move.GetTo() };
			Piece movedPiece{ brd.GetPiece(to) };
			MoveType mtype{ move.GetMoveType() };

			if (move.IsPromotion())
			{
				brd.PutPiece<ctraits::Opposite>(octraits::Pawn, from);
				brd.RemovePiece<ctraits::Opposite>(movedPiece, to);
			}
			else
			{
				brd.UpdatePiece<ctraits::Opposite>(movedPiece, to, from);

				if (mtype == MoveType::King_Side_Castle)
					brd.UpdatePiece<ctraits::Opposite>(octraits::Rook, boctraits::KingSideRookPositionAfterCastling, boctraits::KingSideRookInitialPosition);
				else if (mtype == MoveType::Queen_Side_Castle)
					brd.UpdatePiece<ctraits::Opposite>(octraits::Rook, boctraits::QueenSideRookPositionAfterCastling, boctraits::QueenSideRookInitialPosition);
			}

			if (statePrev.captured_piece != Empty)
			{
				Square capturedPieceLoc = mtype == MoveType::En_Passant_Capture
					? statePrev.enpassant_target + boctraits::PawnReverseMoveDirection
					: to;

				brd.PutPiece<SideToMove>(statePrev.captured_piece, capturedPieceLoc);
			}

			brd.PopFromMoveStack();
		}

	}

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
					Piece p = char_to_piece.at(c);
					Color pieceColor = color::get_color(p);
					if (pieceColor == color::White) 
					{
						if constexpr (std::is_same_v<BoardType, x88Board>)
							brd.PutPiece<color::White>(p, x88Board::BottomToTopOrder[sq++]);
						else
							brd.PutPiece<color::White>(p, sq++);
					}
					else 
					{
						if constexpr (std::is_same_v<BoardType, x88Board>)
							brd.PutPiece<color::Black>(p, x88Board::BottomToTopOrder[sq++]);
						else
							brd.PutPiece<color::Black>(p, sq++);
					}
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

	template<typename BoardType>
	void make_move(BoardType& brd, const Move& move)
	{
		auto sideToMove = brd.GetActiveColor();
		if (sideToMove == color::White)
			impl::make_move<BoardType, color::White>(brd, move);
		else
			impl::make_move<BoardType, color::Black>(brd, move);
	}

	template<typename BoardType>
	void unmake_move(BoardType& brd)
	{
		auto sideToMove = brd.GetActiveColor();
		if (sideToMove == color::White)
			impl::unmake_move<BoardType, color::White>(brd);
		else
			impl::unmake_move<BoardType, color::Black>(brd);
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

	template<typename BoardType>
	inline bool IsInside(Square to, Square from = Empty)
	{
		if constexpr (std::is_same_v<BoardType, BasicBoard>)
			return BasicBoard::IsInside(from, to);
		else
			return typename BoardType::IsInside(to);
	}
}