#include "stdafx.h"
#include "TestMap.h"
#include <iostream>

TestMap::TestMap()
{
    initialize();
}

void TestMap::initialize()
{
    backgroundColor = sf::Color(68, 68, 68);
    mapColor = sf::Color(76, 175, 80);
    uiColor = sf::Color(85, 85, 85);
    borderColor = sf::Color(46, 125, 50);

    setupGameMap();
    setupUI();
}

void TestMap::setupGameMap()
{
    gameMap.setSize(sf::Vector2f(MAP_WIDTH, MAP_HEIGHT));
    gameMap.setFillColor(mapColor);
    gameMap.setOutlineThickness(2.0f);
    gameMap.setOutlineColor(borderColor);
    gameMap.setPosition(0, 0);
}

void TestMap::setupUI()
{
    rightUI.setSize(sf::Vector2f(UI_RIGHT_WIDTH, MAP_HEIGHT));
    rightUI.setFillColor(uiColor);
    rightUI.setPosition(MAP_WIDTH, 0);

    bottomUI.setSize(sf::Vector2f(WINDOW_WIDTH, UI_BOTTOM_HEIGHT));
    bottomUI.setFillColor(uiColor);
    bottomUI.setPosition(0, MAP_HEIGHT);
}

void TestMap::Update(float dt)
{
    // Game Logic if needed.
}

void TestMap::Draw(sf::RenderWindow& window)
{

    window.draw(rightUI);
    window.draw(bottomUI);

    window.draw(gameMap);
}

sf::Vector2f TestMap::getMapSize() const
{
    return sf::Vector2f(MAP_WIDTH, MAP_HEIGHT);
}

bool TestMap::isInMapArea(int x, int y) const
{
    return (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT);
}

void TestMap::setMapColor(sf::Color color)
{
    mapColor = color;
    gameMap.setFillColor(mapColor);
}