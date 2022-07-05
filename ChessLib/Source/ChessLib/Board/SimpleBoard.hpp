#pragma once

#include <ChessLib/Board/ChessBoard.hpp>
#include <ChessLib/Board/PieceCentricBoardBase.hpp>

namespace chesslib 
{
	class BoardFactory;

	class SimpleBoard : 
		public ChessBoard,
		public PieceCentricBoardBase
	{
	public:
		using BoardArray = std::array<Square, BOARDSIZE>;

		const BoardArray& GetBoard() const noexcept { return _board; }
		BoardArray& GetBoard() noexcept				{ return _board; }

		inline Piece GetPiece(Square s) const		{ return _board[s]; }

		template <Color PieceColor>
		void PutPiece(Piece p, Square s)
		{
			PieceCentricBoardBase::PutPiece<PieceColor>(p, s);
			_board[s] = p;
		}

		template <Color PieceColor>
		void RemovePiece(Piece p, Square s)
		{
			PieceCentricBoardBase::RemovePiece<PieceColor>(p, s);
			_board[s] = Empty;
		}

		template <Color PieceColor>
		void UpdatePiece(Piece p, Square from, Square to)
		{
			PieceCentricBoardBase::UpdatePiece<PieceColor>(p, from, to);
			_board[to] = p;
			_board[from] = Empty;
		}

	protected:

		BoardArray _board;

		SimpleBoard() noexcept : PieceCentricBoardBase() { _board.fill(Empty); }

		friend BoardFactory;
	};
}