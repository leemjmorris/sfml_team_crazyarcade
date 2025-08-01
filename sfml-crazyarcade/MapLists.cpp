#include "stdafx.h"
#include "MapLists.h"

MapLists::MapLists(const std::string& name, bool b)
	:UiHud(name, b)
{
}

void MapLists::Init()
{
	sf::Texture& tex = TEXTURE_MGR.Get("assets/SelectMapImg.png");
	background.setTexture(tex);

	// KHI: Center align map settings background image
	Utils::SetOrigin(background, Origins::MC);
	sf::Vector2f center = FRAMEWORK.GetWindowSizeF() * 0.5f;
	background.setPosition(center);

	// KHI: 
	mapList = {
	{ "Example Map1", "mapId"},
	{ "Example Map2", "mapId"},
	{ "Example Map3", "mapId"},
	};
}

void MapLists::Release()
{
}

void MapLists::Reset()
{
}

void MapLists::Update(float dt)
{
}

void MapLists::Draw(sf::RenderWindow& window)
{
	window.draw(background);
}