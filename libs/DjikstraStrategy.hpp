#pragma once

#include "PathfindingStrategy.hpp"

class DjikstraStrategy : public PathfindingStrategy
{
public:
	DjikstraStrategy(Grid* grid);
	~DjikstraStrategy();
	std::vector<Cell*> search(sf::RenderWindow& window);
	std::vector<Cell*> getAdjacentCells(Cell* currentCell);
};
