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
        sf::RectangleShape rect;
        rect.setSize({ tile.bounds.width, tile.bounds.height });
        rect.setFillColor(sf::Color(255, 0, 0, 80));
        rect.setOutlineColor(sf::Color::Red);
        rect.setOutlineThickness(1.f);
        rect.setPosition(tile.bounds.left, tile.bounds.top); // TL 기준

        window.draw(rect);
    }
}

sf::RectangleShape MapCollisionBuilder::DrawRect(int i, int j)
{
    sf::RectangleShape debugRect;
    debugRect.setSize({ static_cast<float>(GRID_SIZE), static_cast<float>(GRID_SIZE) });

    debugRect.setOrigin(GRID_SIZE * 0.5f, GRID_SIZE * 0.5f); // 중심 기준
    debugRect.setPosition(static_cast<float>(j * GRID_SIZE + GRID_SIZE * 0.5f),
        static_cast<float>(i * GRID_SIZE + GRID_SIZE * 0.5f));

    debugRect.setFillColor(sf::Color(255, 0, 0, 80));
    debugRect.setOutlineColor(sf::Color::Red);
    debugRect.setOutlineThickness(1.f);

    return debugRect;
}