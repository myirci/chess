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

		static constexpr Bitset BitMask[BOARDSIZE]
		{
			GetMask(squareset::a1), GetMask(squareset::b1), GetMask(squareset::c1), GetMask(squareset::d1),
			GetMask(squareset::e1), GetMask(squareset::f1), GetMask(squareset::g1), GetMask(squareset::h1),
			GetMask(squareset::a2), GetMask(squareset::b2), GetMask(squareset::c2), GetMask(squareset::d2),
			GetMask(squareset::e2), GetMask(squareset::f2), GetMask(squareset::g2), GetMask(squareset::h2),
			GetMask(squareset::a3), GetMask(squareset::b3), GetMask(squareset::c3), GetMask(squareset::d3),
			GetMask(squareset::e3), GetMask(squareset::f3), GetMask(squareset::g3), GetMask(squareset::h3),
			GetMask(squareset::a4), GetMask(squareset::b4), GetMask(squareset::c4), GetMask(squareset::d4),
			GetMask(squareset::e4), GetMask(squareset::f4), GetMask(squareset::g4), GetMask(squareset::h4),
			GetMask(squareset::a5), GetMask(squareset::b5), GetMask(squareset::c5), GetMask(squareset::d5),
			GetMask(squareset::e5), GetMask(squareset::f5), GetMask(squareset::g5), GetMask(squareset::h5),
			GetMask(squareset::a6), GetMask(squareset::b6), GetMask(squareset::c6), GetMask(squareset::d6),
			GetMask(squareset::e6), GetMask(squareset::f6), GetMask(squareset::g6), GetMask(squareset::h6),
			GetMask(squareset::a7), GetMask(squareset::b7), GetMask(squareset::c7), GetMask(squareset::d7),
			GetMask(squareset::e7), GetMask(squareset::f7), GetMask(squareset::g7), GetMask(squareset::h7),
			GetMask(squareset::a8), GetMask(squareset::b8), GetMask(squareset::c8), GetMask(squareset::d8),
			GetMask(squareset::e8), GetMask(squareset::f8), GetMask(squareset::g8), GetMask(squareset::h8)
		};

		static constexpr Index Pawn{ 0 }, Rook{ 1 }, Knight{ 2 }, Bishop{ 3 }, Queen{ 4 }, King{ 5 }, All { 6 };

		const std::array<Bitset, 7>& GetWhitePieceSet() const noexcept { return _white_pieces; }
		const std::array<Bitset, 7>& GetBlackPieceSet() const noexcept { return _black_pieces; }	

		void SetSquare(Square s, Piece p) noexcept;

	protected:

		std::array<Bitset, 7> _white_pieces;
		std::array<Bitset, 7> _black_pieces;

		BitBoard() noexcept : _white_pieces{0, 0, 0, 0, 0, 0}, _black_pieces{0, 0, 0, 0, 0, 0} { }

		void ClearSquare(Square s) noexcept;
		void ClearWhitePieces(Square s) noexcept;
		void ClearBlackPieces(Square s) noexcept;
		
		friend BoardFactory;
	};
}