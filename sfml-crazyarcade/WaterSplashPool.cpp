#include "stdafx.h"
#include "WaterSplashPool.h"

WaterSplashPool::WaterSplashPool(const std::string& name)
	: GameObject(name)
{
}

void WaterSplashPool::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	//gameObject.setPosition(pos);
}

void WaterSplashPool::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	//gameObject.setRotation(rot);
}

void WaterSplashPool::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	//gameObject.setScale(s);
}

void WaterSplashPool::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	//gameObject.setOrigin(o);
}

void WaterSplashPool::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		//Utils::SetOrigin(gameObject, preset);
	}
}

void WaterSplashPool::Init()
{
}

void WaterSplashPool::Release()
{
}

void WaterSplashPool::Reset()
{
}

void WaterSplashPool::Update(float dt)
{
}

void WaterSplashPool::Draw(sf::RenderWindow& window)
{
}