#include "stdafx.h"
#include "SceneDev2.h"
#include "Player.h"
#include "WaterBalloon.h"

SceneDev2::SceneDev2() : Scene(SceneIds::Dev2)
{
}

void SceneDev2::Init()
{
	texIds.push_back("assets/player/bazzi/right.png");
	texIds.push_back("assets/player/bazzi/up.png");
	texIds.push_back("assets/player/bazzi/down.png");
	ANI_CLIP_MGR.Load("assets/animations/bazzi_run.csv");
	ANI_CLIP_MGR.Load("assets/animations/bazzi_up.csv");
	ANI_CLIP_MGR.Load("assets/animations/bazzi_down.csv");

	texIds.push_back("assets/item/bubble.png");
	texIds.push_back("assets/item/damage.png");
	texIds.push_back("assets/bomb/waterBalloon.png");
	texIds.push_back("assets/bomb/default.png");

	TEXTURE_MGR.Load(texIds);

	WaterBalloon* waterBalloon = new WaterBalloon();
	waterBalloon->Reset();
	AddGameObject(waterBalloon);
	waterBalloon->SetPosition({ 100, 100 });

	Item::Spawn("test1", Item::Balloon, { 0, 0 }, *this);
	Item::Spawn("test2", Item::WaterJet, { 0, 100 }, *this);

	bazzi = static_cast<Player*>(AddGameObject(new Player("Player", CharacterID::BAZZI)));
	item = static_cast<Item*>(AddGameObject(new Item("Item")));
	item->SetPlayer(bazzi);

	Scene::Init();
}

void SceneDev2::Enter()
{
	Scene::Enter();
	bazzi->SetPosition({ 100, 100 });
}

void SceneDev2::Update(float dt)
{
	Scene::Update(dt);
	//std::cout << bazzi->GetPosition().x << " " << bazzi->GetPosition().y << std::endl;
}

void SceneDev2::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
