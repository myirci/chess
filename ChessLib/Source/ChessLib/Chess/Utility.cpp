#include <ChessLib/Chess/Utility.hpp>

#include <charconv>


namespace chesslib::utility 
{
	namespace regex 
	{
		std::pair<csvregex_token_it, csvregex_token_it> parse_string(std::string_view str, const std::regex& pattern)
		{
			return std::make_pair(csvregex_token_it(str.cbegin(), str.cend(), pattern, 0), csvregex_token_it());
		}
	}

	namespace numeric
	{
		std::optional<int> to_int(std::string_view sv) 
		{
			int out;
			const std::from_chars_result result = std::from_chars(sv.data(), sv.data() + sv.size(), out);

			return (result.ec == std::errc::invalid_argument || result.ec == std::errc::result_out_of_range)
				? std::nullopt
				: std::optional<int>(out);
		}
	}
}

