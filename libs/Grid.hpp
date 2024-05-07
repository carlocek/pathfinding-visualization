#pragma once

#include <SFML/Graphics.hpp>
#include "Cell.hpp"
#include "PathfindingStrategy.hpp"

class PathfindingStrategy;

class Grid 
{
private:
    int width;
    int height;
    int cellSize;
    float borderWidth;
    std::vector<std::vector<Cell>> cells;
    Cell* startCell;
    Cell* endCell;
    PathfindingStrategy* strategy;

public:
    Grid(const int width, const int height, const int cellSize, const float borderWidth);
    void draw(sf::RenderWindow& window);
    void setCellStateOnMouseClick(sf::RenderWindow& window, CellState currentState);
    Cell* getCell(int x, int y);
    Cell* getStartCell();
    Cell* getEndCell();
    void setPathfindingStrategy(PathfindingStrategy* strategy);
    PathfindingStrategy* getPathfindingStrategy();
	int getHeight() const;
	void setHeight(int height);
	int getWidth() const;
	void setWidth(int width);
};
