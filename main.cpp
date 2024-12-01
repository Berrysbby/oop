#include "GameBoard.h"
#include "Exceptions.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>

void test1() 
{
	ShipManager ships({ 2,1,3,1,3 });
	GameBoard a(10, 8);
	a.PlaceShip(ships, 0, 1, 2, ShipOrientation::Horisontal);
	a.PlaceShip(ships, 1, 4, 6, ShipOrientation::Vertical);
	a.PlaceShip(ships, 2, 6, 2, ShipOrientation::Horisontal);
	a.PlaceShip(ships, 3, 3, 4, ShipOrientation::Horisontal);
	a.PlaceShip(ships, 4, 7, 4, ShipOrientation::Vertical);
	a.Display(ships);
	
}

bool test2()
{
	ShipManager ships({ 4 });
	GameBoard a(10, 8);
	a.PlaceShip(ships, 0, 4, 6, ShipOrientation::Horisontal);
	auto q=a.Attack(ships, 6, 6);
	if (q != CellState::Occupied)
		return false;
	auto& ship = ships[0];
	if (ship.State(0) != ShipState::Good)
		return false;
	if (ship.State(1) != ShipState::Good)
		return false;
	if (ship.State(2) != ShipState::Damaged)
		return false;
	if (ship.State(3) != ShipState::Good)
		return false;
	q = a.Attack(ships, 6, 6);
	if (ship.State(2) != ShipState::Destroyed)
		return false;
	if (ship.State(0) != ShipState::Good)
		return false;
	if (ship.State(1) != ShipState::Good)
		return false;
	if (ship.State(3) != ShipState::Good)
		return false;
	q = a.Attack(ships, 1, 2);
	if (q != CellState::Empty)
		return false;
	try{
		q= a.Attack(ships, 11, 11);
		return false;
	}
	catch (const MyException& e ){
		std::cout << "it's right: "<< e.GetErrorText() << std::endl;
	}
	return true;
}

int main() 
{
	try {
		test1();
		if (test2())
			std::cout << "Test #2 passed" << std::endl;
		else
			std::cout << "Test #2 failed" << std::endl;
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