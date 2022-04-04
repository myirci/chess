#pragma once

#include <ChessLib/Board/ChessBoard.hpp>
#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Board/BoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Board/PieceObj.hpp>
#include <ChessLib/Board/SquareObj.hpp>

#include <array>
#include <memory>
#include <vector>

namespace chesslib { class BoardFactory; }

namespace chesslib::objboard
{
	class ObjBoard :
		public ChessBoard,
		public BoardBase  
	{
	public:
		
		using SquareArray = std::array<SquareObj, BOARDSIZE>;
		using PieceMap = std::unordered_multimap<Piece, std::unique_ptr<PieceObj>>;

		void SetPiece(Piece p, Square s);
		Piece GetPiece(Square s) const { return _squares[s]._piece ? _squares[s]._piece->_code : Empty; }

		const SquareArray& GetBoard() const noexcept    { return _squares; }
		SquareArray& GetBoard() noexcept		        { return _squares; }
		const PieceMap& GetWhitePieces() const noexcept { return _white_pieces; }
		PieceMap& GetWhitePieces() noexcept			    { return _white_pieces; }
		const PieceMap& GetBlackPieces() const noexcept { return _black_pieces; }
		PieceMap& GetBlackPieces() noexcept	           	{ return _black_pieces; }

		// std::vector<Move> GenerateMoves() const;
	
	protected:

		ObjBoard();

		PieceMap _white_pieces;
		PieceMap _black_pieces;
		SquareArray _squares;

		friend BoardFactory;
	};
} 