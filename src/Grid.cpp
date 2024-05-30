#include "Grid.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>

Grid::Grid(const int width, const int height, const int cellSize, const float borderWidth)
: width(width), height(height), cellSize(cellSize), borderWidth(borderWidth), startCell(nullptr), endCell(nullptr), strategy(nullptr)
{
	int widthCells = floor(width/cellSize);
	int heightCells = floor(height/cellSize);
	this->cells.resize(widthCells, std::vector<Cell>(heightCells, Cell(0, 0, CellState::Empty)));
	for(int x = 0; x < widthCells; x++)
	{
		for(int y = 0; y < heightCells; y++)
		{
			this->cells[x][y] = Cell(x, y, CellState::Empty);
		}
	}
}

void Grid::draw(sf::RenderWindow& window)
{
	int widthCells = floor(width/cellSize);
	int heightCells = floor(height/cellSize);
    for(int x = 0; x < widthCells; x++)
    {
        for(int y = 0; y < heightCells; y++)
        {
        	this->cells[x][y].draw(window, cellSize, borderWidth);
        }
    }
}

void Grid::reset()
{
	int widthCells = floor(width/cellSize);
	int heightCells = floor(height/cellSize);
	for(int x = 0; x < widthCells; x++)
	{
		for(int y = 0; y < heightCells; y++)
		{
			if(this->cells[x][y].getState() == CellState::Visited || this->cells[x][y].getState() == CellState::Path)
				this->cells[x][y].setState(CellState::Empty);
		}
	}
}

void Grid::clear()
{
	this->startCell = nullptr;
	this->endCell = nullptr;
	int widthCells = floor(width/cellSize);
	int heightCells = floor(height/cellSize);
	for(int x = 0; x < widthCells; x++)
	{
		for(int y = 0; y < heightCells; y++)
		{
			this->cells[x][y].setState(CellState::Empty);
		}
	}
}

void Grid::setCellStateOnMouseClick(sf::RenderWindow& window, CellState currentState)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    int x = mousePos.x/cellSize;
    int y = mousePos.y/cellSize;
    Cell* cell = getCell(x, y);
    if (cell != nullptr)
    {
    	if(currentState == CellState::Empty)
		{
			cell->setState(currentState);
		}
    	if(currentState == CellState::Start)
    	{
    		if(startCell != nullptr)
				startCell->setState(CellState::Empty);
    		startCell = cell;
    		cell->setState(currentState);
    	}
    	else if(currentState == CellState::End)
		{
    		if(endCell != nullptr)
				endCell->setState(CellState::Empty);
    		endCell = cell;
			cell->setState(currentState);
		}
    	else if(currentState == CellState::Obstacle)
		{
    		if (cell == startCell || cell == endCell)
				return;
    		cell->setState(CellState::Obstacle);
//    		cell->setState(cell->getState() == CellState::Obstacle ? CellState::Empty : CellState::Obstacle);
		}
    }
}

Cell* Grid::getCell(int x, int y)
{
	if (x >= 0 && x < width/cellSize && y >= 0 && y < height/cellSize)
	{
		return &cells[x][y];
	}
	else
	{
		return nullptr;
	}
}

Cell* Grid::getStartCell()
{
	return startCell;
}

Cell* Grid::getEndCell()
{
	return endCell;
}

void Grid::setPathfindingStrategy(PathfindingStrategy* strategy)
{
	this->strategy = strategy;
}

PathfindingStrategy* Grid::getPathfindingStrategy()
{
	return strategy;
}

int Grid::getHeight() const
{
	return height;
}

void Grid::setHeight(int height)
{
	this->height = height;
}

int Grid::getWidth() const
{
	return width;
}

void Grid::setWidth(int width)
{
	this->width = width;
}

int Grid::getCellSize() const
{
	return cellSize;
}

void Grid::setCellSize(int cellSize)
{
	this->reset();
	this->cellSize = cellSize;
	this->borderWidth = cellSize/10 + 1.0;
	int widthCells = floor(width/cellSize);
	int heightCells = floor(height/cellSize);
	this->cells.resize(widthCells, std::vector<Cell>(heightCells, Cell(0, 0, CellState::Empty)));
	for(int x = 0; x < widthCells; x++)
	{
		for(int y = 0; y < heightCells; y++)
		{
			this->cells[x][y] = Cell(x, y, CellState::Empty);
		}
	}
}
