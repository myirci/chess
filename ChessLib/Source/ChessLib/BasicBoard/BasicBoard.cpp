#include <ChessLib/BasicBoard/BasicBoard.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

namespace chesslib::basic_board
{
	BasicBoard::BasicBoard() : board{} { }

	const BasicBoard::BoardArray& BasicBoard::GetBoard() const { return board; }

	BasicBoard::BoardArray& BasicBoard::GetBoard() { return board; }

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