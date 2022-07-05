#pragma once

#include <array>

#include <ChessLib/Board/ChessBoard.hpp>
#include <ChessLib/Board/BasicBoard.hpp>

namespace chesslib 
{
	namespace conn 
	{
		using IndexArrayType          = std::array<int16_t, 64>;
		using KnightConnectionsType   = std::array<Square, 400>;
		using KingConnectionsType     = std::array<Square, 484>;
		using StraightConnectionsType = std::array<Square, 1152>;
		using DiagonalConnectionsType = std::array<Square, 816>;
		using PawnConnectionsType     = std::array<Square, 388>;

		template <Color Clr>
		struct PawnConnectionMasks {};

		template<>
		struct PawnConnectionMasks<color::White>
		{
			static constexpr Square double_push_mask = 0x01;
			static constexpr Square promote_mask = 0x02;
			static constexpr Square left_capture_mask = 0x04;
			static constexpr Square right_capture_mask = 0x08;
		};

		template<>
		struct PawnConnectionMasks<color::Black>
		{
			static constexpr Square double_push_mask = 0x10;
			static constexpr Square promote_mask = 0x20;
			static constexpr Square left_capture_mask = 0x40;
			static constexpr Square right_capture_mask = 0x80;
		};
	}

	namespace details
	{
		static constexpr conn::KnightConnectionsType ComputeKnightConnections()
		{
			conn::KnightConnectionsType con{ };
			
			int start{ 0 }, j{ 0 };
			for (Square s = 0; s < 64; s++)
			{
				j = start + 1;
				for (Direction dir : BasicBoard::KnightJumps)
				{
					if (BasicBoard::IsInside(s, s + dir))
					{
						con[j++] = s + dir;
						con[start]++;
					}
				}
				start = j;
			}
			return con;
		}

		static constexpr conn::KingConnectionsType ComputeKingConnections()
		{
			std::uint8_t masks[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

			conn::KingConnectionsType con{ };
			con.fill(0);

			int start{ 0 }, j{ 0 };
			for (Square s = 0; s < 64; s++)
			{
				j = start + 1;
				int i{ 0 };
				for (Direction dir : BasicBoard::AllDirections) 
				{
					if (BasicBoard::IsInside(s, s + dir)) 
					{
						con[j++] = s + dir;
						con[start] |= masks[i];
					}
					i++;
				}
				start = j;
			}
			return con;
		}
	
		static constexpr conn::StraightConnectionsType ComputeStraightConnections()
		{
			conn::StraightConnectionsType con{ };
			con.fill(0);

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
						con[j++] = next;
						con[i] = con[i] + 1;
						next += dir;
					}
					i++;
				}
			}
			return con;
		}

		static constexpr conn::DiagonalConnectionsType ComputeDiagonalConnections()
		{
			conn::DiagonalConnectionsType con{ };
			con.fill(0);

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
						con[j++] = next;
						con[i] = con[i] + 1;
						next += dir;
					}
				}
				i++;
			}

			return con;
		}
	
		static constexpr conn::PawnConnectionsType ComputePawnConnections()
		{
			conn::PawnConnectionsType con{ };

			using wbct = traits::board_color_traits<BasicBoard, color::White>;
			using bbct = traits::board_color_traits<BasicBoard, color::Black>;
			using wpc = conn::PawnConnectionMasks<color::White>;
			using bpc = conn::PawnConnectionMasks<color::Black>;

			Square next{ Empty };
			int16_t idx{ 0 }, midx{ 0 };
			for (Square s = 0; s < 64; s++)
			{
				Rank r = ChessBoard::GetRank(s);
				midx = idx;
				con[idx++] = 0;
				int8_t mask{ 0 };

				if (r < 7) 
				{
					// white pawn promotion flag
					if (r == wbct::PawnPromotionRank - 1)
						mask |= wpc::promote_mask;

					// white pawn single push
					if (r == 0)
						con[idx++] = Empty;
					else
						con[idx++] = s + wbct::PawnMoveDirection;

					// white pawn double push
					if (r == wbct::PawnDoublePushRank)
					{
						mask |= wpc::double_push_mask;
						con[idx++] = s + 2 * wbct::PawnMoveDirection;
					}

					// white pawn captures
					next = s + wbct::PawnAttackDirections[0];
					if (BasicBoard::IsInside(s, next))
					{
						con[idx++] = next;
						mask |= wpc::right_capture_mask;
					}

					next = s + wbct::PawnAttackDirections[1];
					if (BasicBoard::IsInside(s, next))
					{
						con[idx++] = next;
						mask |= wpc::left_capture_mask;
					}
				}

				if (r > 0) 
				{
					// black pawn promotion flag
					if (r == bbct::PawnPromotionRank + 1)
						mask |= bpc::promote_mask;

					// black pawn single push
					if (r == 7)
						con[idx++] = Empty;
					else
						con[idx++] = (s + bbct::PawnMoveDirection);

					// black pawn double push
					if (r == bbct::PawnDoublePushRank)
					{
						mask |= bpc::double_push_mask;
						con[idx++] = (s + 2 * bbct::PawnMoveDirection);
					}

					// black pawn captures
					next = s + bbct::PawnAttackDirections[0];
					if (BasicBoard::IsInside(s, next))
					{
						con[idx++] = next;
						mask |= bpc::left_capture_mask;
					}

					next = s + bbct::PawnAttackDirections[1];
					if (BasicBoard::IsInside(s, next))
					{
						con[idx++] = next;
						mask |= bpc::right_capture_mask;
					}
				}

				con[midx] = mask;
			}

			return con;
		}
	}
	
	class Connections 
	{
	public:
		
		// The first entry for the square is the number of knight connections.
		// The following entires are the connected squares with the knight jump.
		static constexpr conn::KnightConnectionsType KnightConnections = details::ComputeKnightConnections();

		// The first entry is a bitfield, indicating whether a connection is available through the direction.
		static constexpr conn::KingConnectionsType KingConnections = details::ComputeKingConnections();

		// The first four entries are the number of staright connections in each direction.
		// The following entries are the connected squares through stright moving. 
		static constexpr conn::StraightConnectionsType StraightConnections = details::ComputeStraightConnections();

		// The first four entries are the number of staright connections in each direction.
		// The following entries are the connected squares through stright moving. 
		static constexpr conn::DiagonalConnectionsType DiagonalConnections = details::ComputeDiagonalConnections();

		// The first entry is a bitfield, indicating double pawn push, promotion and captures.
		// One square forward is implict and the square connected through single push follows the bitfield.
		static constexpr conn::PawnConnectionsType PawnConnections = details::ComputePawnConnections();

		// Computed using the Connectivity class in the Benchmark project.
		static constexpr conn::IndexArrayType KnightSquareIndexes =
		{
			0, 3, 7, 12, 17, 22, 27, 31, 34, 38, 43, 50, 57, 64, 71, 76, 80, 85, 92, 101, 110, 119, 128, 135, 140,
			145, 152, 161, 170, 179, 188, 195, 200, 205, 212, 221, 230, 239, 248, 255, 260, 265, 272, 281, 290, 299,
			308, 315, 320, 324, 329, 336, 343, 350, 357, 362, 366, 369, 373, 378, 383, 388, 393, 397
		};

		static constexpr conn::IndexArrayType KingSquareIndexes =
		{
			0, 4, 10, 16, 22, 28, 34, 40, 44, 50, 59, 68, 77, 86, 95, 104, 110, 116, 125, 134, 143, 152, 161, 170, 176,
			182, 191, 200, 209, 218, 227, 236, 242, 248, 257, 266, 275, 284, 293, 302, 308, 314, 323, 332, 341, 350, 359,
			368, 374, 380, 389, 398, 407, 416, 425, 434, 440, 444, 450, 456, 462, 468, 474, 480
		};

		static constexpr conn::IndexArrayType StraightSquareIndexes =
		{
			0, 18, 36, 54, 72, 90, 108, 126, 144, 162, 180, 198, 216, 234, 252, 270, 288, 306, 324, 342, 360, 378, 396,
			414, 432, 450, 468, 486, 504, 522, 540, 558, 576, 594, 612, 630, 648, 666, 684, 702, 720, 738, 756, 774, 792, 
			810, 828, 846, 864, 882, 900, 918, 936, 954, 972, 990, 1008, 1026, 1044, 1062, 1080, 1098, 1116, 1134
		};

		static constexpr conn::IndexArrayType DiagonalSquareIndexes =
		{
			0, 11, 22, 33, 44, 55, 66, 77, 88, 99, 112, 125, 138, 151, 164, 177, 188, 199, 212, 227, 242, 257, 272, 285,
			296, 307, 320, 335, 352, 369, 384, 397, 408, 419, 432, 447, 464, 481, 496, 509, 520, 531, 544, 559, 574, 589,
			604, 617, 628, 639, 652, 665, 678, 691, 704, 717, 728, 739, 750, 761, 772, 783, 794, 805
		};

		static constexpr conn::IndexArrayType PawnSquareIndexes =
		{
			0, 3, 7, 11, 15, 19, 23, 27, 30, 36, 44, 52, 60, 68, 76, 84, 90, 95, 102, 109, 116, 123, 130, 137, 142, 147, 154,
			161, 168, 175, 182, 189, 194, 199, 206, 213, 220, 227, 234, 241, 246, 251, 258, 265, 272, 279, 286, 293, 298, 304,
			312, 320, 328, 336, 344, 352, 358, 361, 365, 369, 373, 377, 381, 385
		};
	};
}