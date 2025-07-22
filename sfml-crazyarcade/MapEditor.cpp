#include "stdafx.h"
#include "MapEditor.h"

//MapEditor::MapEditor()
//	: tileOptionIndex(0)
//{
//	tileMapTexture.loadFromFile("texture/map/forest/tile_1.bmp");
//	//TileOptions.
//}

void MapEditor::UpdateMapEditor()
{
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
}

void MapEditor::CreateTileAtPosition(const sf::Vector2f& position)
{
}

void MapEditor::DeleteTileAtPosition(const sf::Vector2f& position)
{
}
