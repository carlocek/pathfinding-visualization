#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "Cell.hpp"
#include "Grid.hpp"
#include "PathfindingStrategy.hpp"
#include "DjikstraStrategy.hpp"
#include "AstarStrategy.hpp"
#include <iostream>
#include <chrono>

int main()
{
	const int WIN_WIDTH = 1400;
	const int WIN_HEIGHT = 1000;
	bool mousePressed;
	bool endSearch = false;
	bool search = false;
	float shortestPathLength;
	int cellSize = 10;
	float cellBorderSize = cellSize/10 + 1.0;
	int checksPerFrame = 10;

	sf::Font font;
	if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
	{
		std::cerr << "Failed to load font\n";
		return -1;
	}

    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Pathfinding Visualization");
    window.setFramerateLimit(60);

    //START GUI CREATION
	tgui::Gui gui{window};
	tgui::Theme::setDefault("C:/TGUI-1.2/themes/Black.txt");
	auto panel = tgui::Panel::create();
	panel->setSize({"30%", "100%"});
	panel->setPosition({"70%", "0%"});
	gui.add(panel);

	auto titleText = tgui::Label::create("Pathfinding Visualization");
	titleText->setSize({"80%", "5%"});
	titleText->setPosition({"10%", "5%"});
	titleText->setTextSize(26);
	panel->add(titleText);

	auto instructionsText = tgui::Label::create("To draw cells, press one of the keys and drag on the grid."
    		"\nS: start cell, F: finish cell, O: obstacle cell, E: empty cell");
	instructionsText->setSize({"80%", "10%"});
	instructionsText->setPosition({"10%", "10%"});
	instructionsText->setTextSize(16);
	panel->add(instructionsText);

	auto cellSizeSlider = tgui::Slider::create(10, 30);
	cellSizeSlider->setSize({"80%", "3%"});
	cellSizeSlider->setPosition({"10%", "25%"});
	cellSizeSlider->setStep(2);
	cellSizeSlider->setValue(10); // Default value
	panel->add(cellSizeSlider);

	auto cellSizeText = tgui::Label::create("Cell size: 10");
	cellSizeText->setSize({"80%", "4%"});
	cellSizeText->setPosition({"10%", "30%"});
	cellSizeText->setTextSize(16);
	panel->add(cellSizeText);

	auto checksPerFrameSlider = tgui::Slider::create(1, 50);
	checksPerFrameSlider->setSize({"80%", "3%"});
	checksPerFrameSlider->setPosition({"10%", "35%"});
	checksPerFrameSlider->setValue(10); // Default value
	panel->add(checksPerFrameSlider);

	auto checksPerFrameText = tgui::Label::create("Simulation speed: 10");
	checksPerFrameText->setSize({"80%", "4%"});
	checksPerFrameText->setPosition({"10%", "40%"});
	checksPerFrameText->setTextSize(16);
	panel->add(checksPerFrameText);

	auto simulationComboBox = tgui::ComboBox::create();
	simulationComboBox->setSize({"80%", "4%"});
	simulationComboBox->setPosition({"10%", "50%"});
	simulationComboBox->addItem("Djikstra");
	simulationComboBox->addItem("A star");
	simulationComboBox->setDefaultText("Select a pathfinding algorithm");
	panel->add(simulationComboBox);

	auto runButton = tgui::Button::create("Start/Stop");
	runButton->setSize({"25%", "4%"});
	runButton->setPosition({"10%", "60%"});
	panel->add(runButton);

	auto clearButton = tgui::Button::create("Clear Grid");
	clearButton->setSize({"25%", "4%"});
	clearButton->setPosition({"40%", "60%"});
	panel->add(clearButton);

	auto resetButton = tgui::Button::create("Reset Simulation");
	resetButton->setSize({"25%", "4%"});
	resetButton->setPosition({"70%", "60%"});
	panel->add(resetButton);

	auto shortestPathText = tgui::Label::create("Shortest Path: 0");
	shortestPathText->setSize({"80%", "5%"});
	shortestPathText->setPosition({"10%", "70%"});
	shortestPathText->setTextSize(16);
	panel->add(shortestPathText);
	//END GUI CREATION

//	const int GRID_WIDTH = WIN_WIDTH - panel->getFullSize().x;
	const int GRID_WIDTH = 1000;
	const int GRID_HEIGHT = WIN_HEIGHT;
	PathfindingStrategy* strategy;
	Grid grid = Grid(GRID_WIDTH, GRID_HEIGHT, cellSize, cellBorderSize);
	CellState currentState = CellState::Empty;

	cellSizeSlider->onValueChange([&](int value)
	{
		cellSizeText->setText("Cell size: " + std::to_string(value));
		grid.setCellSize(value);
	});
	checksPerFrameSlider->onValueChange([&](int value)
	{
		checksPerFrameText->setText("Simulation speed: " + std::to_string(value));
		checksPerFrame = value;
		if(strategy != nullptr)
			strategy->setChecksPerFrame(checksPerFrame);
	});
	simulationComboBox->onItemSelect([&](const tgui::String& item)
	{
		if(item == "Djikstra")
		{
			strategy = new DjikstraStrategy(&grid, checksPerFrame);
			grid.setPathfindingStrategy(strategy);
		}
		else if(item == "A star")
		{
			strategy = new AstarStrategy(&grid, checksPerFrame);
			grid.setPathfindingStrategy(strategy);
		}
		else
		{
			strategy = nullptr;
		}
	});
	runButton->onClick([&](){search = !search;});
	clearButton->onClick([&](){grid.clear();});
	resetButton->onClick([&](){grid.reset(); strategy = nullptr; simulationComboBox->deselectItem(); endSearch = false; search = false;});

    while (window.isOpen())
    {
		sf::Event event;
		while(window.pollEvent(event))
		{
			gui.handleEvent(event);
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
			}
		}
		if(search && !endSearch && strategy != nullptr)
		{
//			grid.setPathfindingStrategy(strategy);
			shortestPathLength = grid.getPathfindingStrategy()->search(window);
			if(shortestPathLength != -1)
			{
				endSearch = true;
				search = false;
			}
		}
		window.clear(sf::Color{128, 128, 128});
		shortestPathText->setText("Shortest Path: " + std::to_string(shortestPathLength));
		grid.draw(window);
		gui.draw();
		window.display();
    }
}
