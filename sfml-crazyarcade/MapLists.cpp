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

	// KHI: Set default map
	if (!mapList.empty())
	{
		SceneMgr::SelectedMapPath = mapList.begin()->second;
		tempPath = mapList.begin()->second;
	}
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
	sf::Vector2u windowSize = FRAMEWORK.GetWindowSize();
	background.setPosition(windowSize.x * 0.5f, windowSize.y * 0.5f);

    CreateButtons();
	CreateActionButtons();

	SetRandomMap();

	tempPath = SceneMgr::SelectedMapPath;
	std::string selectedMapName = tempPath.substr(0, tempPath.find_last_of('.'));
	SetMapNameText(selectedMapName);

	for (auto* btn : buttons)
	{
		btn->Reset();
		if (btn->GetName() == "Btn_" + selectedMapName)
		{
			btn->SetClicked(true);
		}
	}
}

void MapLists::Update(float dt)
{
	for (auto* btn : buttons)
	{
		btn->Update(dt);
	}
	confirmBtn->Update(dt);
	cancelBtn->Update(dt);
}

void MapLists::Draw(sf::RenderWindow& window)
{
	window.draw(background);

	for (auto* btn : buttons)
	{
		btn->Draw(window);
	}

	confirmBtn->Draw(window);
	cancelBtn->Draw(window);

	window.draw(mapNameText);
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

void MapLists::SetRandomMap()
{
	int random = Utils::RandomRange(0, mapList.size());

	auto it = std::next(mapList.begin(), Utils::RandomRange(0, mapList.size()));
	SceneMgr::SelectedMapPath = it->second;
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

		btn->SetButton(listBtnTex, { btnPos.x, btnPos.y, 0.f, 0.f }, listBtnTexH, listBtnTexC);
		btn->SetText(pair.first, 13);
		btn->SetOnClick([this, btn, name = pair.first, path = pair.second] {

			for (auto* otherBtn : buttons)
			{
				otherBtn->Reset();
			}

			btn->SetClicked(true);

			tempPath = path;
			SetMapNameText(name);
			});

		btn->Init();
		btn->Reset();

		buttons.push_back(btn);
		idx++;
	}
}

void MapLists::CreateActionButtons()
{
	const float startX = 291;
	const float startY = 501.f;

	confirmBtn = new Button("Btn");
	confirmBtn->SetButton(confirmBtnTex, { startX, startY, 0.f, 0.f }, confirmBtnTexH);
	confirmBtn->Init();
	confirmBtn->Reset();
	confirmBtn->SetOnClick([this] {
		std::cout << "confirm" << std::endl;
		SceneMgr::SelectedMapPath = tempPath;
		});

	cancelBtn = new Button("Btn");
	cancelBtn->SetButton(cancelBtnTex, { startX + 116, startY, 0.f, 0.f }, cancelBtnTexH);
	cancelBtn->Init();
	cancelBtn->Reset();
	cancelBtn->SetOnClick([] {
		std::cout << "cancel" << std::endl;
		});
}

void MapLists::SetMapNameText(std::string str)
{
	mapNameText.setFont(FONT_MGR.Get("assets/font/Daum_Regular.ttf"));
	mapNameText.setString(str);
	mapNameText.setCharacterSize(13);
	mapNameText.setFillColor(sf::Color::White);

	sf::FloatRect bounds = mapNameText.getLocalBounds();
	mapNameText.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);

	mapNameText.setPosition(230.f, 126.5f);
}