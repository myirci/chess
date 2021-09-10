#pragma once

#include <string_view>
#include <regex>
#include <optional>

#include <ChessLib/Chess/Definitions.hpp>

namespace chesslib::utility 
{
	namespace regex 
	{
		using svit = std::string_view::iterator;
		using svcit = std::string_view::const_iterator;
		using csvregex_token_it = std::regex_token_iterator<svcit>;

		const std::regex not_white_space("[^\\s]+");
		const std::regex not_forward_slash("[^/]+");

		std::pair<csvregex_token_it, csvregex_token_it> parse_string(std::string_view str, const std::regex& pattern);
	}

	namespace numeric 
	{
		std::optional<int> to_int(std::string_view sv);
	}
}