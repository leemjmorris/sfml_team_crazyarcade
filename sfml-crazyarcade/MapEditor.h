#pragma once
#include "Scene.h"


class MapEditor : public Scene
{
public:
	enum MouseScrollInput
	{
		ScrollUp,
		ScrollDown,
		None
	};

private:
	MapEditor();
	virtual ~MapEditor() = default;

	void UpdateMapEditor();
	void DrawMapEditor();
	void CreateTileAtPosition(const sf::Vector2f& position);
	void DeleteTileAtPosition(const sf::Vector2f& position);
	void HandleInput();
	void Draw();
private:
	sf::RenderWindow window;

	int tileOptionIndex;

	sf::Texture tileMapTexture;

	std::vector<sf::Sprite> TileOptions;
	std::vector<sf::Sprite> Tiles;

	MouseScrollInput MouseScrollInput;
};

