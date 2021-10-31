#pragma once

#include <ChessLib/Chess/Definitions.hpp>
#include <ChessLib/Board/BasicBoard/Square.hpp>
#include <ChessLib/Board/BasicBoard/BasicBoard.hpp>
#include <ChessLib/Chess/TypeTraits.hpp>

#include <map>

namespace chesslib::movegenerator 
{
	struct Connection
	{
		Square from;
		std::map<Square, std::vector<std::pair<Piece, MoveType>>> edges;
	};

	class MoveConnectivity 
	{
	public:

		static std::array<Connection, 64> ComputeConnections() 
		{
			std::array<Connection, 64> connections;

			for (int i = 0; i < 64; i++)
			{
				connections[i].from = basic_board::bottom_to_top_order[i];
				ComputeKnightConnections(connections[i]);
				ComputeStraightSlidingPieceConnections(connections[i]);
				ComputeDiagonallySlidingPieceConnections(connections[i]);
				ComputeKingConnections(connections[i]);
				ComputeWhitePawnConnections(connections[i]);
				ComputeBlackPawnConnections(connections[i]);
			}

			return connections;
		}

	private:

		using wptraits = traits::board_piece_traits<basic_board::BasicBoard, pieceset::WhitePawn>;
		using bptraits = traits::board_piece_traits<basic_board::BasicBoard, pieceset::BlackPawn>;

		static void ComputeKnightConnections(Connection& connect)
		{
			for (auto dir : basic_board::direction::KnightJumps) 
			{
				if (Square to = connect.from + dir; basic_board::BasicBoard::IsInside(connect.from, to)) 
				{
					connect.edges[to].emplace_back(pieceset::WhiteKnight, MoveType::Quite);
					connect.edges[to].emplace_back(pieceset::WhiteKnight, MoveType::Capture);
					connect.edges[to].emplace_back(pieceset::BlackKnight, MoveType::Quite);
					connect.edges[to].emplace_back(pieceset::BlackKnight, MoveType::Capture);
				}
			}
		}

		static void ComputeStraightSlidingPieceConnections(Connection& connect)
		{
			for (auto dir : basic_board::direction::Straight) 
			{
				for (Square to = connect.from + dir; basic_board::BasicBoard::IsInside(to - dir, to); to += dir)
				{
					connect.edges[to].emplace_back(pieceset::WhiteQueen, MoveType::Quite);
					connect.edges[to].emplace_back(pieceset::WhiteQueen, MoveType::Capture);
					connect.edges[to].emplace_back(pieceset::WhiteRook, MoveType::Quite);
					connect.edges[to].emplace_back(pieceset::WhiteRook, MoveType::Capture);

					connect.edges[to].emplace_back(pieceset::BlackQueen, MoveType::Quite);
					connect.edges[to].emplace_back(pieceset::BlackQueen, MoveType::Capture);
					connect.edges[to].emplace_back(pieceset::BlackRook, MoveType::Quite);
					connect.edges[to].emplace_back(pieceset::BlackRook, MoveType::Capture);
				}
			}	
		}

		static void ComputeDiagonallySlidingPieceConnections(Connection& connect)
		{
			for (auto dir : basic_board::direction::Diagonal)
			{
				for (Square to = connect.from + dir; basic_board::BasicBoard::IsInside(to - dir, to); to += dir)
				{
					connect.edges[to].emplace_back(pieceset::WhiteQueen, MoveType::Quite);
					connect.edges[to].emplace_back(pieceset::WhiteQueen, MoveType::Capture);
					connect.edges[to].emplace_back(pieceset::WhiteBishop, MoveType::Quite);
					connect.edges[to].emplace_back(pieceset::WhiteBishop, MoveType::Capture);

					connect.edges[to].emplace_back(pieceset::BlackQueen, MoveType::Quite);
					connect.edges[to].emplace_back(pieceset::BlackQueen, MoveType::Capture);
					connect.edges[to].emplace_back(pieceset::BlackBishop, MoveType::Quite);
					connect.edges[to].emplace_back(pieceset::BlackBishop, MoveType::Capture);
				}
			}
		}
	
		static void ComputeKingConnections(Connection& connect)
		{
			for (auto dir : basic_board::direction::All)
			{
				if (Square to = connect.from + dir; basic_board::BasicBoard::IsInside(connect.from, to))
				{
					connect.edges[to].emplace_back(pieceset::WhiteKing, MoveType::Quite);
					connect.edges[to].emplace_back(pieceset::WhiteKing, MoveType::Capture);		
					connect.edges[to].emplace_back(pieceset::BlackKing, MoveType::Quite);
					connect.edges[to].emplace_back(pieceset::BlackKing, MoveType::Capture);
				}
			}

			if (connect.from == squareset::e1)
			{
				connect.edges[basic_board::f1].emplace_back(pieceset::WhiteKing, MoveType::King_Side_Castle);
				connect.edges[basic_board::f1].emplace_back(pieceset::WhiteKing, MoveType::Queen_Side_Castle);
			}

			if (connect.from == squareset::e8)
			{
				connect.edges[basic_board::f8].emplace_back(pieceset::BlackKing, MoveType::King_Side_Castle);
				connect.edges[basic_board::f8].emplace_back(pieceset::BlackKing, MoveType::Queen_Side_Castle);
			}
		}
	
		static void ComputeWhitePawnConnections(Connection& connect)
		{
			Rank r = basic_board::get_rank(connect.from);
			if (r > 0 && r < 7) 
			{
				if (r != 6) 
				{
					Square to = connect.from + basic_board::direction::N;
					connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Quite);

					if (r == 1)
					{
						to = connect.from + 2 * basic_board::direction::N;
						connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Double_Pawn_Push);
					}

					to = connect.from + basic_board::direction::NW;
					if (basic_board::BasicBoard::IsInside(connect.from, to)) 
					{
						connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Capture);
						if(r == 4)
							connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::En_Passant_Capture);
					}	

					to = connect.from + basic_board::direction::NE;
					if (basic_board::BasicBoard::IsInside(connect.from, to))
					{
						connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Capture);
						if (r == 4)
							connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::En_Passant_Capture);
					}
				}
				else 
				{
					Square to = connect.from + basic_board::direction::N;
					connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Queen_Promotion);
					connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Knight_Promotion);
					connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Rook_Promotion);
					connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Bishop_Promotion);

					to = connect.from + basic_board::direction::NW;
					if (basic_board::BasicBoard::IsInside(connect.from, to))
					{
						connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Queen_Promotion_Capture);
						connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Knight_Promotion_Capture);
						connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Rook_Promotion_Capture);
						connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Bishop_Promotion_Capture);
					}

					to = connect.from + basic_board::direction::NE;
					if (basic_board::BasicBoard::IsInside(connect.from, to))
					{
						connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Queen_Promotion_Capture);
						connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Knight_Promotion_Capture);
						connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Rook_Promotion_Capture);
						connect.edges[to].emplace_back(pieceset::WhitePawn, MoveType::Bishop_Promotion_Capture);
					}
				}
			}
		}

		static void ComputeBlackPawnConnections(Connection& connect)
		{
			Rank r = basic_board::get_rank(connect.from);
			if (r > 0 && r < 7)
			{
				if (r != 1)
				{
					Square to = connect.from + basic_board::direction::S;
					connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Quite);

					if (r == 6)
					{
						to = connect.from + 2 * basic_board::direction::S;
						connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Double_Pawn_Push);
					}

					to = connect.from + basic_board::direction::SW;
					if (basic_board::BasicBoard::IsInside(connect.from, to))
					{
						connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Capture);
						if (r == 3)
							connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::En_Passant_Capture);
					}

					to = connect.from + basic_board::direction::SE;
					if (basic_board::BasicBoard::IsInside(connect.from, to))
					{
						connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Capture);
						if (r == 3)
							connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::En_Passant_Capture);
					}
				}
				else
				{
					Square to = connect.from + basic_board::direction::S;
					connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Queen_Promotion);
					connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Knight_Promotion);
					connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Rook_Promotion);
					connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Bishop_Promotion);

					to = connect.from + basic_board::direction::SW;
					if (basic_board::BasicBoard::IsInside(connect.from, to))
					{
						connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Queen_Promotion_Capture);
						connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Knight_Promotion_Capture);
						connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Rook_Promotion_Capture);
						connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Bishop_Promotion_Capture);
					}

					to = connect.from + basic_board::direction::SE;
					if (basic_board::BasicBoard::IsInside(connect.from, to))
					{
						connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Queen_Promotion_Capture);
						connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Knight_Promotion_Capture);
						connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Rook_Promotion_Capture);
						connect.edges[to].emplace_back(pieceset::BlackPawn, MoveType::Bishop_Promotion_Capture);
					}
				}
			}
		}
	};

	std::ostream& operator << (std::ostream& os, const Connection& connect);
}