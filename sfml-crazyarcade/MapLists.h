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
	std::string listBtnTexC = "assets/ui/mapClickedBtn.png";

	Button* confirmBtn;
	Button* cancelBtn;
	std::string confirmBtnTex = "assets/ui/confirmBtn.png";
	std::string cancelBtnTex = "assets/ui/cancelBtn.png";
	std::string confirmBtnTexH = "assets/ui/confirmBtn2.png";
	std::string cancelBtnTexH = "assets/ui/cancelBtn2.png";

	std::string tempPath;

	sf::Text mapNameText;

public:
	MapLists(const std::string& name = "MapList", bool b = true);
	~MapLists() = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	std::map<std::string, std::string> LoadMapList(const std::string& folderPath);
	void SetRandomMap();
	void CreateButtons();
	void CreateActionButtons();
	void SetMapNameText(std::string str);
};