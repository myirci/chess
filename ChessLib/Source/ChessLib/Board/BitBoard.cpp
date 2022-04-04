#include <ChessLib/Board/BitBoard.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

namespace chesslib::bitboard
{
	void BitBoard::SetPiece(Piece p, Square s)
	{
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

	Piece BitBoard::GetPiece(Square s) const 
	{
		if (_white_pieces[All] & BitMask[s]) 
		{
			for (int i = 0; i < 6; i++)
				if (_white_pieces[i] & BitMask[s])
					return i + 1;
		}
		
		if (_black_pieces[All] & BitMask[s]) 
		{
			for (int i = 0; i < 6; i++)
				if (_black_pieces[i] & BitMask[s])
					return -i - 1;
		}

		return Empty;
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