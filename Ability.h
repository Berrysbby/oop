#pragma once

#include <memory>
#include <deque>
#include "GameBoard.h"

class Ability 
{
public:
	virtual bool Apply(GameBoard&) = 0;
	virtual ~Ability() {}
};

class AbilityManager
{
public:
	AbilityManager();
	~AbilityManager();
	std::shared_ptr<Ability> GetAbility();
	void AddAbility(std::shared_ptr<Ability>);
	void AddRandomAbility(GameBoard&, ShipManager&);
private:
	std::deque<std::shared_ptr<Ability> > Queue;
};

class DoubleDamage : public Ability 
{
public:
	virtual bool Apply(GameBoard&)override;
};

class Scanner : public Ability
{
public:
	Scanner(size_t x, size_t y);
	virtual bool Apply(GameBoard&)override;
private:
	size_t X, Y;
};

class  Bombing: public Ability
{
public:
	Bombing(ShipManager&);
	virtual bool Apply(GameBoard&)override;
private:
	ShipManager& Shipman;
};