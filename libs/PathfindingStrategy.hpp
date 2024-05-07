#pragma once

#include "Cell.hpp"
#include "Grid.hpp"

class PathfindingStrategy
{
private:
	Cell* startCell;
	Cell* endCell;
	Grid grid;
public:
	virtual PathfindingStrategy(Grid grid);
	virtual ~PathfindingStrategy();
	virtual std::vector<Cell*> search(Grid grid, Cell* startCell, Cell* endCell);
};

