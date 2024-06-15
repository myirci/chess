#pragma once

#include <ChessLib/Chess/Definitions.hpp>

#include <memory>

namespace chesslib::objboard 
{
	struct PieceObj 
	{
		explicit PieceObj(Piece code, Square loc, double val) :
			_code{ code }, 
			_color{ color::get_color(code) }, 
		    _loc{ loc },
			_value{ val } 
		{ }

		Piece  _code;
		Color  _color;
		Square _loc;
		double _value;
	};

	struct Pawn : public PieceObj
	{
		explicit Pawn(Piece code, Square loc) : PieceObj(code, loc, pieceset::PawnValue) { }
	};

	struct Rook : public PieceObj
	{
		explicit Rook(Piece code, Square loc) : PieceObj(code, loc, pieceset::RookValue) { }
	};

	struct Knight : public PieceObj
	{
		explicit Knight(Piece code, Square loc) : PieceObj(code, loc, pieceset::KnightValue) { }
	};

	struct Bishop : public PieceObj
	{
		explicit Bishop(Piece code, Square loc) : PieceObj(code, loc, pieceset::BishopValue) { }
	};

	struct Queen : public PieceObj
	{
		explicit Queen(Piece code, Square loc) : PieceObj(code, loc, pieceset::QueenValue) { }
	};

	struct King : public PieceObj
	{
		explicit King(Piece code, Square loc) : PieceObj(code, loc, pieceset::KingValue) { }
	};

	std::unique_ptr<PieceObj> make_piece(Piece code, Square loc);
}