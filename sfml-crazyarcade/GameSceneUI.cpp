#include "stdafx.h"
#include "GameSceneUI.h"

GameSceneUI::GameSceneUI(const std::string& name)
    :UiHud(name)
{
}

void GameSceneUI::Init()
{
    exitButton = new Button("exitButton");
    exitButton->SetActive(false);
    exitButton->SetOnClick([=] {
        exitButton->SetActive(!exitButton->GetActive());
        if (exitButton->GetActive()) 
            exitButton->SetActive(false);
            SCENE_MGR.ChangeScene(SceneIds::Ready);
        });

    exitButton->SetButton("assets/ui/game/exitbutton.png", { 647.f, 561.f, 141.f, 32.f });

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
    exitButton->Reset();

    draftSprite.setTexture(TEXTURE_MGR.Get("assets/play_ui.png"));

    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    sf::Vector2u texSize = draftSprite.getTexture()->getSize();
    draftSprite.setScale(windowSize.x / texSize.x, windowSize.y / texSize.y);
    std::cout << texSize.x << ", " << texSize.y << std::endl;
    draftSprite.setPosition({ 0, 0 });
}

void GameSceneUI::Update(float dt)
{
    auto& win = FRAMEWORK.GetWindow();
    sf::View old = win.getView();
    win.setView(win.getDefaultView());

    exitButton->Update(dt);

    win.setView(old);
}

void GameSceneUI::Draw(sf::RenderWindow& window)
{
    //window.setView(window.getDefaultView());
    //colorMask.Apply(window, draftSprite);

    exitButton->Draw(window);
}