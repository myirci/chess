#include <ChessLib/Board/BitBoard.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

namespace chesslib::bitboard
{
	void BitBoard::SetSquare(Square s, Piece p) noexcept 
	{
		ClearSquare(s);

		if (p == pieceset::None)
			return;

		if (color::get_color(p) == color::White) 
		{
			_white_pieces[All] |= BitMask[s];
			_white_pieces[(size_t)(p - 1)] |= BitMask[s];
		}
		else 
		{
			_black_pieces[All] |= BitMask[s];
			_black_pieces[(size_t)(- p - 1)] |= BitMask[s];
		}
	}

	void BitBoard::ClearSquare(Square s) noexcept 
	{
		ClearWhitePieces(s);
		ClearBlackPieces(s);
	}

	void BitBoard::ClearWhitePieces(Square s) noexcept 
	{
		std::for_each(_white_pieces.begin(), _white_pieces.end(), [s](auto& bs) { bs &= ~BitMask[s]; });
	}

	void BitBoard::ClearBlackPieces(Square s) noexcept 
	{
		std::for_each(_black_pieces.begin(), _black_pieces.end(), [s](auto& bs) { bs &= ~BitMask[s]; });
	}
}