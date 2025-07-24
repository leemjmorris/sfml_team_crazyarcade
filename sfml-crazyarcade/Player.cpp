#include "stdafx.h"
#include "Player.h"
#include "WaterBalloon.h"
#include <cmath>

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

	if (InputMgr::GetKeyDown(sf::Keyboard::Q))
	{
		animState = AnimState::Dead;
		animator.SetSpeed(0.8);
		animator.Play("animation/bazzi_die.csv");
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Z))
	{
		animState = AnimState::Dying;
		animator.Play("animation/bazzi_trap.csv");
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::R))
	{
		animState = AnimState::live;
		animator.Play("animation/bazzi_live.csv");
	}
}

bool Player::CheckInstallBomb()
{
	const auto& stats = CharacterTable.at(charId);
	if (curWaterBalloonCount > stats.maxBombCount) // LSY: add maxBombCount in struct 'CharactorStats'
	{
		std::cout << "max bomb" << std::endl;
		return false;
	}
	else
	{
		WaterBalloon::Spawn("bomb", GetPosition());
		curWaterBalloonCount++;
		std::cout << "bomb count: " << curWaterBalloonCount << std::endl;
		return true;
	}
}

bool Player::CheckBubblePop()
{
	return true;
}

void Player::Animating(float dt)
{
	if (dir.x != 0 && animator.GetCurrentClipId() != "Run")
	{
		animator.Play("animation/bazzi_run.csv");
		animState = AnimState::live;
	}

	else if ( dir.y < 0 && animator.GetCurrentClipId() != "Up")
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

	if (dir. x < 0)
	{
		SetScale({ -1.f,1.f });
	}
	if (dir.x > 0)
	{
		SetScale({ 1.f,1.f });
	}
}

void Player::AddSpeed(float s)
{
	curSpeed += s;
}

void Player::AddBombCount(int b)
{
	curWaterBalloonCount += b;
}

void Player::AddBombLength(int l)
{
	curWaterBalloonLength += l;
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
	Animating(dt);
	animator.Update(dt);
	SetOrigin(Origins::BC);
	dir=InputMgr::GetPriorityDirection(hAxis, vAxis, playerIndex);

	position = GetPosition() + dir * curSpeed * dt;
	SetPosition(position);
	PlayerEvent(dt);

	hitBox.UpdateTransform(sprite, sprite.getLocalBounds());

	CheckCollWithSplash(); // KHI
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Player::CheckCollWithSplash()
{
	if (animState == AnimState::Dying)
		return;

	auto waterSplashes = SCENE_MGR.GetCurrentScene()->FindGameObjects("WaterSplash");
	for (auto* obj : waterSplashes)
	{
		WaterSplash* splashObj = dynamic_cast<WaterSplash*>(obj);

		if (splashObj && splashObj->GetActive())
		{
			if (Utils::CheckCollision(splashObj->GetHitBox().rect, this->GetHitBox().rect))
			{
				animState = AnimState::Dying;
				animator.Play("animation/bazzi_trap.csv");
				break;
			}
		}
	}
}