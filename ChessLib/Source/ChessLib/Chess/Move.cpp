#include "Move.hpp"

namespace chesslib 
{
	bool operator == (const Move m1, const Move m2) 
	{
		return
			m1.from == m2.from &&
			m1.to == m2.to &&
			m1.mtype == m2.mtype;
	}

	bool operator < (const Move m1, const Move m2) 
	{
		return m1.from < m2.from || m1.from == m2.from && m1.to < m2.to;
	}
}