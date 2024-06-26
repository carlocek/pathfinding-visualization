#include "DjikstraStrategy.hpp"
#include "PathfindingStrategy.hpp"
#include "Grid.hpp"
#include <limits>
#include <queue>
#include <cstdlib>
#include <iostream>
#include <cmath>

DjikstraStrategy::DjikstraStrategy(Grid* grid, int checksPerFrame)
: PathfindingStrategy(grid, checksPerFrame), distance(grid->getWidth(), std::vector<float>(grid->getHeight(), INF)), predecessor(grid->getWidth(), std::vector<Cell*>(grid->getHeight(), nullptr))
{
	distance[grid->getStartCell()->getX()][grid->getStartCell()->getY()] = 0;
	pq.push({grid->getStartCell(), 0});
}

DjikstraStrategy::~DjikstraStrategy()
{}

float DjikstraStrategy::search(sf::RenderWindow& window)
{
	int checks = 0;
	while(!pq.empty() && checks < checksPerFrame)
	{
		checks++;
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

int DjikstraStrategy::getChecksPerFrame() const
{
	return checksPerFrame;
}

void DjikstraStrategy::setChecksPerFrame(int checksPerFrame)
{
	this->checksPerFrame = checksPerFrame;
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

