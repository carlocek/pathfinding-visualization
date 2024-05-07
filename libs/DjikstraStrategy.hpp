#pragma once

#include "Cell.hpp"
#include "Grid.hpp"
#include "PathfindingStrategy.hpp"

class DjikstraStrategy : public PathfindingStrategy
{
public:
	DjikstraStrategy(Grid grid);
	std::vector<Cell*> search(Grid grid, Cell* startCell, Cell* endCell);
};
