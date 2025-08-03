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
	texIds.push_back("assets/player/bazzi/ready.png");
	texIds.push_back("assets/player/cappy/unit_cappy.png");
	texIds.push_back("assets/player/dao/unit_dao.png");
	texIds.push_back("assets/player/marid/unit_marid.png");
	texIds.push_back("assets/player/cappy/ready.png");
	texIds.push_back("assets/player/dao/ready.png");
	texIds.push_back("assets/player/marid/ready.png");
	texIds.push_back("assets/play_bg.bmp");
	texIds.push_back("assets/play_ui.png");

	texIds.push_back("assets/bomb/pop.bmp");

	texIds.push_back("assets/wave/up1.bmp");
	texIds.push_back("assets/wave/down1.bmp");
	texIds.push_back("assets/wave/left1.bmp");
	texIds.push_back("assets/wave/right1.bmp");

	texIds.push_back("assets/wave/up2.bmp");
	texIds.push_back("assets/wave/down2.bmp");
	texIds.push_back("assets/wave/left2.bmp");
	texIds.push_back("assets/wave/right2.bmp");

	texIds.push_back("assets/item/bubble.png");
	texIds.push_back("assets/item/damage.png");
	texIds.push_back("assets/bomb/waterBalloon.png");
	texIds.push_back("assets/bomb/default.png");

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

	texIds.push_back("assets/item/bubble.png");
	texIds.push_back("assets/item/damage.png");
	texIds.push_back("assets/bomb/waterBalloon.png");
	texIds.push_back("assets/bomb/default.png");

	// KHI: wave
	texIds.push_back("assets/bomb/pop.bmp");

	texIds.push_back("assets/wave/up1.bmp");
	texIds.push_back("assets/wave/down1.bmp");
	texIds.push_back("assets/wave/left1.bmp");
	texIds.push_back("assets/wave/right1.bmp");

	texIds.push_back("assets/wave/up2.bmp");
	texIds.push_back("assets/wave/down2.bmp");
	texIds.push_back("assets/wave/left2.bmp");
	texIds.push_back("assets/wave/right2.bmp");

	TEXTURE_MGR.Load(texIds);

	ANI_CLIP_MGR.Load("animation/bazzi_run.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_up.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_down.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_live.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_trap.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_die.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_win.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_ready.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_ready2.csv");

	ANI_CLIP_MGR.Load("animation/cappy_die.csv");
	ANI_CLIP_MGR.Load("animation/cappy_down.csv");
	ANI_CLIP_MGR.Load("animation/cappy_live.csv");
	ANI_CLIP_MGR.Load("animation/cappy_ready.csv");
	ANI_CLIP_MGR.Load("animation/cappy_run.csv");
	ANI_CLIP_MGR.Load("animation/cappy_trap.csv");
	ANI_CLIP_MGR.Load("animation/cappy_up.csv");
	ANI_CLIP_MGR.Load("animation/cappy_win.csv");
	ANI_CLIP_MGR.Load("animation/cappy_idle.csv");
	ANI_CLIP_MGR.Load("animation/cappy_idleSide.csv");
	ANI_CLIP_MGR.Load("animation/cappy_idleUp.csv");

	ANI_CLIP_MGR.Load("animation/dao_die.csv");
	ANI_CLIP_MGR.Load("animation/dao_down.csv");
	ANI_CLIP_MGR.Load("animation/dao_live.csv");
	ANI_CLIP_MGR.Load("animation/dao_ready.csv");
	ANI_CLIP_MGR.Load("animation/dao_run.csv");
	ANI_CLIP_MGR.Load("animation/dao_trap.csv");
	ANI_CLIP_MGR.Load("animation/dao_up.csv");
	ANI_CLIP_MGR.Load("animation/dao_win.csv");
	ANI_CLIP_MGR.Load("animation/dao_idle.csv");
	ANI_CLIP_MGR.Load("animation/dao_idleSide.csv");
	ANI_CLIP_MGR.Load("animation/dao_idleUp.csv");

	ANI_CLIP_MGR.Load("animation/marid_die.csv");
	ANI_CLIP_MGR.Load("animation/marid_down.csv");
	ANI_CLIP_MGR.Load("animation/marid_live.csv");
	ANI_CLIP_MGR.Load("animation/marid_ready.csv");
	ANI_CLIP_MGR.Load("animation/marid_run.csv");
	ANI_CLIP_MGR.Load("animation/marid_trap.csv");
	ANI_CLIP_MGR.Load("animation/marid_up.csv");
	ANI_CLIP_MGR.Load("animation/marid_win.csv");
	ANI_CLIP_MGR.Load("animation/marid_idle.csv");
	ANI_CLIP_MGR.Load("animation/marid_idleSide.csv");
	ANI_CLIP_MGR.Load("animation/marid_idleUp.csv");

	ANI_CLIP_MGR.Load("animation/waterSplashAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashDownAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashDownEndAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashDownExitAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashExitAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashLeftAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashLeftEndAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashLeftExitAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashRightAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashRightEndAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashRightExitAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashUpAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashUpEndAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashUpExitAnim.csv");

	SOUNDBUFFER_MGR.Load("sounds/Blow_WaterBomb.mp3");
	SOUNDBUFFER_MGR.Load("sounds/Boomhill.mp3");
	SOUNDBUFFER_MGR.Load("sounds/Bubble_Sound.wav");
	SOUNDBUFFER_MGR.Load("sounds/Eat_Item.mp3");
	SOUNDBUFFER_MGR.Load("sounds/Game_Start.mp3");
	SOUNDBUFFER_MGR.Load("sounds/Kill_WaterBomb.wav");
	SOUNDBUFFER_MGR.Load("sounds/Lobby.wav");
	SOUNDBUFFER_MGR.Load("sounds/Lobby_Select.mp3");
	SOUNDBUFFER_MGR.Load("sounds/logo.mp3");
	SOUNDBUFFER_MGR.Load("sounds/Result_Draw.mp3");
	SOUNDBUFFER_MGR.Load("sounds/Result_Win.mp3");
	SOUNDBUFFER_MGR.Load("sounds/Set_WaterBomb.mp3");


	//bazzi = static_cast<Player*>(AddGameObject(new Player("Player", CharacterID::BAZZI, 0, "Bazzi")));
	//dao = static_cast<Player*>(AddGameObject(new Player("Player", CharacterID::DAO, 1, "Dao")));
	//player3p = static_cast<Player*>(AddGameObject(new Player("Player", CharacterID::DAO, 2, "player3p")));
	//player4p = static_cast<Player*>(AddGameObject(new Player("Player", CharacterID::DAO, 3, "player4p")));

	//players = { bazzi, dao, player3p, player4p };
	//
	//objectsNeedingClamp.push_back(bazzi);
	//objectsNeedingClamp.push_back(dao);
	//objectsNeedingClamp.push_back(player3p);
	//objectsNeedingClamp.push_back(player4p);

	colorMask.LoadFromFile("assets/shaders/transparent.frag");
	colorMask.SetMaskColor(sf::Color(255, 0, 255));
	colorMask.SetThreshold(0.1f);

	// LSY: "will handle the game result display"
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

	BuildPlayersFromRoomCount();

	if (bazzi) Item::SetPlayer(bazzi);
	if (dao) Item::SetPlayer(dao);
	if (player3p) Item::SetPlayer(player3p);
	if (player4p) Item::SetPlayer(player4p);

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
		std::cout << "Failed to load DemoMap.json, using default positions..." << std::endl;
	}
	else
	{
		if (SceneMgr::SelectedMapPath == "box_1.json")
			SOUND_MGR.PlayBgm("sounds/Boomhill.mp3");
		//if (SceneMgr::SelectedMapPath == "camp_1.json")
		//	SOUND_MGR.PlayBgm("sounds/Boomhill.mp3");
		//if (SceneMgr::SelectedMapPath == "factory_1.json")
		//	SOUND_MGR.PlayBgm("sounds/Boomhill.mp3");
		//if (SceneMgr::SelectedMapPath == "factory_2.json")
		//	SOUND_MGR.PlayBgm("sounds/Boomhill.mp3");
		//if (SceneMgr::SelectedMapPath == "forest_1.json")
		//	SOUND_MGR.PlayBgm("sounds/Boomhill.mp3");
		//if (SceneMgr::SelectedMapPath == "forest_2.json")
		//	SOUND_MGR.PlayBgm("sounds/Boomhill.mp3");
		//if (SceneMgr::SelectedMapPath == "forest_maze.json")
		//	SOUND_MGR.PlayBgm("sounds/Boomhill.mp3");
		//if (SceneMgr::SelectedMapPath == "Ice_1.json")
		//	SOUND_MGR.PlayBgm("sounds/Boomhill.mp3");
		//if (SceneMgr::SelectedMapPath == "Ice_2.json")
		//	SOUND_MGR.PlayBgm("sounds/Boomhill.mp3");
		//if (SceneMgr::SelectedMapPath == "race_1.json")
		//	SOUND_MGR.PlayBgm("sounds/Boomhill.mp3");
		//if (SceneMgr::SelectedMapPath == "scary_1.json")
		//	SOUND_MGR.PlayBgm("sounds/Boomhill.mp3");
		//if (SceneMgr::SelectedMapPath == "ship_1.json")
		//	SOUND_MGR.PlayBgm("sounds/Boomhill.mp3");
	}

	// LMJ: Set player positions using helper functions
	if (bazzi) bazzi->SetPosition(Utils::GetPlayerSpawnPoint(0));
	if (dao) dao->SetPosition(Utils::GetPlayerSpawnPoint(1));
	if (player3p) player3p->SetPosition(Utils::GetPlayerSpawnPoint(2));
	if (player4p) player4p->SetPosition(Utils::GetPlayerSpawnPoint(3));

	goReadyRoom = false;
	if (bazzi) bazzi->SetEnter(true);
	if (dao) dao->SetEnter(true);
	if (player3p) player3p->SetEnter(true);
	if (player4p) player4p->SetEnter(true);

	ui->Reset();
	popUi->Reset();

	isPlayingResultSound = false;
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

	CheckCollisionAmongPlayers(dt);

	EvaluateRoundState(dt);

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

	auto playerObjs = FindGameObjects("Player");
	for (auto* obj : playerObjs) {
		RemoveGameObject(obj);
	}

	players.clear();
	bazzi = dao = player3p = player4p = nullptr;
	objectsNeedingClamp.clear();

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

	if (bazzi) bazzi->Reset();
	if (dao) dao->Reset();
	if (player3p) player3p->Reset();
	if (player4p) player4p->Reset();
	players.clear();
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
			//std::cout << " 2P Player Dead " << std::endl;
			//std::cout << "GameOver" << std::endl;
			return true;
		}
		else if (bazzi->GetPlayerState() == AnimState::Trapped && dao->GetPlayerState() != AnimState::Trapped)
		{
			bazzi->HandleBubbleDeath(AnimState::Dead);
			//std::cout << " 1P Player Dead " << std::endl;
			//std::cout << "GameOver" << std::endl;
			return true;
		}
	}
	return false;
}

bool SceneGame::CheckCollisionAmongPlayers(float dt)
{
	auto isTrapped = [](AnimState s) { return s == AnimState::Trapped; };

	for (size_t i = 0; i < players.size(); ++i) {
		Player* A = players[i];
		if (!A) continue;

		for (size_t j = i + 1; j < players.size(); ++j) {
			Player* B = players[j];
			if (!B) continue;

			if (!Utils::CheckCollision(A->GetHitBox().rect, B->GetHitBox().rect))
				continue;

			AnimState a = A->GetPlayerState();
			AnimState b = B->GetPlayerState();

			if (isTrapped(a) ^ isTrapped(b)) {
				Player* trapped = isTrapped(a) ? A : B;
				Player* other = (trapped == A) ? B : A;

				if (trapped->GetPlayerState() != AnimState::Dead) {
					trapped->HandleBubbleDeath(AnimState::Dead);
					std::cout << trapped->GetName()
						<< " Dead by contact with "
						<< other->GetName() << std::endl;
					SOUND_MGR.PlaySfx("sounds/Kill_WaterBomb.wav");
					return true;
				}
			}
		}
	}
	return false;
}

void SceneGame::EvaluateRoundState(float dt)
{
	std::vector<Player*> alive;
	alive.reserve(players.size());
	for (auto* p : players) {
		if (!p) continue;
		AnimState s = p->GetPlayerState();
		if (s != AnimState::Dead) {
			alive.push_back(p);
		}
	}

	if (alive.size() == 1) {
		isShowingText = true;

		alive[0]->SetGameOver(true, false, dt);

		/*int winnerNo =
			(alive[0]->GetPlayerNo() ? alive[0]->GetPlayerNo() + 1
				: int(std::distance(players.begin(),
					std::find(players.begin(), players.end(), alive[0]))) + 1);*/

					//textResult.setString(std::to_string(winnerNo) + "P Win");
		popUi->SetResult(players);
		popUi->SetWinner(alive[0]->GetPlayerNo() + 1);
		popUi->SetActive(true);

		gameTimer = 0.f;
		goReadyRoom = true;
		if (!isPlayingResultSound)
		{
		 SOUND_MGR.PlaySfx("sounds/Result_Win.mp3");
		 isPlayingResultSound = true;
		}
		return;
	}

	if (gameTimer > 1500.f) {
		if (alive.size() >= 2) {
			isShowingText = true;
			textResult.setString("Draw");
			for (auto* p : alive) p->SetGameOver(false, true, dt);

			gameTimer = 0.f;
			goReadyRoom = true;
			
			popUi->SetResult(players);
			popUi->SetActive(true);
			if (isPlayingResultSound)
			{
				SOUND_MGR.PlaySfx("sounds/Result_Draw.mp3");
				isPlayingResultSound = false;
			}
		}
	}
}

void SceneGame::BuildPlayersFromRoomCount()
{
	for (auto* p : players) {
		if (p) RemoveGameObject(p);
	}
	players.clear();
	bazzi = dao = player3p = player4p = nullptr;
	objectsNeedingClamp.clear();

	int playerSlots = lobbyConf.roomCount + 1;
	for (int i = 0; i < playerSlots; ++i) {
		CharacterID id = CharacterID::BAZZI;
		if (i < (int)lobbyConf.chars.size())
			id = lobbyConf.chars[i];

		std::string displayName;
		switch (id) {
		case CharacterID::BAZZI: displayName = "Bazzi"; break;
		case CharacterID::DAO:   displayName = "Dao";   break;
		case CharacterID::CAPPI: displayName = "Cappi"; break;
		case CharacterID::MARID: displayName = "Marid"; break;
		default:                 displayName = "Player"; break;
		}

		Player* p = static_cast<Player*>(
			AddGameObject(new Player("Player", id, i, displayName))
			);
		players.push_back(p);
		objectsNeedingClamp.push_back(p);
		if (i == 0) bazzi = p;
		else if (i == 1) dao = p;
		else if (i == 2) player3p = p;
		else if (i == 3) player4p = p;
	}
}