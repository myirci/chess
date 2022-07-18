#pragma once

#include <Chesslib/Board/ChessBoard.hpp>
#include <vector>

namespace chesslib 
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
}