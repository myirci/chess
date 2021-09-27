#pragma once

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Chess/BoardBase.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/ObjBoard/PieceObj.hpp>
#include <ChessLib/ObjBoard/SquareObj.hpp>

#include <array>
#include <memory>
#include <vector>

namespace chesslib::objboard
{
	class ObjBoard : public BoardBase  
	{
	public:
		using SquareArray = std::array<SquareObj, 64>;
		using PiecePtr = std::shared_ptr<PieceObj>;
		using PieceMap = std::unordered_multimap<Piece, PiecePtr>;

		const SquareArray& GetBoard() const;
		SquareArray& GetBoard();

		const PieceMap& GetWhitePieces() const;
		PieceMap& GetWhitePieces();

		const PieceMap& GetBlackPieces() const;
		PieceMap& GetBlackPieces();

		std::vector<Move> GenerateMoves() const;
	
	protected:

		ObjBoard();

		PieceMap _white_pieces;
		PieceMap _black_pieces;

		std::array<SquareObj, 64> _squares;

		friend std::unique_ptr<ObjBoard> make_unique_board(std::string_view fen);
		friend std::shared_ptr<ObjBoard> make_shared_board(std::string_view fen);
	};

	std::unique_ptr<ObjBoard> make_unique_board(std::string_view fen);
	std::shared_ptr<ObjBoard> make_shared_board(std::string_view fen);
} 