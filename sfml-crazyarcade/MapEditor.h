#pragma once
#include "Scene.h"

enum ScrollInput
{
    None,
    ScrollUp,
    ScrollDown
};

class MapEditor : public Scene
{
private:
    int tileOptionIndex;
    ScrollInput MouseScrollInput;

    sf::Texture tileMapTexture;
    std::vector<sf::Sprite> TileOptions;
    std::vector<sf::Sprite> Tiles;

public:
    MapEditor();
    ~MapEditor() override = default;

    void Init() override;
    void Release() override;
    void Enter() override;
    void Exit() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;

private:
    void LoadTileSet();
    void HandleInput();
    void CreateTileAtPosition(const sf::Vector2f& position);
    void DeleteTileAtPosition(const sf::Vector2f& position);
    void DrawMapEditor(sf::RenderWindow& window);
};