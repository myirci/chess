#pragma once

#include <ChessLib/Board/BoardBase.hpp>
#include <ChessLib/Board/ChessBoard.hpp>

#include <array>
#include <memory>

namespace chesslib { class BoardFactory; }

namespace chesslib::bitboard 
{
	using Bitset = uint64_t;

	constexpr Bitset GetMask(int i)
	{
		uint64_t mask{ 1 };
		return mask << i;
	}

	class BitBoard : 
		public BoardBase,
		public ChessBoard
	{
	public:

		static constexpr Index Pawn{ 0 }, Rook{ 1 }, Knight{ 2 }, Bishop{ 3 }, Queen{ 4 }, King{ 5 }, All { 6 };
		
		const std::array<Bitset, 7>& GetWhitePieceSet() const noexcept { return _white_pieces; }
		const std::array<Bitset, 7>& GetBlackPieceSet() const noexcept { return _black_pieces; }

		void Clear();

		Piece GetPiece(Square s) const;

		template <Color PieceColor>
		void PutPiece(Piece p, Square s)
		{
			auto idx = GetIndexFromPiece<PieceColor>(p);

			if constexpr (PieceColor == color::White)
			{
				_white_pieces[All] |= BitMask[s];
				_white_pieces[idx] |= BitMask[s];
			}
			else
			{
				_black_pieces[All] |= BitMask[s];
				_black_pieces[idx] |= BitMask[s];
			}
		}

		template <Color PieceColor>
		void RemovePiece(Piece p, Square s)
		{
			auto idx = GetIndexFromPiece<PieceColor>(p);

			if constexpr (PieceColor == color::White)
			{
				_white_pieces[All] &= ~BitMask[s];
				_white_pieces[idx] &= ~BitMask[s];
			}
			else 
			{
				_black_pieces[All] &= ~BitMask[s];
				_black_pieces[idx] &= ~BitMask[s];
			}
		}

		template <Color PieceColor>
		void UpdatePiece(Piece p, Square from, Square to)
		{
			RemovePiece<PieceColor>(p, from);
			PutPiece<PieceColor>(p, to);
		}

	protected:

		static constexpr Bitset BitMask[BOARDSIZE]
		{
			GetMask(a1), GetMask(b1), GetMask(c1), GetMask(d1), GetMask(e1), GetMask(f1), GetMask(g1), GetMask(h1),
			GetMask(a2), GetMask(b2), GetMask(c2), GetMask(d2), GetMask(e2), GetMask(f2), GetMask(g2), GetMask(h2),
			GetMask(a3), GetMask(b3), GetMask(c3), GetMask(d3), GetMask(e3), GetMask(f3), GetMask(g3), GetMask(h3),
			GetMask(a4), GetMask(b4), GetMask(c4), GetMask(d4), GetMask(e4), GetMask(f4), GetMask(g4), GetMask(h4),
			GetMask(a5), GetMask(b5), GetMask(c5), GetMask(d5), GetMask(e5), GetMask(f5), GetMask(g5), GetMask(h5),
			GetMask(a6), GetMask(b6), GetMask(c6), GetMask(d6), GetMask(e6), GetMask(f6), GetMask(g6), GetMask(h6),
			GetMask(a7), GetMask(b7), GetMask(c7), GetMask(d7), GetMask(e7), GetMask(f7), GetMask(g7), GetMask(h7),
			GetMask(a8), GetMask(b8), GetMask(c8), GetMask(d8), GetMask(e8), GetMask(f8), GetMask(g8), GetMask(h8)
		};

		std::array<Bitset, 7> _white_pieces;
		std::array<Bitset, 7> _black_pieces;

		BitBoard() noexcept : _white_pieces{0, 0, 0, 0, 0, 0, 0}, _black_pieces{0, 0, 0, 0, 0, 0, 0} { }

		void ClearSquare(Square s);

	private:

		void ClearWhitePieces(Square s);
		void ClearBlackPieces(Square s);

		template<Color PieceColor>
		inline Index GetIndexFromPiece(Piece p) 
		{
			if constexpr (PieceColor == color::White)
				return p - 1;
			else
				return -p - 1;
		}
		
		friend BoardFactory;
	};
}