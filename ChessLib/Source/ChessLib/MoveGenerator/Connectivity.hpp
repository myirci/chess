#pragma once

#include <Chesslib/Board/ChessBoard.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <vector>

namespace chesslib::precompute 
{
	class Connectivity : public ChessBoard
	{
	public:

		struct ConnectionIndexes
		{
			ConnectionIndexes(Direction d) : dir{ d }, _indexes_start{}, _indexes_end{} { }
			Direction dir;
			std::array<int16_t, 64> _indexes_start;
			std::array<int16_t, 64> _indexes_end;
		};

		Connectivity(bool compute = true);

		void Compute();
		void Export(std::string_view file_path);

		const std::vector<Square>& GetConnections() const       { return _connections; }
		const std::vector<ConnectionIndexes> GetIndexes() const { return _indexes; }
		
	private:

		std::vector<Square> _connections;
		std::vector<ConnectionIndexes> _indexes;
	};

	class PrecomputeMoves : public ChessBoard
	{
	public:
		
		struct MoveIndexes
		{
			MoveIndexes(const std::vector<Piece>& pieces) : 
				_pieces{ pieces }, _indexes_start{ }, _indexes_end{ } 
			{
				_indexes_start.fill(Empty);
				_indexes_end.fill(Empty);
			}
			std::vector<Piece> _pieces;
			std::array<int16_t, 64> _indexes_start;
			std::array<int16_t, 64> _indexes_end;
		};

		PrecomputeMoves();
		void ComputeRookMoves();
		void ComputeBishopMoves();
		void ComputeKnightMoves();
		void ComputeKingMoves();
		void ComputeWhitePawnMoves();
		void ComputeBlackPawnMoves();
		void Compute();

		void Export_PieceMoves(const std::string& folder_path);
		void Export_MovesAndIndexes(const std::string_view& file_path);

		const std::vector<Move>& GetRookMoves() const  { return _rook_moves; }
		const std::vector<Move>& GetBishopMoves() const { return _bishop_moves; }
		const std::vector<Move>& GetKnightMoves() const { return _knight_moves; }
		const std::vector<Move>& GetKingMoves() const { return _king_moves; }
		const std::vector<Move>& GetWhitePawnMoves() const { return _white_pawn_moves; }
		const std::vector<Move>& GetBlackPawnMoves() const { return _black_pawn_moves; }
		const std::vector<Move>& GetMoves() const { return _moves; }
		const std::vector<MoveIndexes>& GetIndexes() const { return _indexes; }
	
	private:

		std::vector<Move> _rook_moves;
		std::vector<Move> _bishop_moves;
		std::vector<Move> _knight_moves;
		std::vector<Move> _king_moves;
		std::vector<Move> _white_pawn_moves;
		std::vector<Move> _black_pawn_moves;

		std::vector<Move> _moves;
		std::vector<MoveIndexes> _indexes;

		static void Export(std::string_view file_path, std::vector<Move>& moves);

	};
}