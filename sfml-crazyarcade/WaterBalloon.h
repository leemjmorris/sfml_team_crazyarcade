#pragma once
#include "GameObject.h"
#include "Animator.h"
#include "WaterSplash.h"
#include "HitBox.h"
#include <unordered_set>

class Player;
class WaterBalloon : public GameObject
{
protected:
	Player* player;

	sf::Sprite balloon;
	Animator animator;

	float countdownTime = 2.f;
	float currentTime = 0;
	bool isCounting = false;
	bool active = true;
	int splashLength = 1;

	sf::Vector2f balloonHitBoxSize = { 50.f, 50.f }; // LSY:
	sf::Vector2f balloonHitBoxOffset = { 0.f, 25.f }; // LSY: 

	static const int GRID_SIZE = 52;
	static inline int GridKey(int gx, int gy) { return gy * 100 + gx; }
	static std::unordered_set<int> occupiedTiles;
	sf::Vector2i gridPos;

public:
	HitBox hitBox;

	const sf::Sprite& getSf() const { return balloon; }
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
	void TargetPlayer(Player* p) { player = p; }

	static sf::Vector2f GetSnappedGridCenter(const sf::Vector2f& worldPos);
	static WaterBalloon* Spawn(const std::string& name, sf::Vector2f spawnPos, int splashLen, Player* p);

	bool GetActive() const { return active; } // LSY:
	sf::FloatRect GetGlobalBounds() const { return hitBox.rect.getGlobalBounds(); } // LSY:
};