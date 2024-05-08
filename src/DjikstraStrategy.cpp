#include "DjikstraStrategy.hpp"
#include "PathfindingStrategy.hpp"
#include "Grid.hpp"
#include <limits>
#include <queue>
#include <cstdlib>
#include <iostream>
#include <cmath>

// comparison function for priority queue
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
	std::vector<std::vector<float>> distance(grid->getWidth(), std::vector<float>(grid->getHeight(), INF));
	std::vector<std::vector<Cell*>> predecessor(grid->getWidth(), std::vector<Cell*>(grid->getHeight(), nullptr));
//	std::priority_queue<std::pair<Cell*, int>, std::vector<std::pair<Cell*, int>>, std::less<>> pq;
	std::priority_queue<std::pair<Cell*, float>, std::vector<std::pair<Cell*, float>>, CompareCellDistance> pq;

	distance[grid->getStartCell()->getX()][grid->getStartCell()->getY()] = 0;
	pq.push({grid->getStartCell(), 0});

	while(!pq.empty())
	{
		Cell* currentCell = pq.top().first;
		float currentDistance = pq.top().second;
		pq.pop();

		if(currentCell->getState() == CellState::Obstacle)
			continue;

		if(currentCell != grid->getStartCell() && currentCell != grid->getEndCell())
			currentCell->setState(CellState::Visited);

		if(currentCell == grid->getEndCell())
			break;

		// update distances of adjacent cells (up, down, left, right)
		std::vector<Cell*> adjacentCells = getAdjacentCells(currentCell);
		for(Cell* adjacentCell : adjacentCells)
		{
			float newDistance;
			if(adjacentCell->getX() != currentCell->getX() && adjacentCell->getY() != currentCell->getY())
				newDistance = currentDistance + sqrt(2);
			else
				newDistance = currentDistance + 1;
			if (newDistance < distance[adjacentCell->getX()][adjacentCell->getY()])
			{
				distance[adjacentCell->getX()][adjacentCell->getY()] = newDistance;
				predecessor[adjacentCell->getX()][adjacentCell->getY()] = currentCell;
				if(grid->getCell(adjacentCell->getX(), adjacentCell->getY())->getState() != CellState::Visited)
					pq.push({grid->getCell(adjacentCell->getX(), adjacentCell->getY()), newDistance});
			}
		}

		grid->draw(window);
		window.display(); // to be able to see algorithm steps
//		sf::Time waitTime = sf::seconds(0.2f);
//		sf::sleep(waitTime);
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
	}

	return shortestPath;
}

std::vector<Cell*> DjikstraStrategy::getAdjacentCells(Cell* currentCell)
{
	std::vector<Cell*> adjacentCells;
	std::vector<Cell*> diagonalAdjacentCells;
	// right, top, left, down
	if(currentCell->getX() < grid->getWidth()/grid->getCellSize()-1)
	{
		adjacentCells.push_back(grid->getCell(currentCell->getX()+1, currentCell->getY()));
	}
	if(currentCell->getY() < grid->getHeight()/grid->getCellSize()-1)
	{
		adjacentCells.push_back(grid->getCell(currentCell->getX(), currentCell->getY()+1));
	}
	if(currentCell->getX() > 0)
	{
		adjacentCells.push_back(grid->getCell(currentCell->getX()-1, currentCell->getY()));
	}
	if(currentCell->getY() > 0)
	{
		adjacentCells.push_back(grid->getCell(currentCell->getX(), currentCell->getY()-1));
	}
	// diagonal cells
	if(currentCell->getX() < grid->getWidth()/grid->getCellSize()-1 && currentCell->getY() < grid->getHeight()/grid->getCellSize()-1
			&& grid->getCell(currentCell->getX()+1, currentCell->getY())->getState() != CellState::Obstacle && grid->getCell(currentCell->getX(), currentCell->getY()+1)->getState() != CellState::Obstacle)
	{
		diagonalAdjacentCells.push_back(grid->getCell(currentCell->getX()+1, currentCell->getY()+1));
	}
	if(currentCell->getX() < grid->getWidth()/grid->getCellSize()-1 && currentCell->getY() > 0
			&& grid->getCell(currentCell->getX()+1, currentCell->getY())->getState() != CellState::Obstacle && grid->getCell(currentCell->getX(), currentCell->getY()-1)->getState() != CellState::Obstacle)
	{
		diagonalAdjacentCells.push_back(grid->getCell(currentCell->getX()+1, currentCell->getY()-1));
	}
	if(currentCell->getX() > 0 && currentCell->getY() < grid->getHeight()/grid->getCellSize()-1
			&& grid->getCell(currentCell->getX()-1, currentCell->getY())->getState() != CellState::Obstacle && grid->getCell(currentCell->getX(), currentCell->getY()+1)->getState() != CellState::Obstacle)
	{
		diagonalAdjacentCells.push_back(grid->getCell(currentCell->getX()-1, currentCell->getY()+1));
	}
	if(currentCell->getX() > 0 && currentCell->getY() > 0
			&& grid->getCell(currentCell->getX()-1, currentCell->getY())->getState() != CellState::Obstacle && grid->getCell(currentCell->getX(), currentCell->getY()-1)->getState() != CellState::Obstacle)
	{
		diagonalAdjacentCells.push_back(grid->getCell(currentCell->getX()-1, currentCell->getY()-1));
	}
	adjacentCells.insert(adjacentCells.end(), diagonalAdjacentCells.begin(), diagonalAdjacentCells.end());
	return adjacentCells;
}

