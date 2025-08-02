#pragma once
#include "GameObject.h"
#include "Animator.h"
#include "HitBox.h"
#include "CharacterStats.h"
#include "WaterBalloon.h"
constexpr float slidePixelsPerSecond = 80.f;
constexpr float tileSize = 52.f;

enum class AnimState { Ready, Normal, Trapped, Dead, Live, Win, Draw };

class Player :
	public GameObject
{
protected:
	std::unordered_set<WaterBalloon*> passThroughBombs;

	float curSpeed;
	int balloonCapacity;
	int activeBalloons;
	int activeWaterBalloonLength;
	int	maxBalloonCount;
	int	maxBalloonLength;

	std::string rName;

	AnimState animState;
	int playerIndex;

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
	float winTimer;
	float readyTimer;
	bool gameOverStarted = false;
	bool atBalloon = false;
	bool isStart = false;
	bool isAnotherDead = false;
	bool isDead;
	bool isDraw;
	bool isAnotherEscapeFailed = false;
	bool isPop = false;
	bool slidePlayer = false;
	sf::Vector2f playerHitBoxSize = { 30.f, 30.f }; // KHI
	sf::Vector2f playerHitBoxOffset = { 0.f, 10.f }; // KHI
	sf::Vector2f playerHitBoxOffset2 = { 0.f, -10.f }; // KHI
	HitBox hitBox;

	float pushCount = 0.4f;
	float pushedCount = 0.f;

public:
	Player(const std::string& name, CharacterID id, int index, const std::string& resultName);
	~Player();
	int GetPlayerNo() const { return playerIndex; }
	const std::string convertAniStr(const std::string & s, const std::string& ss);
	const bool GetOver() const { return gameOverStarted; }
	const std::string& GetPlayerName() const { return rName; }
	void PlayerEvent(float dt);
	void OnBalloonExploded();
	void RefreshPassThroughSet();
	bool CanPlaceBalloon() const;
	bool CheckInstallWaterballoon();
	bool CollectObstacleRects(std::vector<sf::FloatRect>& outRects);
	size_t GetCollidedObstacleInfo(sf::FloatRect& outBounds);
	bool HandleBubbleDeath(AnimState s);
	void PlayMoveAnimation();
	void AddSpeed(float s = 1);
	void AddWaterBalloonCount(int c = 1);
	void AddWaterBalloonLength(int l = 1);
	void SetPlayerState(AnimState s);
	void SetGameOver(bool t, bool l, float dt);
	void SetEnter(bool t);

	float GetSpeed() { return curSpeed; };
	const int GetWaterBalloonCount() { return activeBalloons; };
	const int GetWaterBalloonLength() { return activeWaterBalloonLength; };
	AnimState GetPlayerState() { return animState; };

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;

	void CheckCollWithSplash(); // KHI
	void Movement(float dt); // KHI
	size_t GetCollidedTileInfo(sf::FloatRect& outTileBounds);
	Block* GetCollidedBlock();
	//bool GetCollidedTileInfo(sf::FloatRect& outTileBounds); // KHI

	// KHI
	void SetSpriteColor(const sf::Color& color)
	{
		sprite.setColor(color);
	}

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