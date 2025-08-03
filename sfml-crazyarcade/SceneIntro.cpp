#include "stdafx.h"
#include "SceneIntro.h"

SceneIntro::SceneIntro()
    : Scene(SceneIds::Intro)
{
}

void SceneIntro::Init()
{
    // LMJ: Load texture IDs for first intro
    LoadFirstIntroFrames();

    // LMJ: Load texture IDs for second intro
    LoadSecondIntroFrames();

    // LMJ: Add sound for second intro
    soundIds.push_back(secondIntroSoundId);

    Scene::Init();
}

void SceneIntro::Enter()
{
    Scene::Enter();

    // LMJ: Initialize intro state
    currentPhase = IntroPhase::FirstIntro;
    firstIntroCurrentFrame = 0;
    secondIntroCurrentFrame = 0;
    firstIntroTimer = 0.0f;
    secondIntroTimer = 0.0f;
    transitionTimer = 0.0f;
    isTransitioning = false;

    // LMJ: Setup view
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    uiView.setSize(windowSize);
    uiView.setCenter(windowSize * 0.5f);
    uiView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

    // LMJ: Setup first intro sprites with error checking
    std::cout << "Setting up First Intro sprites..." << std::endl;
    for (size_t i = 0; i < firstIntroTextures.size(); ++i)
    {
        if (TEXTURE_MGR.Exists(firstIntroTextures[i]))
        {
            firstIntroFrames[i].setTexture(TEXTURE_MGR.Get(firstIntroTextures[i]));
            SetupSprite(firstIntroFrames[i]);
            std::cout << "Loaded: " << firstIntroTextures[i] << std::endl;
        }
        else
        {
            std::cout << "ERROR: Failed to load " << firstIntroTextures[i] << std::endl;
        }
    }

    // LMJ: Setup second intro sprites with error checking
    std::cout << "Setting up Second Intro sprites..." << std::endl;
    for (size_t i = 0; i < secondIntroTextures.size(); ++i)
    {
        if (TEXTURE_MGR.Exists(secondIntroTextures[i]))
        {
            secondIntroFrames[i].setTexture(TEXTURE_MGR.Get(secondIntroTextures[i]));
            SetupSprite(secondIntroFrames[i]);
            if (i < 5) // LMJ: Only print first 5 to avoid spam
            {
                std::cout << "Loaded: " << secondIntroTextures[i] << std::endl;
            }
        }
        else
        {
            std::cout << "ERROR: Failed to load " << secondIntroTextures[i] << std::endl;
        }
    }

    std::cout << "===================" << std::endl;
    std::cout << "    Scene Intro" << std::endl;
    std::cout << "First Intro: " << firstIntroFrames.size() << " frames" << std::endl;
    std::cout << "Second Intro: " << secondIntroFrames.size() << " frames" << std::endl;
    std::cout << "===================" << std::endl;
}

void SceneIntro::Update(float dt)
{
    HandleInput();

    switch (currentPhase)
    {
    case IntroPhase::FirstIntro:
        UpdateFirstIntro(dt);
        break;
    case IntroPhase::SecondIntro:
        UpdateSecondIntro(dt);
        break;
    case IntroPhase::Finished:
        SCENE_MGR.ChangeScene(SceneIds::Title); // LMJ: Go to title scene
        break;
    }

    Scene::Update(dt);
}

void SceneIntro::Draw(sf::RenderWindow& window)
{
    window.setView(uiView);

    switch (currentPhase)
    {
    case IntroPhase::FirstIntro:
        if (firstIntroCurrentFrame >= 0 && firstIntroCurrentFrame < firstIntroFrames.size())
        {
            window.draw(firstIntroFrames[firstIntroCurrentFrame]);
        }
        else
        {
            // LMJ: Clamp to valid range
            int clampedFrame = Utils::Clamp(firstIntroCurrentFrame, 0, (int)firstIntroFrames.size() - 1);
            if (!firstIntroFrames.empty())
            {
                window.draw(firstIntroFrames[clampedFrame]);
            }
        }
        break;
    case IntroPhase::SecondIntro:
        if (secondIntroCurrentFrame >= 0 && secondIntroCurrentFrame < secondIntroFrames.size())
        {
            window.draw(secondIntroFrames[secondIntroCurrentFrame]);
        }
        else
        {
            // LMJ: Clamp to valid range
            int clampedFrame = Utils::Clamp(secondIntroCurrentFrame, 0, (int)secondIntroFrames.size() - 1);
            if (!secondIntroFrames.empty())
            {
                window.draw(secondIntroFrames[clampedFrame]);
            }
        }
        break;
    }

    Scene::Draw(window);
}

void SceneIntro::Exit()
{
    // LMJ: Stop any playing sounds
    SOUND_MGR.StopAllSfx();
    SOUND_MGR.StopBgm();

    Scene::Exit();
}

void SceneIntro::UpdateFirstIntro(float dt)
{
    if (firstIntroFrames.empty())
    {
        std::cout << "First intro frames empty, moving to second intro" << std::endl;
        StartSecondIntro();
        return;
    }

    firstIntroTimer += dt;

    if (firstIntroTimer >= firstIntroFrameTime)
    {
        firstIntroTimer = 0.0f;
        firstIntroCurrentFrame++;

        std::cout << "First Intro Frame: " << firstIntroCurrentFrame << "/" << firstIntroFrames.size() << std::endl;

        // LMJ: Check if first intro animation is complete
        if (firstIntroCurrentFrame >= firstIntroFrames.size())
        {
            std::cout << "First intro complete, immediately starting second intro" << std::endl;
            StartSecondIntro(); // LMJ: Direct transition without delay
            return;
        }
    }
}

void SceneIntro::UpdateSecondIntro(float dt)
{
    if (secondIntroFrames.empty())
    {
        std::cout << "ERROR: Second intro frames are empty!" << std::endl;
        FinishIntro();
        return;
    }

    secondIntroTimer += dt;

    if (secondIntroTimer >= secondIntroFrameTime)
    {
        secondIntroTimer = 0.0f;
        secondIntroCurrentFrame++;

        std::cout << "Second Intro Frame: " << secondIntroCurrentFrame << "/" << secondIntroFrames.size() << std::endl;

        // LMJ: Check if second intro animation is complete
        if (secondIntroCurrentFrame >= secondIntroFrames.size())
        {
            std::cout << "Second Intro animation complete" << std::endl;
            FinishIntro();
        }
    }
}

void SceneIntro::StartSecondIntro()
{
    currentPhase = IntroPhase::SecondIntro;
    secondIntroCurrentFrame = 0;
    secondIntroTimer = 0.0f;
    isTransitioning = false;

    std::cout << "=== Starting Second Intro ===" << std::endl;
    std::cout << "Second intro frames available: " << secondIntroFrames.size() << std::endl;
    std::cout << "Current frame index: " << secondIntroCurrentFrame << std::endl;

    // LMJ: Test if first frame is valid
    if (!secondIntroFrames.empty() && secondIntroFrames[0].getTexture() != nullptr)
    {
        std::cout << "First second intro frame texture is valid" << std::endl;
        sf::Vector2u size = secondIntroFrames[0].getTexture()->getSize();
        std::cout << "Texture size: " << size.x << "x" << size.y << std::endl;
    }
    else
    {
        std::cout << "ERROR: First second intro frame texture is invalid!" << std::endl;
    }

    // LMJ: Play intro sound
    if (SOUNDBUFFER_MGR.Exists(secondIntroSoundId))
    {
        SOUND_MGR.PlaySfx(secondIntroSoundId, false);
        std::cout << "Playing intro sound: " << secondIntroSoundId << std::endl;
    }
    else
    {
        std::cout << "WARNING: Intro sound not found: " << secondIntroSoundId << std::endl;
    }
}

void SceneIntro::FinishIntro()
{
    currentPhase = IntroPhase::Finished;
    std::cout << "Intro finished, transitioning to next scene" << std::endl;
}

void SceneIntro::HandleInput()
{
    if (!canSkip) return;

    // LMJ: Skip intro with Enter, Space, or mouse click
    if (InputMgr::GetKeyDown(sf::Keyboard::Enter) ||
        InputMgr::GetKeyDown(sf::Keyboard::Space) ||
        InputMgr::GetMouseButtonDown(sf::Mouse::Left))
    {
        std::cout << "Intro skipped by user input" << std::endl;
        FinishIntro();
    }
}

void SceneIntro::LoadFirstIntroFrames()
{
    // LMJ: Generate first intro texture paths (1.bmp ~ 20.bmp)
    firstIntroTextures.clear();
    for (int i = 1; i <= 20; ++i)
    {
        std::string path = "Intro/First Intro/" + std::to_string(i) + ".bmp";
        firstIntroTextures.push_back(path);
    }

    // LMJ: Add textures to loading list
    for (const auto& texture : firstIntroTextures)
    {
        texIds.push_back(texture);
    }

    // LMJ: Initialize sprites (will be set up in Enter())
    firstIntroFrames.resize(firstIntroTextures.size());
}

void SceneIntro::LoadSecondIntroFrames()
{
    // LMJ: Generate second intro texture paths (11.bmp ~ 83.bmp)
    secondIntroTextures.clear();
    for (int i = 11; i <= 83; ++i)
    {
        std::string path = "Intro/Second Intro/" + std::to_string(i) + ".bmp";
        secondIntroTextures.push_back(path);
    }

    // LMJ: Add textures to loading list
    for (const auto& texture : secondIntroTextures)
    {
        texIds.push_back(texture);
    }

    // LMJ: Initialize sprites (will be set up in Enter())
    secondIntroFrames.resize(secondIntroTextures.size());
}

void SceneIntro::SetupSprite(sf::Sprite& sprite)
{
    // LMJ: Scale sprite to fit full screen
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    sf::Vector2u textureSize = sprite.getTexture()->getSize();

    float scaleX = windowSize.x / static_cast<float>(textureSize.x);
    float scaleY = windowSize.y / static_cast<float>(textureSize.y);

    sprite.setScale(scaleX, scaleY);
    sprite.setPosition(0.f, 0.f);
    Utils::SetOrigin(sprite, Origins::TL);
}