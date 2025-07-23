#include "stdafx.h"
#include "SceneDev1.h"
#include "Item.h"
#include "WaterBalloon.h"
#include "WaterSplash.h"

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
	texIds.push_back("assets/bomb/waterBalloon.png");
	texIds.push_back("assets/bomb/default.png");

	// KHI: wave
	texIds.push_back("assets/bomb/pop.bmp");
	texIds.push_back("assets/wave/up2.bmp");
	texIds.push_back("assets/wave/left2.bmp");

	TEXTURE_MGR.Load(texIds);

	WaterSplash* testObj = new WaterSplash();
	testObj->Reset();
	AddGameObject(testObj);
	testObj->SetPosition({ 100, 100 });

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