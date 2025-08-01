#pragma once
#include "Scene.h"
#include "CharacterStats.h"
#include "Item.h"
#include "ColorMaskShader.h"

class GameSceneUI;
class ResultPop;
class SceneDemo : public Scene
{
protected:
	sf::Text textResult;
	sf::Font font;
	bool isShowingText = false;
	float readyRoomTimer = 0.f;

	//sf::FloatRect clickableArea = sf::FloatRect(647.f,561.f,141.f, 32.f);

	const int GRID_HEIGHT = 13;
	const int GRID_WIDTH = 15;
	const int GRID_SIZE = 52;

	float gameTimer;
	bool isGameOver = false;
	bool goReadyRoom = false;
	Player* bazzi;
	Player* dao;
	Player* player3p;
	Player* player4p;

	Item* item;
	sf::FloatRect worldBounds = FRAMEWORK.GetWindowBounds();
	std::vector<GameObject*> objectsNeedingClamp;

	GameSceneUI* ui;
	ResultPop* popUi;

	sf::Sprite testBlockSprite;

	sf::VertexArray gridLines;

	std::vector<sf::Sprite> sprites;

	// KHI: Layers
	int blockLayer[13][15] = { 0 };

	ColorMaskShader colorMask;

	// KHI: For Testing
	bool toggleActiveDebugDraw = false;

	sf::Sprite uiSprite;

public:
	SceneDemo();
	~SceneDemo() override = default;

	void Init() override;
	void Enter() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
	void Exit() override;

	void ClampToBounds(GameObject& obj);

	// KHI: For TEST !!!
	bool CheckCollisionWithPlayer(float dt);
};