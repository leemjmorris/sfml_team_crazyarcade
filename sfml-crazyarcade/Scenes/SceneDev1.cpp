#include "stdafx.h"
#include "SceneDev1.h"
#include "Item.h"

SceneDev1::SceneDev1() 
	: Scene(SceneIds::Dev1)
	
{
}

SceneDev1::~SceneDev1()
{

}

void SceneDev1::Init()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
	worldView.setSize(windowSize);
	worldView.setCenter(windowSize * 0.5f);

	texIds.push_back("assets/item/bubble.png");
	texIds.push_back("assets/item/damage.png");

	TEXTURE_MGR.Load(texIds);

	Item::Spawn("test1", Item::Balloon, { 0, 0 }, *this);
	Item::Spawn("test2", Item::WaterJet, { 0, 100 }, *this);

	Scene::Init();
}

void SceneDev1::Enter()
{
	Scene::Enter();
}

void SceneDev1::Update(float dt)
{
	Scene::Update(dt);
}

void SceneDev1::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}