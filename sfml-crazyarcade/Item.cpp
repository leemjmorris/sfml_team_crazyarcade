#include "stdafx.h"
#include "Item.h"
#include "Player.h"

std::list<Item*> Item::allItems;
std::vector<Player*> Item::players;

Item::Item(const std::string& name)
	: GameObject(name)
{
}

void Item::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	itemSprite.setPosition(pos);
}

void Item::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	itemSprite.setRotation(rot);
}

void Item::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	itemSprite.setScale(s);
}

void Item::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	itemSprite.setOrigin(o);
}

void Item::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(itemSprite, preset);
	}
}

void Item::Init()
{
}

void Item::Release()
{
	
}

void Item::Reset()
{
	SetOrigin(Origins::MC);
}

void Item::Update(float dt)
{
	FloatingEffect(dt, 4.f);

	hitBox.UpdateTransform(itemSprite, itemSprite.getLocalBounds());

	CheckCollisionWithPlayers();
}

void Item::Draw(sf::RenderWindow& window)
{
	window.draw(itemSprite);
}

void Item::SetItemType(ItemType type)
{
	if (type == ItemType::Balloon)
	{
		texId = "assets/item/bubble.png";
		itemType = ItemType::Balloon;
	}
	else if (type == ItemType::Speed)
	{
		texId = "assets/item/speed.png";
		itemType = ItemType::Speed;
	}
	else if (type == ItemType::WaterJet)
	{
		texId = "assets/item/damage.png";
		itemType = ItemType::WaterJet;
	}

	itemSprite.setTexture(TEXTURE_MGR.Get(texId));
}

void Item::FloatingEffect(float dt, float verticalRange, float speed)
{
	if (moveUp)
	{
		position.y -= dt * speed;
		if (position.y <= originPos.y - verticalRange)
		{
			position.y = originPos.y - verticalRange;
			moveUp = false;
		}
	}
	else
	{
		position.y += dt * speed;
		if (position.y >= originPos.y + verticalRange)
		{
			position.y = originPos.y + verticalRange;
			moveUp = true;
		}
	}
	SetPosition(position);
}

void Item::Use(Player* player)
{
	std::cout << itemType << std::endl;
	if (itemType == ItemType::Balloon)
	{
		player->AddWaterBalloonCount(1);
	}
	else if (itemType == ItemType::Speed)
	{
		player->AddSpeed(100);
	}

	else if (itemType == ItemType::WaterJet)
	{
		player->AddWaterBalloonLength(1);
	}
	//std::cout << "[Cur balloonCount]" << player->BalloonCount << ", [Cur balloonLength]" << player->curWaterBalloonLength << ", [Cur Speed]" << player->curSpeed << std::endl;
	SetActive(false);
	// KHI: Add sound effect here **
}


void Item::CheckCollisionWithPlayers()
{
	if (players.empty())
		return;

	for (int i = 0; i < players.size(); i++)
	{
		if (Utils::CheckCollision(hitBox.rect, players[i]->GetHitBox().rect))
		{
			std::cout << "Item collided with player: " << players[i]->GetName() << std::endl;
			Use(players[i]);
		}
	}
}

// KHI: Static method
void Item::SpawnItem(const std::string& name, ItemType type, sf::Vector2f spawnPos)
{
	Item* item = new Item(name);
	item->SetItemType(type);
	item->SetPosition(spawnPos);
	item->SetOriginPos(spawnPos);
	allItems.push_back(item);

	Scene* currentScene = SCENE_MGR.GetCurrentScene();
	currentScene->AddGameObject(item);
}

// KHI: Static method
void Item::SetPlayer(Player* player)
{
	players.push_back(player);
}

// KHI: Static method
void Item::CheckAndRemoveItem()
{
	if (allItems.empty())
		return;

	Scene* currentScene = SCENE_MGR.GetCurrentScene();

	for (auto it = allItems.begin(); it != allItems.end(); )
	{
		if (*it == nullptr || !(*it)->GetActive())
		{
			currentScene->RemoveGameObject(*it);
			it = allItems.erase(it);
		}
		else
		{
			++it;
		}
	}
}