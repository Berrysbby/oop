#pragma once

#include <memory>
#include <deque>
#include <ostream>
#include <istream>

class Game;

class Ability 
{
public:
	virtual bool Apply(Game&) = 0;
	virtual ~Ability() {}
	virtual void Save(std::ostream&)=0;
	virtual void Load(std::istream&) = 0;
};

class DoubleDamage : public Ability 
{
public:
	virtual bool Apply(Game&)override;
	virtual void Save(std::ostream&)override;
	virtual void Load(std::istream&)override;
};

class Scanner : public Ability
{
public:
	Scanner(size_t x, size_t y);
	virtual bool Apply(Game&)override;	
	virtual void Save(std::ostream&)override;
	virtual void Load(std::istream&)override;
private:
	size_t X, Y;
};

class  Bombing: public Ability
{
public:
	Bombing();
	virtual bool Apply(Game&)override;
	virtual void Save(std::ostream&)override;
	virtual void Load(std::istream&)override;

};