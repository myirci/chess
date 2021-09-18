#include <ChessLib/X88Board/x88Board.hpp>
#include <ChessLib/Chess/ChessUtility.hpp>

namespace chesslib::x88board
{
	x88Board::x88Board() : board{} { }

	const x88Board::BoardArray& x88Board::GetBoard() const { return board; }

	x88Board::BoardArray& x88Board::GetBoard() { return board; }
	
	std::vector<Move> x88Board::GenerateMoves() const 
	{
		std::vector<Move> moves;
		return moves;
	}

	std::unique_ptr<x88Board> make_unique_board(std::string_view fen)
	{
		auto brd = std::unique_ptr<x88Board>(new x88Board());
		utility::chess::set_board(*brd, fen);
		return brd;
	}

	std::shared_ptr<x88Board> make_shared_board(std::string_view fen)
	{
		auto brd = std::shared_ptr<x88Board>(new x88Board());
		utility::chess::set_board(*brd, fen);
		return brd;
	}
}
