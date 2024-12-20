#include "AbilityManager.h"
#include "Exceptions.h"
#include <iomanip>


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

void AbilityManager::AddRandomAbility(size_t w, size_t h, unsigned mask)
{
	unsigned r = std::rand() % 3;
	Ability* p = nullptr;
	for (unsigned i = r;!p; i = (i + 1)%3 ) {
		switch (i) {
		case 0:
			if (!(mask & 1))
				continue;
			p = new DoubleDamage();
			break;
		case 1: {
			if (!(mask & 2))
				continue;
			unsigned x = std::rand() % (w - 1);
			unsigned y = std::rand() % (h - 1);
			p = new Scanner(x, y);
			break;
		}
		case 2:
			if (!(mask & 4))
				continue;
			p = new Bombing();
			break;
		}//switch
		break;
	}//for
	if (p) {
		Queue.push_back(std::shared_ptr<Ability>(p));
	}
}

bool AbilityManager::Empty() const
{
	return Queue.empty();
}

size_t AbilityManager::Size() const
{
	return Queue.size();
}

void AbilityManager::InitForNewGame(size_t w, size_t h)
{
	unsigned  mask = 4 | 2 | 1;
	for (int i = 0; i < 3; i++){
		AddRandomAbility(w, h, mask);
		unsigned bitno = Queue.back()->Type();
		mask &= ~(1 << bitno);
	}
}

void AbilityManager::Save(std::ostream& s)
{
	s << Queue.size() << std::endl;
	for (auto& ab : Queue)
		ab->Save(s);
}

void AbilityManager::Load(std::istream& s)
{
	size_t sz;
	s >> sz;
	Queue.clear();
	for (size_t i = 0; i < sz; i++) {
		int type;
		s >> type;
		Ability* p = nullptr;
		switch (type) {
		case 1:
			p = new DoubleDamage();
			break;
		case 2: {
			p = new Scanner(0,0);
			break;
		}
		case 3:
			p = new Bombing();
			break;
		}
		if (p) {
			p->Load(s);
			Queue.push_back(std::shared_ptr<Ability>(p));
		}
	}
}


