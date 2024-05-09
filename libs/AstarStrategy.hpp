#pragma once

#include "PathfindingStrategy.hpp"

class AstarStrategy : public PathfindingStrategy
{
public:
	AstarStrategy(Grid* grid);
	~AstarStrategy();
	float search(sf::RenderWindow& window);
	float potential(Cell* start, Cell* end);
	std::vector<Cell*> getAdjacentCells(Cell* currentCell);
};
