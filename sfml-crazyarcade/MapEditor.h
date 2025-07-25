#pragma once
#include "Scene.h"
#include "Block.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class LayerType
{
    Background = 0,
    Block = 1,
    BlockState = 2
};

enum class PropertyMode
{
    Destroyable = 0,
    Hidable = 1,
    Movable = 2,
    SpawnItem = 3
};

// LMJ: struct for tile data management
struct TileData
{
    int tileOptionIndex;
    int gridX, gridY;
    float rotation;

    TileData(int optionIdx, int x, int y, float rot = 0.f)
        : tileOptionIndex(optionIdx), gridX(x), gridY(y), rotation(rot) {}

    // LMJ: DATA to Json
    json ToJson() const
    {
        return json{
            {"tileOptionIndex", tileOptionIndex},
            {"gridX", gridX },
            {"gridY", gridY},
            {"rotation", rotation}
        };
    }

    // LMJ: Json to DATA
    static TileData FromJson(const json& j)
    {
        return TileData(
            j.at("tileOptionIndex").get<int>(),
            j.at("gridX").get<int>(),
            j.at("gridY").get<int>(),
            j.value("rotation", 0.f)
        );
    }
};

class MapEditor : public Scene
{
private:
    // LMJ: Vector for saving Tile data
    std::vector<TileData> tileDatas;

    // LMJ: "Current layer and selection states"
    LayerType currentLayer;
    int tileOptionIndex;
    int blockRegistryIndex;
    PropertyMode currentPropertyMode;
    float currentTileRotation;

    // LMJ: "Texture and sprite containers"
    sf::Texture tileMapTexture;
    std::vector<sf::Texture> BlockTextures;
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
    Block* selectedBlock;
    sf::RectangleShape selectionHighlight;

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
    void HandleTileRotation();
    void HandleLayer2Input();

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
    void DrawBlockPropertyIndicators(sf::RenderWindow& window);
    void DrawRightSideUI(sf::RenderWindow& window);
    void DrawLayerInfo(sf::RenderWindow& window, float x, float y);
    void DrawControlsInfo(sf::RenderWindow& window, float x, float y);
    void DrawLayer2Info(sf::RenderWindow& window, float x, float y);

    // LMJ: "Helper methods"
    std::wstring GetPropertyModeString(PropertyMode mode) const;
    sf::Color GetPropertyColor(PropertyMode mode, bool enabled) const;

    // LMJ: "Json methods"
    void SaveMapToJson(const std::string& filename) const;
    void LoadMapFromJson(const std::string& filename);
};