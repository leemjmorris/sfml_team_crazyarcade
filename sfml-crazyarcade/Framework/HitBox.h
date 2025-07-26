#pragma once
class HitBox
{
public:
	bool customized = false;

	HitBox();
	HitBox(const sf::Vector2f& size);
	HitBox(const sf::Vector2f& size, const sf::Vector2f& offset);

	sf::RectangleShape rect;
	sf::Vector2f hitBoxOffset = { 0, 0 };

	void UpdateTransform(const sf::Transformable& tr, const sf::FloatRect& localBounds);
	void Draw(sf::RenderWindow& window);
};

