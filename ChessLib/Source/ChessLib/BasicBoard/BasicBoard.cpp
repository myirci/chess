#include <ChessLib/BasicBoard/BasicBoard.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

namespace chesslib::basic_board
{
	BasicBoard::BasicBoard() : board{} { }

	const BasicBoard::BoardArray& BasicBoard::GetBoard() const { return board; }

	BasicBoard::BoardArray& BasicBoard::GetBoard() { return board; }

	std::vector<Move> BasicBoard::GenerateMoves() const 
	{
		std::vector<Move> moves;
		if(_active_color == color::White)
			GenerateMoves<color::White>(moves);
		else
			GenerateMoves<color::Black>(moves);
		return moves;
	}

	template <Color SideToMove>
	void BasicBoard::GenerateMoves(std::vector<Move>& moves) const
	{
			
	}

	template <Color SideToMove>
	Square BasicBoard::GetKingPosition() const
	{
		if constexpr (SideToMove == color::White) 
		{
			auto res = _white_pieces.find(pieceset::WhiteKing);
		}
		else 
		{
			auto res = _black_pieces.find(pieceset::BlackKing);
		}
			
	}
	

	std::unique_ptr<BasicBoard> make_unique_board(std::string_view fen) 
	{
		auto brd = std::unique_ptr<BasicBoard>(new BasicBoard());
		utility::chess::set_board(*brd, fen);
		return brd;
	}
	
	std::shared_ptr<BasicBoard> make_shared_board(std::string_view fen) 
	{
		auto brd = std::shared_ptr<BasicBoard>(new BasicBoard());
		utility::chess::set_board(*brd, fen);
		return brd;
	}
}