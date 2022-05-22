#pragma once

#include <ChessLib/Chess/Definitions.hpp>

namespace chesslib::movegenerator 
{
	using Ray = std::vector<Square>;
	using Moveables = std::pair<std::vector<Piece>, Ray>;

	struct Edge 
	{
		Square _from;
		// Moveable _to;
	};

	struct Vertex 
	{
		std::string _name;
		Square id;
		// std::vector<>

	};

	class Graph
	{

	};

	class BoardConnectivity 
	{
		std::unordered_multimap<Square, std::vector<Square>> _king_connections;
		std::unordered_multimap<Square, std::vector<Square>> _diagonal_connections;
		std::unordered_multimap<Square, std::vector<Square>> _straight_connections;
		std::unordered_multimap<Square, std::vector<Square>> _knight_connections;
		std::unordered_multimap<Square, std::vector<Square>> _wpawn_connections;
		std::unordered_multimap<Square, std::vector<Square>> _bpawn_connections;

		void ComputeKnightConnections() 
		{
			for (int i = 0; i < 64; i++)
			{
				Square from = basic_board::bottom_to_top_order[i];
				for (auto dir : basic_board::direction::KnightJumps)
				{
					if (Square to = from + dir; basic_board::BasicBoard::IsInside(from, to))
					{
						_knight_connections[from] = to;
					}
				}
			}
		}
		
	};
}