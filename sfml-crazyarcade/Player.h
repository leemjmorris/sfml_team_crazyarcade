#pragma once
#include "GameObject.h"
#include "Animator.h"
#include "HitBox.h"
#include "CharacterStats.h"

class Player :
	public GameObject
{
protected:
	float curSpeed;
	int curWaterBalloonCount;
	int curWaterBalloonLength;
	int playerIndex;

	float lastAxis;

	CharacterID charId;
	GameObject* obj;
	sf::Vector2f dir;
	sf::Vector2f velocity;
	Animator animator;
	sf::Sprite sprite;

	Axis hAxis;
	Axis vAxis;
	sf::Keyboard::Key installWaterBomb;

	HitBox hitBox;

public:
	Player(const std::string& name, CharacterID id, int index);
	~Player();

	bool CheckInstallBomb();
	bool CheckBubblePop();
	void Animating(float dt);
	void AddSpeed(float s =1);
	void AddBombCount(int c =1);
	void AddBombLength(int l =1);

	float GetSpeed() { return curSpeed; };
	int GetBombCount() { return curWaterBalloonCount; };
	int GetBombLength() { return curWaterBalloonLength; };

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

	sf::FloatRect GetGlobalBounds() const
	{
		return sprite.getGlobalBounds();
	}

	sf::FloatRect GetLocalBounds() const
	{
		return sprite.getLocalBounds();
	}

	const HitBox& GetHitBox() const
	{
		return hitBox;
	}
};

