#pragma once

#include <string>
#include <sstream>

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/Chess/TypeTraits.hpp>
#include <ChessLib/Chess/BoardBase.hpp>

namespace chesslib::utility::chess 
{
	// GetBoard()
	// GetWhitePieces()
	// GetBlackPieces()
	// SetActiveColor()
	// SetCastlingRights()
	// SetEnPassantSquare()
	// SetHalfMoveClock()
	// SetFullMoveClock()
	template<typename Board>
	void set_board(Board& brd, std::string_view fen) 
	{
		auto flattened_fields = Fen::GetFlattenedFields(fen);
		if (flattened_fields.size() != 13 && flattened_fields.size() != 11)
			throw std::logic_error("Fen parse error - field error.");

		auto& b = brd.GetBoard();
		auto& wp = brd.GetWhitePieces();
		auto& bp = brd.GetBlackPieces();

		Index idx{ 0 }, mapped_idx{ 0 };
		for (Rank r = 0; r < 8; r++) 
		{
			for (char c : flattened_fields[r]) 
			{
				if (std::isdigit(c))
				{
					if constexpr (traits::board_traits<Board>::IsObjBoard) 
					{
						idx += (c - '0');
					}
					else 
					{
						for (Index i{ 0 }; i < c - '0'; i++)
						{
							mapped_idx = traits::board_traits<Board>::BottomToTop(idx);	
							b[mapped_idx] = squareset::Empty;
							idx++;
						}
					}
				}
				else
				{
					Piece p = char_to_piece.at(c);
					if constexpr (traits::board_traits<Board>::IsObjBoard) 
					{
						auto pobj = objboard::make_shared_piece(p, idx);
						b[idx]._piece = pobj;

						if (color::get_color(p) == color::White) wp.emplace(p, std::move(pobj));
						else                                     bp.emplace(p, std::move(pobj));
					}
					else 
					{
						mapped_idx = traits::board_traits<Board>::BottomToTop(idx);
						if (color::get_color(p) == color::White) wp.emplace(p, mapped_idx);
						else                                     bp.emplace(p, mapped_idx);
						b[mapped_idx] = p;
					}

					idx++;
				}
			}
		}

		brd.SetActiveColor(flattened_fields[8][0]);
		
		brd.SetCastlingRights(flattened_fields[9]);

		if (flattened_fields[10] != "-") 
			brd.SetEnPassantSquare(traits::board_traits<Board>::GetSquareFromChars(flattened_fields[10][0], flattened_fields[10][1]));
			
		if (flattened_fields.size() == 13)
		{
			brd.SetHalfMoveClock(flattened_fields[11]);
			brd.SetFullMoveClock(flattened_fields[12]);
		}
	}

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
		const auto& b = brd.GetBoard();

		int empty_count{ 0 };
		std::stringstream ss;

		Index i{ 0 };
		for (Rank r = 0; r < 8; r++)
		{
			for (File f = 0; f < 8; f++)
			{
				auto idx = traits::board_traits<Board>::TopToBottom(i++);
				
				bool is_empty{ false };
				Piece p{ pieceset::None };
				if constexpr (traits::board_traits<Board>::IsObjBoard) 
				{
					if (b[idx]._piece) 
						p = b[idx]._piece->_code;
					else
						is_empty = true;
				}
				else 
				{
					if (b[idx] != squareset::Empty) 
						p = b[idx];
					else                     
						is_empty = true;
				}
				
				if (is_empty) 
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
		if (ep == squareset::None) ss << '-';
		else
		{
			auto c = traits::board_traits<Board>::ToCharPair(ep);
			ss << c.first << c.second;
		}

		ss << ' ' << brd.GetHalfMoveClock() << ' ' << brd.GetFullMoveClock();

		return ss.str();
	}	


	template<typename Board>
	std::string to_string(const Move& move) 
	{
		using btraits = traits::board_traits<Board>;
		std::string move_string{ "__-__" };
		std::tie(move_string[0], move_string[1]) = btraits::ToCharPair(move.GetFrom());
		std::tie(move_string[3], move_string[4]) = btraits::ToCharPair(move.GetTo());
		return move_string;
	}

	template<typename Board>
	std::string to_string(const utility::IterableStack<BoardBase::State>& move_stack) 
	{
		auto [first, last] = move_stack.bottom_to_top();
		std::stringstream ss{""};
		if (first != last)
			ss << to_string<Board>(first->move);
		for (first++; first != last; first++) 
			ss << ", " << to_string<Board>(first->move);
		return ss.str();
	}
}