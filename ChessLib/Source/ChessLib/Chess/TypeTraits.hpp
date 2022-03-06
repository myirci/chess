#pragma once

#include <ChessLib/Board/BasicBoard.hpp>
#include <ChessLib/Board/x88Board.hpp>
#include <ChessLib/Board/ObjBoard.hpp>

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
	struct board_piece_traits<BasicBoard, pieceset::WhitePawn> 
	{
		static constexpr Piece Opposite = pieceset::BlackPawn;
		
		static constexpr Rank PromotionRank = 7;
		static constexpr Rank DoublePushRank = 1;
		
		static constexpr Direction MoveDirection = BasicBoard::N;
		static constexpr Direction ReverseMoveDirection = BasicBoard::S;

		static constexpr Direction AttackDirections[2] = 
		{ 
			BasicBoard::NE,
			BasicBoard::NW
		};
		
		static constexpr Direction ReverseAttackDirections[2] = 
		{ 
			BasicBoard::SW,
			BasicBoard::SE
		};
	};

	template <>
	struct board_piece_traits<BasicBoard, pieceset::BlackPawn>
	{
		static constexpr Piece Opposite = pieceset::WhitePawn;
		
		static constexpr Rank PromotionRank = 0;
		static constexpr Rank DoublePushRank = 6;

		static constexpr Direction MoveDirection = BasicBoard::S;
		static constexpr Direction ReverseMoveDirection = BasicBoard::N;

		static constexpr Direction AttackDirections[2] = 
		{ 
			BasicBoard::SE,
			BasicBoard::SW
		};

		static constexpr Direction ReverseAttackDirections[2] = 
		{ 
			BasicBoard::NW,
			BasicBoard::NE
		};
	};

	template <>
	struct board_piece_traits<x88Board, pieceset::WhitePawn>
	{
		static constexpr Piece Opposite = pieceset::BlackPawn;

		static constexpr Rank PromotionRank = 7;
		static constexpr Rank DoublePushRank = 1;

		static constexpr Direction MoveDirection = x88Board::N;
		static constexpr Direction ReverseMoveDirection = x88Board::S;

		static constexpr Direction AttackDirections[2] = 
		{ 
			x88Board::NE,
			x88Board::NW
		};
		
		static constexpr Direction ReverseAttackDirections[2] = 
		{ 
			x88Board::SW,
			x88Board::SE
		};
	};

	template <>
	struct board_piece_traits<x88Board, pieceset::BlackPawn>
	{
		static constexpr Piece Opposite = pieceset::WhitePawn;

		static constexpr Rank PromotionRank = 0;
		static constexpr Rank DoublePushRank = 6;

		static constexpr Direction MoveDirection = x88Board::S;
		static constexpr Direction ReverseMoveDirection = x88Board::N;

		static constexpr Direction AttackDirections[2] = 
		{
			x88Board::SE,
			x88Board::SW
		};
		
		static constexpr Direction ReverseAttackDirections[2] = 
		{ 
			x88Board::NW,
			x88Board::NE
		};
	};

#pragma endregion

#pragma region BoardColorTraits
	template<typename Board, Color>
	struct board_color_traits { };

	template <>
	struct board_color_traits<BasicBoard, color::White> 
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
	struct board_color_traits<BasicBoard, color::Black>
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
	struct board_color_traits<x88Board, color::White>
	{
		static constexpr Square KingSideRookInitialPosition = x88Board::h1;
		static constexpr Square QueenSideRookInitialPosition = x88Board::a1;

		static constexpr Square KingSideRookPositionAfterCastling = x88Board::f1;
		static constexpr Square QueenSideRookPositionAfterCastling = x88Board::d1;

		static constexpr Square KingSideCastleCheckSquares[2] = 
		{ 
			x88Board::f1,
			x88Board::g1
		};

		static constexpr Square QueenSideCastleCheckSquares[3] = 
		{ 
			x88Board::d1,
			x88Board::c1,
			x88Board::b1
		};

		static constexpr Square ValidPawnMoveSquares[2] = 
		{ 
			x88Board::a3,
			x88Board::h8
		};
	};

	template <>
	struct board_color_traits<x88Board, color::Black>
	{
		static constexpr Square KingSideRookInitialPosition = x88Board::h8;
		static constexpr Square QueenSideRookInitialPosition = x88Board::a8;

		static constexpr Square KingSideRookPositionAfterCastling = x88Board::f8;
		static constexpr Square QueenSideRookPositionAfterCastling = x88Board::d8;

		static constexpr Square KingSideCastleCheckSquares[2] = 
		{ 
			x88Board::f8,
			x88Board::g8
		};
		
		static constexpr Square QueenSideCastleCheckSquares[3] = 
		{ 
			x88Board::d8,
			x88Board::c8,
			x88Board::b8
		};

		static constexpr Square ValidPawnMoveSquares[2] = 
		{ 
			x88Board::a1,
			x88Board::h6
		};
	};
#pragma endregion
}