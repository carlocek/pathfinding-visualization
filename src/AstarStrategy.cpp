#include "AstarStrategy.hpp"
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
    bool operator()(const std::pair<Cell*, float>& lhs, const std::pair<Cell*, float>& rhs) const
    {
        return lhs.second > rhs.second;
    }
};

AstarStrategy::AstarStrategy(Grid* grid)
: PathfindingStrategy(grid)
{}

AstarStrategy::~AstarStrategy()
{}

float AstarStrategy::search(sf::RenderWindow& window)
{
	const int INF = std::numeric_limits<int>::max();
	std::vector<std::vector<float>> distance(grid->getWidth(), std::vector<float>(grid->getHeight(), INF));
	std::vector<std::vector<Cell*>> predecessor(grid->getWidth(), std::vector<Cell*>(grid->getHeight(), nullptr));
//	std::priority_queue<std::pair<Cell*, int>, std::vector<std::pair<Cell*, int>>, std::less<>> pq;
	std::priority_queue<std::pair<Cell*, float>, std::vector<std::pair<Cell*, float>>, CompareCellDistance> pq;

	distance[grid->getStartCell()->getX()][grid->getStartCell()->getY()] = potential(grid->getStartCell(), grid->getEndCell());
	pq.push({grid->getStartCell(), potential(grid->getStartCell(), grid->getEndCell())});

	int nframes = 0;
	int drawFrame = 10; //to adjust visualization speed (1 to see each iteration with slow speed)
	while(!pq.empty())
	{
		Cell* currentCell = pq.top().first;
		float currentDistancePotential = pq.top().second;
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
			float newDistancePotential;
			if(adjacentCell->getX() != currentCell->getX() && adjacentCell->getY() != currentCell->getY())
			{
				newDistancePotential = currentDistancePotential + sqrt(2) + potential(adjacentCell, grid->getEndCell());
			}
			else
			{
				newDistancePotential = currentDistancePotential + 1 + potential(adjacentCell, grid->getEndCell());
			}
			if (newDistancePotential < distance[adjacentCell->getX()][adjacentCell->getY()])
			{
				distance[adjacentCell->getX()][adjacentCell->getY()] = newDistancePotential;
				predecessor[adjacentCell->getX()][adjacentCell->getY()] = currentCell;
				if(grid->getCell(adjacentCell->getX(), adjacentCell->getY())->getState() != CellState::Visited)
					pq.push({grid->getCell(adjacentCell->getX(), adjacentCell->getY()), newDistancePotential});
			}
		}

		grid->draw(window);
		// remove this to see instantaneous result
		if(nframes % drawFrame == 0)
			window.display();
//		sf::Time waitTime = sf::seconds(0.2f);
//		sf::sleep(waitTime);
		nframes++;
	}

	// reconstruct shortest path if it exists
	float length = distance[grid->getEndCell()->getX()][grid->getEndCell()->getY()];
	if (predecessor[grid->getEndCell()->getX()][grid->getEndCell()->getY()] != nullptr)
	{
		Cell* currentCell = grid->getEndCell();
		while (currentCell != nullptr)
		{
			length -= potential(currentCell, grid->getEndCell());
			if(currentCell != grid->getStartCell() && currentCell != grid->getEndCell())
				currentCell->setState(CellState::Path);
			currentCell = predecessor[currentCell->getX()][currentCell->getY()];
		}
		grid->draw(window);
	}
	return length;
}

float AstarStrategy::potential(Cell *start, Cell *end)
{
	return sqrt(pow(start->getX()-end->getX(), 2) + pow(start->getY()-end->getY(), 2));
}

std::vector<Cell*> AstarStrategy::getAdjacentCells(Cell* currentCell)
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
	// diagonal cells (check also invalid diagonal moves passing between two obstacle cells)
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

