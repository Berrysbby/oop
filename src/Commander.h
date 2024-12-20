#pragma once

class Commander 
{
public:
	struct ShipPlacement { size_t x, y; bool Vert; };
	enum class Cmd { Quit, Load, Save, Move, Error };
	struct NextAction { Cmd cmd; size_t x, y; bool ab;};
public:
	Commander();
	~Commander();
	bool AskPlayerboardType();
	ShipPlacement AskShipPlacement(size_t s);
	NextAction AskNextAction(bool ab);
};