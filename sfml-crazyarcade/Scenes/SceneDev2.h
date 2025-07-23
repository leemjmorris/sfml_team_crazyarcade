#pragma once
#include "Scene.h"
#include "CharacterStats.h"
#include "Item.h"

class Player;
class SceneDev2 : public Scene
{
protected:
	Player* bazzi;
	Item* item;
	sf::FloatRect worldBounds = FRAMEWORK.GetWindowBounds();
	std::vector<GameObject*> objectsNeedingClamp;

public:
	SceneDev2();

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void ClampToBounds(GameObject& obj);
};