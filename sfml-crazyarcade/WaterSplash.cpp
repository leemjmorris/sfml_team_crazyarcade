#include "stdafx.h"
#include "WaterSplash.h"
#include "WaterSplashPool.h"
#include "Item.h"

WaterSplash::WaterSplash(const std::string& name)
	: GameObject(name)
{
}

void WaterSplash::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	waterSplash.setPosition(pos);
}

void WaterSplash::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	waterSplash.setRotation(rot);
}

void WaterSplash::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	waterSplash.setScale(s);
}

void WaterSplash::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	waterSplash.setOrigin(o);
}

void WaterSplash::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(waterSplash, preset);
	} 
}

void WaterSplash::Init()
{
	waterSplash.setTexture(TEXTURE_MGR.Get("assets/bomb/default.png"));

	ANI_CLIP_MGR.Load("animation/waterSplashAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashExitAnim.csv");

	ANI_CLIP_MGR.Load("animation/waterSplashUpAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashUpExitAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashUpEndAnim.csv");

	ANI_CLIP_MGR.Load("animation/waterSplashLeftAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashLeftExitAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashLeftEndAnim.csv");

	animator.SetTarget(&waterSplash);

	sortingLayer = SortingLayers::Foreground;
	sortingOrder = -2;

	SetOrigin(Origins::MC);

	colorMask.LoadFromFile("assets/shaders/transparent.frag");
	colorMask.SetMaskColor(sf::Color(255, 0, 255));
	colorMask.SetThreshold(0.1f);
}

void WaterSplash::Release()
{

}

void WaterSplash::Reset()
{
	skillCountdown = SKILL_DURATION;
	isCounting = true;
}

void WaterSplash::Update(float dt)
{
	animator.Update(dt);

	if (isCounting)
	{
		UpdateSkillDuration(dt);
	}

	CheckCollisionWithItems();

	if (!animator.IsPlaying())
	{
		WaterSplashPool::ReturnToPool(this);
	}

	hitBox.UpdateTransform(waterSplash, waterSplash.getLocalBounds());
}

void WaterSplash::Draw(sf::RenderWindow& window)
{
	colorMask.Apply(window, waterSplash);
}

void WaterSplash::UpdateSkillDuration(float dt)
{
	skillCountdown -= dt;
	if (skillCountdown <= 0.f)
	{
		skillCountdown = 0.f;
		PlayExitAnim();
		isCounting = false;
	}
}

void WaterSplash::PlayAnim()
{
	// KHI: Center
	if (animType == AnimType::Center)
	{
		animator.Play("animation/waterSplashAnim.csv");
		SetOrigin(Origins::MC);
		SetScale({ 1, 1 });
	}
	// KHI: Dir
	else if (animType == AnimType::Up)
	{
		animator.Play("animation/waterSplashUpAnim.csv");
		SetOrigin(Origins::MC);
		SetScale({ 1, 1 });
	}
	else if (animType == AnimType::Down)
	{
		animator.Play("animation/waterSplashUpAnim.csv");
		SetOrigin(Origins::MC);
		SetScale({ 1, -1 });
	}
	else if (animType == AnimType::Left)
	{
		animator.Play("animation/waterSplashLeftAnim.csv");
		SetOrigin(Origins::MC);
		SetScale({ 1, 1 });
	}
	else if (animType == AnimType::Right)
	{
		animator.Play("animation/waterSplashLeftAnim.csv");
		SetOrigin(Origins::MC);
		SetScale({ -1, 1 });
	}
	// KHI: End
	else if (animType == AnimType::UpEnd)
	{
		animator.Play("animation/waterSplashUpEndAnim.csv");
		SetOrigin(Origins::MC);
		SetScale({ 1, 1 });
	}
	else if (animType == AnimType::DownEnd)
	{
		animator.Play("animation/waterSplashUpEndAnim.csv");
		SetOrigin(Origins::MC);
		SetScale({ 1, -1 });
	}
	else if (animType == AnimType::LeftEnd)
	{
		animator.Play("animation/waterSplashLeftEndAnim.csv");
		SetOrigin(Origins::MC);
		SetScale({ 1, 1 });
	}
	else if (animType == AnimType::RightEnd)
	{
		animator.Play("animation/waterSplashLeftEndAnim.csv");
		SetOrigin(Origins::MC);
		SetScale({ -1, 1 });
	}
}

void WaterSplash::PlayExitAnim()
{
	if (animType == AnimType::Center)
	{
		animator.Play("animation/waterSplashExitAnim.csv");
		SetOrigin(Origins::MC);
		SetScale({ 1, 1 });
	}
	else if (animType == AnimType::Up || animType == AnimType::UpEnd)
	{
		animator.Play("animation/waterSplashUpExitAnim.csv");
		SetOrigin(Origins::MC);
		SetScale({ 1, 1 });
	}
	else if (animType == AnimType::Down || animType == AnimType::DownEnd)
	{
		animator.Play("animation/waterSplashUpExitAnim.csv");
		SetOrigin(Origins::MC);
		SetScale({ 1, -1 });
	}
	else if (animType == AnimType::Left || animType == AnimType::LeftEnd)
	{
		animator.Play("animation/waterSplashLeftExitAnim.csv");
		SetOrigin(Origins::MC);
		SetScale({ 1, 1 });
	}
	else if (animType == AnimType::Right || animType == AnimType::RightEnd)
	{
		animator.Play("animation/waterSplashLeftExitAnim.csv");
		SetOrigin(Origins::MC);
		SetScale({ -1, 1 });
	}
}

void WaterSplash::CheckCollisionWithItems()
{
	if (Item::allItems.empty())
		return;

	for (Item* item : Item::allItems)
	{
		if (item == nullptr) continue;

		if (Utils::CheckCollision(hitBox.rect, item->GetHitBox().rect))
		{
			item->SetActive(false);
		}
	}

	Item::CheckAndRemoveItem();
}