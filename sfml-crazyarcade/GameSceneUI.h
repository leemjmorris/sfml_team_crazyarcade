#pragma once
#include "GameObject.h"
#include "ColorMaskShader.h"

class GameSceneUI : public GameObject
{
protected:
	sf::Sprite draftSprite;
	ColorMaskShader colorMask;

	//LSY : SceneDemo have same those
	sf::Text textResult;
	sf::Font font;
	bool isShowingText = false;

	sf::FloatRect worldBounds = FRAMEWORK.GetWindowBounds();
public:
	GameSceneUI();
	~GameSceneUI() = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};