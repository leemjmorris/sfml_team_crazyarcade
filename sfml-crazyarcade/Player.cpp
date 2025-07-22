#include "stdafx.h"
#include "Player.h"

Player::Player(const std::string& name)
	: GameObject(name),
	currentSpeed(0.f),
	currentBombCount(0),
	currentBombLenght(0),
	velocity({ 1.f,1.f })
{
}

Player::~Player()
{
}

bool Player::CheckIntallBomb(bool t)
{
	return false;
}

bool Player::CheckBubblePop()
{
	return false;
}

void Player::Animating(float dt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::S))// LSY: todo:InputMgr::GetKeyDown(sf::Keyboard::Up))
	{
		
		if (animator.GetCurrentClipId() != "Run")
		{
			std::cout << "LFT" << std::endl;
			//std::cout << "LFT" << std::endl;
			if (dt != 0.f)
			{
				animator.Play("assets/animations/bazzi_run.csv");
				std::cout << "LFT" << std::endl;
			}
			
		}
	}
	/*	else if (animator.GetCurrentClipId() == "default")
		{
			if (dt == 0.f)
			{
				animator.Play("animations/idle.csv");
			}
		}
		else if (animator.GetCurrentClipId() == "Bubble")
		{
			if (dt != 0.f)
			{
				animator.Play("animations/idle.csv");
			}
		}*/
}

void Player::AddSpeed(float s)
{
	currentSpeed += s;
}

void Player::AddBombCount(int b)
{
	currentBombCount += b;
}

void Player::AddBombLength(int l)
{
	currentBombLenght += l;
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
}

void Player::Update(float dt)
{
	animator.Update(dt);
	Animating(dt);
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}