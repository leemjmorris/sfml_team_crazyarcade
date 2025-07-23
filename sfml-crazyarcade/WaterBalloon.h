#pragma once
#include "GameObject.h"
#include "Animator.h"

class WaterBalloon : public GameObject
{
protected:
	sf::Sprite balloon;

	Animator animator;

	float countdownTime = 0;
	float currentTime = 0;
	bool isCounting = false;

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

	void StartCastCountdown(float time);
	void Explode();
	void SpawnWaterSplash(int length);
	
	static void Spawn(const std::string& name, sf::Vector2f spawnPos);
};