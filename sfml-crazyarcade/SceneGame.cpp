#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "WaterBalloon.h"
#include "WaterSplashPool.h"
#include "Item.h"
#include "Block.h"
#include "GameSceneUI.h"
#include "ResultPop.h"

SceneGame::SceneGame()
	: Scene(SceneIds::Game), dao(nullptr), bazzi(nullptr), item(nullptr)
{
}

void SceneGame::Init()
{
	ui = new GameSceneUI("ui");
	ui->Init();
	popUi = new ResultPop("popUi");
	popUi->Init();

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

	sf::Color gridColor(255, 255, 255, 255);
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
	texIds.push_back("assets/play_ui.png");

	// KHI: Effect
	texIds.push_back("assets/map/common_block.png");

	// LMJ: "Load forest tileset texture for map loading (same as MapEditor)"
	texIds.push_back(PATH_TILE_SHEET "tile_set.png");

	// LMJ: "Load forest blocks for map loading
	texIds.push_back(PATH_BLOCK_SHEET "block_sheet.png");

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
	ANI_CLIP_MGR.Load("animation/bazzi_die.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_win.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_ready.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_ready2.csv");

	bazzi = static_cast<Player*>(AddGameObject(new Player("Player", CharacterID::BAZZI, 0, "Bazzi")));
	dao = static_cast<Player*>(AddGameObject(new Player("Player", CharacterID::DAO, 1, "Dao")));
	player3p = static_cast<Player*>(AddGameObject(new Player("Player", CharacterID::DAO, 2, "player3p")));
	player4p = static_cast<Player*>(AddGameObject(new Player("Player", CharacterID::DAO, 3, "player4p")));
	
	objectsNeedingClamp.push_back(bazzi);
	objectsNeedingClamp.push_back(dao);
	objectsNeedingClamp.push_back(player3p);
	objectsNeedingClamp.push_back(player4p);

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

void SceneGame::Enter()
{
	Scene::Enter();

	//sf::Texture& tex = TEXTURE_MGR.Get("assets/play_bg.bmp");
	sf::Texture& tex = TEXTURE_MGR.Get("assets/play_ui.png");
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
	Item::SetPlayer(player3p);
	Item::SetPlayer(player4p);

	WaterSplashPool::SetCurScene(SCENE_MGR.GetCurrentScene());
	WaterSplashPool::Init();

	std::cout << "===================" << std::endl;
	std::cout << "     SceneGame"      << std::endl;
	std::cout << "===================" << std::endl;

	// LMJ: Load map from JSON file created in MapEditor
	auto blocks = FindGameObjects("Block");
	for (auto* obj : blocks) RemoveGameObject(obj);
	if (!LOAD_MAP(this, SceneMgr::SelectedMapPath))
	{
		std::cout << "Failed to load " << SceneMgr::SelectedMapPath << ", using default positions..." << std::endl;
	}
	else
	{
		std::cout << "Successfully loaded map from " << SceneMgr::SelectedMapPath << "!" << std::endl;
	}

	// LMJ: Set player positions using helper functions
	bazzi->SetPosition(Utils::GetPlayerSpawnPoint(0));
	dao->SetPosition(Utils::GetPlayerSpawnPoint(1));
	player3p->SetPosition(Utils::GetPlayerSpawnPoint(2));
	player4p->SetPosition(Utils::GetPlayerSpawnPoint(3));

	std::cout << "Player 1 position: (" << bazzi->GetPosition().x << ", " << bazzi->GetPosition().y << ")" << std::endl;
	std::cout << "Player 2 position: (" << dao->GetPosition().x << ", " << dao->GetPosition().y << ")" << std::endl;

	goReadyRoom = false;
	bazzi->SetEnter(true);
	dao->SetEnter(true);
	player3p->SetEnter(true);
	player4p->SetEnter(true);
	popUi->SetResult({ bazzi,dao,player3p });
	// LMJ: Initialize collision system
	//for (int y = 0; y < 13; ++y)
	//{
	//	for (int x = 0; x < 15; ++x)
	//	{
	//		blockLayer[y][x] = Utils::CollBlockLayer[y][x];
	//	}
	//}
	ui->Reset();
	popUi->Reset();
}

void SceneGame::Update(float dt)
{
	InputMgr::UpdateGamepad();
	InputMgr::Update(dt);

	gameTimer += dt;
	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		toggleActiveDebugDraw = !toggleActiveDebugDraw;
	}

	for (auto* obj : objectsNeedingClamp)
		ClampToBounds(*obj);

	static bool printed = false;
	if (bazzi->GetPlayerState() == AnimState::Win)
	{
		popUi->SetResult({ bazzi, dao });
		popUi->SetWinner(1);
		popUi->SetActive(true);
		printed = true;
	}

	if (dao->GetPlayerState() == AnimState::Win)
	{
		popUi->SetResult({ bazzi, dao });
		popUi->SetWinner(2);
		popUi->SetActive(true);
		printed = true;
	}

	CheckCollisionWithPlayer(dt);
	if (bazzi->GetPlayerState() == AnimState::Dead)
	{
		//dao->SetPlayerState(AnimState::Win);
		isShowingText = true;
		dao->SetGameOver(true, false, dt);
		textResult.setString("2P Win");
		gameTimer = 0.f;
		goReadyRoom = true;
		//popUi->SetResult({ bazzi, dao });
		//popUi->SetActive(true);
		//popUi->SetWinner(2);
	}

	if (dao->GetPlayerState() == AnimState::Dead)
	{
		//bazzi->SetPlayerState(AnimState::Win);
		isShowingText = true;
		bazzi->SetGameOver(true, false, dt);
		textResult.setString("1P Win");
		gameTimer = 0.f;
		goReadyRoom = true;
		//popUi->SetResult({ bazzi, dao });
		//popUi->SetActive(true);
		//popUi->SetWinner(1);
	}

	if (gameTimer > 1500.f && bazzi->GetPlayerState() == AnimState::Live && dao->GetPlayerState() == AnimState::Live) // LSY: "Game over after 20 second"
	{
		bazzi->SetPlayerState(AnimState::Draw);
		dao->SetPlayerState(AnimState::Draw);
		isShowingText = true;
		textResult.setString("Draw");
		bazzi->SetGameOver(false, true, dt);
		dao->SetGameOver(false, true, dt);
		gameTimer = 0.f;
		goReadyRoom = true;
		std::cout << "Time's up! Draw!" << std::endl;
		popUi->SetResult({ bazzi, dao });
		popUi->SetActive(true);
		//popUi->SetResult();
	}

	// LSY: click to exit
	//if (InputMgr::GetMouseButton(sf::Mouse::Left) &&
	//	(clickableArea.contains((sf::Vector2f)InputMgr::GetMousePosition())))
	//{
	//	SCENE_MGR.ChangeScene(SceneIds::Ready);
	//}

	if (goReadyRoom)
	{
		readyRoomTimer += dt;
		if (readyRoomTimer > 5.f)
		{
			isShowingText = false;
			goReadyRoom = false;
			readyRoomTimer = 0.f;
			SCENE_MGR.ChangeScene(SceneIds::Ready);
		}
	}
	ui->Update(dt);
	popUi->Update(dt);
	Scene::Update(dt);
}

void SceneGame::Exit()
{
	popUi->SetActive(false);
	// KHI: delete balloons
	auto balloons = FindGameObjects("bomb");
	for (auto* obj : balloons)
	{
		WaterBalloon* b = dynamic_cast<WaterBalloon*>(obj);
		if (b->GetActive() == false)
		{
			RemoveGameObject(obj);
		}
	}

	auto items = FindGameObjects("item");

	for (auto* obj : items)
	{
		obj->SetActive(false);
	}
	isShowingText = false;
	goReadyRoom = false;
	readyRoomTimer = 0.f;

	auto blocks = FindGameObjects("Block");
	for (auto* obj : blocks)
		RemoveGameObject(obj);

	bazzi->Reset();
	dao->Reset();
	player3p->Reset();
	player4p->Reset();
	Item::CheckAndRemoveItem();
	Item::allItems.clear();
	Item::players.clear();

	Scene::Exit();
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	window.setView(window.getDefaultView());
	colorMask.Apply(window, uiSprite);

	ui->Draw(window);

	window.setView(worldView);
	Scene::Draw(window);

	for (const auto& sprite : sprites)
	{
		colorMask.Apply(window, sprite);
	}

	if (toggleActiveDebugDraw)
	{
		window.draw(gridLines);
	}

	/*if (isShowingText)
	{
		window.setView(uiView);
		window.draw(textResult);
	}*/
	window.setView(uiView);
	popUi->Draw(window);
}

void SceneGame::ClampToBounds(GameObject& obj)
{
	sf::Vector2f pos = obj.GetPosition();
	// LSY : if ( origins :: BC ) of obj -> (+) getGlobalBounds().width * 0.5f // Becomes unstable when object sizes are different
	pos.x = Utils::Clamp(pos.x, worldBounds.left + obj.GetGlobalBounds().width * 0.35f, worldBounds.left + worldBounds.width - obj.GetGlobalBounds().width * 0.65f);
	pos.y = Utils::Clamp(pos.y, worldBounds.top + obj.GetGlobalBounds().height * 0.56f, worldBounds.top + worldBounds.height + 78.f);
	obj.SetPosition(pos);
}

bool SceneGame::CheckCollisionWithPlayer(float dt)
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