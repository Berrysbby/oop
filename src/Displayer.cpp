#include "Displayer.h"
#include <iostream>
#include <iomanip>

Display::Display()
{
}

Display::~Display()
{
}

void Display::ShowBoard(const GameBoard& gb)
{
	std::cout << std::endl << gb << std::endl;
}

void Display::ShowMessage(const std::string& msg)
{
	std::cout << msg << std::endl;
}

void Display::ShowGameState(const GameState& gs, size_t num_abilities)
{
	std::cout << "Round: " << gs.RoundNumber << std::endl;
	std::cout << "Your Score: " << gs.UserScore << " Enemy score: " << gs.EnemyScore << std::endl;
	std::cout << "Abilities: " << num_abilities << std::endl;
}
