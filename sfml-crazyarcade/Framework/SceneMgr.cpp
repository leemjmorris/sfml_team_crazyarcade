#include "stdafx.h"
#include "SceneMgr.h"
#include "MapEditor.h"
#include "SceneReady.h"
#include "SceneGame.h"
#include "SceneIntro.h"
#include "SceneTitle.h"

std::string SceneMgr::SelectedMapPath = "";

void SceneMgr::Init()
{
	scenes.push_back(new SceneIntro());
	scenes.push_back(new SceneTitle());
	scenes.push_back(new MapEditor());
	scenes.push_back(new SceneReady());
	scenes.push_back(new SceneGame());

	for (auto scene : scenes)
	{
		scene->Init();
	}

	currentScene = startScene;
	scenes[(int)currentScene]->Enter();
}

void SceneMgr::Release()
{
	for (auto scene : scenes)
	{
		if (scene->Id == currentScene)
		{
			scene->Exit();
		}
		scene->Release();
		delete scene;
	}
	scenes.clear();
}

void SceneMgr::ChangeScene(SceneIds id)
{
	nextScene = id;
}

void SceneMgr::Update(float dt)
{
	if (nextScene != SceneIds::None)
	{
		scenes[(int)currentScene]->Exit();
		currentScene = nextScene;
		nextScene = SceneIds::None;
		scenes[(int)currentScene]->Enter();
	}

	scenes[(int)currentScene]->Update(dt);
}

void SceneMgr::Draw(sf::RenderWindow& window)
{
	scenes[(int)currentScene]->Draw(window);
}
