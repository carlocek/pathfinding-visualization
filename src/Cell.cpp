#include "Cell.hpp"

#include <SFML/Graphics.hpp>

Cell::Cell(int x, int y, CellState state = CellState::Empty) : x(x), y(y), state(state)
{}

void Cell::draw(sf::RenderWindow& window, int cellSize, float borderWidth)
{
        sf::RectangleShape rect(sf::Vector2f(cellSize, cellSize));
        rect.setPosition(x*cellSize, y*cellSize);
        switch(state)
        {
            case CellState::Empty:
                rect.setFillColor(sf::Color::White);
                break;
            case CellState::Obstacle:
                rect.setFillColor(sf::Color{64, 64, 64});//dark grey
                break;
            case CellState::Start:
                rect.setFillColor(sf::Color::Green);
                break;
            case CellState::End:
                rect.setFillColor(sf::Color::Red);
                break;
            case CellState::Visited:
                rect.setFillColor(sf::Color::Blue);
                break;
            case CellState::Path:
				rect.setFillColor(sf::Color::Cyan);
				break;
        }
        rect.setOutlineThickness(borderWidth);
        rect.setOutlineColor(sf::Color::Black);
        window.draw(rect);
}

CellState Cell::getState() const
{
	return state;
}

void Cell::setState(CellState state)
{
	this->state = state;
}

int Cell::getX() const
{
	return x;
}

void Cell::setX(int x)
{
	this->x = x;
}

int Cell::getY() const
{
	return y;
}

void Cell::setY(int y)
{
	this->y = y;
}

