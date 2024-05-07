#include "Grid.hpp"

#include <SFML/Graphics.hpp>

Grid::Grid(const int width, const int height, const int cellSize, const float borderWidth) : width(width), height(height), cellSize(cellSize), borderWidth(borderWidth)
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
        cell->setState(currentState);
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
