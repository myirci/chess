#pragma once

#include <string_view>
#include <array>

namespace chesslib 
{
	// Static class to Procees fen strings w
	class FenLight 
	{
	private:

		FenLight() = delete;
	
	public:

		// Returns the ranks in reverse-order: first element of the array will be rank-8 and the last element will be rank-1
		static std::array<std::string_view, 8> GetRanks(std::string_view fen);
	};
}
