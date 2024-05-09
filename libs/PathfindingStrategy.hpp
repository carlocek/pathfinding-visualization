#pragma once

#include "Grid.hpp"

class Grid;

class PathfindingStrategy
{
protected:
	Grid* grid;
public:
	PathfindingStrategy(Grid* grid) : grid(grid) {};
	virtual ~PathfindingStrategy() {};
	virtual float search(sf::RenderWindow& window);
};

