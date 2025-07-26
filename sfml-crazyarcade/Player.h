#pragma once
#include "GameObject.h"
#include "Animator.h"
#include "HitBox.h"
#include "CharacterStats.h"
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

	sf::Vector2f playerHitBoxSize = { 52.f, 52.f }; // KHI
	sf::Vector2f playerHitBoxOffset = { 7.f, 20.f }; // KHI: 
	HitBox hitBox;

public:
	// LSY: for std::cout in ItemClass
	float curSpeed;
	int balloonCapacity;
	int activeBalloons;
	int activeWaterBalloonCount;
	int activeWaterBalloonLength;
	int	maxBalloonCount;
	int	maxBalloonLength;

	Player(const std::string& name, CharacterID id, int index);
	~Player();
	void PlayerEvent(float dt);
	void OnBalloonExploded();
	bool CanPlaceBalloon() const;
	bool CheckInstallBomb();
	bool CheckBubblePop(AnimState s);
	void MoveAnim(float dt);
	void AddSpeed(float s =1);
	void AddWaterBalloonCount(int c =1);
	void AddWaterBalloonLength(int l =1);
	void GetBalloon(int a) { activeBalloons += a; };
	void SetGameOver();

	float GetSpeed() { return curSpeed; };
	const int GetWaterBalloonCount() { return activeWaterBalloonCount; };
	const int GetWaterBalloonLength() { return activeWaterBalloonLength; };
	AnimState GetPlayerState() { return animState; };

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
