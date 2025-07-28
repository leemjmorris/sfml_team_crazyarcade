#pragma once
#include "Scene.h"
#include "Block.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// LMJ: Editor layer types for different editing modes
enum class LayerType
{
    Background = 0,
    Block = 1,
    BlockState = 2
};

// LMJ: Property modes for block state editing
enum class PropertyMode
{
    Destroyable = 0,
    Hidable = 1,
    Movable = 2,
    SpawnItem = 3
};

// LMJ: Tile data structure with JSON serialization support
struct TileData
{
    int tileOptionIndex;
    int gridX, gridY;
    float rotation;
    sf::Vector2f worldPosition;

    TileData(int optionIdx, int x, int y, float rot = 0.f)
        : tileOptionIndex(optionIdx), gridX(x), gridY(y), rotation(rot)
    {
        // LMJ: Calculate world position from grid coordinates using MapEditor's GRID_SIZE
        worldPosition.x = static_cast<float>(x * 52); // LMJ: MapEditor GRID_SIZE = 52
        worldPosition.y = static_cast<float>(y * 52);
    }

    TileData(int optionIdx, int x, int y, const sf::Vector2f& worldPos, float rot = 0.f)
        : tileOptionIndex(optionIdx), gridX(x), gridY(y), rotation(rot), worldPosition(worldPos)
    {
    }

    // LMJ: JSON serialization methods
    json ToJson() const;
    static TileData FromJson(const json& j);
};

class MapEditor : public Scene
{
private:
    // LMJ: Grid configuration constants
    static const int GRID_WIDTH = 15;
    static const int GRID_HEIGHT = 13;
    static const int GRID_SIZE = 52;

    // LMJ: Editor state variables
    LayerType currentLayer;
    int tileOptionIndex;
    int blockRegistryIndex;
    PropertyMode currentPropertyMode;
    float currentTileRotation;

    // LMJ: Data storage containers
    std::vector<TileData> tileDatas;
    std::vector<Block*> PlacedBlocks;

    // LMJ: Asset containers
    sf::Texture tileMapTexture;
    std::vector<sf::Texture> BlockTextures;
    std::vector<sf::Sprite> TileOptions;
    std::vector<sf::Sprite> BlockPreviewSprites;
    std::vector<sf::Sprite> Tiles;

    // LMJ: Visual elements
    sf::VertexArray gridLines;
    Block* selectedBlock;
    sf::RectangleShape selectionHighlight;

public:
    MapEditor();
    ~MapEditor() override = default;

    // LMJ: Scene lifecycle methods
    void Init() override;
    void Release() override;
    void Enter() override;
    void Exit() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;

    // LMJ: Utility method for block sorting
    static bool CompareBlockAxisY(const Block* a, const Block* b);

private:
    // LMJ: Asset loading methods
    void LoadTileSet();
    void LoadBlockSet();

    // LMJ: Input handling methods
    void HandleInput();
    void HandleLayerSwitching();
    void HandleScrollInput();
    void HandleTileRotation();
    void HandleLayer2Input();

    // LMJ: Object placement methods
    void CreateTileAtPosition(const sf::Vector2f& position);
    void CreateBlockAtPosition(const sf::Vector2f& position);
    void DeleteTileAtPosition(const sf::Vector2f& position);
    void DeleteBlockAtPosition(const sf::Vector2f& position);

    // LMJ: Block property management methods
    void SelectBlockAtPosition(const sf::Vector2f& position);
    void ModifySelectedBlockProperty(PropertyMode mode, bool value);
    void ToggleSelectedBlockProperty(PropertyMode mode);
    void CopyBlockProperties(const sf::Vector2f& sourcePos, const sf::Vector2f& targetPos);

    // LMJ: Utility methods
    sf::Vector2f GetGridPosition(const sf::Vector2f& mousePos);
    bool IsValidGridPosition(const sf::Vector2f& gridPos);
    Block* GetBlockAtPosition(const sf::Vector2f& position);

    // LMJ: Drawing methods
    void DrawMapEditor(sf::RenderWindow& window);
    void DrawTilePreviewAtMouse(sf::RenderWindow& window);
    void DrawBlockPreview(sf::RenderWindow& window);
    void DrawBlockPropertyIndicators(sf::RenderWindow& window);
    void DrawRightSideUI(sf::RenderWindow& window);
    void DrawSaveLoadInfo(sf::RenderWindow& window);
    void DrawLayerInfo(sf::RenderWindow& window, float x, float y);
    void DrawControlsInfo(sf::RenderWindow& window, float x, float y);
    void DrawLayer2Info(sf::RenderWindow& window, float x, float y);

    // LMJ: UI helper methods
    std::wstring GetPropertyModeString(PropertyMode mode) const;
    sf::Color GetPropertyColor(PropertyMode mode, bool enabled) const;
    void ApplyBlockDefaultScale(Block* block);

    // LMJ: File I/O methods
    void SaveMapToJson(const std::string& filename) const;
    void LoadMapFromJson(const std::string& filename);
};