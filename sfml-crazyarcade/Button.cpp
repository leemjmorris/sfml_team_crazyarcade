#include "stdafx.h"
#include "Button.h"

Button::Button(const std::string& name)
	:UiHud(name)
{
}

void Button::SetSize(sf::Vector2f v)
{
	button.setScale(v);
}


void Button::SetOnClick(std::function<void()> func)
{
	onClick = func;
}

void Button::SetPosition(const sf::Vector2f& pos)
{
	UiHud::SetPosition(pos);
	button.setPosition(pos);
}

void Button::Init()
{
}

void Button::Release()
{
}

void Button::Reset()
{
	tex.loadFromFile(texId);
	button.setTexture(tex);
	Utils::SetOrigin(button, Origins::TL);
}

void Button::Update(float dt)
{
	sf::Vector2f mouseWorld =
		FRAMEWORK.GetWindow().mapPixelToCoords(
			sf::Mouse::getPosition(FRAMEWORK.GetWindow()));

	bool isMouseOver = Utils::PointInTransformBounds(button, button.getLocalBounds(), mouseWorld);

	if (isMouseOver && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (onClick) onClick();
	}
}

void Button::Draw(sf::RenderWindow& window)
{
	if (GetActive()) {
		if (isButton) window.draw(button);
	}
}