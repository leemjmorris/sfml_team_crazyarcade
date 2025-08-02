#pragma once
#include "Scene.h"

class SceneTitle : public Scene
{
protected:
    sf::Sprite titleSprite;
    std::string titleTextureId = "Intro/Title/Back_Login.bmp";
    std::string titleSoundId = "Sounds/Title.mp3";

    bool titleMusicStarted = false;

public:
    SceneTitle();
    ~SceneTitle() override = default;

    void Init() override;
    void Enter() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;
    void Exit() override;

private:
    // LMJ: Input handling
    void HandleInput();

    // LMJ: Utility methods
    void SetupTitleSprite();
};