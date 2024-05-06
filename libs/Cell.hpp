#pragma once

#include <SFML/Graphics.hpp>

enum class CellState {
    Empty,
    Obstacle,
    Start,
    End,
    Visited
};

class Cell 
{
private:
    int x;
    int y;
    CellState state;

public:
    Cell(int x, int y, CellState state);
    void draw(sf::RenderWindow& window, int cellSize, float borderWidth);
	int getX() const;
	void setX(int x);
	int getY() const;
	void setY(int y);
	CellState getState() const;
	void setState(CellState state);
};
