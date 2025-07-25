#include "stdafx.h"
#include "SceneReady.h"

SceneReady::SceneReady()
	: Scene(SceneIds::Ready)
{
}

void SceneReady::Init()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
	worldView.setSize(windowSize);
	worldView.setCenter(windowSize * 0.5f);

	texIds.push_back("assets/lobby_scene_bg.bmp");

	TEXTURE_MGR.Load(texIds);

	Scene::Init();
}

void SceneReady::Enter()
{
	sf::Texture& bgTexture = TEXTURE_MGR.Get("assets/lobby_scene_bg.bmp");
	bgSprite.setTexture(bgTexture);

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
	sf::Vector2u textureSize = bgTexture.getSize();
	sf::Vector2f textureSizeF(static_cast<float>(textureSize.x), static_cast<float>(textureSize.y));

	sf::Vector2f scale;
	scale.x = windowSize.x / textureSizeF.x;
	scale.y = windowSize.y / textureSizeF.y;

	bgSprite.setScale(scale);

	Scene::Enter();
}

void SceneReady::Update(float dt)
{
	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		SCENE_MGR.ChangeScene(SceneIds::Demo);
	}

	Scene::Update(dt);
}

void SceneReady::Draw(sf::RenderWindow& window)
{
	window.draw(bgSprite);
}
