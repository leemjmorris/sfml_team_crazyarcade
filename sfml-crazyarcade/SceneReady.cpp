#include "stdafx.h"
#include "SceneReady.h"

SceneReady::SceneReady()
	: Scene(SceneIds::Ready)
{
}

void SceneReady::Init()
{
	texIds.push_back("assets/lobby_bg.png");
	texIds.push_back("assets/SelectMapImg.png");

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
	worldView.setSize(windowSize);
	worldView.setCenter(windowSize * 0.5f);

	TEXTURE_MGR.Load(texIds);

	//Button::LoadHitMask("Assets/UI/lobbyColorMap.png", { 0.f, 0.f }, { 1.f, 1.f });

	/*auto btnMap = ui.Add<Button>("btnMap");
	btnMap->SetButton({ 120.f, 50.f }, sf::Color{ 40, 144, 240 },
		"Fonts/NanumGothic.ttf", sf::Color::Black, 2.f);
	btnMap->SetPosition({ 610.f, 420.f });
	btnMap->SetText("¢¬?¨ù¡¾??");
	btnMap->UseBackground(false);
	btnMap->SetOnClick([this]() { OpenMapSelectPopup(); });*/
	ui = new lobbyUi("loobyUi");
	ui->Init();

	mapListUi = new MapLists("MapList");
	mapListUi->Init();

	Scene::Init();
}

void SceneReady::Enter()
{
	TEXTURE_MGR.Load(texIds);
	sf::Texture& bgTexture = TEXTURE_MGR.Get("assets/lobby_bg.png");
	bgSprite.setTexture(bgTexture);

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
	worldView.setSize(windowSize);
	worldView.setCenter(windowSize * 0.5f);
	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);

	sf::Vector2u textureSize = bgTexture.getSize();
	sf::Vector2f textureSizeF(static_cast<float>(textureSize.x), static_cast<float>(textureSize.y));

	sf::Vector2f scale;
	scale.x = windowSize.x / textureSizeF.x;
	scale.y = windowSize.y / textureSizeF.y;

	bgSprite.setScale(scale);


	mapListUi->Reset();

	Scene::Enter();
}

void SceneReady::Update(float dt)
{
	//if (InputMgr::GetMouseButtonDown(sf::Mouse::Left)) // && readyButton.Contains(mousePos)
	//{
	//	//SCENE_MGR.ChangeScene(SceneIds::Demo);
	//}
	ui->Update(dt);
	Scene::Update(dt);
}

void SceneReady::Draw(sf::RenderWindow& window)
{
	window.setView(worldView);
	window.draw(bgSprite);
	window.setView(window.getDefaultView());
	mapListUi->Draw(window);
	ui->Draw(window);
	Scene::Draw(window);
}