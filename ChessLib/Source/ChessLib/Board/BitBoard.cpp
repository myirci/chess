#include <ChessLib/Board/BitBoard.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

namespace chesslib::bitboard
{
	void BitBoard::SetSquare(Square s, Piece p) noexcept 
	{
		switch (p) 
		{
		  case pieceset::None:
		  {
			  _white_all_pieces &= ~BitMask[s];
		  }
		  case pieceset::WhitePawn:

		  case pieceset::BlackPawn:
			  break;
		}
	}
}