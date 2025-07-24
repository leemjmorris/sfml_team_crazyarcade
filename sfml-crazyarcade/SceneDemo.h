#pragma once
#include "Scene.h"
#include "CharacterStats.h"
#include "Item.h"

class Player;

class SceneDemo : public Scene
{
protected:
	Player* bazzi;
	Player* Dao;
	Item* item;
	sf::FloatRect worldBounds = FRAMEWORK.GetWindowBounds();
	std::vector<GameObject*> objectsNeedingClamp;

public:
	SceneDemo();
	~SceneDemo() override = default;

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void ClampToBounds(GameObject& obj);
};