#pragma once
#include "UiHud.h"
class Button :
    public UiHud
{
protected:
	sf::Font font;
	sf::Texture tex;
	sf::Texture highlitedTex;
	sf::Texture clickedTex;
	sf::Sprite button;
	std::string texId;
	std::string highlitedTexId; // KHI
	std::string clickedTexId;   // KHI
	sf::Text btnText; // KHI
	bool isButton = false;
	bool isClicked = false;

	std::function<void()> onClick;

public:
	Button(const std::string& name = "");
	virtual ~Button()=default;

	void SetSize(sf::Vector2f v);

	void SetButton(const sf::String& str, sf::FloatRect r, const sf::String& hStr = "", const sf::String& cStr = "")
	{
		isButton = true;
		texId = str;
		highlitedTexId = hStr;
		clickedTexId = cStr;
		button.setScale(1.f, 1.f);
		button.setPosition(r.left, r.top);
	}

	void SetText(const std::string& str, int size = 20, sf::Color color = sf::Color::White);
	void SetClicked(bool clicked); // KHI

	const sf::Sprite& getBox() const { return button; }

	void SetOnClick(std::function<void()> func);
	void SetPosition(const sf::Vector2f& pos) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

