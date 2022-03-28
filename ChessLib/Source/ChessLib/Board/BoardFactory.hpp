#pragma once

#include <ChessLib/Chess/ChessUtility.hpp>
#include <memory>
#include <string_view>

namespace chesslib 
{
	class BoardFactory final 
	{
	public:

		template <typename BoardType>
		static std::unique_ptr<BoardType> make_unique_board(std::string_view fen)
		{
			auto brd = std::unique_ptr<BoardType>(new BoardType());
			utility::chess::set_board(*brd, fen);
			return brd;
		}

		template <typename BoardType>
		static std::shared_ptr<BoardType> make_shared_board(std::string_view fen)
		{
			auto brd = std::shared_ptr<BoardType>(new BoardType());
			utility::chess::set_board(*brd, fen);
			return brd;
		}

	private:
		BoardFactory() = default;
		BoardFactory(const BoardFactory&) = delete;
		BoardFactory& operator=(const BoardFactory&) = delete;
		BoardFactory(BoardFactory&&) = delete;
		BoardFactory& operator=(BoardFactory&&) = delete;
	};
}