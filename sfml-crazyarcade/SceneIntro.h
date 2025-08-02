#pragma once
#include "Scene.h"

class SceneIntro : public Scene
{
public:
    enum class IntroPhase
    {
        FirstIntro,  // LMJ: Silent image animation
        SecondIntro, // LMJ: Image animation with sound
        Finished     // LMJ: Ready to transition
    };

protected:
    IntroPhase currentPhase = IntroPhase::FirstIntro;

    // LMJ: First Intro (Silent)
    std::vector<sf::Sprite> firstIntroFrames;
    std::vector<std::string> firstIntroTextures;
    int firstIntroCurrentFrame = 0;
    float firstIntroFrameTime = 0.1f;  // LMJ: Time per frame
    float firstIntroTimer = 0.0f;

    // LMJ: Second Intro (With Sound)
    std::vector<sf::Sprite> secondIntroFrames;
    std::vector<std::string> secondIntroTextures;
    int secondIntroCurrentFrame = 0;
    float secondIntroFrameTime = 0.08f; // LMJ: Time per frame
    float secondIntroTimer = 0.0f;

    // LMJ: Phase transition
    float phaseTransitionDelay = 0.5f; // LMJ: Delay between phases
    float transitionTimer = 0.0f;
    bool isTransitioning = false;

    // LMJ: Skip functionality
    bool canSkip = true;

    // LMJ: Sound
    std::string secondIntroSoundId = "sounds/logo.mp3";

public:
    SceneIntro();
    ~SceneIntro() override = default;

    void Init() override;
    void Enter() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;
    void Exit() override;

private:
    // LMJ: Animation control methods
    void UpdateFirstIntro(float dt);
    void UpdateSecondIntro(float dt);
    void StartSecondIntro();
    void FinishIntro();

    // LMJ: Input handling
    void HandleInput();

    // LMJ: Resource loading
    void LoadFirstIntroFrames();
    void LoadSecondIntroFrames();

    // LMJ: Utility methods
    void SetupSprite(sf::Sprite& sprite);
};