#pragma once
#include "Scene.h"
#include "CharacterStats.h"
#include "Item.h"

class SceneDemo : public Scene
{
protected:
	// KHI: For Testing
	bool toggleActiveGrid = true;

	const int GRID_WIDTH = 15;
	const int GRID_HEIGHT = 13;
	const int GRID_SIZE = 52;

	sf::VertexArray gridLines;

public:
	SceneDemo();
	~SceneDemo() override = default;

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void ClampToBounds(GameObject& obj);
};