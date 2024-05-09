#include <SFML/Graphics.hpp>
#include "Cell.hpp"
#include "Grid.hpp"
#include "PathfindingStrategy.hpp"
#include "DjikstraStrategy.hpp"

int main()
{
	const int WIDTH = 800;
	const int HEIGHT = 800;
	const int CELL_SIZE = 20;
	const float CELL_BORDER_SIZE = 2.0;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Pathfinding Visualization");
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
				else if (event.key.code == sf::Keyboard::Backspace)
				{
					window.clear(sf::Color::Black);
					grid.reset();
				}

				else if (event.key.code == sf::Keyboard::Enter)
				{
					window.clear(sf::Color::Black);
					DjikstraStrategy strategy = DjikstraStrategy(&grid);
					grid.setPathfindingStrategy(&strategy);
					std::vector<Cell*> shortestPath = grid.getPathfindingStrategy()->search(window);
				}
			}
		}

        grid.draw(window);
        window.display();
    }
}
