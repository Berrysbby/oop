#include "Ship.h"
#include "Exceptions.h"

Ship::Ship(size_t ship_size)
{
	Orient=ShipOrientation::Horisontal;
	if (ship_size > 4)
		ship_size = 4;
	if (ship_size < 1)
		ship_size = 1;
	Body.resize(ship_size, ShipState::Good);
}

bool Ship::Attack(size_t n) 
{
	if (n >= Body.size())
		return false;
	if (Body[n] == ShipState::Good)
		Body[n] = ShipState::Damaged;
	else if (Body[n] == ShipState::Damaged )
		Body[n] = ShipState::Destroyed;
	return true;
}

ShipState Ship::State() 
{
	int destr_count = 0;
	int good_count = 0;
	for (auto i = Body.begin(); i != Body.end(); ++i) {
		if (*i == ShipState::Destroyed)
			destr_count++;
		else if (*i == ShipState::Good)
			good_count++;
	}
	if (destr_count ==Body.size())
		return ShipState::Destroyed;
	if (good_count == Body.size())
		return ShipState::Good;
	return ShipState::Damaged;
}

ShipState Ship::State(size_t indx)
{
	if (indx >= Body.size())
		throw OutOfShipSize();
	return Body[indx];
}

size_t Ship::Size()
{
	return Body.size();
}

ShipOrientation Ship::Orientation()
{
	return Orient;
}

void Ship::SetOrientation(ShipOrientation orient)
{
	Orient = orient;
}