#include "DjikstraStrategy.hpp"
#include "PathfindingStrategy.hpp"
#include "Grid.hpp"
#include <limits>
#include <queue>
#include <cstdlib>
#include <iostream>

struct CellDistance
{
    Cell* cell;
    int distance;
    CellDistance(Cell* cell, int distance) : cell(cell), distance(distance) {}
};

// Comparison function for priority queue
struct CompareCellDistance
{
    bool operator()(const std::pair<Cell*, int>& lhs, const std::pair<Cell*, int>& rhs) const
    {
        return lhs.second > rhs.second;
    }
};

DjikstraStrategy::DjikstraStrategy(Grid* grid)
: PathfindingStrategy(grid)
{}

DjikstraStrategy::~DjikstraStrategy()
{}

std::vector<Cell*> DjikstraStrategy::search(sf::RenderWindow& window)
{
	const int INF = std::numeric_limits<int>::max();
	std::vector<std::vector<int>> distance(grid->getWidth(), std::vector<int>(grid->getHeight(), INF));
	std::vector<std::vector<Cell*>> predecessor(grid->getWidth(), std::vector<Cell*>(grid->getHeight(), nullptr));
//	std::priority_queue<std::pair<Cell*, int>, std::vector<std::pair<Cell*, int>>, std::less<>> pq;
	std::priority_queue<std::pair<Cell*, int>, std::vector<std::pair<Cell*, int>>, CompareCellDistance> pq;

	distance[grid->getStartCell()->getX()][grid->getStartCell()->getY()] = 0;
	pq.push({grid->getStartCell(), 0});

	while(!pq.empty())
	{
		Cell* currentCell = pq.top().first;
		int currentDistance = pq.top().second;
		pq.pop();
		if(currentCell != grid->getStartCell() && currentCell != grid->getEndCell())
			currentCell->setState(CellState::Visited);

		if(currentCell == grid->getEndCell())
			break;

		// update distances of adjacent cells (up, down, left, right)
		int newDistance = currentDistance + 1;
		if(currentCell->getX() < grid->getWidth()/grid->getCellSize()-1)
		{
			if (newDistance < distance[currentCell->getX()+1][currentCell->getY()])
			{
				distance[currentCell->getX()+1][currentCell->getY()] = newDistance;
				predecessor[currentCell->getX()+1][currentCell->getY()] = currentCell;
				if(grid->getCell(currentCell->getX()+1, currentCell->getY())->getState() != CellState::Visited)
					pq.push({grid->getCell(currentCell->getX()+1, currentCell->getY()), newDistance});
			}
		}
		if(currentCell->getY() < grid->getHeight()/grid->getCellSize()-1)
		{
			if (newDistance < distance[currentCell->getX()][currentCell->getY()+1])
			{
				distance[currentCell->getX()][currentCell->getY()+1] = newDistance;
				predecessor[currentCell->getX()][currentCell->getY()+1] = currentCell;
				if(grid->getCell(currentCell->getX(), currentCell->getY()+1)->getState() != CellState::Visited)
					pq.push({grid->getCell(currentCell->getX(), currentCell->getY()+1), newDistance});
			}
		}
		if(currentCell->getX() > 0)
		{
			if (newDistance < distance[currentCell->getX()-1][currentCell->getY()])
			{
				distance[currentCell->getX()-1][currentCell->getY()] = newDistance;
				predecessor[currentCell->getX()-1][currentCell->getY()] = currentCell;
				if(grid->getCell(currentCell->getX()-1, currentCell->getY())->getState() != CellState::Visited)
					pq.push({grid->getCell(currentCell->getX()-1, currentCell->getY()), newDistance});
			}
		}
		if(currentCell->getY() > 0)
		{
			if (newDistance < distance[currentCell->getX()][currentCell->getY()-1])
			{
				distance[currentCell->getX()][currentCell->getY()-1] = newDistance;
				predecessor[currentCell->getX()][currentCell->getY()-1] = currentCell;
				if(grid->getCell(currentCell->getX(), currentCell->getY()-1)->getState() != CellState::Visited)
					pq.push({grid->getCell(currentCell->getX(), currentCell->getY()-1), newDistance});
			}
		}

		grid->draw(window);
		window.display();
		sf::Time waitTime = sf::seconds(0.5f);
		sf::sleep(waitTime);
	}

	// reconstruct shortest path if it exists
	std::vector<Cell*> shortestPath;
	if (predecessor[grid->getEndCell()->getX()][grid->getEndCell()->getY()] != nullptr)
	{
		Cell* currentCell = grid->getEndCell();
		while (currentCell != nullptr)
		{
			if(currentCell != grid->getStartCell() && currentCell != grid->getEndCell())
				currentCell->setState(CellState::Path);
			shortestPath.push_back(currentCell);
			currentCell = predecessor[currentCell->getX()][currentCell->getY()];
		}
		std::reverse(shortestPath.begin(), shortestPath.end());
		grid->draw(window);
		window.display();
	}

	return shortestPath;
}

