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


Bombing::Bombing(ShipManager& shipman)
	: Shipman(shipman)
{
}

bool Bombing::Apply(GameBoard& board)
{
	board.MakeBombing(Shipman);
	return false;
}

AbilityManager::AbilityManager():Queue()
{
}

AbilityManager::~AbilityManager()
{
}

std::shared_ptr<Ability> AbilityManager::GetAbility()
{
	if (Queue.empty())
		throw AbilityAbsence();
	auto ab = Queue.front();
	Queue.pop_front();
	return ab;
}

void AbilityManager::AddAbility(std::shared_ptr<Ability> ab)
{
	Queue.push_back(ab);
}

void AbilityManager::AddRandomAbility(GameBoard& board,ShipManager& shipman)
{
	unsigned r = std::rand();
	Ability* p = nullptr;
	switch (r % 3) {
	case 0:
		p = new DoubleDamage();
		break;
	case 1:{
		auto [w, h] = board.GetBoardSize();
		unsigned x = std::rand() % (w-1);
		unsigned y = std::rand() % (h-1);
		p = new Scanner(x,y);
		break;
	}
	case 2:
		p = new Bombing(shipman);
		break;
	}
	if (p) {
		Queue.push_back(std::shared_ptr<Ability>( p));
	}
}
