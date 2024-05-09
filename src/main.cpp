#include <SFML/Graphics.hpp>
#include "Cell.hpp"
#include "Grid.hpp"
#include "PathfindingStrategy.hpp"
#include "DjikstraStrategy.hpp"
#include <iostream>

void drawInfo(sf::RenderWindow& window, sf::Font font)
{
    // title
    sf::Text headerText("Pathfinding Visualization", font, 28);
	headerText.setFillColor(sf::Color::White);
	headerText.setPosition(810, 10);
	window.draw(headerText);

    // instructions
    sf::Text instructionsText("To draw cells, press one of the keys and drag on the grid."
    		"\nS: start cell, F: finish cell, O: obstacle cell, E: empty cell."
    		"\nSelect the algorithm and press ENTER to run the search", font, 18);
    instructionsText.setFillColor(sf::Color::White);
    instructionsText.setPosition(810, 60);
    window.draw(instructionsText);
}

int main()
{
	const int WIN_WIDTH = 1300;
	const int WIN_HEIGHT = 800;
	const int GRID_WIDTH = 800;
	const int GRID_HEIGHT = 800;
	const int CELL_SIZE = 20;
	const float CELL_BORDER_SIZE = CELL_SIZE/10;
	bool mousePressed;
	float shortestPathLength;

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Pathfinding Visualization");
    window.setFramerateLimit(60);

    Grid grid = Grid(GRID_WIDTH, GRID_HEIGHT, CELL_SIZE, CELL_BORDER_SIZE);
    CellState currentState = CellState::Empty;

    sf::Font font;
	if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
	{
		std::cerr << "Failed to load font\n";
		return -1;
	}
    // djikstra button
	sf::RectangleShape djikstraButtonRect(sf::Vector2f(90, 30)); // size
	djikstraButtonRect.setFillColor(sf::Color{96, 96, 96});
	djikstraButtonRect.setOutlineColor(sf::Color::Black);
	djikstraButtonRect.setOutlineThickness(2);
	djikstraButtonRect.setPosition(810, 150);
	sf::Text djikstraButtonText("Dijkstra", font, 16);
	djikstraButtonText.setFillColor(sf::Color::White);
	djikstraButtonText.setPosition(815, 155);
	// astar button
	sf::RectangleShape astarButtonRect(sf::Vector2f(100, 30)); // size
	astarButtonRect.setFillColor(sf::Color{96, 96, 96});
	astarButtonRect.setOutlineColor(sf::Color::Black);
	astarButtonRect.setOutlineThickness(2);
	astarButtonRect.setPosition(910, 150);
	sf::Text astarButtonText("A*", font, 16);
	astarButtonText.setFillColor(sf::Color::White);
	astarButtonText.setPosition(915, 155);

    while (window.isOpen())
    {
    	window.clear(sf::Color{128, 128, 128});
    	drawInfo(window, font);
    	window.draw(djikstraButtonRect);
		window.draw(djikstraButtonText);
		window.draw(astarButtonRect);
		window.draw(astarButtonText);
		sf::Text instructionsText("Shortest path has length: "+std::to_string(shortestPathLength), font, 22);
		instructionsText.setFillColor(sf::Color::White);
		instructionsText.setPosition(810, 200);
		window.draw(instructionsText);

		PathfindingStrategy* strategy;

		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if(event.type == sf::Event::MouseButtonPressed)
			{
				if(event.mouseButton.button == sf::Mouse::Left)
				{
					mousePressed = true;
					sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
					if(djikstraButtonRect.getGlobalBounds().contains(mousePos))
					{
						strategy = new DjikstraStrategy(&grid);
						djikstraButtonRect.setOutlineColor(sf::Color::Cyan);
					}
//					else if(astarButtonRect.getGlobalBounds().contains(mousePos))
//					{
//						//TODO
//					}
					else
					{
						strategy = nullptr;
						djikstraButtonRect.setOutlineColor(sf::Color::Black);
					}
				}
			}
			else if(event.type == sf::Event::MouseButtonReleased)
			{
				if(event.mouseButton.button == sf::Mouse::Left)
				{
					mousePressed = false;
				}
			}
			else if(event.type == sf::Event::MouseMoved && mousePressed)
			{
				grid.setCellStateOnMouseClick(window, currentState);
			}
			else if(event.type == sf::Event::KeyPressed)
			{
				// Change drawing mode based on keys (start, end, obstacle)
				if(event.key.code == sf::Keyboard::E)
				{
					currentState = CellState::Empty;
				}
				else if(event.key.code == sf::Keyboard::S)
				{
					currentState = CellState::Start;
				}
				else if(event.key.code == sf::Keyboard::F)
				{
					currentState = CellState::End;
				}
				else if(event.key.code == sf::Keyboard::O)
				{
					currentState = CellState::Obstacle;
				}
				else if(event.key.code == sf::Keyboard::Backspace)
				{
					grid.reset();
				}

				else if(event.key.code == sf::Keyboard::Enter && strategy != nullptr)
				{
//					DjikstraStrategy strategy = DjikstraStrategy(&grid);
					grid.setPathfindingStrategy(strategy);
					shortestPathLength = grid.getPathfindingStrategy()->search(window);
				}
			}
		}

        grid.draw(window);
        window.display();
    }
}
