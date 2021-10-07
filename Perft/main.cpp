
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <ChessLib/Chess/Fen.hpp>
#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Chess/Utility.hpp>
#include <ChessLib/Perft/Perft.hpp>

#include <ChessLib/BasicBoard/BasicBoard.hpp>
#include <ChessLib/BasicBoard/Square.hpp>

#include <ChessLib/ObjBoard/ObjBoard.hpp>
#include <ChessLib/X88Board/x88Board.hpp>
#include <ChessLib/Bitboard/Bitboard.hpp>

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

	constexpr std::string_view basicboard_argument_specifier{ "basicboard" };
	constexpr std::string_view x88board_argument_specifier{ "x88board" };
	constexpr std::string_view objboard_argument_specifier{ "objboard" };
	constexpr std::string_view bitboard_argument_specifier{ "bitboard" };
}

void print_about();
void print_usage();
void print_help(int exit_code);

void execute_epdfile(std::string_view fpath);

void perft_for_basic_board(std::string_view fen, int depth, bool divide, bool stats, std::ofstream& log);
void perft_for_object_board(std::string_view fen, int depth, bool divide, bool stats, std::ofstream& log);
void perft_for_x88_board(std::string_view fen, int depth, bool divide, bool stats, std::ofstream& log);
void perft_for_bitboard(std::string_view fen, int depth, bool divide, bool stats, std::ofstream& log);

enum class board_type { basic, x88, obj, bitboard };

int main(int argc, char* argv[]) 
{
	std::string_view fen{chesslib::Fen::StartingPosition};
	int depth{ 1 };
	bool divide{ false };
	bool stats{ false };
	board_type board{ board_type::basic };
	std::ofstream log;
	std::string_view epdfile_path{""};

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

				epdfile_path = argv[i];
			}
			else if (arg == arguments::divide_argument_specifier)
				divide = true;
			else if (arg == arguments::stat_argument_specifier)
				stats = true;
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

	if (!epdfile_path.empty()) 
	{

	}
	else 
	{
		// validate depth
		if (depth > 10)
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

		// execute perft
		if (board == board_type::basic)
			perft_for_basic_board(fen, depth, divide, stats, log);
		else
		{
			std::cout << "Perft has not been implemented for the other board types yet." << std::endl;
		}
	}

	if (log.is_open())
		log.close();

	return 0;
}

void perft_for_basic_board(std::string_view fen, int depth, bool divide, bool stats, std::ofstream& log)
{
	auto board = chesslib::basic_board::make_unique_board(fen);
	if (divide) 
	{
		auto num_nodes_per_move = chesslib::perft::perft_divide(*board, depth);

		std::cout << fen << std::endl;
		
		if (log.is_open())
			log << fen << std::endl;

		unsigned long total{ 0 };
		for (const auto& [move, num_nodes] : num_nodes_per_move) 
		{
			auto [c1, c2] = chesslib::basic_board::get_chars(move.GetFrom());
			auto [c3, c4] = chesslib::basic_board::get_chars(move.GetTo());
			total += num_nodes;
			
			std::cout << "\t" << c1 << c2 << "-" << c3 << c4 << "\t" << num_nodes << std::endl;

			if (log.is_open()) 
				log << "\t" << c1 << c2 << "-" << c3 << c4 << "\t" << num_nodes << std::endl;
		}

		std::cout << "\t-------------\n";
		std::cout << "\tSum\t" << total << std::endl;

		if (log.is_open()) 
		{
			log << "\t---------\n";
			log << "\tSum\t" << total << std::endl;
		}
			
	}
	else 
	{
		auto num_nodes = chesslib::perft::perft(*board, depth);
		std::cout << fen << std::endl;
		std::cout << "Number of nodes: " << num_nodes << std::endl;

		if (!log.is_open())
			return;

		log << fen << "\n";
		log << "Number of nodes: " << num_nodes << "\n";
	}
}

void perft_for_object_board(std::string_view fen, int depth, bool divide, bool stats, std::ofstream& log)
{
}

void perft_for_x88_board(std::string_view fen, int depth, bool divide, bool stats, std::ofstream& log)
{
}

void perft_for_bitboard(std::string_view fen, int depth, bool divide, bool stats, std::ofstream& log)
{
}

void print_usage()
{
	std::cout
		<< "Usage: Perft "
		<< "[-" << arguments::fen_string_argument_specifier << " fen_string{\"Starting Position\"}]" << " "
		<< "[-" << arguments::depth_argument_specifier << " depth_value{1}]" << " "
		<< "[-" << arguments::divide_argument_specifier << "]" << " "
		<< "[-" << arguments::stat_argument_specifier << "]" << " "
		<< "[-" << arguments::board_argument_specifier << " board_representation{"
		<< arguments::basicboard_argument_specifier << "}("
		<< arguments::basicboard_argument_specifier << ", "
		<< arguments::x88board_argument_specifier << ", "
		<< arguments::objboard_argument_specifier << ", "
		<< arguments::bitboard_argument_specifier << ")]" << " "
		<< "[-" << arguments::logfile_argument_specifier << " logfile_path]" << " "
		<< "[-" << arguments::epdfile_argument_specifier << " epdfile_path]" << " "
		<< "[-" << arguments::directory_path_argument_specifier << " directory_path]" << " "
		<< "[-" << arguments::help_argument_specifier << "]" << " "
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
	std::cout << "Execute -help command for usage.\n";
	std::cout << "Developed by Murat Yirci.\n";
	std::cout << "\n------------------------------------------\n";
	exit(0);
}

void execute_epdfile(std::string_view fpath)
{
	/*
	auto file = std::ifstream(std::string(fpath), std::ios::out);
	if (!file.is_open())
		throw std::runtime_error("Epd file could not be opened.");

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
			auto depth = std::stoi((token.substr(0, pos)).substr(1));
			auto perft_val = std::stoull(token.substr(pos));
		}
	}

	file.close();
	*/
}