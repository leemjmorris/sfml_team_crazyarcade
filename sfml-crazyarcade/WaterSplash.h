#pragma once
#include "GameObject.h"
#include "Animator.h"

class WaterSplash : public GameObject
{
public:
	enum class AnimType
	{
		Center, // KHI: 0

		Up,		// KHI: 1
		Down,   // KHI: 2
		Left,	// KHI: 3
		Right,  // KHI: 4

		UpEnd,  // KHI: 5
		DownEnd,
		LeftEnd,
		RightEnd,

		UpExit, // KHI: 9
		DownExit,
		LeftExit,
		RightExit
	};

protected:
	sf::Sprite waterSplash;
	AnimType animType = AnimType::Center;

	float skillDuration = 0.2f;
	bool isCounting = true;

	Animator animator;

public:
	WaterSplash(const std::string& name = "");
	virtual ~WaterSplash() = default;

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

	void SetAnimType(AnimType type) { animType = type; }
	void PlayAnim();
	void PlayExitAnim();
};