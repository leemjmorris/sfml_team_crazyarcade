#include "stdafx.h"
#include "Block.h"
#include "Item.h"
#include "Scene.h"

// LMJ: "Default item spawn probability (50%)"
float Block::defaultItemSpawnProbability = 0.5f;

// LMJ: "Static registry initialization"
std::vector<BlockInfo> Block::blockRegistry;
bool Block::registryInitialized = false;

Block::Block(const std::string& name)
    : SpriteGo("", name), blockType(BlockType::None),
    isDestroyable(false), isHidable(false), isMovable(false),
    canSpawnItem(true), itemSpawnProbability(defaultItemSpawnProbability)
{
    sortingLayer = SortingLayers::Foreground;
    sortingOrder = 0;

    // LMJ: "Initialize registry if not done yet"
    if (!registryInitialized)
    {
        InitializeBlockRegistry();
    }
}

void Block::SetBlockType(BlockType type)
{
    blockType = type;
    textureId = GetTextureId(type);

    // LMJ: "Set block properties based on type"
    switch (type)
    {
    case BlockType::SoftBlock:
        isDestroyable = true;
        isHidable = false;
        isMovable = false;
        canSpawnItem = true;
        break;
    case BlockType::HardBlock:
        isDestroyable = false;
        isHidable = false;
        isMovable = false;
        canSpawnItem = false;
        break;
    case BlockType::HideBlock:
        isDestroyable = false;
        isHidable = true;
        isMovable = false;
        canSpawnItem = false;
        break;
    case BlockType::UncoverBlock:
        isDestroyable = false;
        isHidable = false;
        isMovable = false;
        canSpawnItem = false;
        break;
    case BlockType::PushBlock:
        isDestroyable = false;
        isHidable = false;
        isMovable = true;
        canSpawnItem = false;
        break;
    case BlockType::FixBlock:
        isDestroyable = false;
        isHidable = false;
        isMovable = false;
        canSpawnItem = false;
        break;
    default:
        isDestroyable = false;
        isHidable = false;
        isMovable = false;
        canSpawnItem = false;
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

void Block::DestroyBlock(Scene* scene)
{
    if (!isDestroyable || !scene)
        return;

    sf::Vector2f blockPosition = GetPosition();

    // LMJ: "Check if item should spawn based on probability"
    if (canSpawnItem && ShouldSpawnItem())
    {
        // LMJ: "Randomly select item type"
        int randomItemType = Utils::RandomRange(0, 3); // LMJ: "0=Balloon, 1=Speed, 2=WaterJet"
        Item::ItemType itemType = static_cast<Item::ItemType>(randomItemType);

        // LMJ: "Spawn item at block position"
        std::string itemName = "SpawnedItem_" + std::to_string(rand());
        Item::Spawn(itemName, itemType, blockPosition, *scene);
    }

    // LMJ: "Deactivate the block (will be removed by scene)"
    SetActive(false);
}

bool Block::ShouldSpawnItem() const
{
    if (!canSpawnItem)
        return false;

    // LMJ: "Generate random number between 0.0 and 1.0"
    float randomValue = Utils::RandomValue();
    return randomValue < itemSpawnProbability;
}

void Block::InitializeBlockRegistry()
{
    if (registryInitialized) return;

    blockRegistry.clear();

    // LMJ: "Register multiple textures for SoftBlock type"
    RegisterBlock("assets/block/soft_block_1.png", BlockType::SoftBlock, "Soft Block 1");
    RegisterBlock("assets/block/soft_block_2.png", BlockType::SoftBlock, "Soft Block 2");
    RegisterBlock("assets/block/soft_block_3.png", BlockType::SoftBlock, "Soft Block 3");

    // LMJ: "Register multiple textures for HardBlock type"
    RegisterBlock("assets/block/hard_block_1.png", BlockType::HardBlock, "Hard Block 1");
    RegisterBlock("assets/block/hard_block_2.png", BlockType::HardBlock, "Hard Block 2");

    // LMJ: "Register other block types"
    RegisterBlock("assets/block/hide_block.png", BlockType::HideBlock, "Hide Block");
    RegisterBlock("assets/block/uncover_block.png", BlockType::UncoverBlock, "Uncover Block");
    RegisterBlock("assets/block/push_block.png", BlockType::PushBlock, "Push Block");
    RegisterBlock("assets/block/fix_block.png", BlockType::FixBlock, "Fix Block");

    registryInitialized = true;
}

void Block::RegisterBlock(const std::string& textureId, BlockType type, const std::string& displayName)
{
    blockRegistry.emplace_back(textureId, type, displayName);
}

std::vector<BlockInfo> Block::GetBlocksByType(BlockType type)
{
    std::vector<BlockInfo> result;
    for (const auto& blockInfo : blockRegistry)
    {
        if (blockInfo.type == type)
        {
            result.push_back(blockInfo);
        }
    }
    return result;
}

std::vector<BlockInfo> Block::GetAllBlocks()
{
    if (!registryInitialized)
    {
        InitializeBlockRegistry();
    }
    return blockRegistry;
}

BlockInfo Block::GetBlockInfo(int registryIndex)
{
    if (!registryInitialized)
    {
        InitializeBlockRegistry();
    }

    if (registryIndex >= 0 && registryIndex < static_cast<int>(blockRegistry.size()))
    {
        return blockRegistry[registryIndex];
    }

    // LMJ: "Return default BlockInfo if index is invalid"
    return BlockInfo("", BlockType::None, "Invalid");
}

int Block::GetBlockRegistrySize()
{
    if (!registryInitialized)
    {
        InitializeBlockRegistry();
    }
    return static_cast<int>(blockRegistry.size());
}

std::string Block::GetTextureId(BlockType type)
{
    // LMJ: "Legacy method - returns first texture of the type"
    auto blocks = GetBlocksByType(type);
    if (!blocks.empty())
    {
        return blocks[0].textureId;
    }
    return "";
}

Block* Block::CreateBlock(BlockType type, const sf::Vector2f& position)
{
    // LMJ: "Create block using first texture of the type"
    auto blocks = GetBlocksByType(type);
    if (blocks.empty())
        return nullptr;

    Block* block = new Block();
    block->SetBlockType(type);
    block->textureId = blocks[0].textureId; // LMJ: "Use first texture"
    block->Reset();
    block->SetPosition(position);
    return block;
}

Block* Block::CreateBlockFromRegistry(int registryIndex, const sf::Vector2f& position)
{
    BlockInfo blockInfo = GetBlockInfo(registryIndex);
    if (blockInfo.type == BlockType::None)
        return nullptr;

    Block* block = new Block();
    block->SetBlockType(blockInfo.type);
    block->textureId = blockInfo.textureId; // LMJ: "Use specific texture"
    block->Reset();
    block->SetPosition(position);
    return block;
}