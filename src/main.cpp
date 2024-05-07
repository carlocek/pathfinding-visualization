#include <SFML/Graphics.hpp>
#include "Cell.hpp"
#include "Grid.hpp"

int main()
{
	const int WIDTH = 800;
	const int HEIGHT = 800;
	const int CELL_SIZE = 50;
	const float CELL_BORDER_SIZE = 4.0;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "My window");
    window.setFramerateLimit(60);

    Grid grid = Grid(WIDTH, HEIGHT, CELL_SIZE, CELL_BORDER_SIZE);
    CellState currentState = CellState::Empty;

    while (window.isOpen())
    {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					grid.setCellStateOnMouseClick(window, currentState);
				}
			}


			else if (event.type == sf::Event::KeyPressed)
			{
				// Change drawing mode based on keys (start, end, obstacle)
				if (event.key.code == sf::Keyboard::S)
				{
					currentState = CellState::Start;
				}
				else if (event.key.code == sf::Keyboard::E)
				{
					currentState = CellState::End;
				}
				else if (event.key.code == sf::Keyboard::O)
				{
					currentState = CellState::Obstacle;
				}
			}
		}

        window.clear(sf::Color::Cyan);
        grid.draw(window);
        window.display();
    }
}
