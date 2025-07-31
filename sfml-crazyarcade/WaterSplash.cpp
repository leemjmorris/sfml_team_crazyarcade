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

	ANI_CLIP_MGR.Load("animation/waterSplashDownAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashDownExitAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashDownEndAnim.csv");

	ANI_CLIP_MGR.Load("animation/waterSplashLeftAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashLeftExitAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashLeftEndAnim.csv");

	ANI_CLIP_MGR.Load("animation/waterSplashRightAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashRightExitAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashRightEndAnim.csv");

	animator.SetTarget(&waterSplash);

	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

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
	auto localBounds = waterSplash.getLocalBounds();
	hitBox.UpdateCustomTransform(waterSplash, { localBounds.width, localBounds.height }, Origins::MC);

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
	}
	// KHI: Dir
	else if (animType == AnimType::Up)
	{
		animator.Play("animation/waterSplashUpAnim.csv");
		SetOrigin(Origins::MC);
	}
	else if (animType == AnimType::Down)
	{
		animator.Play("animation/waterSplashDownAnim.csv");
		SetOrigin(Origins::MC);
	}
	else if (animType == AnimType::Left)
	{
		animator.Play("animation/waterSplashLeftAnim.csv");
		SetOrigin(Origins::MC);
	}
	else if (animType == AnimType::Right)
	{
		animator.Play("animation/waterSplashRightAnim.csv");
		SetOrigin(Origins::MC);
	}
	// KHI: End
	else if (animType == AnimType::UpEnd)
	{
		animator.Play("animation/waterSplashUpEndAnim.csv");
		SetOrigin(Origins::MC);
	}
	else if (animType == AnimType::DownEnd)
	{
		animator.Play("animation/waterSplashDownEndAnim.csv");
		SetOrigin(Origins::MC);
	}
	else if (animType == AnimType::LeftEnd)
	{
		animator.Play("animation/waterSplashLeftEndAnim.csv");
		SetOrigin(Origins::MC);
	}
	else if (animType == AnimType::RightEnd)
	{
		animator.Play("animation/waterSplashRightEndAnim.csv");
		SetOrigin(Origins::MC);
	}
}

void WaterSplash::PlayExitAnim()
{
	if (animType == AnimType::Center)
	{
		animator.Play("animation/waterSplashExitAnim.csv");
		SetOrigin(Origins::MC);
	}
	else if (animType == AnimType::Up || animType == AnimType::UpEnd)
	{
		animator.Play("animation/waterSplashUpExitAnim.csv");
		SetOrigin(Origins::MC);
	}
	else if (animType == AnimType::Down || animType == AnimType::DownEnd)
	{
		animator.Play("animation/waterSplashDownExitAnim.csv");
		SetOrigin(Origins::MC);
	}
	else if (animType == AnimType::Left || animType == AnimType::LeftEnd)
	{
		animator.Play("animation/waterSplashLeftExitAnim.csv");
		SetOrigin(Origins::MC);
	}
	else if (animType == AnimType::Right || animType == AnimType::RightEnd)
	{
		animator.Play("animation/waterSplashRightExitAnim.csv");
		SetOrigin(Origins::MC);
	}
}

void WaterSplash::CheckCollisionWithItems()
{
	if (Item::allItems.empty())
		return;

	for (Item* item : Item::allItems)
	{
		if (item == nullptr) 
			continue;

		if (!item->GetCanDestroy()) 
			continue;

		sf::Vector2f itemCenter = { item->GetHitBox().rect.getGlobalBounds().left + item->GetHitBox().rect.getGlobalBounds().width * 0.5f,
									item->GetHitBox().rect.getGlobalBounds().top + item->GetHitBox().rect.getGlobalBounds().height * 0.5f };

		sf::FloatRect splashRect = hitBox.rect.getGlobalBounds();

		if (splashRect.contains(itemCenter))
		{
			item->SetActive(false);
		}
	}

	Item::CheckAndRemoveItem();
}

bool WaterSplash::CheckCollisionWithBlocks()
{
	hitBox.UpdateCustomTransform(waterSplash, { 42.f, 42.f }, Origins::MC, { 0.f, 0.f });

	Scene* curScene = SCENE_MGR.GetCurrentScene();
	auto gameObjects = curScene->FindGameObjects("Block");

	for (auto* obj : gameObjects)
	{
		Block* block = dynamic_cast<Block*>(obj);
		if (block && block->IsDestroyable())
		{
			sf::FloatRect blockRect = block->GetGlobalBounds();

			if (IsCompletelyInside(hitBox.rect.getGlobalBounds(), blockRect))
			{
				block->PlayExitAnim(); // KHI: Destroy the object after the block destruction animation ends
				return true;
			}
		}
	}

	return false;
}

bool WaterSplash::CheckCollisionWithWindow()
{
	sf::FloatRect windowBounds = FRAMEWORK.GetWindowBounds();
	sf::FloatRect splashBounds = GetHitBox().rect.getGlobalBounds();

	if (!windowBounds.intersects(splashBounds))
	{
		return true;
	}

	return false;
}

bool WaterSplash::IsCompletelyInside(const sf::FloatRect& inner, const sf::FloatRect& outer)
{
	return inner.left >= outer.left &&
		inner.top >= outer.top &&
		inner.left + inner.width <= outer.left + outer.width &&
		inner.top + inner.height <= outer.top + outer.height;
}