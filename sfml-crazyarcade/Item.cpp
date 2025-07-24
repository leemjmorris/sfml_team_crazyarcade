#include "stdafx.h"
#include "Item.h"
#include "Player.h"

std::vector<Item*> Item::allItems;

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
		type = ItemType::Balloon;
	}
	else if (type == ItemType::Speed)
	{
		texId = "assets/item/speed.png";
		type = ItemType::Speed;
	}
	else if (type == ItemType::WaterJet)
	{
		texId = "assets/item/damage.png";
		type = ItemType::WaterJet;
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
	if (itemType == ItemType::Balloon)
	{
		// KHI: Call the method HERE to increase the number of water balloons the player can use **
		// KHI: EX) player->Example();
	}
	else if (itemType == ItemType::Speed)
	{
		// KHI: Call the method HERE to increase the player's movement speed **
	}
	else if (itemType == ItemType::WaterJet)
	{
		// KHI: Call the method HERE to increase the player's water jet stat **
	}

	SetActive(false); // KHI: Needs to be fixed later
	// KHI: Add sound effect here **
}

void Item::SetPlayer(Player* player)
{
	players.push_back(player);
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
	AddItemToVector(item);

	Scene* currentScene = SCENE_MGR.GetCurrentScene();
	currentScene->AddGameObject(item);
}

// KHI: Static method
void Item::AddItemToVector(Item* itemObj)
{
	allItems.push_back(itemObj);
	std::cout << "ÃÑ ¾ÆÀÌÅÛ ¼ö: " << allItems.size() << std::endl;
}
