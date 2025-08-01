#pragma once
#include <SFML/Graphics.hpp>

class OutlineShader
{
private:
    sf::Shader shader;
    sf::Glsl::Vec4 outlineColor;
    float thickness;
    sf::Glsl::Vec2 texelSize; 

public:
    OutlineShader();

    bool LoadFromFile(const std::string& fragmentShaderPath);

    void SetOutlineColor(const sf::Color& color);
    void SetThickness(float valuePx);
    void SetTextureSize(const sf::Vector2u& texSize);

    void Apply(sf::RenderTarget& target,
        const sf::Drawable& drawable,
        const sf::RenderStates& states = sf::RenderStates::Default);
};
