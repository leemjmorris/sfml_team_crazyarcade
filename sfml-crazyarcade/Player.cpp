#include "stdafx.h"
#include "Player.h"
#include "WaterBalloon.h"

Player::Player(const std::string& name, CharacterID id, int index)
	: GameObject(name),
	curSpeed(100.f),
	curWaterBalloonCount(1),
	curWaterBalloonLength(1),
	velocity({ 1.f,1.f }),
	dir({ 1.f,1.f }),
	playerIndex(index),
	isShowing(true),
	dieTimer(0.f),
	aliveTimer(0.f),
	animState(AnimState::Normal)
{
	const auto& stats = CharacterTable.at(charId);
	curWaterBalloonCount = stats.initBombCount;
	curWaterBalloonLength = stats.initbombLength;
	curSpeed = stats.intiPlayerSpeed;
	maxBalloonCount = stats.maxBombCount;
	maxBalloonLength = stats.maxbombLength;
}

Player::~Player()
{
}

void Player::PlayerEvent(float dt)
{
	if (InputMgr::GetKeyDown(installWaterBomb))
	{
		CheckInstallBomb();
	}
	//if (isAlive)
	//{
	//	isTrapped = false;
	//	animState = AnimState::live;
	//	animator.Play("animation/bazzi_live.csv");
	//}
}

bool Player::CheckInstallBomb()
{
	if (curWaterBalloonCount > maxBalloonCount) // LSY: add maxBombCount in struct 'CharactorStats'
	{
		std::cout << "max bomb" << std::endl;
		return false;
	}
	else
	{
		WaterBalloon::Spawn("bomb", GetPosition(), curWaterBalloonLength, this);
		curWaterBalloonCount--;
		std::cout << "waterBalloon count: " << curWaterBalloonCount << std::endl;
		if (curWaterBalloonCount < 0)
		{
			std::cout << "waterBalloon is finished" << std::endl;
			return true;
		}
		return true;
	}
}

bool Player::CheckBubblePop(AnimState s)
{
	animState = s;
	animator.Play("animation/bazzi_die.csv");
	return true;
}

void Player::MoveAnim(float dt)
{
	if (animState != AnimState::Trapped && animState != AnimState::Dead && animState != AnimState::Win)
	{
		if (dir.x != 0 && animator.GetCurrentClipId() != "Run")
		{
			animator.Play("animation/bazzi_run.csv");
			animState = AnimState::live;
		}

		else if (dir.y < 0 && animator.GetCurrentClipId() != "Up")
		{
			animator.Play("animation/bazzi_up.csv");
			animState = AnimState::live;
		}

		else if (dir.y > 0 && animator.GetCurrentClipId() != "Down")
		{
			animator.Play("animation/bazzi_down.csv");
			animState = AnimState::live;
		}

		else if (dir == sf::Vector2f(0.f, 0.f) &&
			(animator.GetCurrentClipId() == "Run" ||
			animator.GetCurrentClipId() == "Up" ||
			animator.GetCurrentClipId() == "Down"
			))
		{
			if (animator.GetCurrentClipId() == "Run")
			{
				animator.Play("animation/bazzi_run.csv");
				animState = AnimState::live;
			}
			if (animator.GetCurrentClipId() == "Up")
			{
				animator.Play("animation/bazzi_up.csv");
				animState = AnimState::live;
			}
			if (animator.GetCurrentClipId() == "Down")
			{
				animator.Play("animation/bazzi_down.csv");
				animState = AnimState::live;
			}
		}

		if (dir.x < 0)
		{
			SetScale({ -1.f,1.f });
		}
		if (dir.x > 0)
		{
			SetScale({ 1.f,1.f });
		}
	}
}


void Player::AddSpeed(float s)
{
	curSpeed += s;
}

void Player::AddWaterBalloonCount(int b)
{
	curWaterBalloonCount += b;
}

void Player::AddWaterBalloonLength(int l)
{
	curWaterBalloonLength += l;
}

//====================================GAME OVER==========================================
void Player::SetGameOver()
{
	curSpeed = 0.f;
	animState = AnimState::Win;
	animator.Play("animation/bazzi_win.csv");
}

void Player::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	sprite.setPosition(pos);
}

void Player::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	sprite.setRotation(rot);
}

void Player::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	sprite.setScale(s);
}

void Player::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	sprite.setOrigin(o);
}

void Player::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(sprite, preset);
	}
}

void Player::Init()
{
	std::cout << "[Init balloonCount]" << curWaterBalloonCount << ", [Init balloonLength]" << curWaterBalloonLength << ", [Init Speed]" << curSpeed << std::endl;
	SetOrigin(Origins::BC);
	animator.SetTarget(&sprite);

	switch (playerIndex)
	{
		case 0:
			vAxis = Axis::Vertical_1p;
			hAxis = Axis::Horizontal_1p;
			installWaterBomb = sf::Keyboard::LShift;
			break;
		case 1:
			vAxis = Axis::Vertical_2p;
			hAxis = Axis::Horizontal_2p;
			installWaterBomb = sf::Keyboard::RShift;
			break;
	}
}

void Player::Release()
{
}

void Player::Reset()
{
	sortingLayer = SortingLayers::Default;
	sortingOrder = 1; // LSY: waterBalloon / sortingOrder = 0
	animator.Play("animation/bazzi_run.csv");
}

void Player::Update(float dt)
{
	SetOrigin(Origins::BC);

	MoveAnim(dt);
	animator.Update(dt);
	Movement(dt);

	PlayerEvent(dt);
	hitBox.UpdateCustomTransform(sprite, playerHitBoxSize, Origins::BC, playerHitBoxOffset);

	CheckCollWithSplash();

	if (animState == AnimState::Trapped)
	{
		dieTimer += dt;
		if (dieTimer > 5.f)
		{
			animState = AnimState::Dead;
			dieTimer = 0.f;
			animator.Play("animation/bazzi_die.csv");
			std::cout << "TrappedTimer is finished: AnimeState::Dead" << std::endl;
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	hitBox.Draw(window);
}

// KHI
void Player::CheckCollWithSplash()
{
	if (animState == AnimState::Trapped)
		if (animState == AnimState::Trapped)
			return;

	auto waterSplashes = SCENE_MGR.GetCurrentScene()->FindGameObjects("WaterSplash");
	for (auto* obj : waterSplashes)
	{
		WaterSplash* splashObj = dynamic_cast<WaterSplash*>(obj);

		if (splashObj && splashObj->GetActive())
		{

			sf::FloatRect rect(splashObj->GetGlobalBounds()); // left, top, width, height

			if (rect.contains(GetPosition()))
			{
				animState = AnimState::Trapped;
				curSpeed = 5.f;
				isTrapped = true;
				animator.Play("animation/bazzi_trap.csv");
				break;
			}
		}
	}
}

// KHI
bool Player::CheckCollisionWithMap()
{
	sf::FloatRect nextBounds = hitBox.rect.getGlobalBounds();

	for (const auto& tile : mapData)
	{
		if (tile.bounds.intersects(nextBounds))
		{
			return true;
		}
	}

	return false;
}

// KHI
void Player::Movement(float dt)
{
	dir = InputMgr::GetPriorityDirection(hAxis, vAxis, playerIndex);

	sf::Vector2f currentPos = GetPosition();
	sf::Vector2f tempPos = currentPos;

	sf::Vector2f tryX = currentPos + sf::Vector2f(dir.x * curSpeed * dt, 0.f);
	sprite.setPosition(tryX);
	hitBox.UpdateCustomTransform(sprite, playerHitBoxSize, playerHitBoxOffset, Origins::BC);
	if (!CheckCollisionWithMap())
	{
		tempPos.x = tryX.x;
	}

	sf::Vector2f tryY = currentPos + sf::Vector2f(0.f, dir.y * curSpeed * dt);
	sprite.setPosition(sf::Vector2f(tempPos.x, tryY.y));
	hitBox.UpdateCustomTransform(sprite, playerHitBoxSize, playerHitBoxOffset, Origins::BC);
	if (!CheckCollisionWithMap())
	{
		tempPos.y = tryY.y;
	}

	SetPosition(tempPos);
}