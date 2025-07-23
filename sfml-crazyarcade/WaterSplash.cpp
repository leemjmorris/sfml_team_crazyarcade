#include "stdafx.h"
#include "WaterSplash.h"

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

	ANI_CLIP_MGR.Load("animation/waterSplashUpAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashLeftAnim.csv");

	ANI_CLIP_MGR.Load("animation/waterSplashUpExitAnim.csv");
	ANI_CLIP_MGR.Load("animation/waterSplashLeftExitAnim.csv");

	animator.SetTarget(&waterSplash);

	sortingLayer = SortingLayers::Foreground;
	sortingOrder = -2;

	SetAnimType(AnimType::Left);
	PlayAnim();
}

void WaterSplash::Release()
{

}

void WaterSplash::Reset()
{
}

void WaterSplash::Update(float dt)
{
	animator.Update(dt);

	if (isCounting)
	{
		skillDuration -= dt;
		if (skillDuration <= 0)
		{
			skillDuration = 0;
			PlayExitAnim();
			isCounting = false; 
		}
	}

	if (!animator.IsPlaying())
	{
		SetActive(false);
		// KHI: Return to the object HERE **
	}
}

void WaterSplash::Draw(sf::RenderWindow& window)
{
	window.draw(waterSplash);
}

void WaterSplash::PlayAnim()
{
	if (animType == AnimType::Center)
	{
		animator.Play("animation/waterSplashAnim.csv");
		SetScale({ 1, 1 });
	}
	else if (animType == AnimType::Up)
	{
		animator.Play("animation/waterSplashUpAnim.csv");
		SetScale({ 1, 1 });
	}
	else if (animType == AnimType::Down)
	{
		animator.Play("animation/waterSplashUpAnim.csv");
		SetScale({ 1, -1 });
	}
	else if (animType == AnimType::Left)
	{
		animator.Play("animation/waterSplashLeftAnim.csv");
		SetScale({ 1, 1 });
	}
	else if (animType == AnimType::Right)
	{
		animator.Play("animation/waterSplashLeftAnim.csv");
		SetScale({ -1, 1 });
	}
}

void WaterSplash::PlayExitAnim()
{
	if (animType == AnimType::Center)
	{
		animator.Play("animation/waterSplashAnim.csv");
		SetScale({ 1, 1 });
	}
	else if (animType == AnimType::Up)
	{
		animator.Play("animation/waterSplashUpExitAnim.csv");
		SetScale({ 1, 1 });
	}
	else if (animType == AnimType::Down)
	{
		animator.Play("animation/waterSplashUpExitAnim.csv");
		SetScale({ 1, -1 });
	}
	else if (animType == AnimType::Left)
	{
		animator.Play("animation/waterSplashLeftExitAnim.csv");
		SetScale({ 1, 1 });
	}
	else if (animType == AnimType::Right)
	{
		animator.Play("animation/waterSplashLeftExitAnim.csv");
		SetScale({ -1, 1 });
	}
}