#pragma once
#include "UiHud.h"
class Button :
    public UiHud
{
protected:
	sf::Font font;
	sf::Texture tex;
	sf::Sprite button;
	std::string texId;
	bool isButton = false;

	std::function<void()> onClick;

public:
	Button(const std::string& name = "");
	virtual ~Button()=default;

	void SetSize(sf::Vector2f v);
	void SetButton(const sf::String& str, sf::FloatRect r)
	{
		isButton = true;
		texId = str;
		button.setScale(1.f, 1.f);
		button.setPosition(r.left, r.top);
	}
	const sf::Sprite& getBox() const { return button; }

	void SetOnClick(std::function<void()> func);
	void SetPosition(const sf::Vector2f& pos) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

