#include "stdafx.h"
#include "Player.h"
#include "WaterBalloon.h"

Player::Player(const std::string & name, CharacterID id, int index)
	: GameObject(name),
	curSpeed(100.f),
	curWaterBalloonCount(1),
	curWaterBalloonLength(1),
	velocity({ 1.f,1.f }),
	dir({ 1.f,1.f }),
	playerIndex(index)
{
	const auto& stats = CharacterTable.at(charId);
	curWaterBalloonCount = stats.initBombCount;
	curWaterBalloonLength = stats.initbombLength;
	curSpeed = stats.intiPlayerSpeed;
}

Player::~Player()
{
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
	//for(obj : GameObject::GetAllObjects())
	//{
	//	if(obj.getName() == "Bazzi")
	//	{
	//		animator.SetTarget(&obj.getSprite());
	//		break;
	//	}
	//}

	if (playerIndex == 0)
		dir = InputMgr::GetPriorityDirection();

	else if (playerIndex == 1)
		dir2 = InputMgr::GetPriorityDirection();

	if (playerIndex == 0)
	{
		if (dir.x != 0 && animator.GetCurrentClipId() != "Run")
		{
			animator.Play("animation/bazzi_run.csv");
			std::cout << "LFT" << std::endl;
		}

		else if (dir.y < 0 && animator.GetCurrentClipId() != "Up")
		{
			animator.Play("animation/bazzi_up.csv");
		}

		else if (dir.y > 0 && animator.GetCurrentClipId() != "Down")
		{
			animator.Play("animation/bazzi_down.csv");
		}

		else if (dir == sf::Vector2f(0.f, 0.f) &&
			(animator.GetCurrentClipId() == "Run" || animator.GetCurrentClipId() == "Up" || animator.GetCurrentClipId() == "Down"))
		{
			if (animator.GetCurrentClipId() == "Run")
			{
				animator.Play("animation/bazzi_run.csv");
			}
			if (animator.GetCurrentClipId() == "Up")
			{
				animator.Play("animation/bazzi_up.csv");
			}
			if (animator.GetCurrentClipId() == "Down")
			{
				animator.Play("animation/bazzi_down.csv");
			}
		}
	}
	if (playerIndex == 1)
	{
		if (dir2.x != 0 && animator.GetCurrentClipId() != "Run")
		{
			animator.Play("animation/bazzi_run.csv");
			std::cout << "LFT" << std::endl;
		}

		else if (dir2.y < 0 && animator.GetCurrentClipId() != "Up")
		{
			animator.Play("animation/bazzi_up.csv");
		}

		else if (dir2.y > 0 && animator.GetCurrentClipId() != "Down")
		{
			animator.Play("animation/bazzi_down.csv");
		}

		else if (dir2 == sf::Vector2f(0.f, 0.f) &&
			(animator.GetCurrentClipId() == "Run" || animator.GetCurrentClipId() == "Up" || animator.GetCurrentClipId() == "Down"))
		{
			if (animator.GetCurrentClipId() == "Run")
			{
				animator.Play("animation/bazzi_run.csv");
			}
			if (animator.GetCurrentClipId() == "Up")
			{
				animator.Play("animation/bazzi_up.csv");
			}
			if (animator.GetCurrentClipId() == "Down")
			{
				animator.Play("animation/bazzi_down.csv");
			}
		}
	}

	if (playerIndex == 0)
	{
		if (InputMgr::GetKeyDown(sf::Keyboard::Left))
		{
			SetScale({ -1.f,1.f });
		}
		if (InputMgr::GetKeyDown(sf::Keyboard::Right))
		{
			SetScale({ 1.f,1.f });
		}
	}
	else if (playerIndex == 1)
	{
		if (InputMgr::GetKeyDown(sf::Keyboard::A))
		{
			SetScale({ -1.f,1.f });
		}
		if (InputMgr::GetKeyDown(sf::Keyboard::D))
		{
			SetScale({ 1.f,1.f });
		}
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

	//animator.AddEvent("Idle", 0,
	//	[]()
	//	{
	//		//std::cout << "!!" << std::endl;
	//	}
	//);

	animator.AddEvent("Left", 0,
		[]()
		{
			std::cout << "??" << std::endl;
		}
	);
}

void Player::Release()
{
}

void Player::Reset()
{
	sortingLayer = SortingLayers::Default;
	sortingOrder = 1; // LSY: waterBalloon / sortingOrder = 0
}

void Player::Update(float dt)
{
	Animating(dt);
	SetOrigin(Origins::BC);

	if (playerIndex == 0)
	this->position = this->GetPosition() + dir * curSpeed * dt;

	if (playerIndex == 1)
	this->position = this->GetPosition() + dir2 * curSpeed * dt;

	SetPosition(position);
	animator.Update(dt);
	if (playerIndex == 0)
	{
		if (InputMgr::GetKeyDown(sf::Keyboard::LShift))
		{
			CheckInstallBomb();
		}
	}
	else if (playerIndex == 1)
	{
		if (InputMgr::GetKeyDown(sf::Keyboard::RShift))
		{
			CheckInstallBomb();
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}
