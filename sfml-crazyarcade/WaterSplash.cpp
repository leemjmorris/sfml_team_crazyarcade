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
	//waterSplash.setTexture(Tex)
}

void WaterSplash::Release()
{

}

void WaterSplash::Reset()
{
}

void WaterSplash::Update(float dt)
{
}

void WaterSplash::Draw(sf::RenderWindow& window)
{
}