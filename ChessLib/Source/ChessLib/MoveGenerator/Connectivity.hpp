#pragma once

#include <Chesslib/Board/ChessBoard.hpp>
#include <vector>

namespace chesslib 
{
	class Connectivity : public ChessBoard
	{
	public:

		using IndexArrayType = std::array<int16_t, 64>;
		using ConnectionsType = std::vector<Square>;

		Connectivity(bool compute = true);

		void ComputeKnightConnections();
		void ComputeStraightConnections();
		void ComputeDiagonalConnections();
		void ComputeKingConnections();
		void ComputePawnConnections();
		void Export(std::string_view file_path);

		const IndexArrayType& GetKnightIndexes() const   { return _knight_indexes; }
		const IndexArrayType& GetKingIndexes() const     { return _king_indexes; }
		const IndexArrayType& GetStraightIndexes() const { return _straight_indexes; }
		const IndexArrayType& GetDiagonalIndexes() const { return _diagonal_indexes; }
		const IndexArrayType& GetPawnIndexes() const     { return _pawn_indexes; }

		const ConnectionsType& GetKnightConnections() const   { return _knight_connections; }
		const ConnectionsType& GetKingConnections() const     { return _king_connections; }
		const ConnectionsType& GetStraightConnections() const { return _straight_connections; }
		const ConnectionsType& GetDiagonalConnections() const { return _diagonal_connections; }
		const ConnectionsType& GetPawnConnections() const	  { return _pawn_connections; }

	private:
		ConnectionsType _straight_connections;
		ConnectionsType _diagonal_connections;
		ConnectionsType _knight_connections;
		ConnectionsType _king_connections;
		ConnectionsType _pawn_connections;

		IndexArrayType  _straight_indexes;
		IndexArrayType  _diagonal_indexes;
		IndexArrayType  _knight_indexes;
		IndexArrayType  _king_indexes;
		IndexArrayType  _pawn_indexes;
	};
}