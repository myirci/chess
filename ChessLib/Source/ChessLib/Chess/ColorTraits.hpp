#pragma once

#include <ChessLib/Chess/Definitions.hpp>

namespace chesslib::traits 
{
	template<Color>
	struct color_traits { };

	template <>
	struct color_traits<color::White>
	{
		static constexpr Color Opposite = color::Black;

		static constexpr Piece Pawn   = pieceset::WhitePawn;
		static constexpr Piece Rook   = pieceset::WhiteRook;
		static constexpr Piece Knight = pieceset::WhiteKnight;
		static constexpr Piece Bishop = pieceset::WhiteBishop;
		static constexpr Piece Queen  = pieceset::WhiteQueen;
		static constexpr Piece King   = pieceset::WhiteKing;

		static constexpr Castling KingSideCastling  = Castling::WHITE_KS;
		static constexpr Castling QueenSideCastling = Castling::WHITE_QS;

		static constexpr Piece StraightMovingPieces[2]   = { pieceset::WhiteQueen, pieceset::WhiteRook };
		static constexpr Piece DiagonallyMovingPieces[2] = { pieceset::WhiteQueen, pieceset::WhiteBishop };
	};

	template <>
	struct color_traits<color::Black>
	{
		static constexpr Color Opposite = color::White;

		static constexpr Piece Pawn   = pieceset::BlackPawn;
		static constexpr Piece Rook   = pieceset::BlackRook;
		static constexpr Piece Knight = pieceset::BlackKnight;
		static constexpr Piece Bishop = pieceset::BlackBishop;
		static constexpr Piece Queen  = pieceset::BlackQueen;
		static constexpr Piece King   = pieceset::BlackKing;

		static constexpr Castling KingSideCastling = Castling::BLACK_KS;
		static constexpr Castling QueenSideCastling = Castling::BLACK_QS;

		static constexpr Piece StraightMovingPieces[2]   = { pieceset::BlackQueen, pieceset::BlackRook };
		static constexpr Piece DiagonallyMovingPieces[2] = { pieceset::BlackQueen, pieceset::BlackBishop };
	};
}