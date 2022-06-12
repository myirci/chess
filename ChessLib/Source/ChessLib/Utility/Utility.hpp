#pragma once

#include <string_view>
#include <regex>
#include <optional>
#include <vector>

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

		int get_random(int lower_limit, int upper_limit);

		template <typename T>
		constexpr T abs(T value) { return value >= 0 ? value : -value; }
	}

	namespace string_processing 
	{
		// trim from start (in place)
		void ltrim(std::string& s);

		// trim from end (in place)
		void rtrim(std::string& s);

		// trim from both ends (in place)
		void trim(std::string& s);
	}

	namespace filesystem 
	{
		bool IsFileOrDirectoryExist(std::string_view path);

		bool IsDirectory(std::string_view path);

		bool VerifyFileExtension(std::string_view fpath, std::string_view extension);

		std::vector<std::string> GetFilesFromDirectory(
			std::string_view path, 
			std::string_view file_extension,
			bool recursive = false);
	}
}