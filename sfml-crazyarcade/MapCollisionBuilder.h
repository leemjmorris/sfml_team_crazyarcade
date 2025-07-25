#pragma once

// KHI: Constructs collision rectangles based on block data drawn on the map
class MapCollisionBuilder
{
protected:
    const std::vector<std::vector<int>>& mapData;

    const int GRID_SIZE = 52;

public:
    MapCollisionBuilder() = delete;
    MapCollisionBuilder(const std::vector<std::vector<int>>& data);
    ~MapCollisionBuilder() = default;

    void SetMapData(const std::vector<std::vector<int>>& data);
    void BuildMapHitBox(Scene* scene);
    void DrawDebugHitBox(sf::RenderWindow& window);
    void test();
    void CreateCollisionHitBox();
};

struct TileHitBox
{
    sf::FloatRect bounds;

    TileHitBox(float x, float y, float size)
        : bounds(x, y, size, size) {}
};