#include <ChessLib/X88Board/x88Board.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

namespace chesslib::x88board
{
	Board::Board() : board{} { }

	const Board::BoardArray& Board::GetBoard() const { return board; }

	Board::BoardArray& Board::GetBoard() { return board; }
	
	std::unique_ptr<Board> make_unique_x88board(std::string_view fen)
	{
		auto brd = std::unique_ptr<Board>(new Board());
		utility::chess::set_board(*brd, fen);
		return brd;
	}

	std::shared_ptr<Board> make_shared_x88board(std::string_view fen)
	{
		auto brd = std::shared_ptr<Board>(new Board());
		utility::chess::set_board(*brd, fen);
		return brd;
	}
}
