#include "stdafx.h"
#include "Block.h"

Block::Block(const std::string& name)
    : SpriteGo("", name), blockType(BlockType::None), isDestroyable(false)
{
    sortingLayer = SortingLayers::Foreground;
    sortingOrder = 0;
}

void Block::SetBlockType(BlockType type)
{
    blockType = type;
    textureId = GetTextureId(type);

    // LMJ: Block Stat
    switch (type)
    {
    case BlockType::SoftBlock:
        isDestroyable = true;
        break;
    case BlockType::HardBlock:
        isDestroyable = false;
        break;
    case BlockType::HideBlock:
        isHidable = true;
        break;
    case BlockType::UncoverBlock:
        isHidable = false;
        break;
    case BlockType::PushBlock:
        isMovable = true;
        break;
    case BlockType::FixBlock:
        isMovable = false;
        break;
    default:
        isDestroyable = false;
        break;
    }
}

void Block::Init()
{
    SpriteGo::Init();
}

void Block::Release()
{
    SpriteGo::Release();
}

void Block::Reset()
{
    SpriteGo::Reset();
    SetOrigin(Origins::MC);
}

void Block::Update(float dt)
{
    SpriteGo::Update(dt);
}

void Block::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);
}

std::string Block::GetTextureId(BlockType type)
{
    switch (type)
    {
    case BlockType::SoftBlock:
        return "assets/block/soft_block.png";
    case BlockType::HardBlock:
        return "assets/block/hard_block.png";
    default:
        return "";
    }
}

Block* Block::CreateBlock(BlockType type, const sf::Vector2f& position)
{
    Block* block = new Block();
    block->SetBlockType(type);
    block->Reset();
    block->SetPosition(position);
    return block;
}