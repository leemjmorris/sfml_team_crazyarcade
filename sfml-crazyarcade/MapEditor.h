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
    BlockState = 2  // LMJ: "Block property modification layer"
};

// LMJ: "Property modification modes for Layer 2"
enum class PropertyMode
{
    Destroyable = 0,
    Hidable = 1,
    Movable = 2,
    SpawnItem = 3,
    Count
};

class MapEditor : public Scene
{
private:
    // LMJ: "Current layer and selection states"
    LayerType currentLayer;
    int tileOptionIndex;
    int blockRegistryIndex;
    PropertyMode currentPropertyMode; // LMJ: "Current property being modified in Layer 2"
    ScrollInput MouseScrollInput;

    // LMJ: "Texture and sprite containers"
    sf::Texture tileMapTexture;
    std::vector<sf::Sprite> TileOptions;
    std::vector<sf::Sprite> BlockPreviewSprites;
    std::vector<sf::Sprite> Tiles;
    std::vector<Block*> PlacedBlocks;

    sf::VertexArray gridLines;

    // LMJ: "Grid configuration"
    static const int GRID_WIDTH = 15;
    static const int GRID_HEIGHT = 13;
    static const int GRID_SIZE = 40;

    // LMJ: "Layer 2 specific variables"
    Block* selectedBlock; // LMJ: "Currently selected block for property modification"
    sf::RectangleShape selectionHighlight; // LMJ: "Visual highlight for selected block"

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
    void HandleLayer2Input(); // LMJ: "Special input handling for Layer 2"

    // LMJ: "Placement methods"
    void CreateTileAtPosition(const sf::Vector2f& position);
    void CreateBlockAtPosition(const sf::Vector2f& position);
    void DeleteTileAtPosition(const sf::Vector2f& position);
    void DeleteBlockAtPosition(const sf::Vector2f& position);

    // LMJ: "Layer 2 specific methods"
    void SelectBlockAtPosition(const sf::Vector2f& position);
    void ModifySelectedBlockProperty(PropertyMode mode, bool value);
    void ToggleSelectedBlockProperty(PropertyMode mode);
    void CopyBlockProperties(const sf::Vector2f& sourcePos, const sf::Vector2f& targetPos);

    // LMJ: "Utility methods"
    sf::Vector2f GetGridPosition(const sf::Vector2f& mousePos);
    bool IsValidGridPosition(const sf::Vector2f& gridPos);
    Block* GetBlockAtPosition(const sf::Vector2f& position);

    // LMJ: "Drawing methods"
    void DrawMapEditor(sf::RenderWindow& window);
    void DrawTilePreviewAtMouse(sf::RenderWindow& window);
    void DrawBlockPreview(sf::RenderWindow& window);
    void DrawBlockPropertyIndicators(sf::RenderWindow& window); // LMJ: "Show block properties visually"
    void DrawRightSideUI(sf::RenderWindow& window); // LMJ: "Main right-side UI"
    void DrawLayerInfo(sf::RenderWindow& window, float x, float y); // LMJ: "Layer information display"
    void DrawControlsInfo(sf::RenderWindow& window, float x, float y); // LMJ: "Controls help display"
    void DrawLayer2Info(sf::RenderWindow& window, float x, float y); // LMJ: "Layer 2 specific info"

    // LMJ: "Helper methods"
    std::string GetPropertyModeString(PropertyMode mode) const;
    sf::Color GetPropertyColor(PropertyMode mode, bool enabled) const; // LMJ: "Color coding for properties"
};