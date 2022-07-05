#include "Connectivity.hpp"

#include <fstream>
#include <iostream>

#include <ChessLib/Board/BasicBoard.hpp>
#include <ChessLib/Chess/BoardColorTraits.hpp>

namespace chesslib 
{
	Connectivity::Connectivity(bool compute /* = true */) :
		_king_indexes{ }, 
		_knight_indexes{ }, 
		_straight_indexes{ }, 
		_diagonal_indexes{ }
	{
		if (compute) 
		{
			ComputeKnightConnections();
			ComputeStraightConnections();
			ComputeDiagonalConnections();
			ComputeKingConnections();
			ComputePawnConnections();
		}
	}

	void Connectivity::ComputeKnightConnections()
	{
		for (Square s = 0; s < 64; s++)
		{
			ConnectionsType connections;
			for (Direction dir : KnightJumps)
				if (BasicBoard::IsInside(s, s + dir))
					connections.push_back(s + dir);

			_knight_connections.push_back((Square)connections.size());
			_knight_indexes[s] = (int16_t)(_knight_connections.size() - 1);
			_knight_connections.insert(_knight_connections.end(), connections.begin(), connections.end());
		}
	}

	void Connectivity::ComputeStraightConnections()
	{
		for (Square s = 0; s < 64; s++)
		{
			std::vector<ConnectionsType> connections(4);
			int i = 0;
			for (Direction dir : StraightDirections)
			{
				Square next = s + dir;
				while (BasicBoard::IsInside(next - dir, next))
				{
					connections[i].push_back(next);
					next += dir;
				}
				i++;
			}

			_straight_indexes[s] = (int16_t)_straight_connections.size();
			for (int j = 0; j < 4; j++)
				_straight_connections.push_back((Square)connections[j].size()); // Number of squares in each direction N, E, S, W

			for (int j = 0; j < 4; j++)
				_straight_connections.insert(_straight_connections.end(), connections[j].begin(), connections[j].end());
		}
	}

	void Connectivity::ComputeDiagonalConnections()
	{
		for (Square s = 0; s < 64; s++)
		{
			std::vector<ConnectionsType> connections(4);
			int i = 0;
			for (Direction dir : DiagonalDirections) // NE, SE, SW, NW
			{
				Square next = s + dir;
				while (BasicBoard::IsInside(next - dir, next))
				{
					connections[i].push_back(next);
					next += dir;
				}
			}

			_diagonal_indexes[s] = (int16_t)_diagonal_connections.size();
			for (int j = 0; j < 4; j++)
				_diagonal_connections.push_back((Square)connections[j].size()); // Number of squares in each direction NE, SE, SW, NW

			for (int j = 0; j < 4; j++)
				_diagonal_connections.insert(_diagonal_connections.end(), connections[j].begin(), connections[j].end());
		}
	}

	void Connectivity::ComputeKingConnections()
	{
		std::uint8_t masks[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };

		for (Square s = 0; s < 64; s++)
		{
			ConnectionsType connections;
			std::uint8_t mask = 0;
			int i = 0;
			for (Direction dir : AllDirections)  // N, NE, E, SE, S, SW, W, NW
			{
				if (BasicBoard::IsInside(s, s + dir))
				{
					mask |= masks[i];
					connections.push_back(s + dir);
				}
				i++;
			}

			_king_connections.push_back(mask);
			_king_indexes[s] = (int16_t)(_king_connections.size() - 1);
			_king_connections.insert(_king_connections.end(), connections.begin(), connections.end());
		}
	}

	void Connectivity::ComputePawnConnections()
	{
		using wbct = traits::board_color_traits<BasicBoard, color::White>;
		using bbct = traits::board_color_traits<BasicBoard, color::Black>;

		std::int8_t white_double_push_mask = 0x01;
		std::int8_t white_promote_mask = 0x02;
		std::int8_t white_left_capture_mask = 0x04;
		std::int8_t white_right_capture_mask = 0x08;

		std::int8_t black_double_push_mask = 0x10;
		std::int8_t black_promote_mask = 0x20;
		std::int8_t black_left_capture_mask = 0x40;
		std::int8_t black_right_capture_mask = 0x80;

		int16_t idx{ -1 };
		for (Square s = 0; s < 64; s++)
		{
			// bit-field
			_pawn_connections.push_back(0);
			int8_t mask{ 0 };

			idx = _pawn_connections.size() - 1;
			_pawn_indexes[s] = idx;
			
			Rank r = ChessBoard::GetRank(s);
			if (r < 7) 
			{
				// white pawn promotion flag
				if (r == wbct::PawnPromotionRank - 1)
					mask |= white_promote_mask;

				// white pawn single push
				if(r == 0)
					_pawn_connections.push_back(Empty);
				else
					_pawn_connections.push_back(s + wbct::PawnMoveDirection);

				// white pawn double push
				if (r == wbct::PawnDoublePushRank)
				{
					mask |= white_double_push_mask;
					_pawn_connections.push_back(s + 2 * wbct::PawnMoveDirection);
				}

				// white pawn captures
				auto next = s + wbct::PawnAttackDirections[0];
				if (BasicBoard::IsInside(s, next))
				{
					_pawn_connections.push_back(next);
					mask |= white_right_capture_mask;
				}

				next = s + wbct::PawnAttackDirections[1];
				if (BasicBoard::IsInside(s, next))
				{
					_pawn_connections.push_back(next);
					mask |= white_left_capture_mask;
				}
			}

			if (r > 0) 
			{
				// black pawn promotion flag
				if (r == bbct::PawnPromotionRank + 1)
					mask |= black_promote_mask;

				// black pawn single push
				if (r == 7)
					_pawn_connections.push_back(Empty);
				else
					_pawn_connections.push_back(s + bbct::PawnMoveDirection);

				// black pawn double push
				if (r == bbct::PawnDoublePushRank)
				{
					mask |= black_double_push_mask;
					_pawn_connections.push_back(s + 2 * bbct::PawnMoveDirection);
				}

				// black pawn captures
				auto next = s + bbct::PawnAttackDirections[0];
				if (BasicBoard::IsInside(s, next))
				{
					_pawn_connections.push_back(next);
					mask |= black_left_capture_mask;
				}

				next = s + bbct::PawnAttackDirections[1];
				if (BasicBoard::IsInside(s, next))
				{
					_pawn_connections.push_back(next);
					mask |= black_right_capture_mask;
				}
			} 
			_pawn_connections[idx] = mask;
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

		of << "King indexes\n";
		for (int i = 0; i < 64; i++)
			of << _king_indexes[i] << " ";
		of << std::endl;

		of << "Values: " << _king_connections.size() << std::endl;
		for (int i = 0; i < _king_connections.size(); i++)
			of << (int)(_king_connections[i]) << " ";
		of << std::endl;

		of << "Knight indexes\n";
		for (int i = 0; i < 64; i++)
			of << _knight_indexes[i] << " ";
		of << std::endl;

		of << "Values: " << _knight_connections.size() << std::endl;
		for (int i = 0; i < _knight_connections.size(); i++)
			of << (int)(_knight_connections[i]) << " ";
		of << std::endl;

		of << "Straigt indexes\n";
		for (int i = 0; i < 64; i++)
			of << _straight_indexes[i] << " ";
		of << std::endl;

		of << "Values: " << _straight_connections.size() << std::endl;
		for (int i = 0; i < _straight_connections.size(); i++)
			of << (int)(_straight_connections[i]) << " ";
		of << std::endl;

		of << "Diagonal indexes\n";
		for (int i = 0; i < 64; i++)
			of << _diagonal_indexes[i] << " ";
		of << std::endl;

		of << "Values: " << _diagonal_connections.size() << std::endl;
		for (int i = 0; i < _diagonal_connections.size(); i++)
			of << (int)(_diagonal_connections[i]) << " ";
		of << std::endl;

		of << "Pawn indexes\n";
		for (int i = 0; i < 64; i++)
			of << _pawn_indexes[i] << " ";
		of << std::endl;

		of << "Values: " << _pawn_connections.size() << std::endl;
		for (int i = 0; i < _pawn_connections.size(); i++)
			of << (int)(_pawn_connections[i]) << " ";
		of << std::endl;

		of.close();
	}
}




