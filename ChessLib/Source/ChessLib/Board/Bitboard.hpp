#pragma once

#include <ChessLib/Board/BoardBase.hpp>
#include <ChessLib/Board/ChessBoard.hpp>

#include <array>
#include <memory>

namespace chesslib { class BoardFactory; }

namespace chesslib::bitboard 
{
	using Bitset = uint64_t;

	constexpr Bitset GetSquareMask(Square s)
	{
		uint64_t mask{ 1 };
		return mask << s;
	}
	
	constexpr Bitset GetRankMask(Rank r) 
	{
		uint64_t mask{ 255 };
		return mask << (r * 8);
	}

	constexpr Bitset GetFileMask(File f) 
	{
		uint64_t mask{ 0x0101010101010101 };
		return mask << f;
	}

	class BitBoard : 
		public BoardBase,
		public ChessBoard
	{
	public:

		static constexpr Index Pawn{ 0 }, Rook{ 1 }, Knight{ 2 }, Bishop{ 3 }, Queen{ 4 }, King{ 5 }, All { 6 };
		
		static constexpr Bitset SquareMask[BOARDSIZE]
		{
			GetSquareMask(a1), GetSquareMask(b1), GetSquareMask(c1), GetSquareMask(d1), GetSquareMask(e1), GetSquareMask(f1), GetSquareMask(g1), GetSquareMask(h1),
			GetSquareMask(a2), GetSquareMask(b2), GetSquareMask(c2), GetSquareMask(d2), GetSquareMask(e2), GetSquareMask(f2), GetSquareMask(g2), GetSquareMask(h2),
			GetSquareMask(a3), GetSquareMask(b3), GetSquareMask(c3), GetSquareMask(d3), GetSquareMask(e3), GetSquareMask(f3), GetSquareMask(g3), GetSquareMask(h3),
			GetSquareMask(a4), GetSquareMask(b4), GetSquareMask(c4), GetSquareMask(d4), GetSquareMask(e4), GetSquareMask(f4), GetSquareMask(g4), GetSquareMask(h4),
			GetSquareMask(a5), GetSquareMask(b5), GetSquareMask(c5), GetSquareMask(d5), GetSquareMask(e5), GetSquareMask(f5), GetSquareMask(g5), GetSquareMask(h5),
			GetSquareMask(a6), GetSquareMask(b6), GetSquareMask(c6), GetSquareMask(d6), GetSquareMask(e6), GetSquareMask(f6), GetSquareMask(g6), GetSquareMask(h6),
			GetSquareMask(a7), GetSquareMask(b7), GetSquareMask(c7), GetSquareMask(d7), GetSquareMask(e7), GetSquareMask(f7), GetSquareMask(g7), GetSquareMask(h7),
			GetSquareMask(a8), GetSquareMask(b8), GetSquareMask(c8), GetSquareMask(d8), GetSquareMask(e8), GetSquareMask(f8), GetSquareMask(g8), GetSquareMask(h8)
		};

		static constexpr Bitset RankMask[8]
		{
			GetRankMask(Rank1), GetRankMask(Rank2), GetRankMask(Rank3), GetRankMask(Rank4),
			GetRankMask(Rank5), GetRankMask(Rank6), GetRankMask(Rank7), GetRankMask(Rank8)
		};

		static constexpr Bitset FileMask[8]
		{
			GetFileMask(FileA), GetFileMask(FileB), GetFileMask(FileC), GetFileMask(FileD),
			GetFileMask(FileE), GetFileMask(FileF), GetFileMask(FileG), GetFileMask(FileH)
		};

		const std::array<Bitset, 7>& GetWhitePieceSet() const noexcept { return _white_pieces; }
		const std::array<Bitset, 7>& GetBlackPieceSet() const noexcept { return _black_pieces; }

		void Clear();

		Piece GetPiece(Square s) const;

		static Square GetSquareFromMask(Bitset mask) { return BottomToTopOrder[(std::size_t)std::log2(mask)]; }

		template <Color PieceColor>
		void PutPiece(Piece p, Square s)
		{
			auto idx = GetIndexFromPiece<PieceColor>(p);

			if constexpr (PieceColor == color::White)
			{
				_white_pieces[All] |= SquareMask[s];
				_white_pieces[idx] |= SquareMask[s];
			}
			else
			{
				_black_pieces[All] |= SquareMask[s];
				_black_pieces[idx] |= SquareMask[s];
			}
		}

		template <Color PieceColor>
		void RemovePiece(Piece p, Square s)
		{
			auto idx = GetIndexFromPiece<PieceColor>(p);

			if constexpr (PieceColor == color::White)
			{
				_white_pieces[All] &= ~SquareMask[s];
				_white_pieces[idx] &= ~SquareMask[s];
			}
			else 
			{
				_black_pieces[All] &= ~SquareMask[s];
				_black_pieces[idx] &= ~SquareMask[s];
			}
		}

		template <Color PieceColor>
		void UpdatePiece(Piece p, Square from, Square to)
		{
			RemovePiece<PieceColor>(p, from);
			PutPiece<PieceColor>(p, to);
		}

		template <Color PieceColor>
		Square GetKingPosition() const
		{
			if constexpr (PieceColor == color::White)
				return GetSquareFromMask(_white_pieces[King]);
			else
				return GetSquareFromMask(_black_pieces[King]);
		}

	protected:

		
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