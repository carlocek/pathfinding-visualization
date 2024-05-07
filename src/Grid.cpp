#include "Grid.hpp"

#include <SFML/Graphics.hpp>

Grid::Grid(const int width, const int height, const int cellSize, const float borderWidth)
: width(width), height(height), cellSize(cellSize), borderWidth(borderWidth), startCell(nullptr), endCell(nullptr), strategy(nullptr)
{
    this->cells.resize(width / cellSize, std::vector<Cell>(height / cellSize, Cell(0, 0, CellState::Empty)));
    for(int i = 0; i < width/cellSize; i++)
    {
        for(int j = 0; j < height/cellSize; j++)
        {
            this->cells[i][j] = Cell(i, j, CellState::Empty);
        }
    }
}

void Grid::draw(sf::RenderWindow& window)
{
    for(int x = 0; x < width/cellSize; x++)
    {
        for(int y = 0; y < height/cellSize; y++)
        {
        	this->cells[x][y].draw(window, cellSize, borderWidth);
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
    		cell->setState(cell->getState() == CellState::Obstacle ? CellState::Empty : CellState::Obstacle);
		}
    }
}

Cell* Grid::getCell(int x, int y)
{
	if (x >= 0 && x < width / cellSize && y >= 0 && y < height / cellSize)
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
