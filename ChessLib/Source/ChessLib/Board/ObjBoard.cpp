#include "ObjBoard.hpp"

namespace chesslib::objboard
{
	#define A(n) SquareObj{ n }
	ObjBoard::ObjBoard() : 
		_board 
		{
			A(a1), A(b1), A(c1), A(d1), A(e1), A(f1), A(g1), A(h1),
			A(a2), A(b2), A(c2), A(d2), A(e2), A(f2), A(g2), A(h2),
			A(a3), A(b3), A(c3), A(d3), A(e3), A(f3), A(g3), A(h3),
			A(a4), A(b4), A(c4), A(d4), A(e4), A(f4), A(g4), A(h4),
			A(a5), A(b5), A(c5), A(d5), A(e5), A(f5), A(g5), A(h5),
			A(a6), A(b6), A(c6), A(d6), A(e6), A(f6), A(g6), A(h6),
			A(a7), A(b7), A(c7), A(d7), A(e7), A(f7), A(g7), A(h7),
			A(a8), A(b8), A(c8), A(d8), A(e8), A(f8), A(g8), A(h8)
		} { }
}
