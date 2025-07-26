#pragma once

// KHI: Constructs collision rectangles based on block data drawn on the map
class MapCollisionBuilder
{
protected:
    const int(&mapData)[13][15];

    const int GRID_SIZE = 52;

public:
    MapCollisionBuilder() = delete;
    MapCollisionBuilder(const int(&data)[13][15]);
    ~MapCollisionBuilder() = default;

    void BuildMapHitBox(Scene* scene);
    void DrawDebugHitBox(sf::RenderWindow& window);
    sf::RectangleShape DrawRect(int i, int j);
    void CreateCollisionHitBox();
};

struct TileHitBox
{
    sf::FloatRect bounds;

    TileHitBox(float x, float y, float size)
        : bounds(x, y, size, size) {}
};