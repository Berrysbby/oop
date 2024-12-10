#include "ShipManager.h"
#include "Exceptions.h"

ShipManager::ShipManager() 
{
}

ShipManager::ShipManager(const std::vector<size_t>& sizes)
{
	for (auto i = sizes.begin(); i != sizes.end(); ++i)
		Ships.emplace_back(*i);
}

Ship& ShipManager::operator[](size_t index)
{
	if (index >= Ships.size())
		throw OutOfShipSize();
	return Ships[index];
}

const Ship& ShipManager::operator[](size_t index)const
{
	if (index >= Ships.size())
		throw OutOfShipSize();
	return Ships[index];
}


size_t ShipManager::Size()const
{
	return Ships.size();
}

bool ShipManager::AllDestroyed() const
{
	for (const Ship& sh : Ships) {
		if (sh.State() != ShipState::Destroyed)
			return false;
	}
	return true;
}

void ShipManager::Save(std::ostream& str)
{
	str << Ships.size()<<std::endl;
	for (auto& shp : Ships)
		shp.Save(str);
}

void ShipManager::Load(std::istream& str)
{
	size_t sz;
	str >> sz;
	Ships.resize(sz,{1});
	for (auto& shp : Ships)
		shp.Load(str);
}
