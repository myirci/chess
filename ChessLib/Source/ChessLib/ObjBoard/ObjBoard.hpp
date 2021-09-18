#pragma once

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/BoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/ObjBoard/PieceObj.hpp>
#include <ChessLib/ObjBoard/SquareObj.hpp>

#include <array>
#include <memory>
#include <vector>

using namespace chesslib::squareset;

namespace chesslib::objboard
{
	class ObjBoard : public BoardBase  
	{
	public:
		using SquareArray = std::array<SquareObj, 64>;
		using PiecePtr = std::shared_ptr<PieceObj>;
		using PieceMap = std::unordered_multimap<Piece, PiecePtr>;

		const SquareArray& GetBoard() const;
		SquareArray& GetBoard();

		const PieceMap& GetWhitePieces() const;
		PieceMap& GetWhitePieces();

		const PieceMap& GetBlackPieces() const;
		PieceMap& GetBlackPieces();

		std::vector<Move> GenerateMoves() const;
	
	protected:

		ObjBoard();

		PieceMap _white_pieces;
		PieceMap _black_pieces;

		std::array<SquareObj, 64> _squares =
		{
			SquareObj{ a1, { a2,   b2,   b1,   None, None, None, None, None }, { b3, c2, None, None, None, None, None, None } },
			SquareObj{ b1, { b2,   c2,   c1,   None, None, None, a1,   a2   }, { c3, d2, a3,   None, None, None, None, None } },
			SquareObj{ c1, { c2,   d2,   d1,   None, None, None, b1,   b2   }, { d3, e2, a2,   b3,   None, None, None, None } },
			SquareObj{ d1, { d2,   e2,   e1,   None, None, None, c1,   c2   }, { e3, f2, b2,   c3,   None, None, None, None } },
			SquareObj{ e1, { e2,   f2,   f1,   None, None, None, d1,   d2   }, { f3, g2, c2,   d3,   None, None, None, None } },
			SquareObj{ f1, { f2,   g2,   g1,   None, None, None, e1,   e2   }, { g3, h2, d2,   e3,   None, None, None, None } },
			SquareObj{ g1, { g2,   h2,   h1,   None, None, None, f1,   f2   }, { h3, e2, f3,   None, None, None, None, None } },
			SquareObj{ h1, { h2,   None, None, None, None, None, g1,   g2   }, { f2, g3, None, None, None, None, None, None } },

			SquareObj{ a2, { a3,   b3,   b2,   b1,   a1,   None, None, None }, { b4, c3, c1,   None, None, None, None, None } },
			SquareObj{ b2, { b3,   c3,   c2,   c1,   b1,   a1,   a2,   a3   }, { c4, d3, d1,   a4,   None, None, None, None } },
			SquareObj{ c2, { c3,   d3,   d2,   d1,   c1,   b1,   b2,   b3   }, { d4, e3, e1,   a1,   a3,   b4,   None, None } },
			SquareObj{ d2, { d3,   e3,   e2,   e1,   d1,   c1,   c2,   c3   }, { e4, f3, f1,   b1,   b3,   c4,   None, None } },
			SquareObj{ e2, { e3,   f3,   f2,   f1,   e1,   d1,   d2,   d3   }, { f4, g3, g1,   c1,   c3,   d4,   None, None } },
			SquareObj{ f2, { f3,   g3,   g2,   g1,   f1,   e1,   e2,   e3   }, { g4, h3, h1,   d1,   d3,   e4,   None, None } },
			SquareObj{ g2, { g3,   h3,   h2,   h1,   g1,   f1,   f2,   f3   }, { h4, e1, e3,   f4,   None, None, None, None } },
			SquareObj{ h2, { h3,   None, None, None, h1,   g1,   g2,   g3   }, { f1, f3, g4,   None, None, None, None, None } },
			 
			SquareObj{ a3, { a4,   b4,   b3,   b2,   a2,   None, None, None }, { b5, c4, c2,   b1,   None, None, None, None } },
			SquareObj{ b3, { b4,   c4,   c3,   c2,   b2,   a2,   a3,   a4   }, { c5, d4, d2,   c1,   a1,   a5,   None, None } },
			SquareObj{ c3, { c4,   d4,   d3,   d2,   c2,   b2,   b3,   b4   }, { d5, e4, e2,   d1,   b1,   a2,   a4,   b5   } },
			SquareObj{ d3, { d4,   e4,   e3,   e2,   d2,   c2,   c3,   c4   }, { e5, f4, f2,   e1,   c1,   b2,   b4,   c5   } },
			SquareObj{ e3, { e4,   f4,   f3,   f2,   e2,   d2,   d3,   d4   }, { f5, g4, g2,   f1,   d1,   c2,   c4,   c5   } },
			SquareObj{ f3, { f4,   g4,   g3,   g2,   f2,   e2,   e3,   e4   }, { g5, h4, h2,   g1,   e1,   d2,   d4,   e5   } },
			SquareObj{ g3, { g4,   h4,   h3,   h2,   g2,   f2,   f3,   f4   }, { h5, h1, f1,   e2,   e4,   f5,   None, None } },
			SquareObj{ h3, { h4,   None, None, None, h2,   g2,   g3,   g4   }, { g1, f2, f4,   g5,   None, None, None, None } },

			SquareObj{ a4, { a5,   b5,   b4,   b3,   a3,   None, None, None }, { b6, c6, c3,   b2,   None, None, None, None } },
			SquareObj{ b4, { b5,   c5,   c4,   c3,   b3,   a3,   a4,   a5   }, { c6, d5, d3,   c2,   a2,   a6,   None, None } },
			SquareObj{ c4, { c5,   d5,   d4,   d3,   c3,   b3,   b4,   b5   }, { d6, e5, e3,   d2,   b2,   a3,   a5,   b6   } },
			SquareObj{ d4, { d5,   e5,   e4,   e3,   d3,   c3,   c4,   c5   }, { e6, f5, f3,   e2,   c2,   b3,   b5,   c6   } },
			SquareObj{ e4, { e5,   f5,   f4,   f3,   e3,   d3,   d4,   d5   }, { f6, g5, g3,   f2,   d2,   c3,   c5,   d6   } },
			SquareObj{ f4, { f5,   g5,   g4,   g3,   f3,   e3,   e4,   e5   }, { g6, h5, h3,   g2,   e2,   d3,   d5,   e6   } },
			SquareObj{ g4, { g5,   h5,   h4,   h3,   g3,   f3,   f4,   f5   }, { h6, h2, f2,   e3,   e5,   f6,   None, None } },
			SquareObj{ h4, { h5,   None, None, None, h3,   g3,   g4,   g5   }, { g2, f3, f5,   g6,   None, None, None, None } },

			SquareObj{ a5, { a6,   b6,   b5,   b4,   a4,   None, None, None }, { b7, c7, c4,   b3,   None, None, None, None } },
			SquareObj{ b5, { b6,   c6,   c5,   c4,   b4,   a4,   a5,   a6   }, { c7, d6, d4,   c3,   a3,   a7,   None, None } },
			SquareObj{ c5, { c6,   d6,   d5,   d4,   c4,   b4,   b5,   b6   }, { d7, e6, e4,   d3,   b3,   a4,   a6,   b7   } },
			SquareObj{ d5, { d6,   e6,   e5,   e4,   d4,   c4,   c5,   c6   }, { e7, f6, f4,   e3,   c3,   b4,   b6,   c7   } },
			SquareObj{ e5, { e6,   f6,   f5,   f4,   e4,   d4,   d5,   d6   }, { f7, g6, g4,   f3,   d3,   c4,   c6,   d7   } },
			SquareObj{ f5, { f6,   g6,   g5,   g4,   f4,   e4,   e5,   e6   }, { g7, h6, h4,   g3,   e3,   d4,   d6,   e7   } },
			SquareObj{ g5, { g6,   h6,   h5,   h4,   g4,   f4,   f5,   f6   }, { h7, h3, f3,   e4,   e6,   f7,   None, None } },
			SquareObj{ h5, { h6,   None, None, None, h4,   g4,   g5,   g6   }, { g3, f4, f6,   g7,   None, None, None, None } },

			SquareObj{ a6, { a7,   b7,   b6,   b5,   a5,   None, None, None }, { b8, c8, c5,   b4,   None, None, None, None } },
			SquareObj{ b6, { b7,   c7,   c6,   c5,   b5,   a5,   a6,   a7   }, { c8, d7, d5,   c4,   a4,   a8,   None, None } },
			SquareObj{ c6, { c7,   d7,   d6,   d5,   c5,   b5,   b6,   b7   }, { d8, e7, e5,   d4,   b4,   a5,   a7,   b8   } },
			SquareObj{ d6, { d7,   e7,   e6,   e5,   d5,   c5,   c6,   c7   }, { e8, f7, f5,   e4,   c4,   b5,   b7,   c8   } },
			SquareObj{ e6, { e7,   f7,   f6,   f5,   e5,   d5,   d6,   d7   }, { f8, g7, g5,   f4,   d4,   c5,   c7,   d8   } },
			SquareObj{ f6, { f7,   g7,   g6,   g5,   f5,   e5,   e6,   e7   }, { g8, h7, h5,   g4,   e4,   d5,   d7,   e8   } },
			SquareObj{ g6, { g7,   h7,   h6,   h5,   g5,   f5,   f6,   f7   }, { h8, h4, f4,   e5,   e7,   f8,   None, None } },
			SquareObj{ h6, { h7,   None, None, None, h5,   g5,   g6,   g7   }, { g4, f5, f7,   g8,   None, None, None, None } },

			SquareObj{ a7, { a8,   b8,   b7,   b6,   a6,   None, None, None }, { c8, c6, b5,   None, None, None, None, None } },
			SquareObj{ b7, { b8,   c8,   c7,   c6,   b6,   a6,   a7,   a8   }, { d8, d6, c5,   a5,   None, None, None, None } },
			SquareObj{ c7, { c8,   d8,   d7,   d6,   c6,   b6,   b7,   b8   }, { e8, e6, d5,   b5,   a6,   a8,   None, None } },
			SquareObj{ d7, { d8,   e8,   e7,   e6,   d6,   c6,   c7,   c8   }, { f8, f6, e5,   c5,   b6,   b8,   None, None } },
			SquareObj{ e7, { e8,   f8,   f7,   f6,   e6,   d6,   d7,   d8   }, { g8, g6, f5,   d5,   c6,   c8,   None, None } },
			SquareObj{ f7, { f8,   g8,   g7,   g6,   f6,   e6,   e7,   e8   }, { h8, h6, g5,   e5,   d6,   d8,   None, None } },
			SquareObj{ g7, { g8,   h8,   h7,   h6,   g6,   f6,   f7,   f8   }, { h5, f5, e6,   e8,   None, None, None, None } },
			SquareObj{ h7, { h8,   None, None, None, h6,   g6,   g7,   g8   }, { g5, f6, f8,   None, None, None, None, None } },
			 
			SquareObj{ a8, { None, None, b8,   b7,   a7,   None, None, None }, { c7, b6, None, None, None, None, None, None } },
			SquareObj{ b8, { None, None, c8,   c7,   b7,   a7,   a8,   None }, { d7, c6, a6,   None, None, None, None, None } },
			SquareObj{ c8, { None, None, d8,   d7,   c7,   b7,   b8,   None }, { e7, d6, b6,   a7,   None, None, None, None } },
			SquareObj{ d8, { None, None, e8,   e7,   d7,   c7,   c8,   None }, { f7, e6, c6,   b7,   None, None, None, None } },
			SquareObj{ e8, { None, None, f8,   f7,   e7,   d7,   d8,   None }, { g7, f6, d6,   c7,   None, None, None, None } },
			SquareObj{ f8, { None, None, g8,   g7,   f7,   e7,   e8,   None }, { h7, g6, h5,   e6,   d7,   None, None, None } },
			SquareObj{ g8, { None, None, h8,   h7,   g7,   f7,   f8,   None }, { h6, f6, e7,   None, None, None, None, None } },
			SquareObj{ h8, { None, None, None, None, h7,   g7,   g8,   None }, { g6, f7, None, None, None, None, None, None } }
		};

		friend std::unique_ptr<ObjBoard> make_unique_board(std::string_view fen);
		friend std::shared_ptr<ObjBoard> make_shared_board(std::string_view fen);
	};

	std::unique_ptr<ObjBoard> make_unique_board(std::string_view fen);
	std::shared_ptr<ObjBoard> make_shared_board(std::string_view fen);
} 