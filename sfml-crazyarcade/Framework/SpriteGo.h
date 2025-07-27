#pragma once
#include "GameObject.h"
#include "ColorMaskShader.h"

class SpriteGo : public GameObject
{
protected:
	sf::Sprite sprite;
	std::string textureId;
	bool isBmp = true;

	ColorMaskShader colorMask;

public:
	SpriteGo(const std::string& texPlayerId = "", const std::string& name = "");
	~SpriteGo() override = default;

	const std::string& GetTextureId() const { return textureId; }
	void SetTextureId(const std::string& texPlayerId) { textureId = texPlayerId; }

	sf::Sprite& GetSprite() { return sprite; }
	const sf::Sprite& GetSprite() const { return sprite; }

	void SetIsBmp(bool bmp) { isBmp = bmp; }

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float rot) override;
	void SetScale(const sf::Vector2f& s) override;
	void SetOrigin(const sf::Vector2f& o) override;
	void SetOrigin(Origins preset) override;

	// LMJ: "Added GetLocalBounds and GetGlobalBounds overrides"
	sf::FloatRect GetLocalBounds() const override
	{
		return sprite.getLocalBounds();
	}

	sf::FloatRect GetGlobalBounds() const override
	{
		return sprite.getGlobalBounds();
	}

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};