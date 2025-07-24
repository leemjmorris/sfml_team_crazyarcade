#pragma once

class ColorMaskShader
{
private:
    sf::Shader shader;
    sf::Glsl::Vec4 maskColor;
    float threshold;

public:
    ColorMaskShader();
    bool LoadFromFile(const std::string& fragmentShaderPath);
    void SetMaskColor(const sf::Color& color);
    void SetThreshold(float value);

    void Apply(sf::RenderTarget& target, const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
};