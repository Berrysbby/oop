#pragma once

#include "Ship.h"
#include <vector>


class ShipManager
{
private:
	std::vector<Ship> Ships;
public:
	ShipManager(const std::vector<size_t>& sizes);
	Ship& operator[](size_t index);
	size_t Size();
};