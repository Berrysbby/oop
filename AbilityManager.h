#pragma once

#include <memory>
#include <deque>
#include "GameBoard.h"
#include "Ability.h"


class AbilityManager
{
public:
	AbilityManager();
	~AbilityManager();
	std::shared_ptr<Ability> GetAbility();
	void AddAbility(std::shared_ptr<Ability>);
	void AddRandomAbility(GameBoard&);
	bool Empty()const;
private:
	std::deque<std::shared_ptr<Ability> > Queue;
};
