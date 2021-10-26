
#include <ChessLib/Chess/ChessUtility.hpp>

namespace chesslib::utility::chess
{

	namespace fenhelpers
	{
		Color GetColorFromChar(char c)
		{
			if (c == charset::White)
				return color::White;

			if (c == charset::Black)
				return color::Black;

			throw std::logic_error("Color char error.");
		}
	}
}