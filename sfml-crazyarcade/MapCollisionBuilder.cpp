#include "stdafx.h"
#include "MapCollisionBuilder.h"

MapCollisionBuilder::MapCollisionBuilder(const int(&data)[13][15])
    :mapData(data)
{
}

void MapCollisionBuilder::BuildMapHitBox(Scene* scene)
{
    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (mapData[i][j] == 1)
            {

            }
        }
    }
}

void MapCollisionBuilder::DrawDebugHitBox(sf::RenderWindow& window)
{
    for (int i = 0; i < 13; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (mapData[i][j] == 1)
            {
                window.draw(DrawRect(i, j));
            }
        }
    }
}

sf::RectangleShape MapCollisionBuilder::DrawRect(int i, int j)
{
    sf::RectangleShape debugRect;
    debugRect.setSize({ static_cast<float>(GRID_SIZE), static_cast<float>(GRID_SIZE) });
    debugRect.setFillColor(sf::Color(255, 0, 0, 80));
    debugRect.setOutlineColor(sf::Color::Red);
    debugRect.setOutlineThickness(1.f);
    debugRect.setPosition(static_cast<float>(j * GRID_SIZE), static_cast<float>(i * GRID_SIZE));

    return debugRect;
}

void MapCollisionBuilder::CreateCollisionHitBox()
{
}