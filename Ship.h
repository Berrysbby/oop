#pragma once

#include <vector>

enum class ShipState { Good, Damaged, Destroyed };
enum class ShipOrientation { Horisontal, Vertical};


class Ship
{
private:
	std::vector<ShipState> Body;
	ShipOrientation Orient;
	
public:
	Ship(size_t ship_size);
	bool Attack(size_t n);
	ShipState State();
	ShipState State(size_t indx);
	size_t Size();
	ShipOrientation Orientation();
	void SetOrientation(ShipOrientation);
};



