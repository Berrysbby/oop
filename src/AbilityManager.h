#pragma once

#include <memory>
#include <deque>
#include "GameBoard.h"
#include "Ability.h"
#include <ostream>
#include <istream>


class AbilityManager
{
public:
	AbilityManager();
	~AbilityManager();
	std::shared_ptr<Ability> GetAbility();
	void AddAbility(std::shared_ptr<Ability>);
	void AddRandomAbility(size_t w, size_t h, unsigned mask = 0xFFFF ); 
	bool Empty()const;
	size_t Size()const;
	void InitForNewGame(size_t w, size_t h);
	void Save(std::ostream&);
	void Load(std::istream&);
private:
	std::deque<std::shared_ptr<Ability> > Queue;
};
