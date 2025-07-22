#include "stdafx.h"
#include "WaterBalloon.h"
#include "Animator.h"

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
}

void WaterBalloon::Draw(sf::RenderWindow& window)
{
	window.draw(balloon);
}