#pragma once
#include "Scene.h"
#include "CharacterStats.h"

class Player;
class SceneDev2 : public Scene
{
protected:
	Player* player;
public:
	SceneDev2();

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

