#include "GameBoard.h"
#include "Exceptions.h"
#include "Player.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>

void test1() 
{
	ShipManager ships({ 2,1,3,1,3 });
	GameBoard a(ships,10, 8);
	a.PlaceShip( 0, 1, 2, ShipOrientation::Horisontal);
	a.PlaceShip( 1, 4, 6, ShipOrientation::Vertical);
	a.PlaceShip( 2, 6, 2, ShipOrientation::Horisontal);
	a.PlaceShip( 3, 3, 4, ShipOrientation::Horisontal);
	a.PlaceShip( 4, 7, 4, ShipOrientation::Vertical);
	std::cout << a;
	
}

bool test2()
{
	ShipManager ships_init({ 4 });
	GameBoard a(ships_init,10, 8);
	a.PlaceShip( 0, 4, 6, ShipOrientation::Horisontal);
	auto& ship = a.GetShip(0);
	{
		auto [q, w] = a.Attack(6, 6);
		if (q != CellState::Occupied)
			return false;
		if (ship.State(0) != ShipState::Good)
			return false;
		if (ship.State(1) != ShipState::Good)
			return false;
		if (ship.State(2) != ShipState::Damaged)
			return false;
		if (ship.State(3) != ShipState::Good)
			return false;
	}
	{
		auto [q, w] = a.Attack(6, 6);
		if (ship.State(2) != ShipState::Destroyed)
			return false;
		if (ship.State(0) != ShipState::Good)
			return false;
		if (ship.State(1) != ShipState::Good)
			return false;
		if (ship.State(3) != ShipState::Good)
			return false;
	}
	{
		auto [q, w] = a.Attack(1, 2);
		if (q != CellState::Empty)
			return false;
	}
	try{
		auto [q, w] = a.Attack(11, 11);
		return false;
	}
	catch (const MyException& e ){
		std::cout << "it's right: "<< e.GetErrorText() << std::endl;
	}
	return true;
}

void test3()
{
	Game gm;
	gm.InitializeGame();
	gm.Round();
}

int main() 
{
	try {
		test1();
		if (test2())
			std::cout << "Test #2 passed" << std::endl;
		else
			std::cout << "Test #2 failed" << std::endl;
		test3();
		return EXIT_SUCCESS;
	}
	catch (const MyException& e) {
		std::cout << "it's unexpected: " << e.GetErrorText() << std::endl;
	}
	catch (const std::exception& e) {
		std::cout << "it's unexpected: " << e.what() << std::endl;
	}
	return EXIT_FAILURE;
}