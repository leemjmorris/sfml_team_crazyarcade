#include "stdafx.h"
#include "SceneDemo.h"
#include "CharacterStats.h"
#include "Item.h"
#include "Player.h"
#include "WaterBalloon.h"

SceneDemo::SceneDemo()
	: Scene(SceneIds::Demo)
{
}

void SceneDemo::Init()
{
	texIds.push_back("assets/player/bazzi/right.png");
	texIds.push_back("assets/player/bazzi/up.png");
	texIds.push_back("assets/player/bazzi/down.png");
	texIds.push_back("assets/player/bazzi/die.png");
	texIds.push_back("assets/player/bazzi/trap.png");
	texIds.push_back("assets/player/bazzi/live.png");
	ANI_CLIP_MGR.Load("animation/bazzi_run.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_up.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_down.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_live.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_trap.csv");
	ANI_CLIP_MGR.Load("animation/bazzi_die.csv");

	texIds.push_back("assets/item/bubble.png");
	texIds.push_back("assets/item/damage.png");
	texIds.push_back("assets/bomb/waterBalloon.png");
	texIds.push_back("assets/bomb/default.png");

	TEXTURE_MGR.Load(texIds);

	WaterBalloon* waterBalloon = new WaterBalloon();
	waterBalloon->Reset();
	AddGameObject(waterBalloon);
	waterBalloon->SetPosition({ 100, 100 });

	//Item::Spawn("test1", Item::Balloon, { 0, 0 }, *this);
	//Item::Spawn("test2", Item::WaterJet, { 0, 100 }, *this);

	bazzi = static_cast<Player*>(AddGameObject(new Player("Bazzi", CharacterID::BAZZI, 0)));
	Dao = static_cast<Player*>(AddGameObject(new Player("Dao", CharacterID::DAO, 1)));
	item = static_cast<Item*>(AddGameObject(new Item("Item")));
	item->SetPlayer(bazzi);

	objectsNeedingClamp.push_back(bazzi);
	objectsNeedingClamp.push_back(Dao);
	Block::CreateBlock; (BlockType::HardBlock, (100, 100));
	Scene::Init();
}

void SceneDemo::Enter()
{
	std::cout << "===================" << std::endl;
	std::cout << "     SceneDemo" << std::endl;
	std::cout << "===================" << std::endl;

	Scene::Enter();
}

void SceneDemo::Update(float dt)
{
	for (auto* obj : objectsNeedingClamp)
		ClampToBounds(*obj);
	//std::cout << bazzi->GetPosition().x << ", " << bazzi->GetPosition().y << std::endl;

	Scene::Update(dt);
}

void SceneDemo::Draw(sf::RenderWindow& window)
{
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