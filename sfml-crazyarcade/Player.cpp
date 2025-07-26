#include "stdafx.h"
#include "Player.h"
#include "WaterBalloon.h"

Player::Player(const std::string& name, CharacterID id, int index)
	: GameObject(name),
	charId(id),
	curSpeed(100.f),
	activeWaterBalloonCount(1),
	activeWaterBalloonLength(1),
	velocity({ 1.f, 1.f }),
	dir({ 1.f, 1.f }),
	playerIndex(index),
	isStart(false),
	isDead(false),
	isAlive(true),
	isTrapped(false),
	dieTimer(0.f),
	readyTimer(0.f),
	activeBalloons(0),
	animState(AnimState::Normal),
	hAxis(Axis::Horizontal_1p),
	vAxis(Axis::Vertical_1p),
	installWaterBomb(sf::Keyboard::Unknown),
	hitBox(playerHitBoxSize, playerHitBoxOffset),
	obj(nullptr)
{
	const auto& stats = CharacterTable.at(charId);

	curSpeed = stats.intiPlayerSpeed;
	maxBalloonCount = stats.maxBombCount;
	maxBalloonLength = stats.maxbombLength;
	balloonCapacity = stats.initBombCount;
}

Player::~Player()
{
}

void Player::PlayerEvent(float dt)
{
	if (InputMgr::GetKeyDown(installWaterBomb))
	{
		CheckInstallWaterballoon();
	}
	//if (isAlive)
	//{
	//	isTrapped = false;
	//	animState = AnimState::Live;
	//	animator.Play("animation/bazzi_live.csv");
	//}
}

void Player::OnBalloonExploded()
{
	if (activeBalloons > 0) --activeBalloons;
}

bool Player::CanPlaceBalloon() const
{
	return activeBalloons < balloonCapacity;
}

bool Player::CheckInstallWaterballoon()
{
	if (!CanPlaceBalloon()) {
		std::cout << "all waterballoon is installed" << std::endl;
		return false;
	}

	WaterBalloon::Spawn("bomb", GetPosition(),
		GetWaterBalloonLength(), this);

	++activeBalloons;                  // 필드에 살아있는 풍선 +1
	return true;
}

bool Player::HandleBubbleDeath(AnimState s)
{
	animState = s;
	animator.Play("animation/bazzi_die.csv");
	return true;
}

void Player::Move(float dt)
{
	if (animState == AnimState::Live && animState != AnimState::Ready)
	{
		PlayMoveAnimation();
		dir = InputMgr::GetPriorityDirection(hAxis, vAxis, playerIndex);
		std::cout << dir.x << ", " << dir.y << std::endl;
		position = GetPosition() + dir * curSpeed * dt;
		SetPosition(position);
		SetScale({ dir.x < 0 ? -1.f : dir.x > 0 ? 1.f : sprite.getScale().x, 1.f });
	}
}

void Player::AddSpeed(float s)
{
	curSpeed += s;
}

void Player::AddWaterBalloonCount(int b)
{
	balloonCapacity = Utils::Clamp(balloonCapacity + b, 1, maxBalloonCount); 
}

void Player::AddWaterBalloonLength(int l)
{
	activeWaterBalloonLength += l;
}

//====================================GAME OVER==========================================
void Player::SetGameOver()
{
	curSpeed = 0.f;
	animState = AnimState::Win;
	animator.Play("animation/bazzi_win.csv");
}

void Player::Init()
{
	std::cout << "[Init balloonCount]" << activeWaterBalloonCount << ", [Init balloonLength]" << activeWaterBalloonLength << ", [Init Speed]" << curSpeed << std::endl;
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
	sortingOrder = 1;
	curSpeed = CharacterTable.at(charId).intiPlayerSpeed;
	balloonCapacity = CharacterTable.at(charId).initBombCount;
	activeWaterBalloonCount = 1;
	activeWaterBalloonLength = 1;
	animator.Play("animation/bazzi_run.csv");
}

void Player::Update(float dt)
{
	hitBox.UpdateTransform(sprite, sprite.getLocalBounds());
	SetOrigin(Origins::BC);
	if (isStart)
	{
		readyTimer += dt;
		dir = { 0.f, 0.f }; // LSY: stop moving when ready
		//std::cout << readyTimer << std::endl;
		if (readyTimer > 1.0f) // LSY: 1.f is the time to wait for the player to enter the game
		{
			animState = AnimState::Live;
			readyTimer = 0.0f;
			isStart = false;
		}
	}
	else
	{
		Move(dt);
	}
	animator.Update(dt);

	PlayerEvent(dt);
	CheckCollWithSplash(); // KHI
	if (animState == AnimState::Trapped && !isDead)
	{
		dieTimer += dt;
		std::cout << "TrappedTimer: " << dieTimer << std::endl;
		if (dieTimer > 5.f)
		{
			animState = AnimState::Dead;
			dieTimer = 0.f;
			animator.Play("animation/bazzi_die.csv");
			std::cout << "TrappedTimer is finished: AnimeState::Dead" << std::endl;
			isDead = true;
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
				animator.PlayQueue("animation/bazzi_trap.csv");
				animator.Play("animation/bazzi_trap2.csv",true);

				break;
			}
		}
	}
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