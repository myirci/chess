#pragma once

#include <ChessLib/Board/BasicBoard/BasicBoard.hpp>
#include <ChessLib/Board/BasicBoard/Square.hpp>

#include <ChessLib/Board/X88Board/x88Board.hpp>
#include <ChessLib/Board/X88Board/Square.hpp>

#include <ChessLib/Board/ObjBoard/ObjBoard.hpp>

namespace chesslib::traits 
{
#pragma region BoardTraits

	template<typename BoardType>
	struct board_traits 
	{
		static constexpr Direction Reverse(Direction dir) noexcept
		{
			return -dir;
		}
	};

	template <>
	struct board_traits<basic_board::BasicBoard>
	{
		

		static constexpr bool IsInside(Square curr, Square next) noexcept
		{
			return
				next < basic_board::BasicBoard::BOARDSIZE &&
				next >= 0 &&
				std::abs(GetFile(next) - GetFile(curr)) <= 2;
		}
	};

	template <>
	struct board_traits<x88board::x88Board>
	{
		static constexpr Direction N{ 16 }, S{ -16 }, E{ 1 }, W{ -1 }, NE{ 17 }, SW{ -17 }, NW{ 15 }, SE{ -15 };

		static constexpr std::array<Direction, 8> AllDirections{ N, NE, E, SE, S, SW, W, NW };

		static constexpr std::array<Direction, 4> StraightDirections{ N, E, S, W };

		static constexpr std::array<Direction, 4> DiagonalDirections{ NE, SE, SW, NW };

		static constexpr std::array<Direction, 8> KnightJumps{ 33, 18, -14, -31, -33, -18, 14, 31 };

		static constexpr Square TopToBottom(Square s) 
		{ 
			return x88board::top_to_bottom_order[s]; 
		}

		static constexpr Square BottomToTop(Square s) 
		{ 
			return x88board::bottom_to_top_order[s]; 
		}

		static constexpr std::pair<char, char> ToCharPair(Square s) 
		{ 
			return x88board::get_chars(s); 
		}

		static constexpr Square GetSquareFromChars(char f, char r) 
		{ 
			return x88board::get_square_from_chars(f, r); 
		}
	};

#pragma endregion

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

		static constexpr Piece StraightMovingPieces[2] = { pieceset::WhiteQueen, pieceset::WhiteRook };
		static constexpr Piece DiagonalMovingPieces[2] = { pieceset::WhiteQueen, pieceset::WhiteBishop };
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

		static constexpr Piece StraightMovingPieces[2] = { pieceset::BlackQueen, pieceset::BlackRook };
		static constexpr Piece DiagonalMovingPieces[2] = { pieceset::BlackQueen, pieceset::BlackBishop };		
	};

#pragma endregion

#pragma region BoardPieceTraits
	template<typename Board, Piece>
	struct board_piece_traits { };

	template <>
	struct board_piece_traits<basic_board::BasicBoard, pieceset::WhitePawn> 
	{
		static constexpr Piece Opposite = pieceset::BlackPawn;
		
		static constexpr Direction AttackDirections[2] = { basic_board::direction::NE, basic_board::direction::NW };
		static constexpr Direction ReverseAttackDirections[2] = { basic_board::direction::SW, basic_board::direction::SE };
		static constexpr Direction MoveDirection = basic_board::direction::N;
		static constexpr Direction ReverseMoveDirection = basic_board::direction::S;
		
		static constexpr Rank PromotionRank = 7;
		static constexpr Rank DoublePushRank = 1;
	};

	template <>
	struct board_piece_traits<basic_board::BasicBoard, pieceset::BlackPawn>
	{
		static constexpr Piece Opposite = pieceset::WhitePawn;
		
		static constexpr Direction AttackDirections[2] = { basic_board::direction::SE, basic_board::direction::SW };
		static constexpr Direction ReverseAttackDirections[2] = { basic_board::direction::NW, basic_board::direction::NE };
		static constexpr Direction MoveDirection = basic_board::direction::S;
		static constexpr Direction ReverseMoveDirection = basic_board::direction::N;
		
		static constexpr Rank PromotionRank = 0;
		static constexpr Rank DoublePushRank = 6;
	};

	template <>
	struct board_piece_traits<x88board::x88Board, pieceset::WhitePawn>
	{
		static constexpr Piece Opposite = pieceset::BlackPawn;

		static constexpr Direction AttackDirections[2] = { x88board::direction::NE, x88board::direction::NW };
		static constexpr Direction ReverseAttackDirections[2] = { x88board::direction::SW, x88board::direction::SE };
		static constexpr Direction MoveDirection = x88board::direction::N;
		static constexpr Direction ReverseMoveDirection = x88board::direction::S;

		static constexpr Rank PromotionRank = 7;
		static constexpr Rank DoublePushRank = 1;
	};

	template <>
	struct board_piece_traits<x88board::x88Board, pieceset::BlackPawn>
	{
		static constexpr Piece Opposite = pieceset::WhitePawn;

		static constexpr Direction AttackDirections[2] = { x88board::direction::SE, x88board::direction::SW };
		static constexpr Direction ReverseAttackDirections[2] = { x88board::direction::NW, x88board::direction::NE };
		static constexpr Direction MoveDirection = x88board::direction::S;
		static constexpr Direction ReverseMoveDirection = x88board::direction::N;

		static constexpr Rank PromotionRank = 0;
		static constexpr Rank DoublePushRank = 6;
	};

#pragma endregion

#pragma region BoardColorTraits
	template<typename Board, Color>
	struct board_color_traits { };

	template <>
	struct board_color_traits<basic_board::BasicBoard, color::White> 
	{
		static constexpr Square KingSideCastleCheckSquares[2]  = { squareset::f1, squareset::g1 };
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

	template <>
	struct board_color_traits<x88board::x88Board, color::White>
	{
		static constexpr Square KingSideCastleCheckSquares[2] = { x88board::f1, x88board::g1 };
		static constexpr Square QueenSideCastleCheckSquares[3] = { x88board::d1, x88board::c1, x88board::b1 };

		static constexpr Square KingSideRookInitialPosition = x88board::h1;
		static constexpr Square QueenSideRookInitialPosition = x88board::a1;

		static constexpr Square KingSideRookPositionAfterCastling = x88board::f1;
		static constexpr Square QueenSideRookPositionAfterCastling = x88board::d1;

		static constexpr Square ValidPawnMoveSquares[2] = { x88board::a3, x88board::h8 };
	};

	template <>
	struct board_color_traits<x88board::x88Board, color::Black>
	{
		static constexpr Square KingSideCastleCheckSquares[2] = { x88board::f8, x88board::g8 };
		static constexpr Square QueenSideCastleCheckSquares[3] = { x88board::d8, x88board::c8, x88board::b8 };

		static constexpr Square KingSideRookInitialPosition = x88board::h8;
		static constexpr Square QueenSideRookInitialPosition = x88board::a8;

		static constexpr Square KingSideRookPositionAfterCastling = x88board::f8;
		static constexpr Square QueenSideRookPositionAfterCastling = x88board::d8;

		static constexpr Square ValidPawnMoveSquares[2] = { x88board::a1, x88board::h6 };
	};
#pragma endregion
}