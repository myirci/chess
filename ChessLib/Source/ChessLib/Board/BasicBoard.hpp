#pragma once

#include <ChessLib/Board/ChessBoard.hpp>
#include <ChessLib/Board/PieceCentricBoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/ColorTraits.hpp>
#include <ChessLib/Chess/BoardColorTraits.hpp>

#include <array>
#include <string_view>
#include <memory>
#include <vector>

namespace chesslib
{
	class BoardFactory;

	// One dimensional array of 64 squares to represent 8x8 board board.
	// Not optimal for move generation: off-board piece detection is expensive to compute.
	class BasicBoard :
		public ChessBoard,
		public PieceCentricBoardBase
	{
	public: 

		using BoardArray = std::array<Square, BOARDSIZE>;

		static constexpr bool IsInside(Square curr, Square next) noexcept
		{
			return next < BOARDSIZE && next >= 0 && std::abs(GetFile(next) - GetFile(curr)) <= 2;
		}

		const BoardArray& GetBoard() const noexcept { return _board; }
		BoardArray&		  GetBoard() noexcept	    { return _board; }
		
		Piece GetPiece(Square s) const              { return _board[s]; }

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

		BasicBoard() noexcept : PieceCentricBoardBase() { _board.fill(Empty); }

		friend BoardFactory;
	};
}