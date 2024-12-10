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
	//атаковать клетку n. возвращает true, если попали в корабль
	bool Attack(size_t n);
	//состояние корабля в целом
	ShipState State()const;
	//состояние клетки indx корабля
	ShipState State(size_t indx)const;
	//размер корабля в клетках
	size_t Size()const;
	//ориентация
	ShipOrientation Orientation()const;
	void SetOrientation(ShipOrientation);
};



