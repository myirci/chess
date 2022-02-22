#pragma once

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Board/BoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Board/ObjBoard/PieceObj.hpp>
#include <ChessLib/Board/ObjBoard/SquareObj.hpp>

#include <array>
#include <memory>
#include <vector>

namespace chesslib::objboard
{
	class ObjBoard : public BoardBase  
	{
	public:
		constexpr static int BOARDSIZE = 64;
		using SquareArray = std::array<SquareObj, BOARDSIZE>;

		using PiecePtr = std::shared_ptr<PieceObj>;
		using PieceMap = std::unordered_multimap<Piece, PiecePtr>;

		const SquareArray& GetBoard() const	   { return _squares; }
		SquareArray& GetBoard()				   { return _squares; }
		const PieceMap& GetWhitePieces() const { return _white_pieces; }
		PieceMap& GetWhitePieces()			   { return _white_pieces; }
		const PieceMap& GetBlackPieces() const { return _black_pieces; }
		PieceMap& GetBlackPieces()			   { return _black_pieces; }

		std::vector<Move> GenerateMoves() const;
	
	protected:

		ObjBoard();

		PieceMap _white_pieces;
		PieceMap _black_pieces;
		SquareArray _squares;

		friend std::unique_ptr<ObjBoard> make_unique_board(std::string_view fen);
		friend std::shared_ptr<ObjBoard> make_shared_board(std::string_view fen);
	};

	std::unique_ptr<ObjBoard> make_unique_board(std::string_view fen);
	std::shared_ptr<ObjBoard> make_shared_board(std::string_view fen);
} 