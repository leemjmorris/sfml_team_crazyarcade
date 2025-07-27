#include "stdafx.h"
#include "GameSceneUI.h"

GameSceneUI::GameSceneUI()
{
}

void GameSceneUI::Init()
{
    // KHI: ShaderSettings
    colorMask.LoadFromFile("assets/shaders/transparent.frag");
    colorMask.SetMaskColor(sf::Color(255, 0, 255));
    colorMask.SetThreshold(0.1f);
}

void GameSceneUI::Release()
{
}

void GameSceneUI::Reset()
{
    SetActive(true);

    draftSprite.setTexture(TEXTURE_MGR.Get("assets/play_bg.bmp"));

    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    sf::Vector2u texSize = draftSprite.getTexture()->getSize();

    draftSprite.setScale(windowSize.x / texSize.x, windowSize.y / texSize.y);

    SetOrigin(Origins::TL);
    SetPosition({ 0, 0 });

    sortingLayer = SortingLayers::UI;
}

void GameSceneUI::Update(float dt)
{

}

void GameSceneUI::Draw(sf::RenderWindow& window)
{
    colorMask.Apply(window, draftSprite);
}