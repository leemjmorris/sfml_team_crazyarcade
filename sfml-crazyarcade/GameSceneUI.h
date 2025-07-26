#pragma once
#include "GameObject.h"
#include "ColorMaskShader.h"

class GameSceneUI : public GameObject
{
protected:
	sf::Sprite draftSprite;
	ColorMaskShader colorMask;

public:
	GameSceneUI();
	~GameSceneUI() = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};