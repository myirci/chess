#pragma once

#include <ChessLib/Board/BasicBoard/BasicBoard.hpp>
#include <ChessLib/Board/X88Board/x88Board.hpp>
#include <ChessLib/Board/ObjBoard/ObjBoard.hpp>

namespace chesslib::traits 
{

#pragma region ColorTraits

	template<Color>
	struct color_traits { };

	template <>
	struct color_traits<color::White> 
	{
		static constexpr Color Opposite = color::Black;

		static constexpr Piece Pawn = pieceset::WhitePawn;
		static constexpr Piece Rook = pieceset::WhiteRook;
		static constexpr Piece Knight = pieceset::WhiteKnight;
		static constexpr Piece Bishop = pieceset::WhiteBishop;
		static constexpr Piece Queen = pieceset::WhiteQueen;
		static constexpr Piece King = pieceset::WhiteKing;
		
		static constexpr Castling KingSideCastling = Castling::WHITE_KS;
		static constexpr Castling QueenSideCastling = Castling::WHITE_QS;

		static constexpr Piece StraightMovingPieces[2] = 
		{ 
			pieceset::WhiteQueen, 
			pieceset::WhiteRook 
		};
		
		static constexpr Piece DiagonallyMovingPieces[2] = 
		{ 
			pieceset::WhiteQueen, 
			pieceset::WhiteBishop 
		};
	};

	template <>
	struct color_traits<color::Black> 
	{
		static constexpr Color Opposite = color::White;

		static constexpr Piece Pawn = pieceset::BlackPawn;
		static constexpr Piece Rook = pieceset::BlackRook;
		static constexpr Piece Knight = pieceset::BlackKnight;
		static constexpr Piece Bishop = pieceset::BlackBishop;
		static constexpr Piece Queen = pieceset::BlackQueen;
		static constexpr Piece King = pieceset::BlackKing;
		
		static constexpr Castling KingSideCastling = Castling::BLACK_KS;
		static constexpr Castling QueenSideCastling = Castling::BLACK_QS;

		static constexpr Piece StraightMovingPieces[2] = 
		{ 
			pieceset::BlackQueen, 
			pieceset::BlackRook 
		};

		static constexpr Piece DiagonallyMovingPieces[2] = 
		{ 
			pieceset::BlackQueen, 
			pieceset::BlackBishop 
		};		
	};

#pragma endregion

#pragma region BoardPieceTraits
	template<typename Board, Piece>
	struct board_piece_traits { };

	template <>
	struct board_piece_traits<basic_board::BasicBoard, pieceset::WhitePawn> 
	{
		static constexpr Piece Opposite = pieceset::BlackPawn;
		
		static constexpr Rank PromotionRank = 7;
		static constexpr Rank DoublePushRank = 1;
		
		static constexpr Direction MoveDirection = basic_board::N;
		static constexpr Direction ReverseMoveDirection = basic_board::S;

		static constexpr Direction AttackDirections[2] = 
		{ 
			basic_board::NE, 
			basic_board::NW 
		};
		
		static constexpr Direction ReverseAttackDirections[2] = 
		{ 
			basic_board::SW, 
			basic_board::SE 
		};
	};

	template <>
	struct board_piece_traits<basic_board::BasicBoard, pieceset::BlackPawn>
	{
		static constexpr Piece Opposite = pieceset::WhitePawn;
		
		static constexpr Rank PromotionRank = 0;
		static constexpr Rank DoublePushRank = 6;

		static constexpr Direction MoveDirection = basic_board::S;
		static constexpr Direction ReverseMoveDirection = basic_board::N;

		static constexpr Direction AttackDirections[2] = 
		{ 
			basic_board::SE, 
			basic_board::SW 
		};

		static constexpr Direction ReverseAttackDirections[2] = 
		{ 
			basic_board::NW, 
			basic_board::NE 
		};
	};

	template <>
	struct board_piece_traits<x88board::x88Board, pieceset::WhitePawn>
	{
		static constexpr Piece Opposite = pieceset::BlackPawn;

		static constexpr Rank PromotionRank = 7;
		static constexpr Rank DoublePushRank = 1;

		static constexpr Direction MoveDirection = x88board::N;
		static constexpr Direction ReverseMoveDirection = x88board::S;

		static constexpr Direction AttackDirections[2] = 
		{ 
			x88board::NE, 
			x88board::NW 
		};
		
		static constexpr Direction ReverseAttackDirections[2] = 
		{ 
			x88board::SW, 
			x88board::SE 
		};
	};

	template <>
	struct board_piece_traits<x88board::x88Board, pieceset::BlackPawn>
	{
		static constexpr Piece Opposite = pieceset::WhitePawn;

		static constexpr Rank PromotionRank = 0;
		static constexpr Rank DoublePushRank = 6;

		static constexpr Direction MoveDirection = x88board::S;
		static constexpr Direction ReverseMoveDirection = x88board::N;

		static constexpr Direction AttackDirections[2] = 
		{
			x88board::SE, 
			x88board::SW 
		};
		
		static constexpr Direction ReverseAttackDirections[2] = 
		{ 
			x88board::NW, 
			x88board::NE 
		};
	};

#pragma endregion

#pragma region BoardColorTraits
	template<typename Board, Color>
	struct board_color_traits { };

	template <>
	struct board_color_traits<basic_board::BasicBoard, color::White> 
	{
		static constexpr Square KingSideRookInitialPosition = squareset::h1;
		static constexpr Square QueenSideRookInitialPosition = squareset::a1;

		static constexpr Square KingSideRookPositionAfterCastling = squareset::f1;
		static constexpr Square QueenSideRookPositionAfterCastling = squareset::d1;

		static constexpr Square KingSideCastleCheckSquares[2]  = 
		{ 
			squareset::f1, 
			squareset::g1 
		};
		
		static constexpr Square QueenSideCastleCheckSquares[3] = 
		{ 
			squareset::d1, 
			squareset::c1, 
			squareset::b1 
		};

		static constexpr Square ValidPawnMoveSquares[2] = 
		{ 
			squareset::a3, 
			squareset::h8 
		};
	};

	template <>
	struct board_color_traits<basic_board::BasicBoard, color::Black>
	{
		static constexpr Square KingSideRookInitialPosition = squareset::h8;
		static constexpr Square QueenSideRookInitialPosition = squareset::a8;

		static constexpr Square KingSideRookPositionAfterCastling = squareset::f8;
		static constexpr Square QueenSideRookPositionAfterCastling = squareset::d8;

		static constexpr Square KingSideCastleCheckSquares[2] = 
		{ 
			squareset::f8, 
			squareset::g8 
		};
		
		static constexpr Square QueenSideCastleCheckSquares[3] = 
		{ 
			squareset::d8, 
			squareset::c8, 
			squareset::b8 
		};

		static constexpr Square ValidPawnMoveSquares[2] = 
		{ 
			squareset::a1, 
			squareset::h6 
		};
	};

	template <>
	struct board_color_traits<x88board::x88Board, color::White>
	{
		static constexpr Square KingSideRookInitialPosition = x88board::h1;
		static constexpr Square QueenSideRookInitialPosition = x88board::a1;

		static constexpr Square KingSideRookPositionAfterCastling = x88board::f1;
		static constexpr Square QueenSideRookPositionAfterCastling = x88board::d1;

		static constexpr Square KingSideCastleCheckSquares[2] = 
		{ 
			x88board::f1, 
			x88board::g1 
		};

		static constexpr Square QueenSideCastleCheckSquares[3] = 
		{ 
			x88board::d1, 
			x88board::c1, 
			x88board::b1 
		};

		static constexpr Square ValidPawnMoveSquares[2] = 
		{ 
			x88board::a3, 
			x88board::h8 
		};
	};

	template <>
	struct board_color_traits<x88board::x88Board, color::Black>
	{
		static constexpr Square KingSideRookInitialPosition = x88board::h8;
		static constexpr Square QueenSideRookInitialPosition = x88board::a8;

		static constexpr Square KingSideRookPositionAfterCastling = x88board::f8;
		static constexpr Square QueenSideRookPositionAfterCastling = x88board::d8;

		static constexpr Square KingSideCastleCheckSquares[2] = 
		{ 
			x88board::f8, 
			x88board::g8 
		};
		
		static constexpr Square QueenSideCastleCheckSquares[3] = 
		{ 
			x88board::d8, 
			x88board::c8, 
			x88board::b8 
		};

		static constexpr Square ValidPawnMoveSquares[2] = 
		{ 
			x88board::a1, 
			x88board::h6 
		};
	};
#pragma endregion
}