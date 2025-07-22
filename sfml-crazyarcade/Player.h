#pragma once
#include "GameObject.h"
#include "Animator.h"

class Player :
	public GameObject
{
protected:
	float currentSpeed;
	int currentBombCount;
	int currentBombLenght;

	sf::Vector2f velocity;
	Animator animator;
	sf::Sprite sprite;

public:
	Player(const std::string& name);
	~Player();

	bool CheckIntallBomb(bool t);
	bool CheckBubblePop();

	void Animating(float dt);
	void AddSpeed(float currentSpeed=1);
	void AddBombCount(int currentBombCount=1);
	void AddBombLength(int currentBombLenght=1);

	float GetSpeed() { return currentSpeed; };
	int GetBombCount() { return currentBombCount; };
	int GetBombLength() { return currentBombLenght; };

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
};

