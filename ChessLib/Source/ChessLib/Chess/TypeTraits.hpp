#pragma once

#include <ChessLib/Board/BasicBoard/BasicBoard.hpp>
#include <ChessLib/Board/BasicBoard/Square.hpp>

#include <ChessLib/Board/X88Board/x88Board.hpp>
#include <ChessLib/Board/X88Board/Square.hpp>

#include <ChessLib/Board/ObjBoard/ObjBoard.hpp>

namespace chesslib::traits 
{
#pragma region BoardTraits
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

#pragma endregion

#pragma region ColorTraits

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
		
		static constexpr Piece StraightMovingPieces[2] = { pieceset::WhiteQueen, pieceset::WhiteRook };
		static constexpr Piece DiagonalMovingPieces[2] = { pieceset::WhiteQueen, pieceset::WhiteBishop };

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
		
		static constexpr Piece StraightMovingPieces[2] = { pieceset::BlackQueen, pieceset::BlackRook };
		static constexpr Piece DiagonalMovingPieces[2] = { pieceset::BlackQueen, pieceset::BlackBishop };

		static constexpr Color Opposite = color::White;

		static constexpr Castling KingSideCastling = Castling::BLACK_KS;
		static constexpr Castling QueenSideCastling = Castling::BLACK_QS;
	};

#pragma endregion

#pragma region BoardPieceTraits
	template<typename Board, Piece>
	struct board_piece_traits { };

	template <>
	struct board_piece_traits<basic_board::BasicBoard, pieceset::WhitePawn> 
	{
		static constexpr Direction AttackDirections[2] = { basic_board::direction::NE, basic_board::direction::NW };
		static constexpr Direction ReverseAttackDirections[2] = { basic_board::direction::SW, basic_board::direction::SE };
		static constexpr Direction MoveDirection = basic_board::direction::N;
		static constexpr Direction ReverseMoveDirection = basic_board::direction::S;
		static constexpr Rank PromotionRank = 7;
		static constexpr Rank DoublePushRank = 1;
		static constexpr Piece Opposite = pieceset::BlackPawn;
	};

	template <>
	struct board_piece_traits<basic_board::BasicBoard, pieceset::BlackPawn>
	{
		static constexpr Direction AttackDirections[2] = { basic_board::direction::SE, basic_board::direction::SW };
		static constexpr Direction ReverseAttackDirections[2] = { basic_board::direction::NW, basic_board::direction::NE };
		static constexpr Direction MoveDirection = basic_board::direction::S;
		static constexpr Direction ReverseMoveDirection = basic_board::direction::N;
		static constexpr Rank PromotionRank = 0;
		static constexpr Rank DoublePushRank = 6;
		static constexpr Piece Opposite = pieceset::WhitePawn;
	};
#pragma endregion

#pragma region BoardColorTraits
	template<typename Board, Color>
	struct board_color_traits { };

	template <>
	struct board_color_traits<basic_board::BasicBoard, color::White> 
	{
		static constexpr Square KingSideCastleCheckSquares[2] = { squareset::f1, squareset::g1 };
		static constexpr Square QueenSideCastleCheckSquares[3] = { squareset::d1, squareset::c1, squareset::b1 };
		
		static constexpr Square KingSideRookInitialPosition = squareset::h1;
		static constexpr Square QueenSideRookInitialPosition = squareset::a1;

		static constexpr Square KingSideRookPositionAfterCastling = squareset::f1;
		static constexpr Square QueenSideRookPositionAfterCastling = squareset::d1;

		static constexpr Square ValidPawnMoveSquares[2] = { squareset::a3, squareset::h8 };
	};

	template <>
	struct board_color_traits<basic_board::BasicBoard, color::Black>
	{
		static constexpr Square KingSideCastleCheckSquares[2] = { squareset::f8, squareset::g8 };
		static constexpr Square QueenSideCastleCheckSquares[3] = { squareset::d8, squareset::c8, squareset::b8 };
		
		static constexpr Square KingSideRookInitialPosition = squareset::h8;
		static constexpr Square QueenSideRookInitialPosition = squareset::a8;

		static constexpr Square KingSideRookPositionAfterCastling = squareset::f8;
		static constexpr Square QueenSideRookPositionAfterCastling = squareset::d8;

		static constexpr Square ValidPawnMoveSquares[2] = { squareset::a1, squareset::h6 };
	};
#pragma endregion
}