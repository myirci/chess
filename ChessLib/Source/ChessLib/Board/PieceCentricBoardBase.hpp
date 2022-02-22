#pragma once

#include <ChessLib/Board/BoardBase.hpp>
#include <stdexcept>

namespace chesslib 
{
	class PieceCentricBoardBase : public BoardBase
	{
	public:

		using PieceMap = std::unordered_multimap<Piece, Square>;
		using EqualPieceRange = std::pair<PieceMap::const_iterator, PieceMap::const_iterator>;

		const PieceMap& GetWhitePieces() const noexcept { return _white_pieces; }
		PieceMap& GetWhitePieces() noexcept			    { return _white_pieces; }
		const PieceMap& GetBlackPieces() const noexcept { return _black_pieces; }
		PieceMap& GetBlackPieces() noexcept			    { return _black_pieces; }

	protected:

		PieceCentricBoardBase() noexcept : BoardBase() { }

		PieceMap _white_pieces;
		PieceMap _black_pieces;

		template <Color Clr>
		Square GetKingPosition() const
		{
			if constexpr (Clr == color::White)
			{
				auto it = _white_pieces.find(pieceset::WhiteKing);
				if (it == _white_pieces.end())
					throw std::logic_error("White king could not be found");
				return it->second;
			}
			else
			{
				auto it = _black_pieces.find(pieceset::BlackKing);
				if (it == _black_pieces.end())
					throw std::logic_error("Black king could not be found");
				return it->second;
			}
		}

		template <Color Clr>
		EqualPieceRange GetPiecePositions(Piece p) const
		{
			if constexpr (Clr == color::White)
				return _white_pieces.equal_range(p);
			else
				return _black_pieces.equal_range(p);
		}

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
				if (first->second == current_pos)
				{
					first->second = new_pos;
					return;
				}
			}

			throw std::logic_error("Piece in the given position could not be found.");
		}

		template <Color Clr>
		void RemovePiece(Piece p, Square current_pos)
		{
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

			throw std::logic_error("Piece in the given position could not be found.");
		}

		template <Color Clr>
		void AddNewPiece(Piece p, Square pos)
		{
			if constexpr (Clr == color::White)
				_white_pieces.emplace(p, pos);
			else
				_black_pieces.emplace(p, pos);
		}
	};
}
