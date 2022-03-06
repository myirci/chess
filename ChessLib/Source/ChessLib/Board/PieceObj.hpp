#pragma once

#include <ChessLib/Chess/Definitions.hpp>

#include <memory>

namespace chesslib::objboard 
{
	struct PieceObj 
	{
		PieceObj(Piece code, Square loc, double val) :
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
		Pawn(Piece code, Square loc) : PieceObj(code, loc, pieceset::PawnValue) { }
	};

	struct Rook : public PieceObj
	{
		Rook(Piece code, Square loc) : PieceObj(code, loc, pieceset::RookValue) { }
	};

	struct Knight : public PieceObj
	{
		Knight(Piece code, Square loc) : PieceObj(code, loc, pieceset::KnightValue) { }
	};

	struct Bishop : public PieceObj
	{
		Bishop(Piece code, Square loc) : PieceObj(code, loc, pieceset::BishopValue) { }
	};

	struct Queen : public PieceObj
	{
		Queen(Piece code, Square loc) : PieceObj(code, loc, pieceset::QueenValue) { }
	};

	struct King : public PieceObj
	{
		King(Piece code, Square loc) : PieceObj(code, loc, pieceset::KingValue) { }
	};

	std::shared_ptr<PieceObj> make_shared_piece(Piece code, Square loc);
}