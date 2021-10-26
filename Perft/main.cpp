
#include <iostream>
#include <fstream>
#include <sstream>

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Utility/Utility.hpp>
#include <ChessLib/Utility/Timer.hpp>
#include <ChessLib/Perft/Perft.hpp>

#include <ChessLib/Board/BasicBoard/BasicBoard.hpp>
#include <ChessLib/Board/BasicBoard/Square.hpp>

#include <ChessLib/Board/ObjBoard/ObjBoard.hpp>
#include <ChessLib/Board/X88Board/x88Board.hpp>
#include <ChessLib/Board/Bitboard/Bitboard.hpp>

namespace arguments
{
	constexpr std::string_view board_argument_specifier{ "board" };
	constexpr std::string_view stat_argument_specifier{ "stats" };
	constexpr std::string_view divide_argument_specifier{ "divide" };
	constexpr std::string_view fen_string_argument_specifier{ "fen" };
	constexpr std::string_view depth_argument_specifier{ "depth" };
	constexpr std::string_view help_argument_specifier{ "help" };
	constexpr std::string_view about_argument_specifier{ "about" };
	constexpr std::string_view logfile_argument_specifier{ "logfile" };
	constexpr std::string_view epdfile_argument_specifier{ "epdfile" };
	constexpr std::string_view directory_path_argument_specifier{ "directory" };
	constexpr std::string_view read_files_recursively_argument_specifier{ "recursive" };
	constexpr std::string_view basicboard_argument_specifier{ "basicboard" };
	constexpr std::string_view x88board_argument_specifier{ "x88board" };
	constexpr std::string_view objboard_argument_specifier{ "objboard" };
	constexpr std::string_view bitboard_argument_specifier{ "bitboard" };
	constexpr std::string_view time_argument_specifier{ "time" };
}

enum class board_type { basic, x88, obj, bitboard };

void print_about();
void print_usage();
void print_help(int exit_code);

std::vector<std::pair<std::string, uint64_t>> get_positions_from_epd_file(std::string_view fpath, int depth);

void execute_perft(
	board_type board, 
	std::string_view fen, 
	int depth, 
	bool divide, 
	bool stats,
	bool measeure_time,
	std::ofstream& log, 
	std::optional<uint64_t> expected_result = std::nullopt);

uint64_t perft_for_basic_board(std::string_view fen, int depth, bool divide, bool stats, bool measure_time, std::ofstream& log);
uint64_t perft_for_object_board(std::string_view fen, int depth, bool divide, bool stats, bool measure_time, std::ofstream& log);
uint64_t perft_for_x88_board(std::string_view fen, int depth, bool divide, bool stats, bool measure_time, std::ofstream& log);
uint64_t perft_for_bitboard(std::string_view fen, int depth, bool divide, bool stats, bool measure_time, std::ofstream& log);

int main(int argc, char* argv[]) 
{
	std::string_view fen{chesslib::Fen::StartingPosition};
	int depth{ 1 };
	bool divide{ false };
	bool stats{ false };
	bool measure_time{ false };
	board_type board{ board_type::basic };
	std::ofstream log;
	std::string_view epdfile_path{ "" };
	std::string_view directory_path{ "" };
	bool read_files_recursively{ false };

	for (int i{ 1 }; i < argc; i++)
	{
		if (auto pos = std::string_view(argv[i]).find_last_of('-'); pos != std::string::npos)
		{
			auto arg{ std::string_view(argv[i]).substr(pos+1) };
			if (arg == arguments::board_argument_specifier)
			{
				i++;
				if (i >= argc)
					print_help(1);

				if (argv[i] == arguments::basicboard_argument_specifier)
					board = board_type::basic;
				else if (argv[i] == arguments::bitboard_argument_specifier)
					board = board_type::bitboard;
				else if (argv[i] == arguments::objboard_argument_specifier)
					board = board_type::obj;
				else if (argv[i] == arguments::x88board_argument_specifier)
					board = board_type::x88;
				else
					print_help(1);
			}
			else if (arg == arguments::fen_string_argument_specifier)
			{
				i++;
				if (i >= argc)
					print_help(1);
				fen = argv[i];
			}
			else if (arg == arguments::depth_argument_specifier)
			{
				i++;
				if (i >= argc)
					print_help(1);

				depth = std::stoi(argv[i]);
			}
			else if (arg == arguments::logfile_argument_specifier)
			{
				i++;
				if (i >= argc)
					print_help(1);

				log.open(argv[i], std::ios::app);
				if (!log.is_open())
				{
					std::cout << "File could not be opened" << std::endl;
					exit(3);
				}
			}
			else if (arg == arguments::epdfile_argument_specifier)
			{
				i++;
				if (i >= argc)
					print_help(1);

				if (!chesslib::utility::filesystem::IsFileOrDirectoryExist(argv[i]))
				{
					std::cout << argv[i] << " could not be found, please verify the given path\n";
					print_help(1);
				}

				if (!chesslib::utility::filesystem::VerifyFileExtension(argv[i], ".epd"))
				{
					std::cout << argv[i] << " is not an epd file.\n";
					print_help(1);
				}

				epdfile_path = argv[i];
			}
			else if (arg == arguments::directory_path_argument_specifier)
			{
				i++;
				if (i >= argc)
					print_help(1);

				if (!chesslib::utility::filesystem::IsFileOrDirectoryExist(argv[i]))
				{
					std::cout << argv[i] << " could not be found, please verify the given path\n";
					print_help(1);
				}

				if (!chesslib::utility::filesystem::IsDirectory(argv[i]))
				{
					std::cout << argv[i] << " is not a directory\n";
					print_help(1);
				}

				directory_path = argv[i];
			}
			else if (arg == arguments::divide_argument_specifier)
				divide = true;
			else if (arg == arguments::read_files_recursively_argument_specifier)
				read_files_recursively = true;
			else if (arg == arguments::stat_argument_specifier)
				stats = true;
			else if (arg == arguments::time_argument_specifier)
				measure_time = true;
			else if (arg == arguments::about_argument_specifier)
				print_about();
			else if (arg == arguments::help_argument_specifier)
				print_help(0);
			else
				print_help(1);
		}
		else 
			print_help(1);
	}

	if (!epdfile_path.empty() || !directory_path.empty()) 
	{
		std::vector<std::string> files;
		if (!directory_path.empty()) 
			files = chesslib::utility::filesystem::GetFilesFromDirectory(directory_path, ".epd", read_files_recursively);
		
		if (!epdfile_path.empty())
			files.emplace_back(epdfile_path);

		for (const auto& fpath : files) 
		{
			auto fen_positions = get_positions_from_epd_file(fpath, depth);
			for (const auto& [fen, expected_perft_result] : fen_positions) 
				execute_perft(board, fen, depth, divide, stats, measure_time, log, expected_perft_result);
		}
	}
	else 
	{
		// validate depth
		if (depth > 8)
		{
			std::cout << "Computing perft will take a long time, would you like to continue? (press y for yes, n for no)" << std::endl;
			char response{ 'n' };
			std::cin >> response;
			if (response != 'y')
				exit(0);
		}

		// validate fen
		try
		{
			chesslib::Fen(fen);
		}
		catch (const std::logic_error& e)
		{
			std::cout << e.what() << std::endl;
			exit(2);
		}

		execute_perft(board, fen, depth, divide, stats, measure_time, log);
	}

	if (log.is_open())
		log.close();

	return 0;
}

void execute_perft(
	board_type board, 
	std::string_view fen, 
	int depth, 
	bool divide, 
	bool stats, 
	bool measure_time,
	std::ofstream& log, 
	std::optional<uint64_t> expected_result /* = std::nullopt*/)
{
	static uint64_t count{ 0 };
	count++;

	std::cout << "\n---------------------------------------------------------------------------\n\n";
	std::cout << "Position: " << count << std::endl;
	std::cout << fen << "\nDepth: " << depth << std::endl;

	if (log.is_open())
	{
		log << "\n---------------------------------------------------------------------------\n\n";
		log << "Position: " << count << std::endl;
		log << fen << "\nDepth: " << depth << std::endl;
	}

	// execute perft
	uint64_t perft_res{ 0 };
	if (board == board_type::basic)
		perft_res = perft_for_basic_board(fen, depth, divide, stats, measure_time, log);
	else
	{
		std::cout << "Perft has not been implemented for the other board types yet." << std::endl;
	}

	if (expected_result.has_value())
	{
		bool pass{ perft_res == expected_result.value() };
		if (pass) 
		{
			std::cout << "PASS\n";
			if (log.is_open())
				log << "PASS\n";
		}
		else 
		{
			std::cout << "FAIL\tEXPECTED: " << expected_result.value() << std::endl;
			if (log.is_open())
				log << "FAIL\tEXPECTED: " << expected_result.value() << std::endl;
		}
	}
}

uint64_t perft_for_basic_board(std::string_view fen, int depth, bool divide, bool stats, bool measure_time, std::ofstream& log)
{
	auto board = chesslib::basic_board::make_unique_board(fen);
	uint64_t total{ 0 };

	using Timer = chesslib::utility::time::Timer<std::chrono::milliseconds>;
	std::optional<Timer> timer{std::nullopt};
	if (measure_time) 
	{
		timer = Timer{};
		timer.value().Start("");
	}

	if (divide) 
	{
		if (stats) 
		{
			auto res = chesslib::perft::perft_divide_statistics(*board, depth);
			for (const auto& p : res) 
			{
				if (p.first.has_value()) 
				{
					auto mv_str = chesslib::utility::chess::to_string<chesslib::basic_board::BasicBoard>(p.first.value());
					std::cout << mv_str << std::endl;
					if (log.is_open())
						log << mv_str << std::endl;

					std::cout << *p.second << "\n\n";
					if (log.is_open())
						log << *p.second << "\n\n";
				}
			}
		}
		else 
		{
			auto num_nodes_per_move = chesslib::perft::perft_divide(*board, depth);
			for (const auto& [move, num_nodes] : num_nodes_per_move)
			{
				total += num_nodes;
				auto mv_str = chesslib::utility::chess::to_string<chesslib::basic_board::BasicBoard>(move);

				std::cout << "\t" << mv_str << "\t" << num_nodes << std::endl;
				if (log.is_open())
					log << "\t" << mv_str << "\t" << num_nodes << std::endl;
			}

			std::cout << "\t-------------\n";
			std::cout << "\tSum\t" << total << std::endl;

			if (log.is_open())
			{
				log << "\t---------\n";
				log << "\tSum\t" << total << std::endl;
			}
		}
	}
	else 
	{
		if (stats) 
		{
			auto stats = chesslib::perft::perft_statistics(*board, depth);
			std::cout << *stats << std::endl;
			if(log.is_open())
				log << *stats << std::endl;
		}
		else 
		{
			total = chesslib::perft::perft(*board, depth);
			std::cout << "Number of nodes: " << total << std::endl;

			if (log.is_open())
				log << "Number of nodes: " << total << "\n";
		}
	}

	if (timer.has_value())
	{
		timer.value().End("Perft ended:");
		std::cout << timer.value();
		if (log.is_open())
			log << "\n" << timer.value();
	}

	return total;
}

uint64_t perft_for_object_board(std::string_view fen, int depth, bool divide, bool stats, bool measure_time, std::ofstream& log)
{
	return 0;
}

uint64_t perft_for_x88_board(std::string_view fen, int depth, bool divide, bool stats, bool measure_time, std::ofstream& log)
{
	return 0;
}

uint64_t perft_for_bitboard(std::string_view fen, int depth, bool divide, bool stats, bool measure_time, std::ofstream& log)
{
	return 0;
}

void print_usage()
{
	std::cout
		<< "Usage:\nPerft "
		<< "[-" << arguments::fen_string_argument_specifier << " fen_string{\"Starting Position\"}]" << "\n"
		<< "[-" << arguments::depth_argument_specifier << " depth_value{1}]" << "\n"
		<< "[-" << arguments::divide_argument_specifier << "]" << "\n"
		<< "[-" << arguments::stat_argument_specifier << "]" << "\n"
		<< "[-" << arguments::time_argument_specifier << "]" << "\n"
		<< "[-" << arguments::board_argument_specifier << " board_representation{"
		<< arguments::basicboard_argument_specifier << "}("
		<< arguments::basicboard_argument_specifier << ", "
		<< arguments::x88board_argument_specifier << ", "
		<< arguments::objboard_argument_specifier << ", "
		<< arguments::bitboard_argument_specifier << ")]" << "\n"
		<< "[-" << arguments::logfile_argument_specifier << " logfile_path]" << "\n"
		<< "[-" << arguments::epdfile_argument_specifier << " epdfile_path]" << "\n"
		<< "[-" << arguments::directory_path_argument_specifier << " directory_path] -> Directory path to look for .epd files." << "\n"
		<< "[-" << arguments::read_files_recursively_argument_specifier << "] -> Read the files recursively in the given directory."  << "\n"
		<< "[-" << arguments::help_argument_specifier << "]" << "\n"
		<< "[-" << arguments::about_argument_specifier << "]" 
		<< std::endl;
}

void print_help(int exit_code) 
{
	print_usage();
	exit(exit_code);
}

void print_about() 
{
	std::cout << "\n------------------------------------------\n";
	std::cout << "Computes perft for different board representations.\n";
	std::cout << "Not the fastest perft application but well tested and trustable.\n";
	std::cout << "Execute -help command for getting information on how to use.\n";
	std::cout << "Developed by Murat Yirci.\n";
	std::cout << "\n------------------------------------------\n";
	exit(0);
}

std::vector<std::pair<std::string, uint64_t>> get_positions_from_epd_file(std::string_view fpath, int depth)
{
	auto file = std::ifstream(std::string(fpath), std::ios::out);
	if (!file.is_open())
		throw std::runtime_error("Epd file could not be opened.");

	std::vector<std::pair<std::string, uint64_t>> fen_positions;

	std::string line, token, fen;
	while (std::getline(file, line))
	{
		std::stringstream ss{ line };
		int i{ 0 };
		while (std::getline(ss, token, ';'))
		{
			chesslib::utility::string_processing::trim(token);

			if (i++ == 0)
			{
				fen = token;
				continue;
			}

			auto pos = token.find_first_of(' ');
			auto d = std::stoi((token.substr(0, pos)).substr(1));
			if (depth == d) 
			{
				fen_positions.emplace_back(fen, std::stoull(token.substr(pos)));
				break;
			}
		}
	}

	file.close();
	return fen_positions;
}