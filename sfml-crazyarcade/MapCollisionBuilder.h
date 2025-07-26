#pragma once

struct TileHitBox
{
    sf::FloatRect bounds;

    TileHitBox(float x, float y, float size)
        : bounds(x, y, size, size) {}
};

// KHI: Constructs collision rectangles based on block data drawn on the map
class MapCollisionBuilder
{
protected:
    const int(&mapData)[13][15];
    std::vector<TileHitBox> tileHitBoxes;

    const int GRID_SIZE = 52;

public:
    MapCollisionBuilder() = delete;
    MapCollisionBuilder(const int(&data)[13][15]);
    ~MapCollisionBuilder() = default;

    void CreateCollisionHitBox();
    const std::vector<TileHitBox>& GetTileHitBoxes() const { return tileHitBoxes; }

    void DrawDebugHitBox(sf::RenderWindow& window);
    sf::RectangleShape DrawRect(int i, int j);
};
