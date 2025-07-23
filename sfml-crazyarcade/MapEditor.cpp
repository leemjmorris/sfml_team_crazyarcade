#include "stdafx.h"
#include "MapEditor.h"

MapEditor::MapEditor()
	: tileOptionIndex(0)
	, MouseScrollInput(None)
{
	tileMapTexture.loadFromFile(PATH_MAP_FOREST"tile/tile_1.bmp");
	for (int x = 0; x < 15; ++x)
	{
		for (int y = 0; y < 13; ++y)
		{
			sf::Sprite tile;
			float size_x = tileMapTexture.getSize().x;
			float size_y = tileMapTexture.getSize().y;

			tile.setTexture(tileMapTexture);
			tile.setTextureRect(sf::IntRect(size_x, size_y, size_x, size_y));
			tile.setScale(sf::Vector2f(10, 10));
			tile.setOrigin(sf::Vector2f(size_x / 2, size_y / 2));
			TileOptions.push_back(tile); 
		}
	}
	TileOptions.emplace_back();
}

void MapEditor::UpdateMapEditor()
{
	HandleInput();
}

void MapEditor::DrawMapEditor()
{
	sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);
	TileOptions[tileOptionIndex].setPosition(mousePosition);

	window.draw(TileOptions[tileOptionIndex]);
}

void MapEditor::CreateTileAtPosition(const sf::Vector2f& position)
{
}

void MapEditor::DeleteTileAtPosition(const sf::Vector2f& position)
{
}

void MapEditor::HandleInput()
{
	if (MouseScrollInput == ScrollUp)
	{
		tileOptionIndex++;
		if (tileOptionIndex >= TileOptions.size())
		{
			tileOptionIndex = 0;
		}
	}
	else if (MouseScrollInput == ScrollDown)
	{
		tileOptionIndex--;
		if (tileOptionIndex < 0)
		{
			tileOptionIndex = TileOptions.size() - 1;
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);

		CreateTileAtPosition(mousePosition);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);

		DeleteTileAtPosition(mousePosition);
	}

	sf::Event event;
	MouseScrollInput = None;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;

		case sf::Event::MouseWheelScrolled:
			if (event.mouseWheelScroll.delta > 0)
			{
				MouseScrollInput = ScrollUp;
			}
			else
			{
				MouseScrollInput = ScrollDown;
			}
		}
	}
}

void MapEditor::Draw()
{
	DrawMapEditor();
}
