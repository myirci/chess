#include "ObjBoard.hpp"

namespace chesslib::objboard
{
	ObjBoard::ObjBoard() : _board
	{
		SquareObj{ a1, { a2,   b2,   b1,   Empty, Empty, Empty, Empty, Empty }, { b3, c2, Empty, Empty, Empty, Empty, Empty, Empty } },
		SquareObj{ b1, { b2,   c2,   c1,   Empty, Empty, Empty, a1,   a2   }, { c3, d2, a3,   Empty, Empty, Empty, Empty, Empty } },
		SquareObj{ c1, { c2,   d2,   d1,   Empty, Empty, Empty, b1,   b2   }, { d3, e2, a2,   b3,   Empty, Empty, Empty, Empty } },
		SquareObj{ d1, { d2,   e2,   e1,   Empty, Empty, Empty, c1,   c2   }, { e3, f2, b2,   c3,   Empty, Empty, Empty, Empty } },
		SquareObj{ e1, { e2,   f2,   f1,   Empty, Empty, Empty, d1,   d2   }, { f3, g2, c2,   d3,   Empty, Empty, Empty, Empty } },
		SquareObj{ f1, { f2,   g2,   g1,   Empty, Empty, Empty, e1,   e2   }, { g3, h2, d2,   e3,   Empty, Empty, Empty, Empty } },
		SquareObj{ g1, { g2,   h2,   h1,   Empty, Empty, Empty, f1,   f2   }, { h3, e2, f3,   Empty, Empty, Empty, Empty, Empty } },
		SquareObj{ h1, { h2,   Empty, Empty, Empty, Empty, Empty, g1,   g2   }, { f2, g3, Empty, Empty, Empty, Empty, Empty, Empty } },
		
		SquareObj{ a2, { a3,   b3,   b2,   b1,   a1,   Empty, Empty, Empty }, { b4, c3, c1,   Empty, Empty, Empty, Empty, Empty } },
		SquareObj{ b2, { b3,   c3,   c2,   c1,   b1,   a1,   a2,   a3   }, { c4, d3, d1,   a4,   Empty, Empty, Empty, Empty } },
		SquareObj{ c2, { c3,   d3,   d2,   d1,   c1,   b1,   b2,   b3   }, { d4, e3, e1,   a1,   a3,   b4,   Empty, Empty } },
		SquareObj{ d2, { d3,   e3,   e2,   e1,   d1,   c1,   c2,   c3   }, { e4, f3, f1,   b1,   b3,   c4,   Empty, Empty } },
		SquareObj{ e2, { e3,   f3,   f2,   f1,   e1,   d1,   d2,   d3   }, { f4, g3, g1,   c1,   c3,   d4,   Empty, Empty } },
		SquareObj{ f2, { f3,   g3,   g2,   g1,   f1,   e1,   e2,   e3   }, { g4, h3, h1,   d1,   d3,   e4,   Empty, Empty } },
		SquareObj{ g2, { g3,   h3,   h2,   h1,   g1,   f1,   f2,   f3   }, { h4, e1, e3,   f4,   Empty, Empty, Empty, Empty } },
		SquareObj{ h2, { h3,   Empty, Empty, Empty, h1,   g1,   g2,   g3   }, { f1, f3, g4,   Empty, Empty, Empty, Empty, Empty } },

		SquareObj{ a3, { a4,   b4,   b3,   b2,   a2,   Empty, Empty, Empty }, { b5, c4, c2,   b1,   Empty, Empty, Empty, Empty } },
		SquareObj{ b3, { b4,   c4,   c3,   c2,   b2,   a2,   a3,   a4   }, { c5, d4, d2,   c1,   a1,   a5,   Empty, Empty } },
		SquareObj{ c3, { c4,   d4,   d3,   d2,   c2,   b2,   b3,   b4   }, { d5, e4, e2,   d1,   b1,   a2,   a4,   b5   } },
		SquareObj{ d3, { d4,   e4,   e3,   e2,   d2,   c2,   c3,   c4   }, { e5, f4, f2,   e1,   c1,   b2,   b4,   c5   } },
		SquareObj{ e3, { e4,   f4,   f3,   f2,   e2,   d2,   d3,   d4   }, { f5, g4, g2,   f1,   d1,   c2,   c4,   c5   } },
		SquareObj{ f3, { f4,   g4,   g3,   g2,   f2,   e2,   e3,   e4   }, { g5, h4, h2,   g1,   e1,   d2,   d4,   e5   } },
		SquareObj{ g3, { g4,   h4,   h3,   h2,   g2,   f2,   f3,   f4   }, { h5, h1, f1,   e2,   e4,   f5,   Empty, Empty } },
		SquareObj{ h3, { h4,   Empty, Empty, Empty, h2,   g2,   g3,   g4   }, { g1, f2, f4,   g5,   Empty, Empty, Empty, Empty } },
		
		SquareObj{ a4, { a5,   b5,   b4,   b3,   a3,   Empty, Empty, Empty }, { b6, c6, c3,   b2,   Empty, Empty, Empty, Empty } },
		SquareObj{ b4, { b5,   c5,   c4,   c3,   b3,   a3,   a4,   a5   }, { c6, d5, d3,   c2,   a2,   a6,   Empty, Empty } },
		SquareObj{ c4, { c5,   d5,   d4,   d3,   c3,   b3,   b4,   b5   }, { d6, e5, e3,   d2,   b2,   a3,   a5,   b6   } },
		SquareObj{ d4, { d5,   e5,   e4,   e3,   d3,   c3,   c4,   c5   }, { e6, f5, f3,   e2,   c2,   b3,   b5,   c6   } },
		SquareObj{ e4, { e5,   f5,   f4,   f3,   e3,   d3,   d4,   d5   }, { f6, g5, g3,   f2,   d2,   c3,   c5,   d6   } },
		SquareObj{ f4, { f5,   g5,   g4,   g3,   f3,   e3,   e4,   e5   }, { g6, h5, h3,   g2,   e2,   d3,   d5,   e6   } },
		SquareObj{ g4, { g5,   h5,   h4,   h3,   g3,   f3,   f4,   f5   }, { h6, h2, f2,   e3,   e5,   f6,   Empty, Empty } },
		SquareObj{ h4, { h5,   Empty, Empty, Empty, h3,   g3,   g4,   g5   }, { g2, f3, f5,   g6,   Empty, Empty, Empty, Empty } },

		SquareObj{ a5, { a6,   b6,   b5,   b4,   a4,   Empty, Empty, Empty }, { b7, c7, c4,   b3,   Empty, Empty, Empty, Empty } },
		SquareObj{ b5, { b6,   c6,   c5,   c4,   b4,   a4,   a5,   a6   }, { c7, d6, d4,   c3,   a3,   a7,   Empty, Empty } },
		SquareObj{ c5, { c6,   d6,   d5,   d4,   c4,   b4,   b5,   b6   }, { d7, e6, e4,   d3,   b3,   a4,   a6,   b7   } },
		SquareObj{ d5, { d6,   e6,   e5,   e4,   d4,   c4,   c5,   c6   }, { e7, f6, f4,   e3,   c3,   b4,   b6,   c7   } },
		SquareObj{ e5, { e6,   f6,   f5,   f4,   e4,   d4,   d5,   d6   }, { f7, g6, g4,   f3,   d3,   c4,   c6,   d7   } },
		SquareObj{ f5, { f6,   g6,   g5,   g4,   f4,   e4,   e5,   e6   }, { g7, h6, h4,   g3,   e3,   d4,   d6,   e7   } },
		SquareObj{ g5, { g6,   h6,   h5,   h4,   g4,   f4,   f5,   f6   }, { h7, h3, f3,   e4,   e6,   f7,   Empty, Empty } },
		SquareObj{ h5, { h6,   Empty, Empty, Empty, h4,   g4,   g5,   g6   }, { g3, f4, f6,   g7,   Empty, Empty, Empty, Empty } },
		
		SquareObj{ a6, { a7,   b7,   b6,   b5,   a5,   Empty, Empty, Empty }, { b8, c8, c5,   b4,   Empty, Empty, Empty, Empty } },
		SquareObj{ b6, { b7,   c7,   c6,   c5,   b5,   a5,   a6,   a7   }, { c8, d7, d5,   c4,   a4,   a8,   Empty, Empty } },
		SquareObj{ c6, { c7,   d7,   d6,   d5,   c5,   b5,   b6,   b7   }, { d8, e7, e5,   d4,   b4,   a5,   a7,   b8   } },
		SquareObj{ d6, { d7,   e7,   e6,   e5,   d5,   c5,   c6,   c7   }, { e8, f7, f5,   e4,   c4,   b5,   b7,   c8   } },
		SquareObj{ e6, { e7,   f7,   f6,   f5,   e5,   d5,   d6,   d7   }, { f8, g7, g5,   f4,   d4,   c5,   c7,   d8   } },
		SquareObj{ f6, { f7,   g7,   g6,   g5,   f5,   e5,   e6,   e7   }, { g8, h7, h5,   g4,   e4,   d5,   d7,   e8   } },
		SquareObj{ g6, { g7,   h7,   h6,   h5,   g5,   f5,   f6,   f7   }, { h8, h4, f4,   e5,   e7,   f8,   Empty, Empty } },
		SquareObj{ h6, { h7,   Empty, Empty, Empty, h5,   g5,   g6,   g7   }, { g4, f5, f7,   g8,   Empty, Empty, Empty, Empty } },

		SquareObj{ a7, { a8,   b8,   b7,   b6,   a6,   Empty, Empty, Empty }, { c8, c6, b5,   Empty, Empty, Empty, Empty, Empty } },
		SquareObj{ b7, { b8,   c8,   c7,   c6,   b6,   a6,   a7,   a8   }, { d8, d6, c5,   a5,   Empty, Empty, Empty, Empty } },
		SquareObj{ c7, { c8,   d8,   d7,   d6,   c6,   b6,   b7,   b8   }, { e8, e6, d5,   b5,   a6,   a8,   Empty, Empty } },
		SquareObj{ d7, { d8,   e8,   e7,   e6,   d6,   c6,   c7,   c8   }, { f8, f6, e5,   c5,   b6,   b8,   Empty, Empty } },
		SquareObj{ e7, { e8,   f8,   f7,   f6,   e6,   d6,   d7,   d8   }, { g8, g6, f5,   d5,   c6,   c8,   Empty, Empty } },
		SquareObj{ f7, { f8,   g8,   g7,   g6,   f6,   e6,   e7,   e8   }, { h8, h6, g5,   e5,   d6,   d8,   Empty, Empty } },
		SquareObj{ g7, { g8,   h8,   h7,   h6,   g6,   f6,   f7,   f8   }, { h5, f5, e6,   e8,   Empty, Empty, Empty, Empty } },
		SquareObj{ h7, { h8,   Empty, Empty, Empty, h6,   g6,   g7,   g8   }, { g5, f6, f8,   Empty, Empty, Empty, Empty, Empty } },
		
		SquareObj{ a8, { Empty, Empty, b8,   b7,   a7,   Empty, Empty, Empty }, { c7, b6, Empty, Empty, Empty, Empty, Empty, Empty } },
		SquareObj{ b8, { Empty, Empty, c8,   c7,   b7,   a7,   a8,   Empty }, { d7, c6, a6,   Empty, Empty, Empty, Empty, Empty } },
		SquareObj{ c8, { Empty, Empty, d8,   d7,   c7,   b7,   b8,   Empty }, { e7, d6, b6,   a7,   Empty, Empty, Empty, Empty } },
		SquareObj{ d8, { Empty, Empty, e8,   e7,   d7,   c7,   c8,   Empty }, { f7, e6, c6,   b7,   Empty, Empty, Empty, Empty } },
		SquareObj{ e8, { Empty, Empty, f8,   f7,   e7,   d7,   d8,   Empty }, { g7, f6, d6,   c7,   Empty, Empty, Empty, Empty } },
		SquareObj{ f8, { Empty, Empty, g8,   g7,   f7,   e7,   e8,   Empty }, { h7, g6, h5,   e6,   d7,   Empty, Empty, Empty } },
		SquareObj{ g8, { Empty, Empty, h8,   h7,   g7,   f7,   f8,   Empty }, { h6, f6, e7,   Empty, Empty, Empty, Empty, Empty } },
		SquareObj{ h8, { Empty, Empty, Empty, Empty, h7,   g7,   g8,   Empty }, { g6, f7, Empty, Empty, Empty, Empty, Empty, Empty } }
	}
	{ }

	/*MoveList ObjBoard::GenerateMoves() const 
	{
		MoveList moves;
		return moves;
	}*/
}
