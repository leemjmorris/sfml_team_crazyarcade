#include "stdafx.h"
#include "SceneReady.h"

SceneReady::SceneReady()
	: Scene(SceneIds::Ready)
{
}

void SceneReady::Init()
{
	texIds.push_back("assets/lobby_bg.png");

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
	worldView.setSize(windowSize);
	worldView.setCenter(windowSize * 0.5f);

	TEXTURE_MGR.Load(texIds);

	Scene::Init();
}

void SceneReady::Enter()
{
	TEXTURE_MGR.Load(texIds);
	sf::Texture& bgTexture = TEXTURE_MGR.Get("assets/lobby_bg.png");
	bgSprite.setTexture(bgTexture);

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
	worldView.setSize(windowSize);
	worldView.setCenter(windowSize * 0.5f);
	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);

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
	window.setView(worldView);
	window.draw(bgSprite);

	Scene::Draw(window);
}