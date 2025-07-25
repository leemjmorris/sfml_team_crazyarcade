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

    // LMJ: "Set block properties based on type (backward compatibility)"
    switch (type)
    {
    case BlockType::SoftBlock:
        SetBlockProperties(true, false, false, true);
        break;
    case BlockType::HardBlock:
        SetBlockProperties(false, false, false, false);
        break;
    case BlockType::HideBlock:
        SetBlockProperties(false, true, false, false);
        break;
    case BlockType::UncoverBlock:
        SetBlockProperties(false, false, false, false);
        break;
    case BlockType::PushBlock:
        SetBlockProperties(false, false, true, false);
        break;
    case BlockType::FixBlock:
        SetBlockProperties(false, false, false, false);
        break;
    default:
        SetBlockProperties(false, false, false, false);
        break;
    }
}

void Block::SetBlockProperties(const BlockInfo& info)
{
    textureId = info.textureId;
    isDestroyable = info.isDestroyable;
    isHidable = info.isHidable;
    isMovable = info.isMovable;
    canSpawnItem = info.canSpawnItem;
    blockType = BlockType::Custom; // LMJ: "Mark as custom when using BlockInfo"
}

void Block::SetBlockProperties(bool destroyable, bool hidable, bool movable, bool spawnItem)
{
    isDestroyable = destroyable;
    isHidable = hidable;
    isMovable = movable;
    canSpawnItem = spawnItem;
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

        // LMJ: "Spawn item at block position using existing SpawnItem method"
        std::string itemName = "SpawnedItem_" + std::to_string(rand());
        Item::SpawnItem(itemName, itemType, blockPosition);
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

    // LMJ: "Register forest blocks with specified properties"
    // LMJ: "All blocks: not movable, destructable, not hidable, no item spawn"
    RegisterBlock("assets/map/forest/block/block_1.bmp", "Forest Block 1", true, false, false, false);
    RegisterBlock("assets/map/forest/block/block_2.bmp", "Forest Block 2", true, false, false, false);
    RegisterBlock("assets/map/forest/block/block_3.bmp", "Forest Block 3", true, false, false, false);
    RegisterBlock("assets/map/forest/block/block_4.bmp", "Forest Block 4", true, false, false, false);
    RegisterBlock("assets/map/forest/block/block_5.bmp", "Forest Block 5", true, false, false, false);
    RegisterBlock("assets/map/forest/block/block_6.bmp", "Forest Block 6", true, false, false, false);

    registryInitialized = true;
}

void Block::RegisterBlock(const std::string& textureId, const std::string& displayName,
    bool destroyable, bool hidable, bool movable, bool spawnItem)
{
    blockRegistry.emplace_back(textureId, displayName, destroyable, hidable, movable, spawnItem);
}

void Block::RegisterBlock(const std::string& textureId, BlockType type, const std::string& displayName)
{
    // LMJ: "Legacy registration method - convert BlockType to properties"
    bool destroyable = false, hidable = false, movable = false, spawnItem = false;

    switch (type)
    {
    case BlockType::SoftBlock:
        destroyable = true; spawnItem = true;
        break;
    case BlockType::HardBlock:
        // All false (default)
        break;
    case BlockType::HideBlock:
        hidable = true;
        break;
    case BlockType::UncoverBlock:
        // All false (default)
        break;
    case BlockType::PushBlock:
        movable = true;
        break;
    case BlockType::FixBlock:
        // All false (default)
        break;
    }

    RegisterBlock(textureId, displayName, destroyable, hidable, movable, spawnItem);
}

std::vector<BlockInfo> Block::GetBlocksByType(BlockType type)
{
    std::vector<BlockInfo> result;

    // LMJ: "For backward compatibility, filter by properties that match the type"
    for (const auto& blockInfo : blockRegistry)
    {
        bool matches = false;

        switch (type)
        {
        case BlockType::SoftBlock:
            matches = blockInfo.isDestroyable && blockInfo.canSpawnItem;
            break;
        case BlockType::HardBlock:
            matches = !blockInfo.isDestroyable && !blockInfo.isHidable && !blockInfo.isMovable;
            break;
        case BlockType::HideBlock:
            matches = blockInfo.isHidable;
            break;
        case BlockType::UncoverBlock:
            matches = !blockInfo.isDestroyable && !blockInfo.isHidable && !blockInfo.isMovable && !blockInfo.canSpawnItem;
            break;
        case BlockType::PushBlock:
            matches = blockInfo.isMovable;
            break;
        case BlockType::FixBlock:
            matches = !blockInfo.isDestroyable && !blockInfo.isHidable && !blockInfo.isMovable && !blockInfo.canSpawnItem;
            break;
        }

        if (matches)
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
    return BlockInfo("", "Invalid", false, false, false, false);
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
    if (blockInfo.textureId.empty())
        return nullptr;

    Block* block = new Block();
    block->SetBlockProperties(blockInfo); // LMJ: "Use new property system"
    block->textureId = blockInfo.textureId;
    block->Reset();
    block->SetPosition(position);
    return block;
}

Block* Block::CreateBlockWithProperties(const std::string& textureId, const sf::Vector2f& position,
    bool destroyable, bool hidable, bool movable, bool spawnItem)
{
    Block* block = new Block();
    block->textureId = textureId;
    block->SetBlockProperties(destroyable, hidable, movable, spawnItem);
    block->Reset();
    block->SetPosition(position);
    return block;
}