#include "Connectivity.hpp"

#include <fstream>
#include <iostream>

#include <ChessLib/Board/BasicBoard.hpp>
#include <ChessLib/Chess/BoardColorTraits.hpp>

namespace chesslib 
{
	Connectivity::Connectivity(bool compute /* = true */)
	{
		if (compute)
			Compute();
	}
	
	void Connectivity::Compute() 
	{
		// per direction - rays
		{
			for (Direction dir : AllDirections)  // N, NE, E, SE, S, SW, W, NW
			{
				auto indexes = ConnectionIndexes(dir);

				for (Square s = 0; s < 64; s++)
				{
					indexes._indexes_start[s] = Empty;
					indexes._indexes_end[s] = Empty;

					std::vector<Square> connections;
					Square next = s + dir;
					while (BasicBoard::IsInside(next - dir, next))
					{
						connections.push_back(next);
						next += dir;
					}

					if (!connections.empty())
					{
						indexes._indexes_start[s] = (int16_t)_connections.size();
						_connections.insert(_connections.end(), connections.begin(), connections.end());
						indexes._indexes_end[s] = (int16_t)_connections.size();
					}
				}

				_indexes.push_back(indexes);
			}
		}
		
		// knight_connections, per-square
		{
			auto indexes = ConnectionIndexes(Empty);
			for (Square s = 0; s < 64; s++)
			{
				std::vector<Square> connections;
				for (Direction dir : KnightJumps) 
				{
					if (BasicBoard::IsInside(s, s + dir))
						connections.push_back(s + dir);
				}

				indexes._indexes_start[s] = (int16_t)_connections.size();
				_connections.insert(_connections.end(), connections.begin(), connections.end());
				indexes._indexes_end[s] = (int16_t)_connections.size();
			}
			_indexes.push_back(indexes);
		}

		// per direction single-square
		{
			auto indexes = ConnectionIndexes(Empty);
			for (Square s = 0; s < 64; s++)
			{
				std::vector<Square> connections;
				for (Direction dir : AllDirections)  // N, NE, E, SE, S, SW, W, NW
				{
					if (BasicBoard::IsInside(s, s + dir)) 
						connections.push_back(s + dir);
					else
						connections.push_back(Empty);
				}

				indexes._indexes_start[s] = (int16_t)_connections.size();
				_connections.insert(_connections.end(), connections.begin(), connections.end());
				// No need to store the end_index as we store 8 neighbors for each square.
			}
			_indexes.push_back(indexes);
		}

		// Pawn connections
		{
			static constexpr Square promotion_flag = -99;

			// white pawn-connections
			{
				using wbct = traits::board_color_traits<BasicBoard, color::White>;
				auto indexes = ConnectionIndexes(Empty);
				for (Square s = 0; s < 64; s++)
				{
					std::vector<Square> connections;
					indexes._indexes_start[s] = Empty;
					Rank r = ChessBoard::GetRank(s);

					Square next = s + wbct::PawnMoveDirection;
					if (BasicBoard::IsInside(s, next))
					{
						// white pawn single push
						connections.push_back(next);

						// white pawn double push / white pawn promotion flag
						if (r == wbct::PawnDoublePushRank)
							connections.push_back(next + wbct::PawnMoveDirection);
						else if (r == wbct::PawnPromotionRank - 1)
							connections.push_back(promotion_flag);
						else
							connections.push_back(Empty);

						// white pawn captures
						for (int i = 0; i < 2; i++)
						{
							next = s + wbct::PawnAttackDirections[i];
							if (BasicBoard::IsInside(s, next))
								connections.push_back(next);
							else
								connections.push_back(Empty);
						}

						indexes._indexes_start[s] = (int16_t)_connections.size();
						_connections.insert(_connections.end(), connections.begin(), connections.end());
						// No need to store the end_index as we store 4 squares for each square.
					}
				}
				_indexes.push_back(indexes);
			}

			// black pawn-connections
			{
				using bbct = traits::board_color_traits<BasicBoard, color::Black>;

				auto indexes = ConnectionIndexes(Empty);
				for (Square s = 0; s < 64; s++)
				{
					std::vector<Square> connections;

					Rank r = ChessBoard::GetRank(s);
					indexes._indexes_start[s] = Empty;
					Square next = s + bbct::PawnMoveDirection;
					if (BasicBoard::IsInside(s, next))
					{
						// black pawn single push
						connections.push_back(next);

						// black pawn double push / black pawn promotion flag
						if (r == bbct::PawnDoublePushRank)
							connections.push_back(next + bbct::PawnMoveDirection);
						else if (r == bbct::PawnPromotionRank + 1)
							connections.push_back(promotion_flag);
						else
							connections.push_back(Empty);

						// black pawn captures
						for (int i = 0; i < 2; i++)
						{
							next = s + bbct::PawnAttackDirections[i];
							if (BasicBoard::IsInside(s, next))
								connections.push_back(next);
							else
								connections.push_back(Empty);
						}

						indexes._indexes_start[s] = (int16_t)_connections.size();
						_connections.insert(_connections.end(), connections.begin(), connections.end());
						// No need to store the end_index as we store 4 squares for each square.
					}
				}
				_indexes.push_back(indexes);
			}
		}
	}

	void Connectivity::Export(std::string_view file_path)
	{
		std::ofstream of(file_path.data(), std::ios::out);
		if (!of.is_open())
		{
			std::cout << "File could not be opened" << std::endl;
			return;
		}

		of << "Connections: " << _connections.size() << std::endl;
		for (Square s : _connections)
			of << (int)s << " ";
		of << std::endl;
		of << std::endl;

		of << "Indexes: " << std::endl;

		std::vector<std::string> dirNames
		{
			"North", "North-East", "East", "South-East", "South", "South-West", "West", "North-West",
			"Knight-Connections", "Single-square", "White-pawns", "Black-pawns"
		};

		for (int i = 0; i < dirNames.size(); i++) 
		{
			of << "\t" << dirNames[i] << ": " << (int)_indexes[i].dir << std::endl;
			of << "\t\tStart: " << std::endl;
			for (int16_t idx : _indexes[i]._indexes_start)
				of << idx << " ";
			of << std::endl;

			of << "\t\tEnd: " << std::endl;
			for (int16_t idx : _indexes[i]._indexes_end)
				of << idx << " ";
			of << std::endl;
		}
		of.close();
	}
}




