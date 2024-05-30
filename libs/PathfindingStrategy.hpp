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
	int checksPerFrame;
public:
	PathfindingStrategy(Grid* grid, int checksPerFrame) : grid(grid), checksPerFrame(checksPerFrame) {};
	virtual ~PathfindingStrategy() {};
	virtual float search(sf::RenderWindow& window);
	virtual int getChecksPerFrame() const;
	virtual void setChecksPerFrame(int checksPerFrame);
};

