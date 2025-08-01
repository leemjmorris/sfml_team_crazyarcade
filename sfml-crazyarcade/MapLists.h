#pragma once
#include "UiHud.h"

// KHI
class MapLists : public UiHud
{
protected:
	sf::Sprite background;

	std::map<std::string, std::string> mapList;

public:
	MapLists(const std::string& name = "MapList", bool b = true);
	~MapLists() = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};