#include "stdafx.h"
#include "OutlineShader.h"

OutlineShader::OutlineShader()
    : outlineColor(1.f, 1.f, 1.f, 1.f)
    , thickness(1.5f)
    , texelSize(1.f, 1.f) 
{
}

bool OutlineShader::LoadFromFile(const std::string& fragmentShaderPath)
{
    if (!shader.loadFromFile(fragmentShaderPath, sf::Shader::Fragment))
        return false;

    // 초기 유니폼 세팅
    shader.setUniform("texture", sf::Shader::CurrentTexture);
    shader.setUniform("outlineColor", outlineColor);
    shader.setUniform("thickness", thickness);
    shader.setUniform("texelSize", texelSize);
    return true;
}

void OutlineShader::SetOutlineColor(const sf::Color& color)
{
    outlineColor = sf::Glsl::Vec4(color);
    shader.setUniform("outlineColor", outlineColor);
}

void OutlineShader::SetThickness(float valuePx)
{
    thickness = valuePx;
    shader.setUniform("thickness", thickness);
}

void OutlineShader::SetTextureSize(const sf::Vector2u& texSize)
{
    if (texSize.x == 0 || texSize.y == 0) return;
    texelSize = sf::Glsl::Vec2(1.f / texSize.x, 1.f / texSize.y);
    shader.setUniform("texelSize", texelSize);
}

void OutlineShader::Apply(sf::RenderTarget& target,
    const sf::Drawable& drawable,
    const sf::RenderStates& states)
{
    sf::RenderStates newStates = states;
    newStates.shader = &shader;
    target.draw(drawable, newStates);
}
