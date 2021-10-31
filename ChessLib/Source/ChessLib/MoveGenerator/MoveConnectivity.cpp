#include "MoveConnectivity.hpp"
#include <ChessLib/Chess/ChessUtility.hpp>
#include <iostream>

namespace chesslib::movegenerator 
{
	std::ostream& operator << (std::ostream& os, const Connection& connect) 
	{
		auto [c1, c2] = basic_board::get_chars(connect.from);

		for (const auto& kvp : connect.edges) 
		{	
			auto [c3, c4] = basic_board::get_chars(kvp.first);

			for (const auto& p : kvp.second) 
				os << c1 << c2 << "\t" 
				   << c3 << c4 << "\t" 
				   << piece_to_char.at(p.first) << "\t" 
				   << utility::chess::to_string(p.second) << "\n";
		}
		return os;
	}
}