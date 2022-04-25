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

		const BoardArray& GetBoard() const noexcept			{ return _board; }
		BoardArray& GetBoard() noexcept						{ return _board; }

		const PieceMap& GetWhitePieces() const noexcept		{ return _white_pieces; }
		PieceMap& GetWhitePieces() noexcept					{ return _white_pieces; }
		
		const PieceMap& GetBlackPieces() const noexcept		{ return _black_pieces; }
		PieceMap& GetBlackPieces() noexcept					{ return _black_pieces; }

		Piece GetPiece(Square s) const { return _board[s]._piece ? _board[s]._piece->_code : Empty; }

		template <Color PieceColor>
		void PutPiece(Piece p, Square s)
		{
			auto pobj = make_piece(p, s);
			_board[s]._piece = pobj.get();

			if constexpr (PieceColor == color::White)
				_white_pieces.emplace(p, std::move(pobj));
			else
				_black_pieces.emplace(p, std::move(pobj));
		}

		template <Color PieceColor>
		void RemovePiece(Piece p, Square s)
		{
			PieceMap::iterator first, last;
			if constexpr (PieceColor == color::White)
				std::tie(first, last) = _white_pieces.equal_range(p);
			else
				std::tie(first, last) = _black_pieces.equal_range(p);

			for (; first != last; first++)
			{
				if (first->second->_loc == s)
				{
					if constexpr (PieceColor == color::White)
						_white_pieces.erase(first);
					else
						_black_pieces.erase(first);
					break;
				}
			}

			_board[s]._piece = nullptr;
		}

		template <Color PieceColor>
		void UpdatePiece(Piece p, Square from, Square to)
		{
			PieceMap::iterator first, last;
			if constexpr (PieceColor == color::White)
				std::tie(first, last) = _white_pieces.equal_range(p);
			else
				std::tie(first, last) = _black_pieces.equal_range(p);

			for (; first != last; first++)
			{
				if (first->second->_loc == from)
				{
					first->second->_loc = to;
					_board[to]._piece = first->second.get();
					_board[from]._piece = nullptr;
					return;
				}
			}

			throw std::logic_error("Piece could not be found in the given position.");
		}
	
	protected:

		ObjBoard();

		PieceMap _white_pieces;
		PieceMap _black_pieces;
		BoardArray _board;

		friend BoardFactory;
	};
} 