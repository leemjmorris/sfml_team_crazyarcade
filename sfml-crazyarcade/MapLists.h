#pragma once
#include "UiHud.h"
#include "Button.h"

// KHI
class MapLists : public UiHud
{
protected:
	sf::Sprite background;

	std::map<std::string, std::string> mapList; // KHI: <mapName - mapFilePath>
	std::vector<Button*> buttons;
	std::string listBtnTex = "assets/ui/mapListBtn.png";
	std::string listBtnTexH = "assets/ui/mapListHighlightedBtn.png";

public:
	MapLists(const std::string& name = "MapList", bool b = true);
	~MapLists() = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	std::map<std::string, std::string> LoadMapList(const std::string& folderPath);
	void CreateButtons();
};