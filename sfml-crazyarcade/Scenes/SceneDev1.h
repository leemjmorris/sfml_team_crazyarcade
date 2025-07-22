#pragma once
#include "Scene.h"
#include "Item.h"

class SceneDev1 : public Scene
{
protected:
	
public:
	SceneDev1();
	~SceneDev1() override;

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

