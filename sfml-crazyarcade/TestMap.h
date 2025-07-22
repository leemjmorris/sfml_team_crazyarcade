#define TESTMAP_H

#include "stdafx.h"

class TestMap
{
private:
    static const int WINDOW_WIDTH = 800;
    static const int WINDOW_HEIGHT = 600;
    static const int MAP_WIDTH = 600;
    static const int MAP_HEIGHT = 520;
    static const int UI_RIGHT_WIDTH = 200;
    static const int UI_BOTTOM_HEIGHT = 80;

    sf::Font font;

    sf::RectangleShape gameMap;
    sf::RectangleShape rightUI;
    sf::RectangleShape bottomUI;

    sf::Color backgroundColor;
    sf::Color mapColor;
    sf::Color uiColor;
    sf::Color borderColor;

    void setupGameMap();
    void setupUI();

public:
    TestMap();

    void initialize();

    void Update(float dt);
    void Draw(sf::RenderWindow& window);

    sf::Vector2f getMapSize() const;

    bool isInMapArea(int x, int y) const;
    void setMapColor(sf::Color color);
};