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
	sf::Text textDraw;
	sf::Font font;
	bool isShowingText = false;

	sf::FloatRect clickableArea = sf::FloatRect(647.f,561.f,141.f, 32.f);

	const int GRID_HEIGHT = 13;
	const int GRID_WIDTH = 15;
	const int GRID_SIZE = 52;

	float gameTimer;
	bool isGameOver = false;

	Player* bazzi;
	Player* dao;
	Item* item;
	sf::FloatRect worldBounds = FRAMEWORK.GetWindowBounds();
	//sf::FloatRect worldBounds = { 0, 780, 0, 676 };
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