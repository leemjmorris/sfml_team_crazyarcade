#pragma once
#include <SFML/Graphics.hpp>

// LMJ: Simple gamepad input using SFML joystick support
class GamepadInput
{
public:
    // LMJ: Get 4-direction movement from gamepad
    static sf::Vector2f GetDirection(int gamepadIndex);

    // LMJ: Check if bomb button (button 0) is pressed this frame
    static bool GetBombButtonPressed(int gamepadIndex);

    // LMJ: Update internal state (call once per frame)
    static void Update();

private:
    static bool prevBombState[4]; // LMJ: Previous frame button states for press detection
};