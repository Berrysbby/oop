#include "ShipManager.h"
#include <exception>

ShipManager::ShipManager(const std::vector<size_t>& sizes) 
{
	for (auto i = sizes.begin(); i != sizes.end(); ++i)
		Ships.emplace_back(*i);
}

Ship& ShipManager::operator[](size_t index)
{
	if (index >= Ships.size())
		throw std::exception();
	return Ships[index];
}

size_t ShipManager::Size()
{
	return Ships.size();
}
