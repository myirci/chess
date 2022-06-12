#pragma once

#include <array>

#include <ChessLib/Board/ChessBoard.hpp>
#include <ChessLib/Board/BasicBoard.hpp>

namespace chesslib 
{
	using IndexArrayType = std::array<int16_t, 64>;
	using KnightConnectionsType = std::array<Square, 400>;
	using KingConnectionsType = std::array<Square, 484>;
	using StraightConnectionsType = std::array<Square, 1152>;
	using DiagonalConnectionsType = std::array<Square, 816>;

	namespace details
	{
		static constexpr KnightConnectionsType ComputeKnightConnections()
		{
			KnightConnectionsType conn{ };
			
			int start{ 0 }, j{ 0 };
			for (Square s = 0; s < 64; s++)
			{
				j = start + 1;
				for (Direction dir : BasicBoard::KnightJumps)
				{
					if (BasicBoard::IsInside(s, s + dir))
					{
						conn[j++] = s + dir;
						conn[start]++;
					}
				}
				start = j;
			}
			return conn;
		}

		static constexpr KingConnectionsType ComputeKingConnections()
		{
			std::uint8_t masks[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };

			KingConnectionsType conn{ };
			conn.fill(0);

			int start{ 0 }, j{ 0 };
			for (Square s = 0; s < 64; s++)
			{
				j = start + 1;
				int i{ 0 };
				for (Direction dir : BasicBoard::AllDirections) 
				{
					if (BasicBoard::IsInside(s, s + dir)) 
					{
						conn[j++] = s + dir;
						conn[start] |= masks[i];
					}
					i++;
				}
				start = j;
			}
			return conn;
		}
	
		static constexpr StraightConnectionsType ComputeStraightConnections() 
		{
			StraightConnectionsType conn{ };
			conn.fill(0);

			int j{ 0 };
			for (Square s = 0; s < 64; s++)
			{
				int i = j;
				j += 4;
				for (Direction dir : BasicBoard::StraightDirections)
				{
					Square next = s + dir;
					while (BasicBoard::IsInside(next - dir, next))
					{
						conn[j++] = next;
						conn[i] = conn[i] + 1;
						next += dir;
					}
					i++;
				}
			}
			return conn;
		}

		static constexpr DiagonalConnectionsType ComputeDiagonalConnections()
		{
			DiagonalConnectionsType conn{ };
			conn.fill(0);

			int j{ 0 };
			for (Square s = 0; s < 64; s++)
			{
				int i = j;
				j += 4;
				for (Direction dir : BasicBoard::DiagonalDirections)
				{
					Square next = s + dir;
					while (BasicBoard::IsInside(next - dir, next))
					{
						conn[j++] = next;
						conn[i] = conn[i] + 1;
						next += dir;
					}
				}
				i++;
			}

			return conn;
		}
	}
	
	class Connections 
	{
	public:
		
		static constexpr KnightConnectionsType KnightConnections = details::ComputeKnightConnections();

		static constexpr KingConnectionsType KingConnections = details::ComputeKingConnections();

		static constexpr StraightConnectionsType StraightConnections = details::ComputeStraightConnections();

		static constexpr DiagonalConnectionsType DiagonalConnections = details::ComputeDiagonalConnections();

		// Computed using the Connectivity class in the Benchmark project.
		static constexpr IndexArrayType KnightSquareIndexes =
		{
			0, 3, 7, 12, 17, 22, 27, 31, 34, 38, 43, 50, 57, 64, 71, 76, 80, 85, 92, 101, 110, 119, 128, 135, 140,
			145, 152, 161, 170, 179, 188, 195, 200, 205, 212, 221, 230, 239, 248, 255, 260, 265, 272, 281, 290, 299,
			308, 315, 320, 324, 329, 336, 343, 350, 357, 362, 366, 369, 373, 378, 383, 388, 393, 397
		};

		static constexpr IndexArrayType KingSquareIndexes =
		{
			0, 4, 10, 16, 22, 28, 34, 40, 44, 50, 59, 68, 77, 86, 95, 104, 110, 116, 125, 134, 143, 152, 161, 170, 176,
			182, 191, 200, 209, 218, 227, 236, 242, 248, 257, 266, 275, 284, 293, 302, 308, 314, 323, 332, 341, 350, 359,
			368, 374, 380, 389, 398, 407, 416, 425, 434, 440, 444, 450, 456, 462, 468, 474, 480
		};

		static constexpr IndexArrayType StraightSquareIndexes =
		{
			0, 18, 36, 54, 72, 90, 108, 126, 144, 162, 180, 198, 216, 234, 252, 270, 288, 306, 324, 342, 360, 378, 396,
			414, 432, 450, 468, 486, 504, 522, 540, 558, 576, 594, 612, 630, 648, 666, 684, 702, 720, 738, 756, 774, 792, 
			810, 828, 846, 864, 882, 900, 918, 936, 954, 972, 990, 1008, 1026, 1044, 1062, 1080, 1098, 1116, 1134
		};

		static constexpr IndexArrayType DiagonalSquareIndexes =
		{
			0, 11, 22, 33, 44, 55, 66, 77, 88, 99, 112, 125, 138, 151, 164, 177, 188, 199, 212, 227, 242, 257, 272, 285,
			296, 307, 320, 335, 352, 369, 384, 397, 408, 419, 432, 447, 464, 481, 496, 509, 520, 531, 544, 559, 574, 589,
			604, 617, 628, 639, 652, 665, 678, 691, 704, 717, 728, 739, 750, 761, 772, 783, 794, 805
		};
	
	};
}