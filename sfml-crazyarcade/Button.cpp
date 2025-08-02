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
	if (highlitedTexId != "")
	{
		highlitedTex.loadFromFile(highlitedTexId);
	}
	button.setTexture(tex);
	Utils::SetOrigin(button, Origins::TL);
}

void Button::Update(float dt)
{
	sf::Vector2f mouseWorld =
		FRAMEWORK.GetWindow().mapPixelToCoords(
			sf::Mouse::getPosition(FRAMEWORK.GetWindow()));

	bool isMouseOver = Utils::PointInTransformBounds(button, button.getLocalBounds(), mouseWorld);

	if (isMouseOver && InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		if (onClick) onClick();
	}

	// KHI: If a highlight texture exists, apply the highlight texture on mouse hover
    if (highlitedTexId != "")
    {
        if (isMouseOver && button.getTexture() != &highlitedTex)
        {
            button.setTexture(highlitedTex);
        }
        else if (!isMouseOver && button.getTexture() != &tex)
        {
            button.setTexture(tex);
        }
    }
}

void Button::Draw(sf::RenderWindow& window)
{
	if (GetActive()) {
		if (isButton) window.draw(button);
	}
}