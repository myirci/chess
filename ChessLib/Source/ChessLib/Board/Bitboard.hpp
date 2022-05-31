#pragma once

#include <ChessLib/Board/BoardBase.hpp>
#include <ChessLib/Board/ChessBoard.hpp>

#include <array>
#include <memory>

namespace chesslib { class BoardFactory; }

namespace chesslib::bitboard 
{
	using Bitset = uint64_t;

	inline constexpr uint64_t debruijn_seq = 0x218A392CD3D5DBF;

	// for bitpos 0 : 63
	// debruijn_index_table [(debrujin_seq << bitpos) >> 58] = bitpos;
	inline constexpr Index debruijn_index_table[64] =
	{
		0,  1,   2,  7,  3, 13,  8, 19,  4, 25, 14, 28,  9, 34, 20, 40,
		5,  17, 26, 38, 15, 46, 29, 48, 10, 31, 35, 54, 21, 50, 41, 57,
		63, 6,  12, 18, 24, 27, 33, 39, 16, 37, 45, 47, 30, 53, 49, 56,
		62, 11, 23, 32, 36, 44, 52, 55, 61, 22, 43, 51, 60, 42, 59, 58
	};

	constexpr Square GetSquareFromBitSet(Bitset b) { return debruijn_index_table[(b * debruijn_seq) >> 58]; }

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
		
		#define	A(n) GetSquareMask(n), GetSquareMask(n+1), GetSquareMask(n+2), GetSquareMask(n+3), GetSquareMask(n+4), GetSquareMask(n+5), GetSquareMask(n+6), GetSquareMask(n+7)
		static constexpr Bitset SquareMask[BOARDSIZE] { A(a1), A(a2), A(a3), A(a4), A(a5), A(a6), A(a7), A(a8) };

		#define B(n) GetRankMask(n)
		static constexpr Bitset RankMask[8] { B(Rank1), B(Rank2), B(Rank3), B(Rank4), B(Rank5), B(Rank6), B(Rank7), B(Rank8) };

		#define C(n) GetFileMask(n)
		static constexpr Bitset FileMask[8] { C(FileA), C(FileB), C(FileC), C(FileD), C(FileE), C(FileF), C(FileG), C(FileH) };

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
				return GetSquareFromBitSet(_white_pieces[King]);
			else
				return GetSquareFromBitSet(_black_pieces[King]);
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