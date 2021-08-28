#include "FenLight.hpp"

namespace chesslib 
{
	std::array<std::string_view, 8> FenLight::GetRanks(std::string_view fen)
	{
		std::array<std::string_view, 8> ranks;
		
		int rank = 0;
		
		auto start = fen.begin();
		auto it = start;

		while (rank < 8 && it != fen.end()) 
		{
			if (*it != '/' && *it != ' ')
			{
				it++;
				continue;
			}
				
			ranks[rank++] = std::string_view{ start, it++ };
			start = it;
		}

		return ranks;
	}
}