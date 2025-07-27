#pragma once

class HitBox
{
public:
    HitBox();

    sf::RectangleShape rect;

    void UpdateTransform(const sf::Transformable& tr, const sf::FloatRect& localBounds);
    void Draw(sf::RenderWindow& window);

    void UpdateCustomTransform(const sf::Transformable& tr, const sf::Vector2f& customSize, Origins spriteOrigin);
    void UpdateCustomTransform(const sf::Transformable& tr, const sf::Vector2f& customSize, Origins spriteOrigin, const sf::Vector2f& offset);
    void UpdateCustomTransform(const sf::Transformable& tr, const sf::Vector2f& customSize, const sf::Vector2f& offset, Origins spriteOrigin);

    sf::FloatRect GetGlobalBounds() const;
    sf::Vector2f CalculateOriginOffset(Origins spriteOrigin, const sf::FloatRect& spriteBounds);
};