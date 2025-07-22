#include "stdafx.h"
#include "SceneDev2.h"
#include "Player.h"

SceneDev2::SceneDev2() : Scene(SceneIds::Dev2)
{
}

void SceneDev2::Init()
{
	/*Scene::Init();
	texIds.push_back("assets/player/bazzi/left.png");
	ANI_CLIP_MGR.Load("assets/animations/left.csv");
	player = static_cast<Player*>(AddGameObject(new Player("Player")));*/
}

void SceneDev2::Enter()
{
	Scene::Enter();
	player->SetPosition({ 10, 10 });
}

void SceneDev2::Update(float dt)
{
	Scene::Update(dt);
}

void SceneDev2::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
