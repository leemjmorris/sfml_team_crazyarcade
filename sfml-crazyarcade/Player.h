#pragma once
#include "GameObject.h"
#include "Animator.h"
#include "HitBox.h"
#include "CharacterStats.h"
#include "MapCollisionBuilder.h"
#include "WaterBalloon.h"

enum class AnimState { Ready, Normal, Trapped, Dead, Live, Win};

class Player :
	public GameObject
{
protected:
	WaterBalloon* spawnBalloon;

	float curSpeed;
	int balloonCapacity;
	int activeBalloons;
	int activeWaterBalloonLength;
	int	maxBalloonCount;
	int	maxBalloonLength;

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
	bool isAnotherDead =false;
	bool isDead;
	bool isDraw;
	bool isAnotherEscapeFailed =false;
	bool isPop = false;

	sf::Vector2f playerHitBoxSize = { 40.f, 40.f }; // KHI
	sf::Vector2f playerHitBoxOffset = { 0.f, 20.f }; // KHIs
	HitBox hitBox;

	std::vector<TileHitBox> mapData;

public:
	Player(const std::string& name, CharacterID id, int index);
	~Player();
	void PlayerEvent(float dt);
	void OnBalloonExploded();
	bool CanPlaceBalloon() const;
	bool CheckInstallWaterballoon();
	bool HandleBubbleDeath(AnimState s);
	void PlayMoveAnimation()
	{
		const std::string clipId = animator.GetCurrentClipId();
		if (animState == AnimState::Live)
		{
			if (dir.x != 0 && clipId != "Run")
				animator.Play("animation/bazzi_run.csv");
			else if (dir.y < 0 && clipId != "Up")
				animator.Play("animation/bazzi_up.csv");
			else if (dir.y > 0 && clipId != "Down")
				animator.Play("animation/bazzi_down.csv");
			else if (dir == sf::Vector2f(0.f, 0.f)) {
				if (clipId == "Run") animator.Play("animation/bazzi_run.csv");
				else if (clipId == "Up") animator.Play("animation/bazzi_up.csv");
				else if (clipId == "Down") animator.Play("animation/bazzi_down.csv");
			}
		}
	}
	void AddSpeed(float s =1);
	void AddWaterBalloonCount(int c =1);
	void AddWaterBalloonLength(int l =1);
	void SetGameOver(bool t, bool l, float dt);
	void SetEnter(bool t)
	{
		animator.Play("animation/bazzi_ready.csv",true);
		animator.PlayQueue("animation/bazzi_ready2.csv");
		animator.PlayQueue("animation/bazzi_ready2.csv");
		animator.PlayQueue("animation/bazzi_ready2.csv");
		animator.PlayQueue("animation/bazzi_ready2.csv");
		animator.PlayQueue("animation/bazzi_ready2.csv");
		
		animState = AnimState::Ready;
		isStart = t;
	}

	float GetSpeed() { return curSpeed; };
	const int GetWaterBalloonCount() { return activeBalloons; };
	const int GetWaterBalloonLength() { return activeWaterBalloonLength; };
	AnimState GetPlayerState() { return animState; };
	void SetMapData(const std::vector<TileHitBox>& data) { mapData = data; }
	void ClearspawnBalloonBomb(WaterBalloon* b);

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

	bool CheckCollWithBalloon(); // LSY
	void CheckCollWithSplash(); // KHI
	void Movement(float dt); // KHI
	bool GetCollidedTileInfo(sf::FloatRect& outTileBounds); // KHI

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
