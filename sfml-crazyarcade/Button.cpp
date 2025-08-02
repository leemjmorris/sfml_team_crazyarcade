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
	if (highlitedTexId != "") // KHI
	{
		highlitedTex.loadFromFile(highlitedTexId);
	}
	button.setTexture(tex);
	Utils::SetOrigin(button, Origins::TL);

	font = FONT_MGR.Get("assets/font/Daum_Regular.ttf"); // KHI

	if (btnText.getString() != "")
	{
		SetText(btnText.getString(), btnText.getCharacterSize());
	}
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
		if (btnText.getString() != "") window.draw(btnText);
	}
}

// KHI
void Button::SetText(const std::string& str, int size, sf::Color color)
{
	btnText.setFont(font);
	btnText.setString(str);
	btnText.setCharacterSize(size);
	btnText.setFillColor(color);

	sf::FloatRect textBounds = btnText.getLocalBounds();
	btnText.setOrigin(textBounds.left * 0.5f, textBounds.height * 0.5f);

	sf::FloatRect btnBounds = button.getGlobalBounds();
	btnText.setPosition(btnBounds.left + 10, btnBounds.top + btnBounds.height * 0.5f);
}