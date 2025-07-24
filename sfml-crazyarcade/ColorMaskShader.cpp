#include "stdafx.h"
#include "ColorMaskShader.h"

ColorMaskShader::ColorMaskShader()
    : maskColor(1.f, 0.f, 1.f, 1.f), threshold(0.1f)
{
}

bool ColorMaskShader::LoadFromFile(const std::string& fragmentShaderPath)
{
    bool loaded = shader.loadFromFile(fragmentShaderPath, sf::Shader::Fragment);
    if (loaded)
    {
        shader.setUniform("maskColor", maskColor);
        shader.setUniform("threshold", threshold);
    }
    return loaded;
}

void ColorMaskShader::SetMaskColor(const sf::Color& color)
{
    maskColor = sf::Glsl::Vec4(color);
    shader.setUniform("maskColor", maskColor);
}

void ColorMaskShader::SetThreshold(float value)
{
    threshold = value;
    shader.setUniform("threshold", threshold);
}

void ColorMaskShader::Apply(sf::RenderTarget& target, const sf::Drawable& drawable, const sf::RenderStates& states)
{
    sf::RenderStates newStates = states;
    newStates.shader = &shader;
    shader.setUniform("texture", sf::Shader::CurrentTexture);
    target.draw(drawable, newStates);
}