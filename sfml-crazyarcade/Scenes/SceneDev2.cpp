#include "stdafx.h"
#include "SceneDev2.h"
#include "Player.h"

SceneDev2::SceneDev2() : Scene(SceneIds::Dev2)
{
}

void SceneDev2::Init()
{
	//texIds.push_back("assets/player/bazzi/right.png");
	texIds.push_back("graphics/right.png");
	ANI_CLIP_MGR.Load("assets/animations/bazzi_run.csv");
	player = static_cast<Player*>(AddGameObject(new Player("Player")));
	Scene::Init();
}

void SceneDev2::Enter()
{
	Scene::Enter();
	player->SetPosition({ 10, 10 });
}

void SceneDev2::Update(float dt)
{
	Scene::Update(dt);
	//std::cout << player->GetPosition().x << " " << player->GetPosition().y << std::endl;
}

void SceneDev2::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
