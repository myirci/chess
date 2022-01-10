#include <iostream>

#include <ChessLib/Chess/Move.hpp>
#include <ChessLib/Chess/LightMove.hpp>

int main() 
{
	chesslib::Move move1(0, 0, chesslib::MoveType::Quite);
	chesslib::LightMove move2(0, 0, chesslib::MoveType::Quite);
	std::cout << sizeof(move1) << std::endl;
	std::cout << sizeof(move2) << std::endl;
}