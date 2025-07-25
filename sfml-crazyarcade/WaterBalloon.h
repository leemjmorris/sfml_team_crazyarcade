#pragma once
#include "GameObject.h"
#include "Animator.h"
#include "WaterSplash.h"

class WaterBalloon : public GameObject
{
protected:
	sf::Sprite balloon;

	Animator animator;

	float countdownTime = 1.5f;
	float currentTime = 0;
	bool isCounting = false;
	int splashLength = 1;

	static const int GRID_SIZE = 52;

public:
	WaterBalloon(const std::string& name = "");
	virtual ~WaterBalloon() = default;

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

	void StartCastCountdown();
	void Explode();
	void ExplodeInAllDirections(int upLen, int downLen, int leftLen, int rightLen);
	void SpawnWaterSplash(WaterSplash::AnimType dir, int length = 1);
	void SetSplashLen(int len) { splashLength = len; }

	static sf::Vector2f GetSnappedGridCenter(const sf::Vector2f& worldPos);
	static void Spawn(const std::string& name, sf::Vector2f spawnPos, int splashLen);
};