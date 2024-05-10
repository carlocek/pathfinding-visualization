#pragma once

#include "PathfindingStrategy.hpp"
#include <queue>
#include <limits>

class AstarStrategy : public PathfindingStrategy
{
private:
	const int INF = std::numeric_limits<int>::max();
	std::vector<std::vector<float>> distance;
	std::vector<std::vector<Cell*>> predecessor;
	std::priority_queue<std::pair<Cell*, float>, std::vector<std::pair<Cell*, float>>, CompareCellDistance> pq;
	int checksPerFrame;
public:
	AstarStrategy(Grid* grid, int checksPerFrame);
	~AstarStrategy();
	float search(sf::RenderWindow& window);
	float potential(Cell* start, Cell* end);
	std::vector<Cell*> getAdjacentCells(Cell* currentCell);
};
