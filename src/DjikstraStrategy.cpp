#include "DjikstraStrategy.hpp"
#include "PathfindingStrategy.hpp"
#include "Grid.hpp"
#include <limits>
#include <queue>

DjikstraStrategy::DjikstraStrategy(Grid* grid)
: PathfindingStrategy(grid)
{}

DjikstraStrategy::~DjikstraStrategy()
{}

std::vector<Cell*> DjikstraStrategy::search()
{
	const int INF = std::numeric_limits<int>::max();
	std::vector<std::vector<int>> distance(grid->getWidth(), std::vector<int>(grid->getHeight(), INF));
	std::vector<std::vector<Cell*>> predecessor(grid->getWidth(), std::vector<Cell*>(grid->getHeight(), nullptr));
	std::priority_queue<std::pair<Cell*, int>, std::vector<std::pair<Cell*, int>>, std::greater<>> pq;

	distance[grid->getStartCell()->getX()][grid->getStartCell()->getY()] = 0;
	pq.push({grid->getStartCell(), 0});

	while(!pq.empty())
	{
		Cell* currentCell = pq.top().first;
		int currentDistance = pq.top().second;
		pq.pop();

		if (currentCell == grid->getEndCell())
		{
			break;
		}

		// update distances of adjacent cells (up, down, left, right)
		int newDistance = currentDistance + 1;
		if(currentCell->getX() < grid->getWidth())
		{
			if (newDistance < distance[currentCell->getX()+1][currentCell->getY()])
			{
				distance[currentCell->getX()+1][currentCell->getY()] = newDistance;
				predecessor[currentCell->getX()+1][currentCell->getY()] = currentCell;
				pq.push({grid->getCell(currentCell->getX()+1, currentCell->getY()), newDistance});
			}
		}
		if(currentCell->getY() < grid->getHeight())
		{
			if (newDistance < distance[currentCell->getX()][currentCell->getY()+1])
			{
				distance[currentCell->getX()][currentCell->getY()+1] = newDistance;
				predecessor[currentCell->getX()][currentCell->getY()+1] = currentCell;
				pq.push({grid->getCell(currentCell->getX(), currentCell->getY()+1), newDistance});
			}
		}
		if(currentCell->getX() > 0)
		{
			if (newDistance < distance[currentCell->getX()-1][currentCell->getY()])
			{
				distance[currentCell->getX()-1][currentCell->getY()] = newDistance;
				predecessor[currentCell->getX()-1][currentCell->getY()] = currentCell;
				pq.push({grid->getCell(currentCell->getX()-1, currentCell->getY()), newDistance});
			}
		}
		if(currentCell->getY() > 0)
		{
			if (newDistance < distance[currentCell->getX()][currentCell->getY()-1])
			{
				distance[currentCell->getX()][currentCell->getY()-1] = newDistance;
				predecessor[currentCell->getX()][currentCell->getY()-1] = currentCell;
				pq.push({grid->getCell(currentCell->getX(), currentCell->getY()-1), newDistance});
			}
		}
	}

	// reconstruct shortest path if it exists
	std::vector<Cell*> shortestPath;
	if (predecessor[grid->getEndCell()->getX()][grid->getEndCell()->getY()] != nullptr)
	{
		Cell* current = grid->getEndCell();
		while (current != nullptr)
		{
			shortestPath.push_back(current);
			current = predecessor[current->getX()][current->getY()];
		}
		std::reverse(shortestPath.begin(), shortestPath.end());
	}

	return shortestPath;
}

