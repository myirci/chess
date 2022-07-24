#pragma once

#include <ChessLib/Chess/Definitions.hpp>

namespace chesslib 
{
	template <typename BoardType>
	class AttackDetectorBase
	{
	public:

		struct AttackInfo
		{
			Square    attacker;	  // attacker location (pinner or checker)
			Square    attacked;	  // attacked piece (pinned piece or checked king) location 
			Direction attack_dir; // direction from attacker to attacked (pinned piece or checked king)
		};

		const AttackInfo& GetFirstCheck() const  { return _checks[0]; }
		
		std::uint8_t GetNumberOfChecks() const noexcept { return _numChecks; }

		Direction GetPinDirection(Square piece_loc) const noexcept
		{
			for (int i = 0; i < _numPins; i++)
				if (_pins[i].attacked == piece_loc)
					return _pins[i].attack_dir;
			return Empty;
		}

		bool IsPinned(Square piece_loc) const noexcept { return GetPinDirection(piece_loc) != Empty; }
		
		template<Color Attacker>
		bool VerifyKingMovingDirection(const BoardType& board, Direction dir) const
		{
			using ctraits = traits::color_traits<Attacker>;
			for (int i = 0; i < this->_numChecks; i++)
			{
				if (this->_checks[i].attack_dir == dir)
				{
					// Away from checker, possible only if the checker is a pawn
					if (board.GetPiece(this->_checks[i].attacker) != ctraits::Pawn)
						return false;
				}
				else if (this->_checks[i].attack_dir == direction::Reverse(dir))
				{
					// Towards checker, possible only if the checker is one square away from the king
					if (this->_checks[i].attacked != (this->_checks[i].attacker + this->_checks[i].attack_dir))
						return false;
				}
			}
			return true;
		}

	protected:

		std::uint8_t _numPins;
		std::uint8_t _numChecks;
		AttackInfo	 _pins[8];
		AttackInfo	 _checks[2];

		inline void Clear() noexcept 
		{
			_numPins = 0;
			_numChecks = 0;
		}
	};
}