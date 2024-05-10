#pragma once

#include "PathfindingStrategy.hpp"
#include <queue>
#include <limits>

class DjikstraStrategy : public PathfindingStrategy
{
private:
	const int INF = std::numeric_limits<int>::max();
	std::vector<std::vector<float>> distance;
	std::vector<std::vector<Cell*>> predecessor;
	std::priority_queue<std::pair<Cell*, float>, std::vector<std::pair<Cell*, float>>, CompareCellDistance> pq;
	int checksPerFrame;
public:
	DjikstraStrategy(Grid* grid, int checksPerFrame);
	~DjikstraStrategy();
	float search(sf::RenderWindow& window);
	std::vector<Cell*> getAdjacentCells(Cell* currentCell);
};
