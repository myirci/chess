#include "Connectivity.hpp"

#include <fstream>
#include <iostream>

#include <ChessLib/Board/BasicBoard.hpp>
#include <ChessLib/Chess/BoardColorTraits.hpp>

namespace chesslib::precompute
{

#pragma region Connectivity
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
				}

				indexes._indexes_start[s] = (int16_t)_connections.size();
				_connections.insert(_connections.end(), connections.begin(), connections.end());
				indexes._indexes_end[s] = (int16_t)_connections.size();
			}
			_indexes.push_back(indexes);
		}

		// Pawn connections
		{
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
						// push the rank
						connections.push_back(r);

						// white pawn single push
						connections.push_back(next);

						// white pawn double push
						next += wbct::PawnMoveDirection;
						if (BasicBoard::IsInside(s, next)) 
							connections.push_back(next);
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
						// No need to store the end_index as we store 5 bytes for each square.
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
						// push the rank
						connections.push_back(r);

						// black pawn single push
						connections.push_back(next);

						// black pawn double push 
						next += bbct::PawnMoveDirection;
						if (BasicBoard::IsInside(s, next))
							connections.push_back(next);
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
						// No need to store the end_index as we store 5 squares for each square.
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

#pragma endregion

#pragma region PrecomputeMoves

	PrecomputeMoves::PrecomputeMoves() { }

	void PrecomputeMoves::ComputeRookMoves()
	{
		for (Square s = 0; s < 64; s++)
		{
			// Straight sliding piece moves 
			for (Direction dir : StraightDirections) 
			{
				Square next = s + dir;
				while (BasicBoard::IsInside(next - dir, next))
				{
					_rook_moves.emplace_back(s, next, MoveType::Quite);
					_rook_moves.emplace_back(s, next, MoveType::Capture);
					next += dir;
				}
			}
		}
	}

	void PrecomputeMoves::ComputeBishopMoves()
	{
		for (Square s = 0; s < 64; s++)
		{
			// Diagonally sliding piece moves 
			for (Direction dir : DiagonalDirections)
			{
				Square next = s + dir;
				while (BasicBoard::IsInside(next - dir, next))
				{
					_bishop_moves.emplace_back(s, next, MoveType::Quite);
					_bishop_moves.emplace_back(s, next, MoveType::Capture);
					next += dir;
				}
			}
		}
	}

	void PrecomputeMoves::ComputeKnightMoves()
	{
		for (Square s = 0; s < 64; s++)
		{
			// Knight Moves
			for (Direction dir : KnightJumps)
			{
				if (BasicBoard::IsInside(s, s + dir))
				{
					_knight_moves.emplace_back(s, s + dir, MoveType::Quite);
					_knight_moves.emplace_back(s, s + dir, MoveType::Capture);
				}
			}
		}
	}

	void PrecomputeMoves::ComputeKingMoves()
	{
		for (Square s = 0; s < 64; s++)
		{
			for (Direction dir : AllDirections) 
			{
				if (BasicBoard::IsInside(s, s + dir))
				{
					_king_moves.emplace_back(s, s + dir, MoveType::Quite);
					_king_moves.emplace_back(s, s + dir, MoveType::Capture);
				}
			}

			// white king - castling moves
			if (s == ChessBoard::e1)
			{
				_king_moves.emplace_back(s, ChessBoard::g1, MoveType::King_Side_Castle);
				_king_moves.emplace_back(s, ChessBoard::c1, MoveType::Queen_Side_Castle);
			}
			// black king
			else if (s == ChessBoard::e8)
			{
				_king_moves.emplace_back(s, ChessBoard::g8, MoveType::King_Side_Castle);
				_king_moves.emplace_back(s, ChessBoard::c8, MoveType::Queen_Side_Castle);
			}
		}
	}

	void PrecomputeMoves::ComputeWhitePawnMoves()
	{
		for (Square s = 0; s < 64; s++)
		{
			// Pawn Moves
			Rank r = ChessBoard::GetRank(s);

			if (r == 0 || r == 7)
				continue;

			using wbct = traits::board_color_traits<BasicBoard, color::White>;
			Square next = s + wbct::PawnMoveDirection;
			if (r == 1)
			{
				_white_pawn_moves.emplace_back(s, next, MoveType::Quite);
				next += wbct::PawnMoveDirection;
				_white_pawn_moves.emplace_back(s, next, MoveType::Double_Pawn_Push);
			}
			else if (r == 6)
			{
				_white_pawn_moves.emplace_back(s, next, MoveType::Queen_Promotion);
				_white_pawn_moves.emplace_back(s, next, MoveType::Rook_Promotion);
				_white_pawn_moves.emplace_back(s, next, MoveType::Knight_Promotion);
				_white_pawn_moves.emplace_back(s, next, MoveType::Bishop_Promotion);
			}
			else
				_white_pawn_moves.emplace_back(s, next, MoveType::Quite);

			for (int i = 0; i < 2; i++)
			{
				next = s + wbct::PawnAttackDirections[i];
				if (BasicBoard::IsInside(s, next))
				{
					if (r == 6)
					{
						_white_pawn_moves.emplace_back(s, next, MoveType::Queen_Promotion_Capture);
						_white_pawn_moves.emplace_back(s, next, MoveType::Rook_Promotion_Capture);
						_white_pawn_moves.emplace_back(s, next, MoveType::Knight_Promotion_Capture);
						_white_pawn_moves.emplace_back(s, next, MoveType::Bishop_Promotion_Capture);
					}
					else
					{
						_white_pawn_moves.emplace_back(s, next, MoveType::Capture);
						if (r == 4)
							_white_pawn_moves.emplace_back(s, next, MoveType::En_Passant_Capture);
					}
				}
			}
		}
	}

	void PrecomputeMoves::ComputeBlackPawnMoves()
	{
		for (Square s = 0; s < 64; s++)
		{
			// Pawn Moves
			Rank r = ChessBoard::GetRank(s);

			if (r == 0 || r == 7)
				continue;

			using bbct = traits::board_color_traits<BasicBoard, color::Black>;
			Square next = s + bbct::PawnMoveDirection;
			if (r == 6)
			{
				_black_pawn_moves.emplace_back(s, next, MoveType::Quite);
				next += bbct::PawnMoveDirection;
				_black_pawn_moves.emplace_back(s, next, MoveType::Double_Pawn_Push);
			}
			else if (r == 1)
			{
				_black_pawn_moves.emplace_back(s, next, MoveType::Queen_Promotion);
				_black_pawn_moves.emplace_back(s, next, MoveType::Rook_Promotion);
				_black_pawn_moves.emplace_back(s, next, MoveType::Knight_Promotion);
				_black_pawn_moves.emplace_back(s, next, MoveType::Bishop_Promotion);
			}
			else
				_black_pawn_moves.emplace_back(s, next, MoveType::Quite);

			for (int i = 0; i < 2; i++)
			{
				next = s + bbct::PawnAttackDirections[i];
				if (BasicBoard::IsInside(s, next))
				{
					if (r == 1)
					{
						_black_pawn_moves.emplace_back(s, next, MoveType::Queen_Promotion_Capture);
						_black_pawn_moves.emplace_back(s, next, MoveType::Rook_Promotion_Capture);
						_black_pawn_moves.emplace_back(s, next, MoveType::Knight_Promotion_Capture);
						_black_pawn_moves.emplace_back(s, next, MoveType::Bishop_Promotion_Capture);
					}
					else
					{
						_black_pawn_moves.emplace_back(s, next, MoveType::Capture);
						if (r == 4)
							_black_pawn_moves.emplace_back(s, next, MoveType::En_Passant_Capture);
					}
				}
			}
		}
	}

	void PrecomputeMoves::Compute()
	{
		Square next = Empty;

		// Straight moves
		auto straight_move_indexes = MoveIndexes({ pieceset::WhiteRook, pieceset::BlackRook, pieceset::WhiteQueen, pieceset::BlackQueen });
		for (Square s = 0; s < 64; s++)
		{
			std::vector<Move> moves;
			for (Direction dir : StraightDirections)
			{
				next = s + dir;
				while (BasicBoard::IsInside(next - dir, next))
				{
					moves.emplace_back(s, next, MoveType::Quite);
					moves.emplace_back(s, next, MoveType::Capture);
					next += dir;
				}
			}

			straight_move_indexes._indexes_start[s] = (int16_t)_moves.size();
			_moves.insert(_moves.end(), moves.begin(), moves.end());
			straight_move_indexes._indexes_end[s] = (int16_t)_moves.size();
		}
		_indexes.push_back(straight_move_indexes);

		// Diagonal moves
		auto diagonal_move_indexes = MoveIndexes({ pieceset::WhiteBishop, pieceset::BlackBishop, pieceset::WhiteQueen, pieceset::BlackQueen });
		for (Square s = 0; s < 64; s++)
		{
			std::vector<Move> moves;
			for (Direction dir : DiagonalDirections)
			{
				next = s + dir;
				while (BasicBoard::IsInside(next - dir, next))
				{
					moves.emplace_back(s, next, MoveType::Quite);
					moves.emplace_back(s, next, MoveType::Capture);
					next += dir;
				}
			}

			diagonal_move_indexes._indexes_start[s] = (int16_t)_moves.size();
			_moves.insert(_moves.end(), moves.begin(), moves.end());
			diagonal_move_indexes._indexes_end[s] = (int16_t)_moves.size();
		}
		_indexes.push_back(diagonal_move_indexes);

		// Knight moves
		auto knight_move_indexes = MoveIndexes({ pieceset::WhiteKnight, pieceset::BlackKnight });
		for (Square s = 0; s < 64; s++)
		{
			std::vector<Move> moves;
			for (Direction dir : KnightJumps)
			{
				next = s + dir;
				if (BasicBoard::IsInside(s, next))
				{
					moves.emplace_back(s, next, MoveType::Quite);
					moves.emplace_back(s, next, MoveType::Capture);
				}
			}

			knight_move_indexes._indexes_start[s] = (int16_t)_moves.size();
			_moves.insert(_moves.end(), moves.begin(), moves.end());
			knight_move_indexes._indexes_end[s] = (int16_t)_moves.size();
		}
		_indexes.push_back(knight_move_indexes);

		// King moves
		auto king_move_indexes = MoveIndexes({ pieceset::WhiteKing, pieceset::BlackKing });
		for (Square s = 0; s < 64; s++)
		{
			std::vector<Move> moves;
			for (Direction dir : AllDirections)
			{
				next = s + dir;
				if (BasicBoard::IsInside(s, next))
				{
					moves.emplace_back(s, next, MoveType::Quite);
					moves.emplace_back(s, next, MoveType::Capture);
				}
			}

			if (s == e1)
			{
				moves.emplace_back(s, g1, MoveType::King_Side_Castle);
				moves.emplace_back(s, c1, MoveType::Queen_Side_Castle);
			}
			else if (s == e8)
			{
				moves.emplace_back(s, g8, MoveType::King_Side_Castle);
				moves.emplace_back(s, c8, MoveType::Queen_Side_Castle);
			}

			king_move_indexes._indexes_start[s] = (int16_t)_moves.size();
			_moves.insert(_moves.end(), moves.begin(), moves.end());
			king_move_indexes._indexes_end[s] = (int16_t)_moves.size();
		}
		_indexes.push_back(king_move_indexes);

		// White-pawn moves
		using wbct = traits::board_color_traits<BasicBoard, color::White>;
		auto white_pawn_move_indexes = MoveIndexes({ pieceset::WhitePawn });
		for (Square s = 0; s < 64; s++)
		{
			Rank r = ChessBoard::GetRank(s);
			if (r == 0 || r == 7)
				continue;

			std::vector<Move> moves;
			next = s + wbct::PawnMoveDirection;
			if (r == 1)
			{
				moves.emplace_back(s, next, MoveType::Quite);
				next += wbct::PawnMoveDirection;
				moves.emplace_back(s, next, MoveType::Double_Pawn_Push);
			}
			else if (r == 6)
			{
				moves.emplace_back(s, next, MoveType::Queen_Promotion);
				moves.emplace_back(s, next, MoveType::Rook_Promotion);
				moves.emplace_back(s, next, MoveType::Knight_Promotion);
				moves.emplace_back(s, next, MoveType::Bishop_Promotion);
			}
			else
				moves.emplace_back(s, next, MoveType::Quite);

			for (int i = 0; i < 2; i++)
			{
				next = s + wbct::PawnAttackDirections[i];
				if (BasicBoard::IsInside(s, next))
				{
					if (r == 6)
					{
						moves.emplace_back(s, next, MoveType::Queen_Promotion_Capture);
						moves.emplace_back(s, next, MoveType::Rook_Promotion_Capture);
						moves.emplace_back(s, next, MoveType::Knight_Promotion_Capture);
						moves.emplace_back(s, next, MoveType::Bishop_Promotion_Capture);
					}
					else
					{
						moves.emplace_back(s, next, MoveType::Capture);
						if (r == 4)
							moves.emplace_back(s, next, MoveType::En_Passant_Capture);
					}
				}
			}

			white_pawn_move_indexes._indexes_start[s] = (int16_t)_moves.size();
			_moves.insert(_moves.end(), moves.begin(), moves.end());
			white_pawn_move_indexes._indexes_end[s] = (int16_t)_moves.size();
		}
		_indexes.push_back(white_pawn_move_indexes);

		// Black-pawn moves
		using bbct = traits::board_color_traits<BasicBoard, color::Black>;
		auto black_pawn_move_indexes = MoveIndexes({ pieceset::BlackPawn });
		for (Square s = 0; s < 64; s++)
		{
			Rank r = ChessBoard::GetRank(s);
			if (r == 0 || r == 7)
				continue;

			std::vector<Move> moves;
			next = s + bbct::PawnMoveDirection;
			if (r == 6)
			{
				moves.emplace_back(s, next, MoveType::Quite);
				next += bbct::PawnMoveDirection;
				moves.emplace_back(s, next, MoveType::Double_Pawn_Push);
			}
			else if (r == 1)
			{
				moves.emplace_back(s, next, MoveType::Queen_Promotion);
				moves.emplace_back(s, next, MoveType::Rook_Promotion);
				moves.emplace_back(s, next, MoveType::Knight_Promotion);
				moves.emplace_back(s, next, MoveType::Bishop_Promotion);
			}
			else
				moves.emplace_back(s, next, MoveType::Quite);

			for (int i = 0; i < 2; i++)
			{
				next = s + bbct::PawnAttackDirections[i];
				if (BasicBoard::IsInside(s, next))
				{
					if (r == 1)
					{
						moves.emplace_back(s, next, MoveType::Queen_Promotion_Capture);
						moves.emplace_back(s, next, MoveType::Rook_Promotion_Capture);
						moves.emplace_back(s, next, MoveType::Knight_Promotion_Capture);
						moves.emplace_back(s, next, MoveType::Bishop_Promotion_Capture);
					}
					else
					{
						moves.emplace_back(s, next, MoveType::Capture);
						if (r == 3)
							moves.emplace_back(s, next, MoveType::En_Passant_Capture);
					}
				}
			}

			black_pawn_move_indexes._indexes_start[s] = (int16_t)_moves.size();
			_moves.insert(_moves.end(), moves.begin(), moves.end());
			black_pawn_move_indexes._indexes_end[s] = (int16_t)_moves.size();
		}
		_indexes.push_back(black_pawn_move_indexes);
	}

	void PrecomputeMoves::Export_PieceMoves(const std::string& folder_path)
	{
		std::string path = folder_path + "/" + "RookMoves.txt";
		Export(path, _rook_moves);

		path = folder_path + "/" + "BishopMoves.txt";
		Export(path, _bishop_moves);

		path = folder_path + "/" + "KnightMoves.txt";
		Export(path, _knight_moves);

		path = folder_path + "/" + "KingMoves.txt";
		Export(path, _king_moves);
	
		path = folder_path + "/" + "WhitePawnMoves.txt";
	    Export(path, _white_pawn_moves);
		
		path = folder_path + "/" + "BlackPawnMoves.txt";
		Export(path, _black_pawn_moves);
	}

	void PrecomputeMoves::Export_MovesAndIndexes(const std::string_view& file_path)
	{
		std::ofstream ofs(file_path.data(), std::ios::out);
		if (!ofs.is_open())
		{
			std::cout << "File could not be opened" << std::endl;
			return;
		}

		ofs << "# of moves: " << _moves.size() << std::endl;
		for (const Move& m : _moves)
		{
			auto [c1, c2] = GetCharPair(m.GetFrom());
			auto [c3, c4] = GetCharPair(m.GetTo());
			ofs << c1 << c2 << "-" << c3 << c4 << " " << to_string(m.GetMoveType()) << std::endl;
		}

		ofs << "Indexes:\n" << std::endl;
		for (const auto& idx : _indexes) 
		{
			ofs << "Pieces: ";
			for (Piece p : idx._pieces)
				ofs << piece_to_char.at(p) << " ";
			ofs << std::endl;

			ofs << "Start Indexes:" << std::endl;
			for (const auto id : idx._indexes_start)
				ofs << id << " ";
			ofs << std::endl;

			ofs << "End Indexes:" << std::endl;
			for (const auto id : idx._indexes_end)
				ofs << id << " ";
			ofs << std::endl;
			ofs << std::endl;
		}
		ofs.close();
	}

	void PrecomputeMoves::Export(std::string_view file_path, std::vector<Move>& moves)
	{
		std::ofstream ofs(file_path.data(), std::ios::out);
		if (!ofs.is_open())
		{
			std::cout << "File could not be opened" << std::endl;
			return;
		}

		ofs << "# of moves: " << moves.size() << std::endl;
		for (const Move& m : moves)
		{
			auto [c1, c2] = GetCharPair(m.GetFrom());
			auto [c3, c4] = GetCharPair(m.GetTo());
			ofs << c1 << c2 << "-" << c3 << c4 << " " << to_string(m.GetMoveType()) << std::endl;
		}
		ofs.close();
	}

#pragma endregion

}




