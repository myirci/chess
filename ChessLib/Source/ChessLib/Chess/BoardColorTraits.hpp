#pragma once

#include <ChessLib/Chess/Definitions.hpp>

namespace chesslib::traits 
{
	template<typename BoardType, Color>
	struct board_color_traits { };

	template <typename BoardType>
	struct board_color_traits<BoardType, color::White>
	{
		static constexpr Square    KingSideRookInitialPosition        = BoardType::h1;
		static constexpr Square    QueenSideRookInitialPosition       = BoardType::a1;
		static constexpr Square    KingSideRookPositionAfterCastling  = BoardType::f1;
		static constexpr Square    QueenSideRookPositionAfterCastling = BoardType::d1;
		static constexpr Square    KingSideCastleCheckSquares[2]      = { BoardType::f1, BoardType::g1 };
		static constexpr Square    QueenSideCastleCheckSquares[3]     = { BoardType::d1, BoardType::c1, BoardType::b1 };
		static constexpr Square    ValidPawnMoveSquares[2]            = { BoardType::a3, BoardType::h8 };

		static constexpr Rank	   PawnPromotionRank		          = 6;
		static constexpr Rank	   PromotedPawnRank					  = 7;
		static constexpr Rank	   PawnDoublePushRank		          = 1;
		static constexpr Rank	   DoublePushedPawnRank				  = 3;
		static constexpr Direction PawnMoveDirection		          = BoardType::N;
		static constexpr Direction PawnReverseMoveDirection           = BoardType::S;
		static constexpr Direction PawnAttackDirections[2]            = { BoardType::NE, BoardType::NW };
		static constexpr Direction PawnReverseAttackDirections[2]     = { BoardType::SW, BoardType::SE };
	};

	template <typename BoardType>
	struct board_color_traits<BoardType, color::Black>
	{
		static constexpr Square    KingSideRookInitialPosition        = BoardType::h8;
		static constexpr Square    QueenSideRookInitialPosition       = BoardType::a8;
		static constexpr Square    KingSideRookPositionAfterCastling  = BoardType::f8;
		static constexpr Square    QueenSideRookPositionAfterCastling = BoardType::d8;
		static constexpr Square    KingSideCastleCheckSquares[2]      = { BoardType::f8, BoardType::g8 };
		static constexpr Square    QueenSideCastleCheckSquares[3]     = { BoardType::d8, BoardType::c8, BoardType::b8 };
		static constexpr Square    ValidPawnMoveSquares[2]            = { BoardType::a1, BoardType::h6 };

		static constexpr Rank	   PawnPromotionRank			      = 1;
		static constexpr Rank	   PromotedPawnRank					  = 0;
		static constexpr Rank	   PawnDoublePushRank			      = 6;
		static constexpr Rank	   DoublePushedPawnRank				  = 4;
		static constexpr Direction PawnMoveDirection			      = BoardType::S;
		static constexpr Direction PawnReverseMoveDirection		      = BoardType::N;
		static constexpr Direction PawnAttackDirections[2]		      = { BoardType::SE, BoardType::SW };
		static constexpr Direction PawnReverseAttackDirections[2]     = { BoardType::NW, BoardType::NE };
	};
}