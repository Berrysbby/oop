#include <cstdlib>
#include "Ability.h"
#include "Exceptions.h"
#include "ShipManager.h"


bool DoubleDamage::Apply(GameBoard& board)
{
	board.SetNextAttackDouble();
	return false;
}

Scanner::Scanner(size_t x, size_t y)
	: X(x),Y(y)
{
}

bool Scanner::Apply(GameBoard& board)
{
	return board.Scan2x2(X,Y);
}


Bombing::Bombing()
{
}

bool Bombing::Apply(GameBoard& board)
{
	board.MakeBombing();
	return false;
}


