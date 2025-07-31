#pragma once
#include "UiHud.h"
#include "ColorMaskShader.h"
#include "Button.h"

class GameSceneUI : public UiHud
{
protected:
	Button* exitButton;

	ColorMaskShader colorMask;

	sf::Sprite draftSprite;

	//LSY : SceneDemo have same those
	sf::Text textResult;
	sf::Font font;
	bool isShowingText = false;

	sf::FloatRect worldBounds = FRAMEWORK.GetWindowBounds();
public:
	GameSceneUI(const std::string& name);
	~GameSceneUI() = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};