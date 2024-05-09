#pragma once

#include "PathfindingStrategy.hpp"

class DjikstraStrategy : public PathfindingStrategy
{
public:
	DjikstraStrategy(Grid* grid);
	~DjikstraStrategy();
	float search(sf::RenderWindow& window);
	std::vector<Cell*> getAdjacentCells(Cell* currentCell);
};
