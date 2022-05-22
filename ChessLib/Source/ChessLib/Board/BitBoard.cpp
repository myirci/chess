#include <ChessLib/Board/BitBoard.hpp>

#include <algorithm>

namespace chesslib::bitboard
{
	void BitBoard::Clear() 
	{
		_white_pieces.fill(0);
		_black_pieces.fill(0);
	}

	Piece BitBoard::GetPiece(Square s) const 
	{
		if (_white_pieces[All] & SquareMask[s]) 
		{
			for (int i = 0; i < 6; i++)
				if (_white_pieces[i] & SquareMask[s])
					return i + 1;
		}
		
		if (_black_pieces[All] & SquareMask[s]) 
		{
			for (int i = 0; i < 6; i++)
				if (_black_pieces[i] & SquareMask[s])
					return -i - 1;
		}

		return Empty;
	}

	void BitBoard::ClearSquare(Square s) 
	{
		ClearWhitePieces(s);
		ClearBlackPieces(s);
	}

	void BitBoard::ClearWhitePieces(Square s) 
	{
		std::for_each(_white_pieces.begin(), _white_pieces.end(), [s](auto& bs) { bs &= ~SquareMask[s]; });
	}

	void BitBoard::ClearBlackPieces(Square s) 
	{
		std::for_each(_black_pieces.begin(), _black_pieces.end(), [s](auto& bs) { bs &= ~SquareMask[s]; });
	}
}