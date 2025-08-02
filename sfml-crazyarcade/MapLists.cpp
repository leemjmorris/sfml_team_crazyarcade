#include "stdafx.h"
#include "MapLists.h"
#include <windows.h>

MapLists::MapLists(const std::string& name, bool b)
	:UiHud(name, b)
{
}

void MapLists::Init()
{
	// KHI: Load JSON files located in the map folder
	std::map<std::string, std::string> mapList = LoadMapList("map");

	// KHI: Results output for TESTING
	std::cout << "======== ¸Ê ¸ñ·Ï ========" << std::endl;
	for (const auto& pair : mapList) 
	{
		std::cout << pair.first << ": " << pair.second << std::endl;
	}
	std::cout << "ÃÑ " << mapList.size() << "°³ ·Îµå" << std::endl;
}

void MapLists::Release()
{
}

void MapLists::Reset()
{
	TEXTURE_MGR.Load("assets/SelectMapImg.png");
	background.setTexture(TEXTURE_MGR.Get("assets/SelectMapImg.png"));

	// KHI: Center align map settings background image
	Utils::SetOrigin(background, Origins::MC);
	sf::View currentView = FRAMEWORK.GetWindow().getView();
	sf::Vector2f viewCenter = currentView.getCenter();
	background.setPosition(viewCenter);
}

void MapLists::Update(float dt)
{
}

void MapLists::Draw(sf::RenderWindow& window)
{
	window.draw(background);
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
				std::string fullPath = folderPath + "/" + fileName;

				mapList[nameOnly] = fullPath;
			}
		} while (FindNextFileA(hFind, &findData));

		FindClose(hFind);
	}

	return mapList;
}