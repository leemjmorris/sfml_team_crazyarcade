#include "stdafx.h"
#include "MapLists.h"
#include "Button.h"
#include <windows.h>

MapLists::MapLists(const std::string& name, bool b)
	:UiHud(name, b)
{
}

void MapLists::Init()
{
	// KHI: Load JSON files located in the map folder
	mapList = LoadMapList("map");

	// KHI: Results output for TESTING
	std::cout << "-------------------------" << std::endl;
	for (const auto& pair : mapList)
	{
		std::cout << pair.first << ": " << pair.second << std::endl;
	}
	std::cout << "ÃÑ " << mapList.size() << "°³ ·Îµå" << std::endl;
	std::cout << "-------------------------" << std::endl;
}

void MapLists::Release()
{
	for (auto* btn : buttons) 
	{ 
		btn->Release(); 
		delete btn;
	}
	buttons.clear();
}

void MapLists::Reset()
{
    TEXTURE_MGR.Load("assets/SelectMapImg.png");
    background.setTexture(TEXTURE_MGR.Get("assets/SelectMapImg.png"));

    Utils::SetOrigin(background, Origins::MC);
    sf::View currentView = FRAMEWORK.GetWindow().getView();
    sf::Vector2f viewCenter = currentView.getCenter();
    background.setPosition(viewCenter);

    CreateButtons();
}

void MapLists::Update(float dt)
{
	for (auto* btn : buttons)
	{
		btn->Update(dt);
	}
}

void MapLists::Draw(sf::RenderWindow& window)
{
	window.draw(background);

	for (auto* btn : buttons)
	{
		btn->Draw(window);
	}
}

std::map<std::string, std::string> MapLists::LoadMapList(const std::string& folderPath)
{
	std::map<std::string, std::string> mapList;

	std::string searchPath = folderPath + "/*.json";
	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::string fileName = findData.cFileName;
				std::string nameOnly = fileName.substr(0, fileName.find_last_of('.'));

				mapList[nameOnly] = fileName;
			}
		} while (FindNextFileA(hFind, &findData));

		FindClose(hFind);
	}

	return mapList;
}

void MapLists::CreateButtons()
{
	const float startX = 335.f;
	const float startY = 180.f;
	const float gapY = 1.f;

	int idx = 0;

	for (const auto& pair : mapList)
	{
		Button* btn = new Button("Btn_" + pair.first);

		sf::Texture tex;
		tex.loadFromFile(listBtnTex);
		sf::Vector2u texSize = tex.getSize();

		sf::Vector2f btnPos;
		btnPos.x = startX;
		btnPos.y = startY + idx * (texSize.y + gapY);

		btn->SetButton(listBtnTex, { btnPos.x, btnPos.y, 0.f, 0.f }, listBtnTexH);
		btn->SetText(pair.first, 13);
		btn->SetOnClick([path = pair.second] {
			SceneMgr::SelectedMapPath = path;
			SCENE_MGR.ChangeScene(SceneIds::Demo);
			});


		btn->Init();
		btn->Reset();

		buttons.push_back(btn);
		idx++;
	}
}