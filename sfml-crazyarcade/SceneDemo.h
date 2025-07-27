#pragma once
#include "Scene.h"
#include "CharacterStats.h"
#include "Item.h"
#include "ColorMaskShader.h"
#include "MapCollisionBuilder.h"

class GameSceneUI;

class SceneDemo : public Scene
{
protected:
	const int GRID_HEIGHT = 13;
	const int GRID_WIDTH = 15;
	const int GRID_SIZE = 52;

	float killTimer = 0.0f;

	Player* bazzi;
	Player* dao;
	Item* item;
	sf::FloatRect worldBounds = FRAMEWORK.GetWindowBounds();
	std::vector<GameObject*> objectsNeedingClamp;

	GameSceneUI* ui;

	sf::Sprite testBlockSprite;

	sf::VertexArray gridLines;

	std::vector<sf::Sprite> sprites;

	// KHI: Layers
	int blockLayer[13][15] = { 0 };

	ColorMaskShader colorMask;

	std::vector<TileHitBox> collData;
	std::unique_ptr<MapCollisionBuilder> collBuilder;

	// KHI: For Testing
	bool toggleActiveDebugDraw = false;

public:

	SceneDemo();
	~SceneDemo() override = default;

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void ClampToBounds(GameObject& obj);
	const std::vector<TileHitBox>& GetCollData() const { return collData; }

	// KHI: For TEST !!!
	bool CheckCollisionWithPlayer(float dt);
};