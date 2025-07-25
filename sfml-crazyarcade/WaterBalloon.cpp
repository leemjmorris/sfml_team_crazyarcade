#include "stdafx.h"
#include "WaterBalloon.h"
#include "Animator.h"
#include "WaterSplash.h"
#include "WaterSplashPool.h"
#include "Player.h"

WaterBalloon::WaterBalloon(const std::string& name)
	: GameObject(name)
{
}

void WaterBalloon::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	balloon.setPosition(pos);
}

void WaterBalloon::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	balloon.setRotation(rot);
}

void WaterBalloon::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	balloon.setScale(s);
}

void WaterBalloon::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	balloon.setOrigin(o);
}

void WaterBalloon::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(balloon, preset);
	}
}

void WaterBalloon::Init()
{
	balloon.setTexture(TEXTURE_MGR.Get("assets/bomb/default.png"));

	ANI_CLIP_MGR.Load("animation/waterBalloonAnim.csv");

	animator.SetTarget(&balloon);
	animator.Play("animation/waterBalloonAnim.csv");

	SetOrigin(Origins::MC);

	sortingLayer = SortingLayers::Foreground;
	sortingOrder = -1;
}

void WaterBalloon::Release()
{
}

void WaterBalloon::Reset()
{
}

void WaterBalloon::Update(float dt)
{
	animator.Update(dt);

		if (isCounting)
		{
			currentTime -= dt;
			if (currentTime <= 0)
			{
				currentTime = 0;
				isCounting = false;
				//targetPlayer->AddWaterBalloonCount(); // LSY: Todo
				Explode();
			}
		}
}

void WaterBalloon::Draw(sf::RenderWindow& window)
{
	window.draw(balloon);
}

void WaterBalloon::StartCastCountdown()
{
	currentTime = countdownTime;
	isCounting = true;
}
	
void WaterBalloon::Explode()
{
	ExplodeInAllDirections(splashLength, splashLength, splashLength, splashLength);
	Scene* currentScene = SCENE_MGR.GetCurrentScene();
	currentScene->RemoveGameObject(this);
	player->SetBalloonCount();
}

void WaterBalloon::ExplodeInAllDirections(int upLen, int downLen, int leftLen, int rightLen)
{
	if (upLen > 10)
		upLen = 10;
	if (downLen > 10)
		downLen = 10;
	if (leftLen > 10)
		leftLen = 10;
	if (rightLen > 10)
		rightLen = 10;

	SpawnWaterSplash(WaterSplash::AnimType::Center);
	SpawnWaterSplash(WaterSplash::AnimType::Up, upLen);
	SpawnWaterSplash(WaterSplash::AnimType::Down, downLen);
	SpawnWaterSplash(WaterSplash::AnimType::Left, leftLen);
	SpawnWaterSplash(WaterSplash::AnimType::Right, rightLen);
}

void WaterBalloon::SpawnWaterSplash(WaterSplash::AnimType dir, int length)
{
	sf::Vector2f centerPos = GetPosition();
	float texSize = 52.f;

	std::unordered_map<WaterSplash::AnimType, sf::Vector2f> dirs {
	{ WaterSplash::AnimType::Up, sf::Vector2f(0.f, -1.f) },
	{ WaterSplash::AnimType::Down, sf::Vector2f(0.f, 1.f) },
	{ WaterSplash::AnimType::Left, sf::Vector2f(-1.f, 0.f) },
	{ WaterSplash::AnimType::Right, sf::Vector2f(1.f, 0.f) }
	};

	if (dir == WaterSplash::AnimType::Center)
	{
		WaterSplash* splashObj = WaterSplashPool::GetFromPool();
		splashObj->SetAnimType(WaterSplash::AnimType::Center);
		splashObj->SetPosition(GetPosition());
		splashObj->Reset();
		splashObj->PlayAnim();
	}
	else
	{
		for (int i = 1; i <= length; i++)
		{
			WaterSplash* splashObj = WaterSplashPool::GetFromPool();
			sf::Vector2f pos = centerPos + (dirs[dir] * (texSize * i));

			if (i == length)
			{
				splashObj->SetAnimType(WaterSplash::AnimType((int)dir + 4));
			}
			else
			{
				splashObj->SetAnimType(dir);
			}

			splashObj->SetPosition(pos);
			splashObj->Reset();
			splashObj->PlayAnim();
		}
	}
}

// KHI: Static method
sf::Vector2f WaterBalloon::GetSnappedGridCenter(const sf::Vector2f& worldPos)
{
	int gridX = static_cast<int>(worldPos.x / GRID_SIZE);
	int gridY = static_cast<int>(worldPos.y / GRID_SIZE);

	float centerX = gridX * GRID_SIZE + GRID_SIZE / 2.0f;
	float centerY = gridY * GRID_SIZE + GRID_SIZE / 2.0f;

	return { centerX, centerY };
}

// KHI: Static method
void WaterBalloon::Spawn(const std::string& name, sf::Vector2f spawnPos, int splashLen, Player* p)
{
	WaterBalloon* waterBalloon = new WaterBalloon(name);
	waterBalloon->Init();
	waterBalloon->StartCastCountdown();
	waterBalloon->SetSplashLen(splashLen);
	waterBalloon->SetPosition(GetSnappedGridCenter(spawnPos));
	waterBalloon->TargetPlayer(p);

	Scene* currentScene = SCENE_MGR.GetCurrentScene();
	currentScene->AddGameObject(waterBalloon);
}