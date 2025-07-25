#pragma once
#include "Scene.h"

class SceneReady : public Scene
{
protected:
	sf::Sprite bgSprite; // KHI: DRAFT!!

public:
	SceneReady();
	~SceneReady() override = default;

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};