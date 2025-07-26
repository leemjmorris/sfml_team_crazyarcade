#pragma once
#include "GameObject.h"
#include "Animator.h"
#include "HitBox.h"
#include "CharacterStats.h"
#include "MapCollisionBuilder.h"
enum class AnimState { Normal, Trapped, Dead, live, Win };

class Player :
	public GameObject
{
protected:
	AnimState animState;


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

	sf::Vector2f playerHitBoxSize = { 40.f, 40.f }; // KHI
	sf::Vector2f playerHitBoxOffset = { 0.f, 10.f }; // KHI: 
	HitBox hitBox;

	std::vector<TileHitBox> mapData;

public:
	// LSY: for std::cout in ItemClass 
	float curSpeed;
	int curWaterBalloonCount;
	int curWaterBalloonLength;
	int	maxBalloonCount;
	int	maxBalloonLength;

	Player(const std::string& name, CharacterID id, int index);
	~Player();
	void PlayerEvent(float dt);
	bool CheckInstallBomb();
	bool CheckBubblePop(AnimState s);
	void MoveAnim(float dt);
	void AddSpeed(float s =1);
	void AddWaterBalloonCount(int c =1);
	void AddWaterBalloonLength(int l =1);
	void SetGameOver();
	void SetBalloonCount() {
		curWaterBalloonCount--;
		if (curWaterBalloonCount < 0)
			curWaterBalloonCount = 0;
	};
	float GetSpeed() { return curSpeed; };
	const int GetWaterBalloonCount() { return curWaterBalloonCount; };
	const int GetWaterBalloonLength() { return curWaterBalloonLength; };
	AnimState GetPlayerState() { return animState; };
	void SetMapData(const std::vector<TileHitBox>& data) { mapData = data; }

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
