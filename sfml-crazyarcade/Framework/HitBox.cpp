#include "stdafx.h"
#include "HitBox.h"

HitBox::HitBox()
{
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Green);
    rect.setOutlineThickness(1.f);
}

void HitBox::UpdateTransform(const sf::Transformable& tr, const sf::FloatRect& localBounds)
{
    rect.setSize({ localBounds.width, localBounds.height });
    rect.setOutlineColor(sf::Color::Green);
    rect.setOrigin(tr.getOrigin());
    rect.setPosition(tr.getPosition());
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

void HitBox::UpdateCustomTransform(const sf::Transformable& tr, const sf::Vector2f& customSize, Origins spriteOrigin)
{
    UpdateCustomTransform(tr, customSize, sf::Vector2f(0.f, 0.f), spriteOrigin);
}

void HitBox::UpdateCustomTransform(const sf::Transformable& tr, const sf::Vector2f& customSize, Origins spriteOrigin, const sf::Vector2f& offset)
{
    UpdateCustomTransform(tr, customSize, offset, spriteOrigin);
}

void HitBox::UpdateCustomTransform(const sf::Transformable& tr, const sf::Vector2f& customSize, const sf::Vector2f& offset, Origins spriteOrigin)
{
    rect.setSize(customSize);
    rect.setOutlineColor(sf::Color::Green);

    sf::Vector2f hitBoxOrigin = customSize * 0.5f;
    rect.setOrigin(hitBoxOrigin);

    sf::FloatRect spriteBounds;

    const sf::Sprite* sprite = dynamic_cast<const sf::Sprite*>(&tr);
    if (sprite != nullptr)
    {
        spriteBounds = sprite->getLocalBounds();
    }

    sf::Vector2f originOffsetFromCenter = CalculateOriginOffset(spriteOrigin, spriteBounds);

    sf::Vector2f finalPosition = tr.getPosition() + originOffsetFromCenter + offset;
    rect.setPosition(finalPosition);

    rect.setScale(tr.getScale());
    rect.setRotation(tr.getRotation());
}

sf::Vector2f HitBox::CalculateOriginOffset(Origins spriteOrigin, const sf::FloatRect& spriteBounds)
{
    sf::Vector2f spriteOriginPos;
    spriteOriginPos.x = spriteBounds.width * ((int)spriteOrigin % 3) * 0.5f;
    spriteOriginPos.y = spriteBounds.height * ((int)spriteOrigin / 3) * 0.5f;

    sf::Vector2f spriteCenter = sf::Vector2f(spriteBounds.width * 0.5f, spriteBounds.height * 0.5f);

    return spriteCenter - spriteOriginPos;
}

sf::FloatRect HitBox::GetGlobalBounds() const
{
    return rect.getGlobalBounds();
}