#pragma once
#include "Scene.h"

class SceneDemo : public Scene
{
protected:

public:
	SceneDemo();
	~SceneDemo() override = default;

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};