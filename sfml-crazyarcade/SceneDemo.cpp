#include "stdafx.h"
#include "SceneDemo.h"
#include "Player.h"
#include "WaterBalloon.h"
#include "WaterSplashPool.h"
#include "Item.h"
#include "Block.h"

SceneDemo::SceneDemo()
	: Scene(SceneIds::Demo)
{
}

void SceneDemo::Init()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
	worldView.setSize(windowSize);
	worldView.setCenter(windowSize * 0.5f);

	// KHI: For Testing (Draw Grids)
	gridLines.setPrimitiveType(sf::Lines);
	gridLines.clear();

	sf::Color gridColor(255, 255, 255, 100);
	for (int x = 0; x <= GRID_WIDTH; ++x)
	{
		float xPos = x * GRID_SIZE;
		gridLines.append(sf::Vertex(sf::Vector2f(xPos, 0), gridColor));
		gridLines.append(sf::Vertex(sf::Vector2f(xPos, GRID_HEIGHT * GRID_SIZE), gridColor));
	}
	for (int y = 0; y <= GRID_HEIGHT; ++y)
	{
		float yPos = y * GRID_SIZE;
		gridLines.append(sf::Vertex(sf::Vector2f(0, yPos), gridColor));
		gridLines.append(sf::Vertex(sf::Vector2f(GRID_WIDTH * GRID_SIZE, yPos), gridColor));
	}

	// KHI: Player
	texIds.push_back("assets/player/bazzi/right.png");
	texIds.push_back("assets/player/bazzi/up.png");
	texIds.push_back("assets/player/bazzi/down.png");
	texIds.push_back("assets/player/bazzi/die.png");
	texIds.push_back("assets/player/bazzi/trap.png");
	texIds.push_back("assets/player/bazzi/live.png");
	texIds.push_back("assets/player/bazzi/jump.png");

	// KHI: Blocks
	texIds.push_back("assets/map/forest/tile/tile_9.bmp");
	texIds.push_back("assets/map/forest/object/object_3.bmp");

	// KHI: Items
	texIds.push_back("assets/item/bubble.png");
	texIds.push_back("assets/item/damage.png");
	texIds.push_back("assets/item/speed.png");

	ANI_CLIP_MGR.Load("animation/bazzi_run.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_up.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_down.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_live.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_trap.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_die.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_win.csv");

	bazzi = static_cast<Player*>(AddGameObject(new Player("Bazzi", CharacterID::BAZZI, 0)));
	Dao = static_cast<Player*>(AddGameObject(new Player("Dao", CharacterID::DAO, 1)));

	objectsNeedingClamp.push_back(bazzi);
	objectsNeedingClamp.push_back(Dao);

	colorMask.LoadFromFile("assets/shaders/transparent.frag");
	colorMask.SetMaskColor(sf::Color(255, 0, 255));
	colorMask.SetThreshold(0.1f);

	Scene::Init();
}

void SceneDemo::Enter()
{
	Scene::Enter();

	Item::SetPlayer(bazzi);
	Item::SetPlayer(Dao);

	WaterSplashPool::SetCurScene(SCENE_MGR.GetCurrentScene());
	WaterSplashPool::Init();

	Item::SpawnItem("item", Item::ItemType::Balloon, { 200.f, 200.f });
	Item::SpawnItem("item", Item::ItemType::Speed, { 300.f, 200.f });
	Item::SpawnItem("item", Item::ItemType::WaterJet, { 400.f, 200.f });

	std::cout << "===================" << std::endl;
	std::cout << "     SceneDemo"      << std::endl;
	std::cout << "===================" << std::endl;

	bazzi->SetPosition({ 100,100 });
	Dao->SetPosition({ 200,100 });

	//Block* testBlock = new Block();
	//testBlock->SetBlockType(BlockType::SoftBlock);

	//testBlockSprite.setTexture(TEXTURE_MGR.Get("assets/map/forest/block/block_1.bmp"));
	//testBlockSprite.setPosition({300.f, 300.f});

	SetLayerForTest();
}

void SceneDemo::Update(float dt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		toggleActiveGrid = !toggleActiveGrid;
	}
	
	for (auto* obj : objectsNeedingClamp)
		ClampToBounds(*obj);

	CheckCollisionWithPlayer();
	
	Scene::Update(dt);
}

void SceneDemo::Draw(sf::RenderWindow& window)
{
	for (int i = 0; i < sprites.size(); i++)
	{
		colorMask.Apply(window, sprites[i]);
	}

	if (toggleActiveGrid)
		window.draw(gridLines);

	Scene::Draw(window);
}

void SceneDemo::ClampToBounds(GameObject& obj)
{
	sf::Vector2f pos = obj.GetPosition();
	// LSY : if ( origins :: BC ) of obj -> (+) getGlobalBounds().width * 0.5f // Becomes unstable when object sizes are different
	pos.x = Utils::Clamp(pos.x, worldBounds.left + obj.GetGlobalBounds().width * 0.5f, worldBounds.left + worldBounds.width - obj.GetGlobalBounds().width * 0.5f);
	pos.y = Utils::Clamp(pos.y, worldBounds.top + obj.GetGlobalBounds().height * 0.5f, worldBounds.top + worldBounds.height);
	obj.SetPosition(pos);
}

bool SceneDemo::CheckCollisionWithPlayer()
{
	if (Utils::CheckCollision(Dao->GetHitBox().rect, bazzi->GetHitBox().rect))
	{
		if (Dao->GetPlayerState() == AnimState::Trapped)
		{
			Dao->CheckBubblePop(AnimState::Dead);
			std::cout << " 2P Player Dead " << std::endl;
			std::cout << "GameOver" << std::endl;
			bazzi->SetGameOver();
		}
		else if (bazzi->GetPlayerState() == AnimState::Trapped)
		{
			bazzi->CheckBubblePop(AnimState::Dead);
			std::cout << " 1P Player Dead " << std::endl;
			std::cout << "GameOver" << std::endl;
			Dao->SetGameOver();
		}
		return true;
	}
}

void SceneDemo::SetLayerForTest()
{
	// KHI: background
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			if (layer0[i][j] == 1)
			{
				sf::Sprite sprite;
				sprite.setTexture(TEXTURE_MGR.Get("assets/map/forest/tile/tile_9.bmp"));
				sprite.setPosition(sf::Vector2f(j * GRID_SIZE, i * GRID_SIZE));
				sprites.push_back(sprite);
			}
		}
	}

	// KHI: objects
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			if (layer1[i][j] == 1)
			{
				sf::Sprite sprite;
				sprite.setTexture(TEXTURE_MGR.Get("assets/map/forest/object/object_3.bmp"));
				sprite.setPosition(sf::Vector2f(j * GRID_SIZE, i * GRID_SIZE));
				sprites.push_back(sprite);
			}
		}
	}
}