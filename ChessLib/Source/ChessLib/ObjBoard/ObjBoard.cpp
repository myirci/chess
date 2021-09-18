#include "ObjBoard.hpp"

#include <ChessLib/Chess/ChessUtility.hpp>

namespace chesslib::objboard
{
	ObjBoard::ObjBoard() { }

	const std::array<SquareObj, 64>& ObjBoard::GetBoard() const { return _squares; }
	std::array<SquareObj, 64>& ObjBoard::GetBoard() { return _squares; }

	const ObjBoard::PieceMap& ObjBoard::GetWhitePieces() const { return _white_pieces;  }
	ObjBoard::PieceMap& ObjBoard::GetWhitePieces() { return _white_pieces; }

	const ObjBoard::PieceMap& ObjBoard::GetBlackPieces() const { return _black_pieces; }
	ObjBoard::PieceMap& ObjBoard::GetBlackPieces() { return _black_pieces; }

	std::vector<Move> ObjBoard::GenerateMoves() const 
	{
		std::vector<Move> moves;
		return moves;
	}

	std::unique_ptr<ObjBoard> make_unique_board(std::string_view fen)
	{
		auto brd = std::unique_ptr<ObjBoard>(new ObjBoard());
		utility::chess::set_board(*brd, fen);
		return brd;
	}

	std::shared_ptr<ObjBoard> make_shared_board(std::string_view fen)
	{
		auto brd = std::shared_ptr<ObjBoard>(new ObjBoard());
		utility::chess::set_board(*brd, fen);
		return brd;
	}
}
