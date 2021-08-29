#pragma once

#include <array>
#include <unordered_map>
#include <string_view>

#include<Chess/Definitions.hpp>

namespace chesslib 
{
	class Board
	{
	public:
		
		using BoardArray = std::array<Square, 64>;
		using PieceMap = std::unordered_multimap<Piece, Square>;


		Board(std::string_view fen);

		const BoardArray& GetBoard() const;
		const PieceMap& GetWhitePieces() const;
		const PieceMap& GetBlackPieces() const;

		/*
		bool IsKingPositionsAreValid();
		bool IsPawnLocationsAreValid();
		bool GetNumberOfChecksForWhiteKing();
		bool GetNumberOfChecksForBlackKing();
		*/
	private:
		BoardArray board;
		PieceMap white_pieces;
		PieceMap black_pieces;
	};
}


