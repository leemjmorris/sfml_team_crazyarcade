#pragma once
#include "GameObject.h"
#include "HitBox.h"

class Player;

class Item : public GameObject
{
public:
	enum ItemType
	{
		Balloon,
		Speed,
		WaterJet,
	};

	static Item* Spawn(const std::string& name, ItemType type, sf::Vector2f spawnPos, Scene& scene);

protected:
	ItemType itemType = ItemType::Balloon;
	sf::Sprite itemSprite;
	std::string texId;

	sf::Vector2f originPos = { 0.f, 0.f };
	bool moveUp = false;

	Player* target = nullptr;

	HitBox hitBox;

public:
	Item(const std::string& name = "");
	virtual ~Item() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetItemType(ItemType type);
	void SetOriginPos(sf::Vector2f pos) { originPos = pos; }
	void FloatingEffect(float dt, float verticalRange = 5.f, float speed = 10.f);
	void Use(Player& p);
};