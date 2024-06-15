#pragma once

#include <ChessLib/Board/ObjBoard.hpp>
#include <ChessLib/MoveGenerator/AttackDetectorBase.hpp>


namespace chesslib 
{
	using ObjBoard = objboard::ObjBoard;

	class AttackDetectorObjBoard : public AttackDetectorBase<objboard::ObjBoard> 
	{
	public:
		void ComputeChecksAndPins(const ObjBoard& board, Square king_pos);
		bool IsUnderAttack(const ObjBoard& board, Square sq) const;

	private:

		template<Color Attacker, typename DirectionArray>
		void ComputeSlidingPieceChecksAndPins(const ObjBoard& board, const DirectionArray& dir_arr, Piece attacking_piece, Square king_pos);

	};

	template<Color Attacker, typename DirectionArray>
	void AttackDetectorObjBoard::ComputeSlidingPieceChecksAndPins(const ObjBoard& board, const DirectionArray& dir_arr, Piece attacking_piece, Square king_pos)
	{


		/*using ctraits = traits::color_traits<Attacker>;
		for (Direction dir : dir_arr)
		{
			Square pin_loc{ Empty };
			for (Square next{ king_pos + dir }; IsInside<BoardType>(next, next - dir); next += dir)
			{
				auto p = board.GetPiece(next);
				if (p == Empty)
					continue;

				if (p == attacking_piece)
				{
					if (pin_loc != Empty)
						this->_pins[this->_numPins++] = { next, pin_loc, direction::Reverse(dir) };
					else
						this->_checks[this->_numChecks++] = { next, king_pos, direction::Reverse(dir) };
					break;
				}
				else
				{
					if (pin_loc == Empty && color::get_color(p) == ctraits::Opposite)
						pin_loc = next;
					else
						break;
				}
			}
		}*/
	}
}

 