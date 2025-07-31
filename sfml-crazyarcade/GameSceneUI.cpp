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

    font.loadFromFile("assets/font/ARCADECLASSIC.TTF");
    textResult.setFont(font);
    textResult.setOrigin(textResult.getGlobalBounds().width * 0.5f, textResult.getGlobalBounds().height * 0.5f);
    textResult.setOutlineThickness(2);
    textResult.setOutlineColor(sf::Color::Black);
    textResult.setCharacterSize(100);
    textResult.setFillColor(sf::Color::White);
    textResult.setPosition(worldBounds.width * 0.5f - 170.f, 100.f);
}

void GameSceneUI::Release()
{
}

void GameSceneUI::Reset()
{
    SetActive(true);

    draftSprite.setTexture(TEXTURE_MGR.Get("assets/play_ui.png"));

    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    sf::Vector2u texSize = draftSprite.getTexture()->getSize();

    draftSprite.setScale(windowSize.x / texSize.x, windowSize.y / texSize.y);
    std::cout << texSize.x << ", " << texSize.y << std::endl;
    SetOrigin(Origins::TL);
    SetPosition({ 0, 0 });

    sortingLayer = SortingLayers::UI;
}

void GameSceneUI::Update(float dt)
{

}

void GameSceneUI::Draw(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());
    colorMask.Apply(window, draftSprite);
}