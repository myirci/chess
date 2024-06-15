#pragma once

#include <array>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Board/ChessBoard.hpp>
#include <ChessLib/Board/BasicBoard.hpp>

namespace chesslib::precomputed_moves 
{
	// Required array size is precomputed using the PrecomputeMoves moves class.
	using MoveArrayType = std::array<Move, 4868>;
	using IndexArrayType = std::array<int16_t, 64>;
	using IndicesArray = std::array<IndexArrayType, 12>;
	using MovesAndIndicesPair = std::pair<MoveArrayType, IndicesArray>;

	inline constexpr int SSI = 0;   // Straight Start Index
	inline constexpr int SEI = 1;   // Straight End Index
	inline constexpr int DSI = 2;   // Diagonal Start Index
	inline constexpr int DEI = 3;   // Diagonal End Index
	inline constexpr int NSI = 4;   // Knight Start Index
	inline constexpr int NEI = 5;   // Knight End Index
	inline constexpr int KSI = 6;   // King Start Index
	inline constexpr int KEI = 7;   // King End Index
	inline constexpr int WSI = 8;   // White Pawn Start Index
	inline constexpr int WEI = 9;   // White Pawn End Index
	inline constexpr int SWSI = 10; // Black Pawn Start Index
	inline constexpr int SWEI = 11; // Black Pawn End Index
	
	namespace details 
	{
		static constexpr MovesAndIndicesPair ComputeMovesAndIndices()
		{
			MovesAndIndicesPair mip{};
			int si{ 0 }, ei{ 1 }, mi{ 0 }, start_idx{ 0 };
			Square next{ Empty };

			// Straight moves
			for (Square s = 0; s < ChessBoard::BOARDSIZE; s++)
			{
				start_idx = mi;
				for (Direction dir : ChessBoard::StraightDirections)
				{
					next = s + dir;
					while (BasicBoard::IsInside(next - dir, next))
					{
						mip.first[mi].SetFrom(s);
						mip.first[mi].SetTo(next);
						mip.first[mi++].SetMoveType(MoveType::Quite);

						mip.first[mi].SetFrom(s);
						mip.first[mi].SetTo(next);
						mip.first[mi++].SetMoveType(MoveType::Capture);

						next += dir;
					}
				}

				mip.second[si][s] = start_idx;
				mip.second[ei][s] = mi;
			}
			si += 2;
			ei = si + 1;
			
			// Diagonal moves
			for (Square s = 0; s < ChessBoard::BOARDSIZE; s++)
			{
				start_idx = mi;
				for (Direction dir : ChessBoard::DiagonalDirections)
				{
					next = s + dir;
					while (BasicBoard::IsInside(next - dir, next))
					{
						mip.first[mi].SetFrom(s);
						mip.first[mi].SetTo(next);
						mip.first[mi++].SetMoveType(MoveType::Quite);

						mip.first[mi].SetFrom(s);
						mip.first[mi].SetTo(next);
						mip.first[mi++].SetMoveType(MoveType::Capture);

						next += dir;
					}
				}

				mip.second[si][s] = start_idx;
				mip.second[ei][s] = mi;
			}
			si += 2;
			ei = si + 1;

			// Knight moves
			for (Square s = 0; s < ChessBoard::BOARDSIZE; s++)
			{
				start_idx = mi;

				for (Direction dir : ChessBoard::KnightJumps)
				{
					next = s + dir;
					if (BasicBoard::IsInside(s, next))
					{
						mip.first[mi].SetFrom(s);
						mip.first[mi].SetTo(next);
						mip.first[mi++].SetMoveType(MoveType::Quite);

						mip.first[mi].SetFrom(s);
						mip.first[mi].SetTo(next);
						mip.first[mi++].SetMoveType(MoveType::Capture);
					}
				}

				mip.second[si][s] = start_idx;
				mip.second[ei][s] = mi;
			}
			si += 2;
			ei = si + 1;

			// King moves
			for (Square s = 0; s < ChessBoard::BOARDSIZE; s++)
			{
				start_idx = mi;

				for (Direction dir : ChessBoard::AllDirections)
				{
					next = s + dir;
					if (BasicBoard::IsInside(s, next))
					{
						mip.first[mi].SetFrom(s);
						mip.first[mi].SetTo(next);
						mip.first[mi++].SetMoveType(MoveType::Quite);

						mip.first[mi].SetFrom(s);
						mip.first[mi].SetTo(next);
						mip.first[mi++].SetMoveType(MoveType::Capture);
					}
				}

				if (s == ChessBoard::e1)
				{
					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(ChessBoard::g1);
					mip.first[mi++].SetMoveType(MoveType::King_Side_Castle);

					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(ChessBoard::c1);
					mip.first[mi++].SetMoveType(MoveType::Queen_Side_Castle);
				}
				else if (s == ChessBoard::e8)
				{
					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(ChessBoard::g8);
					mip.first[mi++].SetMoveType(MoveType::King_Side_Castle);

					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(ChessBoard::c8);
					mip.first[mi++].SetMoveType(MoveType::Queen_Side_Castle);
				}

				mip.second[si][s] = start_idx;
				mip.second[ei][s] = mi;
			}
			si += 2;
			ei = si + 1;

			// White pawn moves
			using wbct = traits::board_color_traits<BasicBoard, color::White>;
			for (Square s = 0; s < ChessBoard::BOARDSIZE; s++)
			{
				start_idx = mi;

				Rank r = ChessBoard::GetRank(s);
				if (r == 0 || r == 7) 
				{
					mip.second[si][s] = Empty;
					mip.second[ei][s] = Empty;
					continue;
				}
				
				next = s + wbct::PawnMoveDirection;
				if (r == 1)
				{
					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(next);
					mip.first[mi++].SetMoveType(MoveType::Quite);
					
					next += wbct::PawnMoveDirection;

					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(next);
					mip.first[mi++].SetMoveType(MoveType::Double_Pawn_Push);
				}
				else if (r == 6)
				{
					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(next);
					mip.first[mi++].SetMoveType(MoveType::Queen_Promotion);

					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(next);
					mip.first[mi++].SetMoveType(MoveType::Rook_Promotion);

					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(next);
					mip.first[mi++].SetMoveType(MoveType::Knight_Promotion);
					
					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(next);
					mip.first[mi++].SetMoveType(MoveType::Bishop_Promotion);
				}
				else
				{
					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(next);
					mip.first[mi++].SetMoveType(MoveType::Quite);
				}

				for (int i = 0; i < 2; i++)
				{
					next = s + wbct::PawnAttackDirections[i];
					if (BasicBoard::IsInside(s, next))
					{
						if (r == 6)
						{
							mip.first[mi].SetFrom(s);
							mip.first[mi].SetTo(next);
							mip.first[mi++].SetMoveType(MoveType::Queen_Promotion_Capture);

							mip.first[mi].SetFrom(s);
							mip.first[mi].SetTo(next);
							mip.first[mi++].SetMoveType(MoveType::Rook_Promotion_Capture);

							mip.first[mi].SetFrom(s);
							mip.first[mi].SetTo(next);
							mip.first[mi++].SetMoveType(MoveType::Knight_Promotion_Capture);

							mip.first[mi].SetFrom(s);
							mip.first[mi].SetTo(next);
							mip.first[mi++].SetMoveType(MoveType::Bishop_Promotion_Capture);
						}
						else
						{
							mip.first[mi].SetFrom(s);
							mip.first[mi].SetTo(next);
							mip.first[mi++].SetMoveType(MoveType::Capture);

							if (r == 4) 
							{
								mip.first[mi].SetFrom(s);
								mip.first[mi].SetTo(next);
								mip.first[mi++].SetMoveType(MoveType::En_Passant_Capture);
							}
						}
					}
				}
			
				mip.second[si][s] = start_idx;
				mip.second[ei][s] = mi;
			}
			si += 2;
			ei = si + 1;

			// Black pawn moves
			using bbct = traits::board_color_traits<BasicBoard, color::Black>;
			for (Square s = 0; s < ChessBoard::BOARDSIZE; s++)
			{
				start_idx = mi;

				Rank r = ChessBoard::GetRank(s);
				if (r == 0 || r == 7)
				{
					mip.second[si][s] = Empty;
					mip.second[ei][s] = Empty;
					continue;
				}

				next = s + bbct::PawnMoveDirection;
				if (r == 6)
				{
					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(next);
					mip.first[mi++].SetMoveType(MoveType::Quite);

					next += bbct::PawnMoveDirection;

					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(next);
					mip.first[mi++].SetMoveType(MoveType::Double_Pawn_Push);
				}
				else if (r == 1)
				{
					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(next);
					mip.first[mi++].SetMoveType(MoveType::Queen_Promotion);

					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(next);
					mip.first[mi++].SetMoveType(MoveType::Rook_Promotion);

					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(next);
					mip.first[mi++].SetMoveType(MoveType::Knight_Promotion);

					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(next);
					mip.first[mi++].SetMoveType(MoveType::Bishop_Promotion);
				}
				else
				{
					mip.first[mi].SetFrom(s);
					mip.first[mi].SetTo(next);
					mip.first[mi++].SetMoveType(MoveType::Quite);
				}
			
				for (int i = 0; i < 2; i++)
				{
					next = s + bbct::PawnAttackDirections[i];
					if (BasicBoard::IsInside(s, next))
					{
						if (r == 1)
						{
							mip.first[mi].SetFrom(s);
							mip.first[mi].SetTo(next);
							mip.first[mi++].SetMoveType(MoveType::Queen_Promotion_Capture);

							mip.first[mi].SetFrom(s);
							mip.first[mi].SetTo(next);
							mip.first[mi++].SetMoveType(MoveType::Rook_Promotion_Capture);

							mip.first[mi].SetFrom(s);
							mip.first[mi].SetTo(next);
							mip.first[mi++].SetMoveType(MoveType::Knight_Promotion_Capture);

							mip.first[mi].SetFrom(s);
							mip.first[mi].SetTo(next);
							mip.first[mi++].SetMoveType(MoveType::Bishop_Promotion_Capture);
						}
						else
						{
							mip.first[mi].SetFrom(s);
							mip.first[mi].SetTo(next);
							mip.first[mi++].SetMoveType(MoveType::Capture);

							if (r == 3)
							{
								mip.first[mi].SetFrom(s);
								mip.first[mi].SetTo(next);
								mip.first[mi++].SetMoveType(MoveType::En_Passant_Capture);
							}
						}
					}
				}
			
				mip.second[si][s] = start_idx;
				mip.second[ei][s] = mi;
			}
			
			return mip;
		}
	}

	inline constexpr MovesAndIndicesPair MovesAndIndices = details::ComputeMovesAndIndices();
}