#include "stdafx.h"
#include "HitBox.h"

HitBox::HitBox()
{
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Green);
	rect.setOutlineThickness(1.f);

	customized = false;
}

HitBox::HitBox(const sf::Vector2f& size)
{
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Green);
	rect.setOutlineThickness(1.f);
	rect.setSize(size);

	customized = true;
}

HitBox::HitBox(const sf::Vector2f& size, const sf::Vector2f& offset)
{
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Green);
	rect.setOutlineThickness(1.f);
	rect.setSize(size);
	hitBoxOffset = offset;

	customized = true;
}

void HitBox::UpdateTransform(const sf::Transformable& tr, const sf::FloatRect& localBounds)
{
	if (!customized)
	{
		rect.setSize({ localBounds.width, localBounds.height });
	}

	rect.setOrigin(tr.getOrigin());

	sf::Vector2f adjustedOffset = hitBoxOffset;
	if (tr.getScale().x < 0.f)
	{
		adjustedOffset.x *= -1.f;
	}

	rect.setPosition(tr.getPosition() + adjustedOffset);

	rect.setScale(tr.getScale());
	rect.setRotation(tr.getRotation());
}

void HitBox::Draw(sf::RenderWindow& window)
{
	if (Variables::isDrawHitBox)
	{
		window.draw(rect);
	}
}