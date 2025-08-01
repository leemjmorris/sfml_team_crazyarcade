#include "stdafx.h"
#include "GamepadInput.h"
#include <cmath>

// LMJ: Static member initialization
bool GamepadInput::prevBombState[4] = { false, false, false, false };

void GamepadInput::Update()
{
    // LMJ: Update joystick states (SFML handles this automatically)
    // This function exists for future expansion if needed
}

sf::Vector2f GamepadInput::GetDirection(int gamepadIndex)
{
    // LMJ: Check if gamepad is connected
    if (!sf::Joystick::isConnected(gamepadIndex))
        return { 0.f, 0.f };

    // LMJ: Get left analog stick values (-100 to 100)
    float x = sf::Joystick::getAxisPosition(gamepadIndex, sf::Joystick::X);
    float y = sf::Joystick::getAxisPosition(gamepadIndex, sf::Joystick::Y);

    // LMJ: Normalize to -1 to 1 range
    x /= 100.f;
    y /= 100.f;

    // LMJ: Apply deadzone
    const float deadzone = 0.8f;
    if (std::abs(x) < deadzone && std::abs(y) < deadzone)
        return { 0.f, 0.f };

    // LMJ: Convert analog to digital 4-direction
    sf::Vector2f direction(0.f, 0.f);

    if (std::abs(x) > std::abs(y))
    {
        // LMJ: Horizontal movement dominant
        direction.x = (x > 0.f) ? 1.f : -1.f;
    }
    else
    {
        // LMJ: Vertical movement dominant
        direction.y = (y > 0.f) ? 1.f : -1.f;
    }

    return direction;
}

bool GamepadInput::GetBombButtonPressed(int gamepadIndex)
{
    // LMJ: Check if gamepad is connected
    if (!sf::Joystick::isConnected(gamepadIndex))
        return false;

    // LMJ: Check button 0 (A button on Xbox controller)
    bool currentPressed = sf::Joystick::isButtonPressed(gamepadIndex, 0);
    bool wasPressed = currentPressed && !prevBombState[gamepadIndex];

    // LMJ: Update previous state
    prevBombState[gamepadIndex] = currentPressed;

    return wasPressed;
}