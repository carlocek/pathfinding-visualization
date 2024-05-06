#pragma once

#include <SFML/Graphics.hpp>
#include "../libs/Cell.hpp"

class Grid 
{
private:
    int width;
    int height;
    int cellSize;
    float borderWidth;
    std::vector<std::vector<Cell>> cells;

public:
    Grid(const int width, const int height, const int cellSize, const float borderWidth);
    void draw(sf::RenderWindow& window);
    void setCellStateOnMouseClick(sf::RenderWindow& window, CellState currentState);
    Cell* getCell(int x, int y);
};