#pragma once

#include <ChessLib/BasicBoard/BasicBoard.hpp>
#include <ChessLib/BasicBoard/Square.hpp>

#include <ChessLib/X88Board/x88Board.hpp>
#include <ChessLib/X88Board/Square.hpp>

#include <ChessLib/ObjBoard/ObjBoard.hpp>

namespace chesslib::traits 
{
	template<typename Board>
	struct board_traits { };

	template <>
	struct board_traits<basic_board::BasicBoard>
	{
		static constexpr bool IsObjBoard = false;

		static constexpr Square TopToBottom(Square s) { return basic_board::top_to_bottom_order[s]; }

		static constexpr Square BottomToTop(Square s) { return basic_board::bottom_to_top_order[s]; }
		
		static constexpr std::pair<char, char> ToCharPair(Square s) { return basic_board::get_chars(s); }

		static constexpr Square GetSquareFromChars(char f, char r) { return basic_board::get_square_from_chars(f, r); }
	};

	template <>
	struct board_traits<x88board::x88Board>
	{
		static constexpr bool IsObjBoard = false;

		static constexpr Square TopToBottom(Square s) { return x88board::top_to_bottom_order[s]; }

		static constexpr Square BottomToTop(Square s) { return x88board::bottom_to_top_order[s]; }
		
		static constexpr std::pair<char, char> ToCharPair(Square s) { return x88board::get_chars(s); }

		static constexpr Square GetSquareFromChars(char f, char r) { return x88board::get_square_from_chars(f, r); }
	};

	template <>
	struct board_traits<objboard::ObjBoard>
	{
		static constexpr bool IsObjBoard = true;

		static constexpr Square TopToBottom(Square s) { return basic_board::top_to_bottom_order[s]; }

		static constexpr std::pair<char, char> ToCharPair(Square s) { return basic_board::get_chars(s); }

		static constexpr Square GetSquareFromChars(char f, char r) { return basic_board::get_square_from_chars(f, r); }
	};


	template<Color>
	struct color_traits { };

	template <>
	struct color_traits<color::White> 
	{
		static constexpr Piece Pawn = pieceset::WhitePawn;
		static constexpr Piece Rook = pieceset::WhiteRook;
		static constexpr Piece Knight = pieceset::WhiteKnight;
		static constexpr Piece Bishop = pieceset::WhiteBishop;
		static constexpr Piece Queen = pieceset::WhiteQueen;
		static constexpr Piece King = pieceset::WhiteKing;
		
		static constexpr Color Opposite = color::Black;

		static constexpr Castling KingSideCastling = Castling::WHITE_KS;
		static constexpr Castling QueenSideCastling = Castling::WHITE_QS;
	};

	template <>
	struct color_traits<color::Black> 
	{
		static constexpr Piece Pawn = pieceset::BlackPawn;
		static constexpr Piece Rook = pieceset::BlackRook;
		static constexpr Piece Knight = pieceset::BlackKnight;
		static constexpr Piece Bishop = pieceset::BlackBishop;
		static constexpr Piece Queen = pieceset::BlackQueen;
		static constexpr Piece King = pieceset::BlackKing;
		
		static constexpr Color Opposite = color::White;

		static constexpr Castling KingSideCastling = Castling::BLACK_KS;
		static constexpr Castling QueenSideCastling = Castling::BLACK_QS;
	};

	template<typename Board, Piece>
	struct board_piece_traits { };

	template <>
	struct board_piece_traits<basic_board::BasicBoard, pieceset::WhitePawn> 
	{
		static constexpr Direction AttackDirections[2] = { basic_board::direction::NE, basic_board::direction::NW };
	};

	template <>
	struct board_piece_traits<basic_board::BasicBoard, pieceset::BlackPawn>
	{
		static constexpr Direction AttackDirections[2] = { basic_board::direction::SE, basic_board::direction::SW };
	};

	template<typename Board, Color>
	struct board_color_traits { };

	template <>
	struct board_color_traits<basic_board::BasicBoard, color::White> 
	{
		static constexpr Square KingSideCastleCheckSquares[2] = { squareset::f1, squareset::g1 };
		static constexpr Square QueenSideCastleCheckSquares[3] = { squareset::d1, squareset::c1, squareset::b1 };
	};

	template <>
	struct board_color_traits<basic_board::BasicBoard, color::Black>
	{
		static constexpr Square KingSideCastleCheckSquares[2] = { squareset::f8, squareset::g8 };
		static constexpr Square QueenSideCastleCheckSquares[3] = { squareset::d8, squareset::c8, squareset::b8 };
	};
}