#pragma once

#include <cstdint>

#include <ChessLib/Chess/Definitions.hpp>

namespace chesslib 
{
	/// A move is represented by a 16-bit unsigned integer.
	/// 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
	/// 6-bits: (15 - 10)  -> from  
	/// 6-bits: (9  -  4)  -> to
	/// 4-bits: (3  -  0)  -> move flags

	class LightMove 
	{
	private:
		constexpr static uint16_t ClearFrom = 0x03FF;
		constexpr static uint16_t ReadFrom = ~ClearFrom;
		constexpr static uint16_t ClearTo = 0xFC0F;
		constexpr static uint16_t ReadTo = ~ClearTo;
		constexpr static uint16_t ClearMoveType = 0xFFF0;
		constexpr static uint16_t ReadMoveType = ~ClearMoveType;
		constexpr static uint16_t PromotionBitMask = 0x0008;
		constexpr static uint16_t CaptureBitMask = 0x0004;
	public:

		LightMove(Square from, Square to, MoveType mtype)
		{
			SetFrom(from);
			SetTo(to);
			SetMoveType(mtype);
		}
		
		inline Square GetFrom() const		{ return static_cast<Square>((_move & ReadFrom) >> 10); }
		inline Square GetTo() const			{ return static_cast<Square>((_move & ReadTo) >> 4); }
		inline MoveType GetMoveType() const { return static_cast<MoveType>(_move & ReadMoveType); }
		inline bool IsPromotion() const		{ return (_move & PromotionBitMask) > 0; }
		inline bool IsCapture() const		{ return (_move & CaptureBitMask) > 0; }

	private:

		std::uint16_t _move;

		inline void SetFrom(Square from) 
		{
			_move &= ClearFrom;
			_move |= (static_cast<std::uint16_t>(from) << 10);
		}
		
		inline void SetTo(Square to) 
		{
			_move &= ClearTo;
			_move |= (static_cast<std::uint16_t>(to) << 4);
		}
		
		inline void SetMoveType(MoveType mtype) 
		{
			_move &= ClearMoveType;
			_move |= static_cast<std::uint16_t>(mtype);
		}
	};
}