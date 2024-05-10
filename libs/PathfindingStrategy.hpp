#pragma once

#include "Grid.hpp"

struct CompareCellDistance
{
    bool operator()(const std::pair<Cell*, float>& lhs, const std::pair<Cell*, float>& rhs) const
    {
        return lhs.second > rhs.second;
    }
};

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

