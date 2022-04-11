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
#include <stdexcept>

namespace chesslib { class BoardFactory; }

namespace chesslib::objboard
{
	class ObjBoard :
		public ChessBoard,
		public BoardBase  
	{
	public:
		
		using BoardArray = std::array<SquareObj, BOARDSIZE>;
		using PieceMap = std::unordered_multimap<Piece, std::unique_ptr<PieceObj>>;

		const BoardArray& GetBoard() const noexcept { return _board; }
		BoardArray& GetBoard() noexcept { return _board; }
		const PieceMap& GetWhitePieces() const noexcept { return _white_pieces; }
		PieceMap& GetWhitePieces() noexcept { return _white_pieces; }
		const PieceMap& GetBlackPieces() const noexcept { return _black_pieces; }
		PieceMap& GetBlackPieces() noexcept { return _black_pieces; }

		void SetPiece(Piece p, Square s);
		Piece GetPiece(Square s) const { return _board[s]._piece ? _board[s]._piece->_code : Empty; }

		template<Color SideToMove>
		void MakeQuiteMove(Square from, Square to)
		{
			UpdatePiecePosition<SideToMove>(_board[from]._piece, from, to);
			_board[to]._piece = _board[from]._piece;
			_board[from]._piece = nullptr;
		}

		template <Color Clr>
		void RemovePiece(Piece p, Square current_pos)
		{
		/*	if (_squares[current_pos]._piece->_code != p)
				throw std::logic_error("Piece could not be found in the given position.");*/


			/*
			PieceMap::iterator first, last;
			if constexpr (Clr == color::White)
				std::tie(first, last) = _white_pieces.equal_range(p);
			else
				std::tie(first, last) = _black_pieces.equal_range(p);

			for (; first != last; first++)
			{
				if (first->second == current_pos)
				{
					if constexpr (Clr == color::White)
						_white_pieces.erase(first);
					else
						_black_pieces.erase(first);
					return;
				}
			}
			*/

			
		}
	
	protected:

		ObjBoard();

		PieceMap _white_pieces;
		PieceMap _black_pieces;
		BoardArray _board;

		template <Color Clr>
		void UpdatePiecePosition(Piece p, Square current_pos, Square new_pos)
		{
			PieceMap::iterator first, last;
			if constexpr (Clr == color::White)
				std::tie(first, last) = _white_pieces.equal_range(p);
			else
				std::tie(first, last) = _black_pieces.equal_range(p);

			for (; first != last; first++)
			{
				if (first->second->_loc == current_pos)
				{
					first->second->_loc = new_pos;
					return;
				}
			}

			throw std::logic_error("Piece in the given position could not be found.");
		}

		friend BoardFactory;
	};
} 