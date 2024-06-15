#pragma once

#include <array>
#include <ChessLib/Board/ChessBoard.hpp>
#include <ChessLib/Board/BasicBoard.hpp>

namespace chesslib::conn 
{
	using ConnectionsType = std::array<Square, 2772>; // Required array size is precomputed.
	using IndexArrayType = std::array<int16_t, 64>;
	using IndicesArray = std::array<IndexArrayType, 22>;
	using ConnectionsAndIndicesPair = std::pair<ConnectionsType, IndicesArray>;

	using NeighborSquares = std::array<Square, 16>;

	inline constexpr Direction KnightJumps = -101;
	inline constexpr Direction AllDir = -102;
	inline constexpr Direction WPDir = -103;
	inline constexpr Direction BPDir = -104;

	inline constexpr int NSI = 0;   // North Start Index
	inline constexpr int NEI = 1;   // North End Index
	inline constexpr int NESI = 2;  // North-East Start Index
	inline constexpr int NEEI = 3;  // North-East End Index
	inline constexpr int ESI = 4;   // East Start Index
	inline constexpr int EEI = 5;   // East End Index
	inline constexpr int SESI = 6;  // South-East Start Index
	inline constexpr int SEEI = 7;  // South-East End Index
	inline constexpr int SSI = 8;   // South Start Index
	inline constexpr int SEI = 9;   // South End Index
	inline constexpr int SWSI = 10; // South-West Start Index
	inline constexpr int SWEI = 11; // South-West End Index
	inline constexpr int WSI = 12;  // West Start Index
	inline constexpr int WEI = 13;  // West End Index
	inline constexpr int NWSI = 14; // North-West Start Index
	inline constexpr int NWEI = 15; // North-West End Index
	inline constexpr int KCSI = 16; // Knight Connections Start Index
	inline constexpr int KCEI = 17; // Knight Connections End Index
	inline constexpr int ADSI = 18; // All Directions Start Index
	inline constexpr int ADEI = 19; // All Directions Start Index
	inline constexpr int WPSI = 20; // White Pawn Connections Start Index
	inline constexpr int BPSI = 21; // Black Pawn Connections Start Index

	// Traits for selecting pawn connections based on color
	template <Color Clr> struct PawnDirectionTrait {};
	template <> struct PawnDirectionTrait<color::White> { static constexpr Direction Dir = WPDir; };
	template <> struct PawnDirectionTrait<color::Black> { static constexpr Direction Dir = BPDir; };

	namespace details
	{
		static constexpr NeighborSquares GetDirectNeighbors(Square s)
		{
			NeighborSquares con{};
			con.fill(Empty);

			int i = 0;
			for (Direction dir : BasicBoard::AllDirections)
			{
				if (BasicBoard::IsInside(s, s + dir))
					con[i] = s + dir;
				i++;
			}

			for (Direction dir : BasicBoard::KnightJumps)
			{
				if (BasicBoard::IsInside(s, s + dir))
					con[i] = s + dir;
				i++;
			}

			return con;
		}

		static constexpr ConnectionsAndIndicesPair ComputeConnectionsAndIndices()
		{
			ConnectionsAndIndicesPair cp{};

			int idx1{ 0 }, idx2{ 1 }, i{ 0 }, start_idx{ 0 };
			Square next{ Empty };

			// per direction - rays
			for (Direction dir : ChessBoard::AllDirections)  // N, NE, E, SE, S, SW, W, NW
			{
				for (Square s = 0; s < ChessBoard::BOARDSIZE; s++)
				{
					cp.second[idx1][s] = (int16_t)Empty;
					cp.second[idx2][s] = (int16_t)Empty;
					start_idx = i;

					next = s + dir;
					while (BasicBoard::IsInside(next - dir, next))
					{
						cp.first[i++] = next;
						next += dir;
					}

					if (start_idx != i)
					{
						cp.second[idx1][s] = (int16_t)start_idx;
						cp.second[idx2][s] = (int16_t)i;
					}
				}
				idx1 += 2;
				idx2 = idx1 + 1;
			}

			// knight_connections, per-square
			for (Square s = 0; s < ChessBoard::BOARDSIZE; s++)
			{
				cp.second[idx1][s] = (int16_t)i;

				for (Direction dir : ChessBoard::KnightJumps)
				{
					if (BasicBoard::IsInside(s, s + dir))
						cp.first[i++] = s + dir;
				}

				cp.second[idx2][s] = (int16_t)i;
			}
			idx1 += 2;
			idx2 = idx1 + 1;

			// per direction single-square
			for (Square s = 0; s < ChessBoard::BOARDSIZE; s++)
			{
				cp.second[idx1][s] = (int16_t)i;

				for (Direction dir : ChessBoard::AllDirections)  // N, NE, E, SE, S, SW, W, NW
				{
					if (BasicBoard::IsInside(s, s + dir))
						cp.first[i++] = s + dir;
				}

				cp.second[idx2][s] = (int16_t)i;
			}
			idx1 += 2;

			// white pawn-connections
			using wbct = traits::board_color_traits<BasicBoard, color::White>;
			for (Square s = 0; s < 64; s++)
			{
				cp.second[idx1][s] = (int16_t)Empty;

				Rank r = ChessBoard::GetRank(s);

				Square next = s + wbct::PawnMoveDirection;
				if (BasicBoard::IsInside(s, next))
				{
					cp.second[idx1][s] = (int16_t)i;

					// rank
					cp.first[i++] = r;

					// white pawn single push
					cp.first[i++] = next;

					// white pawn double push
					next += wbct::PawnMoveDirection;
					if (BasicBoard::IsInside(s, next))
						cp.first[i] = next;
					else
						cp.first[i] = Empty;
					i++;

					// white pawn captures
					for (int j = 0; j < 2; j++)
					{
						cp.first[i] = Empty;
						next = s + wbct::PawnAttackDirections[j];
						if (BasicBoard::IsInside(s, next))
							cp.first[i] = next;
						i++;
					}
				}
			}
			idx1++;

			// black pawn-connections
			using bbct = traits::board_color_traits<BasicBoard, color::Black>;
			for (Square s = 0; s < ChessBoard::BOARDSIZE; s++)
			{
				cp.second[idx1][s] = (int16_t)Empty;

				Rank r = ChessBoard::GetRank(s);

				Square next = s + bbct::PawnMoveDirection;
				if (BasicBoard::IsInside(s, next))
				{
					cp.second[idx1][s] = (int16_t)i;

					// rank
					cp.first[i++] = r;

					// black pawn single push
					cp.first[i++] = next;

					// black pawn double push
					next += bbct::PawnMoveDirection;
					if (BasicBoard::IsInside(s, next))
						cp.first[i] = next;
					else
						cp.first[i] = Empty;
					i++;

					// black pawn captures
					for (int j = 0; j < 2; j++)
					{
						cp.first[i] = Empty;
						next = s + bbct::PawnAttackDirections[j];
						if (BasicBoard::IsInside(s, next))
							cp.first[i] = next;
						i++;
					}
				}
			}
			return cp;
		}
	}

	inline constexpr ConnectionsAndIndicesPair ConnectionsAndIndices = details::ComputeConnectionsAndIndices();

	template <Direction Dir>
	class View
	{
	public:
		View(Square s);
		inline int8_t GetSize() const { return size; }
		inline Square Next() { return ConnectionsAndIndices.first[currIdx++]; }
		inline Square Next(int8_t advance)
		{
			currIdx += advance;
			return ConnectionsAndIndices.first[currIdx];
		}

	private:
		int8_t size;
		int16_t currIdx;
	};

	template <Direction Dir>
	View<Dir>::View(Square s) : size{ 0 }, currIdx{ 0 }
	{
		if constexpr (Dir == ChessBoard::N)
		{
			currIdx = ConnectionsAndIndices.second[NSI][s];
			size = static_cast<int8_t>(ConnectionsAndIndices.second[NEI][s] - currIdx);
		}
		else if constexpr (Dir == ChessBoard::NE)
		{
			currIdx = ConnectionsAndIndices.second[NESI][s];
			size = static_cast<int8_t>(ConnectionsAndIndices.second[NEEI][s] - currIdx);
		}
		else if constexpr (Dir == ChessBoard::E)
		{
			currIdx = ConnectionsAndIndices.second[ESI][s];
			size = static_cast<int8_t>(ConnectionsAndIndices.second[EEI][s] - currIdx);
		}
		else if constexpr (Dir == ChessBoard::SE)
		{
			currIdx = ConnectionsAndIndices.second[SESI][s];
			size = static_cast<int8_t>(ConnectionsAndIndices.second[SEEI][s] - currIdx);
		}
		if constexpr (Dir == ChessBoard::S)
		{
			currIdx = ConnectionsAndIndices.second[SSI][s];
			size = static_cast<int8_t>(ConnectionsAndIndices.second[SEI][s] - currIdx);
		}
		else if constexpr (Dir == ChessBoard::SW)
		{
			currIdx = ConnectionsAndIndices.second[SWSI][s];
			size = static_cast<int8_t>(ConnectionsAndIndices.second[SWEI][s] - currIdx);
		}
		else if constexpr (Dir == ChessBoard::W)
		{
			currIdx = ConnectionsAndIndices.second[WSI][s];
			size = static_cast<int8_t>(ConnectionsAndIndices.second[WEI][s] - currIdx);
		}
		else if constexpr (Dir == ChessBoard::NW)
		{
			currIdx = ConnectionsAndIndices.second[NWSI][s];
			size = static_cast<int8_t>(ConnectionsAndIndices.second[NWEI][s] - currIdx);
		}
		else if constexpr (Dir == conn::KnightJumps)
		{
			// Knight Jumps
			currIdx = ConnectionsAndIndices.second[KCSI][s];
			size = static_cast<int8_t>(ConnectionsAndIndices.second[KCEI][s] - currIdx);
		}
		else if constexpr (Dir == AllDir)
		{
			// All Directions
			currIdx = ConnectionsAndIndices.second[ADSI][s];
			size = static_cast<int8_t>(ConnectionsAndIndices.second[ADEI][s] - currIdx);
		}
		else if constexpr (Dir == WPDir)
		{
			// White Pawn Connections
			currIdx = ConnectionsAndIndices.second[WPSI][s];
			size = currIdx != Empty ? 5 : 0;
		}
		else if constexpr (Dir == BPDir)
		{
			// Black Pawn Connections
			currIdx = ConnectionsAndIndices.second[BPSI][s];
			size = currIdx != Empty ? 5 : 0;
		}
	}
}