#include "stdafx.h"
#include "SceneDemo.h"
#include "Player.h"
#include "WaterBalloon.h"
#include "WaterSplashPool.h"
#include "Item.h"
#include "Block.h"
#include "MapCollisionBuilder.h"
#include "GameSceneUI.h"

SceneDemo::SceneDemo()
	: Scene(SceneIds::Demo), dao(nullptr), bazzi(nullptr), item(nullptr)
{
}

void SceneDemo::Init()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();

	float scale = 1.30f;
	sf::Vector2f worldSize = windowSize * scale;

	sf::Vector2f topLeft = { 27.f, 55.f };
	worldView.setSize(worldSize);
	worldView.setCenter(worldSize.x * 0.5f - topLeft.x, worldSize.y * 0.5f - topLeft.y);

	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);
	uiView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

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
	texIds.push_back("assets/player/bazzi/trap2.png");
	texIds.push_back("assets/player/bazzi/live.png");
	texIds.push_back("assets/player/bazzi/jump.png");
	texIds.push_back("assets/player/bazzi/ready.png");
	texIds.push_back("assets/player/bazzi/flash_short.png");
	texIds.push_back("assets/play_bg.bmp");

	// LMJ: "Load forest tileset texture for map loading (same as MapEditor)"
	texIds.push_back(PATH_MAP_FOREST_TILE "forest_tile_set.png");

	// LMJ: "Load forest blocks for map loading
	texIds.push_back("assets/map/forest/block/block_1.bmp");
	texIds.push_back("assets/map/forest/block/block_2.bmp");
	texIds.push_back("assets/map/forest/block/block_3.bmp");
	texIds.push_back("assets/map/forest/block/block_4.bmp");
	texIds.push_back("assets/map/forest/block/block_5.bmp");
	texIds.push_back("assets/map/forest/block/block_6.bmp");

	// KHI: Blocks - Keep existing block textures
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
	//ANI_CLIP_MGR.Load("animation/bazzi_trap2.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_die.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_win.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_ready.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_ready2.csv");

	bazzi = static_cast<Player*>(AddGameObject(new Player("Bazzi", CharacterID::BAZZI, 0)));
	dao = static_cast<Player*>(AddGameObject(new Player("Dao", CharacterID::DAO, 1)));

	objectsNeedingClamp.push_back(bazzi);
	objectsNeedingClamp.push_back(dao);

	colorMask.LoadFromFile("assets/shaders/transparent.frag");
	colorMask.SetMaskColor(sf::Color(255, 0, 255));
	colorMask.SetThreshold(0.1f);

	//ui = static_cast<GameSceneUI*>(AddGameObject(new GameSceneUI()));

	// LSY: "will handle the game result display"
	//fontIds.push_back("assets/font/ARCADECLASSIC.TTF");

	font.loadFromFile("assets/font/ARCADECLASSIC.TTF");
	textResult.setFont(font);
	textResult.setOrigin(textResult.getGlobalBounds().width * 0.5f, textResult.getGlobalBounds().height * 0.5f);
	textResult.setOutlineThickness(2);
	textResult.setOutlineColor(sf::Color::Black);
	textResult.setCharacterSize(100);
	textResult.setFillColor(sf::Color::White);
	textResult.setPosition(worldBounds.width * 0.5f - 170.f, 100.f);

	Scene::Init();
}

void SceneDemo::Enter()
{
	Scene::Enter();

	sf::Texture& tex = TEXTURE_MGR.Get("assets/play_bg.bmp");
	uiSprite.setTexture(tex);

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
	sf::Vector2u texSize = tex.getSize();

	sf::Vector2f scale;
	scale.x = windowSize.x / static_cast<float>(texSize.x);
	scale.y = windowSize.y / static_cast<float>(texSize.y);

	uiSprite.setScale(scale);

	uiSprite.setPosition(0.f, 0.f);
	Utils::SetOrigin(uiSprite, Origins::TL);


	Item::SetPlayer(bazzi);
	Item::SetPlayer(dao);

	WaterSplashPool::SetCurScene(SCENE_MGR.GetCurrentScene());
	WaterSplashPool::Init();

	Item::SpawnItem("item", Item::ItemType::Balloon, { 200.f, 200.f });
	Item::SpawnItem("item", Item::ItemType::Speed, { 300.f, 200.f });
	Item::SpawnItem("item", Item::ItemType::WaterJet, { 400.f, 200.f });

	std::cout << "===================" << std::endl;
	std::cout << "     SceneDemo" << std::endl;
	std::cout << "===================" << std::endl;

	bazzi->SetPosition({ 234, 260 });
	dao->SetPosition({ 546, 468 });

	goReadyRoom = false;

	bazzi->SetEnter(true);
	dao->SetEnter(true);
	// LMJ: "Load map from JSON file created in MapEditor"
	// LMJ: "This replaces the old manual tile/block creation"
	if (!LOAD_MAP(this, "DemoMap.json"))
	{
		std::cout << "Failed to load temp_map.json, falling back to manual setup..." << std::endl;
		// LMJ: "Fallback to old method if JSON loading fails"
	}
	else
	{
		std::cout << "Successfully loaded map from temp_map.json!" << std::endl;
	}

	for (int y = 0; y < 13; ++y)
	{
		for (int x = 0; x < 15; ++x)
		{
			blockLayer[y][x] = Utils::CollBlockLayer[y][x];
		}
	}

	collBuilder = std::make_unique<MapCollisionBuilder>(blockLayer);
	collBuilder->CreateCollisionHitBox();
	collData = collBuilder->GetTileHitBoxes();

	bazzi->SetMapData(collData);
	dao->SetMapData(collData);
}

void SceneDemo::Update(float dt)
{
	gameTimer += dt;
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		toggleActiveDebugDraw = !toggleActiveDebugDraw;
	}

	for (auto* obj : objectsNeedingClamp)
		ClampToBounds(*obj);


	CheckCollisionWithPlayer(dt);
	if (bazzi->GetPlayerState() == AnimState::Dead)
	{
		isShowingText = true;
		dao->SetGameOver(true, false, dt);
		textResult.setString("2P Win");
		gameTimer = 0.f;
	}

	if (dao->GetPlayerState() == AnimState::Dead)
	{
		isShowingText = true;
		bazzi->SetGameOver(true, false, dt);
		textResult.setString("1P Win");
		gameTimer = 0.f;
		goReadyRoom = true;
	}

	//if (gameTimer > 20.f && bazzi->GetPlayerState() == AnimState::Live && dao->GetPlayerState() == AnimState::Live) // LSY: "Game over after 20 second"
	//{
	//	isShowingText = true;
	//	textResult.setString("Draw");
	//	bazzi->SetGameOver(false, true, dt);
	//	dao->SetGameOver(false, true, dt);
	//	gameTimer = 0.f;
	//	goReadyRoom = true;
	//	std::cout << "Time's up! Draw!" << std::endl;
	//}

	// LSY: click to exit
	if (InputMgr::GetMouseButton(sf::Mouse::Left) &&
		(clickableArea.contains((sf::Vector2f)InputMgr::GetMousePosition())))
	{
		SCENE_MGR.ChangeScene(SceneIds::Ready);
	}

	//if (goReadyRoom)
	//{
	//	readyRoomTimer += dt;
	//	if (readyRoomTimer > 8.f)
	//	{
	//		goReadyRoom = false;
	//		isShowingText = false;
	//		readyRoomTimer = 0.f;
	//		SCENE_MGR.ChangeScene(SceneIds::Ready);
	//	}
	//}
	Scene::Update(dt);
}

void SceneDemo::Exit()
{
	auto items = FindGameObjects("item");

	for (auto* obj : items)
	{
		obj->SetActive(false);
	}

	Item::CheckAndRemoveItem();
	Item::allItems.clear();
	Item::players.clear();

	Scene::Exit();
}

void SceneDemo::Draw(sf::RenderWindow& window)
{
	window.setView(window.getDefaultView());
	colorMask.Apply(window, uiSprite);

	window.setView(worldView);
	Scene::Draw(window);

	for (const auto& sprite : sprites)
	{
		colorMask.Apply(window, sprite);
	}

	if (toggleActiveDebugDraw)
	{
		window.draw(gridLines);
		collBuilder->DrawDebugHitBox(window);
	}

	if (isShowingText)
	{
		window.setView(uiView);
		window.draw(textResult);
		isShowingText = false;
	}
}

void SceneDemo::ClampToBounds(GameObject& obj)
{
	sf::Vector2f pos = obj.GetPosition();
	// LSY : if ( origins :: BC ) of obj -> (+) getGlobalBounds().width * 0.5f // Becomes unstable when object sizes are different
	pos.x = Utils::Clamp(pos.x, worldBounds.left + obj.GetGlobalBounds().width * 0.35f, worldBounds.left + worldBounds.width - obj.GetGlobalBounds().width * 0.65f);
	pos.y = Utils::Clamp(pos.y, worldBounds.top + obj.GetGlobalBounds().height * 0.75f, worldBounds.top + worldBounds.height+78.f);
	obj.SetPosition(pos);
}

bool SceneDemo::CheckCollisionWithPlayer(float dt)
{
	if (Utils::CheckCollision(dao->GetHitBox().rect, bazzi->GetHitBox().rect))
	{
		if (dao->GetPlayerState() == AnimState::Trapped && bazzi->GetPlayerState() != AnimState::Trapped)
		{
			dao->HandleBubbleDeath(AnimState::Dead);
			std::cout << " 2P Player Dead " << std::endl;
			std::cout << "GameOver" << std::endl;
			return true;
		}
		else if (bazzi->GetPlayerState() == AnimState::Trapped && dao->GetPlayerState() != AnimState::Trapped)
		{
			bazzi->HandleBubbleDeath(AnimState::Dead);
			std::cout << " 1P Player Dead " << std::endl;
			std::cout << "GameOver" << std::endl;
			return true;
		}
	}
	return false;
}