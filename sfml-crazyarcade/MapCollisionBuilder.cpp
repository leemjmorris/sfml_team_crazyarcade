#include "stdafx.h"
#include "MapCollisionBuilder.h"

MapCollisionBuilder::MapCollisionBuilder(const std::vector<std::vector<int>>& data)
    :mapData(data)
{
}

void MapCollisionBuilder::BuildMapHitBox(Scene* scene)
{
    for (int i = 0; i < mapData.size(); ++i)
    {
        for (int j = 0; j < mapData[i].size(); ++j)
        {
            if (mapData[i][j] == 1)
            {

            }
        }
    }
}

void MapCollisionBuilder::DrawDebugHitBox(sf::RenderWindow& window)
{
    for (int i = 0; i < mapData.size(); ++i)
    {
        for (int j = 0; j < mapData[i].size(); ++j)
        {
            if (mapData[i][j] == 1)
            {

                sf::RectangleShape debugRect;
                debugRect.setSize({ static_cast<float>(GRID_SIZE), static_cast<float>(GRID_SIZE) });
                debugRect.setFillColor(sf::Color(255, 0, 0, 100));
                debugRect.setOutlineColor(sf::Color::Red);
                debugRect.setOutlineThickness(1.f);
                debugRect.setPosition(static_cast<float>(j * GRID_SIZE), static_cast<float>(i * GRID_SIZE));

                window.draw(debugRect);
            }
        }
    }
}

void MapCollisionBuilder::test()
{

}

void MapCollisionBuilder::CreateCollisionHitBox()
{
}