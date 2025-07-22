#pragma once
#include "Scene.h"

class MapEditor : public Scene
{
private:
	MapEditor();
	virtual ~MapEditor() = default;

	void UpdateMapEditor();
	void CreateTileAtPosition(const sf::Vector2f& position);
	void DeleteTileAtPosition(const sf::Vector2f& position);
private:
	sf::RenderWindow window;

	int tileOptionIndex;

	sf::Texture tileMapTexture;

	std::vector<sf::Sprite> TileOptions;
	std::vector<sf::Sprite> Tiles;
};

