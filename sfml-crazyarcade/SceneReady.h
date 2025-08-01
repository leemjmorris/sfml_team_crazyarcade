#pragma once
#include "Scene.h"
#include "Button.h"
#include "lobbyUi.h"
#include "MapLists.h"

class SceneReady : public Scene
{
protected:
	sf::Sprite bgSprite; // KHI: DRAFT!!

	lobbyUi* ui;
	MapLists* mapListUi;

public:
	SceneReady();
	~SceneReady() override = default;

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};