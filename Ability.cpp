#include <cstdlib>
#include "Ability.h"
#include "Exceptions.h"
#include "ShipManager.h"
#include "Player.h"
#include <iomanip>


bool DoubleDamage::Apply(Game& game)
{
	game.SetNextAttackDouble();
	return false;
}

void DoubleDamage::Save(std::ostream& s )
{
	s << 1<<std::endl;
}

void DoubleDamage::Load(std::istream& s )
{
}

Scanner::Scanner(size_t x, size_t y)
	: X(x),Y(y)
{
}

bool Scanner::Apply(Game& game)
{
	return game.Scan2x2(X,Y);
}

void Scanner::Save(std::ostream& s)
{
	s << 2 << std::endl;
	s << X << " " << Y << std::endl;
}

void Scanner::Load(std::istream& s)
{
	s >> X >> Y;
}


Bombing::Bombing()
{
}

bool Bombing::Apply(Game& game)
{
	game.MakeBombing();
	return false;
}

void Bombing::Save(std::ostream& s)
{
	s << 3 << std::endl;
}

void Bombing::Load(std::istream& s)
{
}


