#include "stdafx.h"
#include "MapCollisionBuilder.h"

MapCollisionBuilder::MapCollisionBuilder(const int(&data)[13][15])
    :mapData(data)
{
}

// KHI: For using
void MapCollisionBuilder::CreateCollisionHitBox()
{
    tileHitBoxes.clear();
    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (mapData[i][j] == 1)
            {
                float x = j * GRID_SIZE;
                float y = i * GRID_SIZE;
                tileHitBoxes.emplace_back(x, y, GRID_SIZE);
            }
        }
    }
}

// KHI: For debuging
void MapCollisionBuilder::DrawDebugHitBox(sf::RenderWindow& window)
{
    for (const auto& tile : tileHitBoxes)
    {
        sf::RectangleShape rect = DrawRect(tile);
        window.draw(rect);
    }
}

sf::RectangleShape MapCollisionBuilder::DrawRect(const TileHitBox& tile)
{
    sf::RectangleShape debugRect;
    debugRect.setSize({ tile.bounds.width, tile.bounds.height });
    Utils::SetOrigin(debugRect, Origins::TL);

    debugRect.setPosition(tile.bounds.left, tile.bounds.top);
    debugRect.setFillColor(sf::Color(255, 0, 0, 80));
    debugRect.setOutlineColor(sf::Color::Red);
    debugRect.setOutlineThickness(1.f);

    return debugRect;
}