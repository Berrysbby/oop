#include "AbilityManager.h"
#include "Exceptions.h"


AbilityManager::AbilityManager() :Queue()
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

void AbilityManager::AddRandomAbility(GameBoard& board)
{
	unsigned r = std::rand();
	Ability* p = nullptr;
	switch (r % 3) {
	case 0:
		p = new DoubleDamage();
		break;
	case 1: {
		auto [w, h] = board.GetBoardSize();
		unsigned x = std::rand() % (w - 1);
		unsigned y = std::rand() % (h - 1);
		p = new Scanner(x, y);
		break;
	}
	case 2:
		p = new Bombing();
		break;
	}
	if (p) {
		Queue.push_back(std::shared_ptr<Ability>(p));
	}
}

bool AbilityManager::Empty() const
{
	return Queue.empty();
}
