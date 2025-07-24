#pragma once
#include "Scene.h"
#include "Block.h"

enum ScrollInput
{
    None,
    ScrollUp,
    ScrollDown
};

enum class LayerType
{
    Background = 0,
    Block = 1,
    BlockState = 2
};

class MapEditor : public Scene
{
private:
    // LMJ: "Current layer and selection states"
    LayerType currentLayer;
    int tileOptionIndex;
    int blockRegistryIndex; // LMJ: "Index into block registry"
    ScrollInput MouseScrollInput;

    // LMJ: "Texture and sprite containers"
    sf::Texture tileMapTexture;
    std::vector<sf::Sprite> TileOptions;
    std::vector<sf::Sprite> BlockPreviewSprites; // LMJ: "Preview sprites for all registered blocks"
    std::vector<sf::Sprite> Tiles;
    std::vector<Block*> PlacedBlocks;

    sf::VertexArray gridLines;

    // LMJ: "Grid configuration"
    static const int GRID_WIDTH = 15;
    static const int GRID_HEIGHT = 13;
    static const int GRID_SIZE = 40;

    // LMJ: "UI Elements"
    sf::Text layerInfoText;
    sf::Text selectionInfoText;

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
    // LMJ: "Asset loading methods"
    void LoadTileSet();
    void LoadBlockSet();

    // LMJ: "Input handling methods"
    void HandleInput();
    void HandleLayerSwitching();
    void HandleScrollInput();

    // LMJ: "Placement methods"
    void CreateTileAtPosition(const sf::Vector2f& position);
    void CreateBlockAtPosition(const sf::Vector2f& position);
    void DeleteTileAtPosition(const sf::Vector2f& position);
    void DeleteBlockAtPosition(const sf::Vector2f& position);

    // LMJ: "Utility methods"
    sf::Vector2f GetGridPosition(const sf::Vector2f& mousePos);
    bool IsValidGridPosition(const sf::Vector2f& gridPos);
    Block* GetBlockAtPosition(const sf::Vector2f& position);

    // LMJ: "Drawing methods"
    void DrawMapEditor(sf::RenderWindow& window);
    void DrawTilePreviewAtMouse(sf::RenderWindow& window); // LMJ: "Separated from DrawMapEditor"
    void DrawTilePreview(sf::RenderWindow& window);
    void DrawBlockPreview(sf::RenderWindow& window);
    void DrawUI(sf::RenderWindow& window);
    void UpdateUI();
};