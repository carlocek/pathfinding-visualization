#include "AstarStrategy.hpp"
#include "PathfindingStrategy.hpp"
#include "Grid.hpp"
#include <limits>
#include <queue>
#include <cstdlib>
#include <iostream>
#include <cmath>

// comparison function for priority queue
//struct CompareCellDistance
//{
//    bool operator()(const std::pair<Cell*, float>& lhs, const std::pair<Cell*, float>& rhs) const
//    {
//        return lhs.second > rhs.second;
//    }
//};

AstarStrategy::AstarStrategy(Grid* grid, int checksPerFrame)
: PathfindingStrategy(grid, checksPerFrame), distance(grid->getWidth(), std::vector<float>(grid->getHeight(), INF)), predecessor(grid->getWidth(), std::vector<Cell*>(grid->getHeight(), nullptr))
{
	distance[grid->getStartCell()->getX()][grid->getStartCell()->getY()] = potential(grid->getStartCell(), grid->getEndCell());
	pq.push({grid->getStartCell(), potential(grid->getStartCell(), grid->getEndCell())});
}

AstarStrategy::~AstarStrategy()
{}

float AstarStrategy::search(sf::RenderWindow& window)
{
	int checks = 0;
	while(!pq.empty() && checks < checksPerFrame)
	{
		checks++;
		Cell* currentCell = pq.top().first;
		float currentDistancePotential = pq.top().second;
		float currentDistance = currentDistancePotential - potential(currentCell, grid->getEndCell());;
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
				newDistancePotential = currentDistance + sqrt(2) + potential(adjacentCell, grid->getEndCell());
			}
			else
			{
				newDistancePotential = currentDistance + 1 + potential(adjacentCell, grid->getEndCell());
			}
			if (newDistancePotential < distance[adjacentCell->getX()][adjacentCell->getY()])
			{
				distance[adjacentCell->getX()][adjacentCell->getY()] = newDistancePotential;
				predecessor[adjacentCell->getX()][adjacentCell->getY()] = currentCell;
				if(grid->getCell(adjacentCell->getX(), adjacentCell->getY())->getState() != CellState::Visited)
					pq.push({grid->getCell(adjacentCell->getX(), adjacentCell->getY()), newDistancePotential});
			}
		}
	}

	// reconstruct shortest path if it exists
	if (predecessor[grid->getEndCell()->getX()][grid->getEndCell()->getY()] != nullptr)
	{
		Cell* currentCell = grid->getEndCell();
		while (currentCell != nullptr)
		{
			if(currentCell != grid->getStartCell() && currentCell != grid->getEndCell())
				currentCell->setState(CellState::Path);
			currentCell = predecessor[currentCell->getX()][currentCell->getY()];
		}
		grid->draw(window);
		return distance[grid->getEndCell()->getX()][grid->getEndCell()->getY()];
	}
	return -1;
}

float AstarStrategy::potential(Cell *start, Cell *end)
{
	return sqrt(pow(start->getX()-end->getX(), 2) + pow(start->getY()-end->getY(), 2));
}

int AstarStrategy::getChecksPerFrame() const
{
	return checksPerFrame;
}

void AstarStrategy::setChecksPerFrame(int checksPerFrame)
{
	this->checksPerFrame = checksPerFrame;
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

