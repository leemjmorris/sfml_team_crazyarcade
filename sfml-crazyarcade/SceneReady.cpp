#include "stdafx.h"
#include "SceneReady.h"

SceneReady::SceneReady()
	: Scene(SceneIds::Ready)
{
}

void SceneReady::Init()
{
	Scene::Init();
}

void SceneReady::Enter()
{
	Scene::Enter();
}

void SceneReady::Update(float dt)
{
	Scene::Update(dt);
}

void SceneReady::Draw(sf::RenderWindow& window)
{

}
