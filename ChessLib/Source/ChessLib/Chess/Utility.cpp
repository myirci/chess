#include <ChessLib/Chess/Utility.hpp>

#include <charconv>
#include <random>


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

		int get_random(int lower_limit, int upper_limit)
		{
			std::random_device rd;
			std::default_random_engine generator{ rd() };
			std::uniform_int_distribution<int> distribution(lower_limit, upper_limit);
			return distribution(generator);
		}
	}

	namespace string_processing 
	{
		void ltrim(std::string& s)
		{
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), 
				[](int ch) { return !std::isspace(ch); }));
		}

		void rtrim(std::string& s)
		{
			s.erase(std::find_if(s.rbegin(), s.rend(), 
				[](int ch) { return !std::isspace(ch); }).base(), s.end());
		}

		void trim(std::string& s)
		{
			ltrim(s);
			rtrim(s);
		}
	}
}

