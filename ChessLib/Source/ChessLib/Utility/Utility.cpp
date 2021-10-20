#include <ChessLib/Utility/Utility.hpp>

#include <charconv>
#include <random>
#include <filesystem>

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

	namespace filesystem 
	{
		bool IsFileOrDirectoryExist(std::string_view path)
		{
			std::filesystem::path p{ path };
			return std::filesystem::exists(p);
		}

		bool IsDirectory(std::string_view path) 
		{
			std::filesystem::path p{ path };
			return std::filesystem::is_directory(p);
		}

		bool VerifyFileExtension(std::string_view fpath, std::string_view extension)
		{
			std::filesystem::path p{ fpath };
			return p.extension() == extension;
		}

		std::vector<std::string> GetFilesFromDirectory(
			std::string_view path,
			std::string_view file_extension,
			bool recursive /* = false */) 
		{
			std::vector<std::string> files;
			if (recursive) 
			{
				for (const auto& entry : std::filesystem::recursive_directory_iterator{ path }) 
				{
					if (std::filesystem::is_regular_file(entry.status()) && 
						entry.path().extension() == file_extension)
						files.emplace_back(entry.path().string());
				}
			}
			else 
			{
				for (const auto& entry : std::filesystem::directory_iterator{ path })
				{
					if (std::filesystem::is_regular_file(entry.status()) &&
						entry.path().extension() == file_extension)
						files.emplace_back(entry.path().string());
				}
			}

			return files;
		}
	}
}