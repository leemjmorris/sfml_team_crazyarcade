#pragma once
#include "GameObject.h"
#include "Animator.h"
#include "HitBox.h"
#include "CharacterStats.h"
enum class AnimState { Normal, Trapped, Dead, live };

class Player :
	public GameObject
{
protected:
	AnimState animState;

	float curSpeed;
	int curWaterBalloonCount;
	int curWaterBalloonLength;
	int playerIndex;

	bool isTrapped;
	bool isDead;
	bool isAlive;

	CharacterID charId;
	GameObject* obj;
	sf::Vector2f dir;
	sf::Vector2f velocity;
	Animator animator;
	sf::Sprite sprite;

	Axis hAxis;
	Axis vAxis;
	sf::Keyboard::Key installWaterBomb;

	float dieTimer;
	float aliveTimer;
	bool isShowing;
	bool checkDieTimer=false;
	HitBox hitBox;

public:
	Player(const std::string& name, CharacterID id, int index);
	~Player();
	void PlayerEvent(float dt);
	bool CheckInstallBomb();
	bool CheckBubblePop();
	void MoveAnim(float dt);
	void AddSpeed(float s =1);
	void AddBombCount(int c =1);
	void AddBombLength(int l =1);

	float GetSpeed() { return curSpeed; };
	const int GetBombCount() { return curWaterBalloonCount; };
	const int GetBombLength() { return curWaterBalloonLength; };
	void SetPlayerTrapped(bool t) { isTrapped = t; };
	void SetPlayerDead(bool t) { isDead = t; };

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

	void CheckCollWithSplash(); // KHI

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
