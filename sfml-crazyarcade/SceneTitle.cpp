#include "stdafx.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle()
    : Scene(SceneIds::Title)
{
}

void SceneTitle::Init()
{
    // LMJ: Add title texture and sound to loading list
    texIds.push_back(titleTextureId);
    soundIds.push_back(titleSoundId);

    Scene::Init();
}

void SceneTitle::Enter()
{
    Scene::Enter();

    // LMJ: Reset state
    titleMusicStarted = false;

    // LMJ: Setup view
    sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
    uiView.setSize(windowSize);
    uiView.setCenter(windowSize * 0.5f);
    uiView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

    // LMJ: Setup title sprite
    SetupTitleSprite();

    // LMJ: Start title music with loop
    if (SOUNDBUFFER_MGR.Exists(titleSoundId))
    {
        SOUND_MGR.PlayBgm(titleSoundId, true); // LMJ: Loop = true
        titleMusicStarted = true;
        std::cout << "Playing title music: " << titleSoundId << std::endl;
    }
    else
    {
        std::cout << "WARNING: Title music not found: " << titleSoundId << std::endl;
    }

    std::cout << "===================" << std::endl;
    std::cout << "    Scene Title" << std::endl;
    std::cout << "Press Enter or Space to continue..." << std::endl;
    std::cout << "===================" << std::endl;
}

void SceneTitle::Update(float dt)
{
    HandleInput();
    Scene::Update(dt);
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
    window.setView(uiView);

    // LMJ: Draw title background
    window.draw(titleSprite);

    Scene::Draw(window);
}

void SceneTitle::Exit()
{
    // LMJ: Stop title music when leaving
    SOUND_MGR.StopBgm();
    titleMusicStarted = false;

    std::cout << "Exiting Title scene..." << std::endl;

    Scene::Exit();
}

void SceneTitle::HandleInput()
{
    // LMJ: Continue to lobby/ready scene with Enter, Space, or mouse click
    if (InputMgr::GetKeyDown(sf::Keyboard::Enter) ||
        InputMgr::GetKeyDown(sf::Keyboard::Space) ||
        InputMgr::GetMouseButtonDown(sf::Mouse::Left))
    {
        std::cout << "Title scene: User input detected, moving to Ready scene" << std::endl;
        SCENE_MGR.ChangeScene(SceneIds::Ready); // LMJ: Go to lobby/ready scene
    }

    // LMJ: Exit game with Escape
    if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
    {
        std::cout << "Title scene: Exit game requested" << std::endl;
        FRAMEWORK.GetWindow().close();
    }
}

void SceneTitle::SetupTitleSprite()
{
    // LMJ: Set title texture
    if (TEXTURE_MGR.Exists(titleTextureId))
    {
        titleSprite.setTexture(TEXTURE_MGR.Get(titleTextureId));

        // LMJ: Scale sprite to fit full screen
        sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();
        sf::Vector2u textureSize = titleSprite.getTexture()->getSize();

        float scaleX = windowSize.x / static_cast<float>(textureSize.x);
        float scaleY = windowSize.y / static_cast<float>(textureSize.y);

        titleSprite.setScale(scaleX, scaleY);
        titleSprite.setPosition(0.f, 0.f);
        Utils::SetOrigin(titleSprite, Origins::TL);

        std::cout << "Title sprite loaded successfully" << std::endl;
        std::cout << "Texture size: " << textureSize.x << "x" << textureSize.y << std::endl;
        std::cout << "Window size: " << windowSize.x << "x" << windowSize.y << std::endl;
        std::cout << "Scale: " << scaleX << "x" << scaleY << std::endl;
    }
    else
    {
        std::cout << "ERROR: Failed to load title texture: " << titleTextureId << std::endl;
    }
}