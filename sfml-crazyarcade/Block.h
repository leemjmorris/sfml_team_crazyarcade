#pragma once
#include "SpriteGo.h"

enum class BlockType
{
    None = -1,
    SoftBlock,
    HardBlock,
    HideBlock,
    UncoverBlock,
    PushBlock,
    FixBlock,
    Count
};

class Block : public SpriteGo
{
protected:
    BlockType blockType;
    bool isDestroyable;
    bool isHidable;
    bool isMovable;

public:
    Block(const std::string& name = "Block");
    ~Block() override = default;

    void SetBlockType(BlockType type);
    BlockType GetBlockType() const { return blockType; }

    bool IsDestroyable() const { return isDestroyable; }
    void SetDestroyable(bool destroyable) { isDestroyable = destroyable; }

    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;

    // LMJ: Texture ID per Blocks
    static std::string GetTextureId(BlockType type);

    static Block* CreateBlock(BlockType type, const sf::Vector2f& position);
};