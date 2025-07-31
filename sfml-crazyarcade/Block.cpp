#include "stdafx.h"
#include "Block.h"
#include "Item.h"
#include "Scene.h"
#include "Player.h"

// LMJ: "Default item spawn probability (50%)" 
float Block::defaultItemSpawnProbability = 0.5f;

// LMJ: "Static registry initialization"
std::vector<BlockInfo> Block::blockRegistry;
bool Block::registryInitialized = false;

Block::Block(const std::string& name)
    : SpriteGo("", name), blockType(BlockType::None),
    isDestroyable(false), isHidable(false), isMovable(false),
    canSpawnItem(true), itemSpawnProbability(defaultItemSpawnProbability),
    registryIndex(-1), blockIndex(0)
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
    blockIndex = info.blockIndex; // LMJ: "Set sprite sheet index"
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

    ANI_CLIP_MGR.Load("animation/block_destroy.csv"); // KHI 

    animator.SetTarget(&sprite); // KHI
}

void Block::Release()
{
    SpriteGo::Release();
}

void Block::Reset()
{
    // LMJ: "Load sprite sheet texture and set texture rect"
    sprite.setTexture(TEXTURE_MGR.Get(PATH_BLOCK_SHEET "block_sheet.png"));

    // LMJ: "Calculate texture rect from block index"
    sf::IntRect textureRect = GetBlockTextureRect(blockIndex);
    sprite.setTextureRect(textureRect);

    SetOrigin(Origins::BC);
    hasAnimStarted = false;
}

void Block::Update(float dt)
{
    SpriteGo::Update(dt);

    // KHI: Update HitBox
    hitBox.UpdateCustomTransform(sprite, hitBoxSize, hitBoxOffset, Origins::BC);

    // KHI: Update Animation
    animator.Update(dt);

    if (!animator.IsPlaying() && hasAnimStarted)
    {
        Scene* curScene = SCENE_MGR.GetCurrentScene();
        this->DestroyBlock(curScene);
        hasAnimStarted = false;
    }

    if (isMoving)
    {
        Movement(dt);
    }
}

void Block::Draw(sf::RenderWindow& window)
{
    hitBox.Draw(window);
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
        std::string itemName = "item";
        Item::SpawnItem(itemName, itemType, blockPosition);

        std::cout << "Block position : " << blockPosition.x << ", " << blockPosition.y << std::endl;
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

    // LMJ: "Register 74 blocks (37x2 grid, indices 0-73)"
    for (int i = 0; i < 74; ++i)
    {
        std::string displayName = "Block " + std::to_string(i + 1);
        RegisterBlockFromSheet(i, displayName, true, false, false, false);
    }

    registryInitialized = true;
}

void Block::RegisterBlockFromSheet(int sheetIndex, const std::string& displayName,
    bool destroyable, bool hidable, bool movable, bool spawnItem)
{
    // LMJ: "Use sprite sheet path as textureId, store sheet index separately"
    std::string textureId = PATH_BLOCK_SHEET "block_sheet.png";
    blockRegistry.emplace_back(textureId, displayName, destroyable, hidable, movable, spawnItem, sheetIndex);
}

void Block::RegisterBlock(const std::string& textureId, const std::string& displayName,
    bool destroyable, bool hidable, bool movable, bool spawnItem)
{
    // LMJ: "Legacy method - assume index 0 for backward compatibility"
    blockRegistry.emplace_back(textureId, displayName, destroyable, hidable, movable, spawnItem, 0);
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

sf::IntRect Block::GetBlockTextureRect(int blockIndex)
{
    // LMJ: "Get actual texture size (same as LoadBlockSet)"
    sf::Texture& texture = TEXTURE_MGR.Get(PATH_BLOCK_SHEET "block_sheet.png");
    sf::Vector2u textureSize = texture.getSize();

    const float blockWidth = textureSize.x / 37.0f;
    const float blockHeight = textureSize.y / 2.0f;

    int row = blockIndex / 37;
    int col = blockIndex % 37;

    return sf::IntRect(
        static_cast<int>(col * blockWidth),
        static_cast<int>(row * blockHeight),
        static_cast<int>(blockWidth),
        static_cast<int>(blockHeight)
    );
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
    return BlockInfo("", "Invalid", false, false, false, false, 0);
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
    // LMJ: "Legacy method - returns sprite sheet path"
    return PATH_BLOCK_SHEET "block_sheet.png";
}

Block* Block::CreateBlock(BlockType type, const sf::Vector2f& position)
{
    // LMJ: "Create block using first texture of the type"
    auto blocks = GetBlocksByType(type);
    if (blocks.empty())
        return nullptr;

    Block* block = new Block();
    block->SetBlockType(type);
    block->textureId = PATH_BLOCK_SHEET "block_sheet.png";
    block->blockIndex = blocks[0].blockIndex; // LMJ: "Set sprite sheet index"
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
    block->SetRegistryIndex(registryIndex);
    block->SetBlockProperties(blockInfo); // LMJ: "Use new property system"
    block->textureId = blockInfo.textureId;
    block->blockIndex = blockInfo.blockIndex; // LMJ: "Set sprite sheet index"
    block->Reset();
    block->SetPosition(position);

    return block;
}

// LMJ: "JSON serialization methods"
json Block::ToJson(const Block* block, int registryIndex)
{
    if (!block) return json{};

    json j;
    j["registryIndex"] = registryIndex;
    j["blockIndex"] = block->blockIndex; // LMJ: "Save sprite sheet index"
    j["x"] = block->GetPosition().x;     // LMJ: "Keep original field name for compatibility"
    j["y"] = block->GetPosition().y;     // LMJ: "Keep original field name for compatibility" 
    j["isDestroyable"] = block->IsDestroyable();
    j["isHidable"] = block->IsHidable();
    j["isMovable"] = block->IsMovable();
    j["canSpawnItem"] = block->CanSpawnItem();
    j["itemSpawnProbability"] = block->itemSpawnProbability; // LMJ: "New field for sprite sheet"

    return j;
}

Block* Block::FromJson(const json& j)
{
    try
    {
        int registryIndex = j.at("registryIndex").get<int>();

        // LMJ: "Support both old (x,y) and new (positionX,positionY) field names"
        float posX = j.contains("positionX") ? j.at("positionX").get<float>() : j.at("x").get<float>();
        float posY = j.contains("positionY") ? j.at("positionY").get<float>() : j.at("y").get<float>();

        // LMJ: "Create block from registry"
        Block* block = Block::CreateBlockFromRegistry(registryIndex, sf::Vector2f(posX, posY));
        if (!block) return nullptr;

        // LMJ: "Load sprite sheet index if available (new feature)"
        if (j.contains("blockIndex"))
        {
            block->blockIndex = j.at("blockIndex").get<int>();
        }

        // LMJ: "Load block properties with backward compatibility"
        bool isDestroyable = j.value("isDestroyable", false);
        bool isHidable = j.value("isHidable", false);
        bool isMovable = j.value("isMovable", false);
        bool canSpawnItem = j.value("canSpawnItem", false);

        block->SetBlockProperties(isDestroyable, isHidable, isMovable, canSpawnItem);

        // LMJ: "Load item spawn probability if available"
        if (j.contains("itemSpawnProbability"))
        {
            block->itemSpawnProbability = j.at("itemSpawnProbability").get<float>();
        }

        return block;
    }
    catch (const std::exception& e)
    {
        //std::cerr << "Error loading block from JSON: " << e.what() << std::endl;
        return nullptr;
    }
}

// KHI
void Block::PlayExitAnim()
{
    animator.Play("animation/block_destroy.csv");
    hasAnimStarted = true;
}

// KHI
void Block::PushBlock(sf::Vector2f dir)
{
    originPos = GetPosition();
    targetPos = originPos + (dir * 52.f);

    if (IsBlockedAtTarget())
    {
        isMoving = false;
    }
    else
    {
        isMoving = true;
    }

    sprite.setPosition(originPos);
}

// KHI
void Block::Movement(float dt)
{
    sf::Vector2f currentPos = GetPosition();
    sf::Vector2f toTarget = targetPos - currentPos;
    float distance = Utils::Magnitude(toTarget);

    float moveAmount = 150 * dt;

    if (distance <= moveAmount)
    {
        SetPosition(targetPos);
        isMoving = false;
    }
    else
    {
        sf::Vector2f moveDir = Utils::GetNormal(toTarget);
        SetPosition(currentPos + moveDir * moveAmount);
    }
}

// KHI
bool Block::IsBlockedAtTarget()
{
    Scene* curScene = SCENE_MGR.GetCurrentScene();

    sf::FloatRect splashBounds = GetHitBox().rect.getGlobalBounds();

    sprite.setPosition(targetPos);
    hitBox.UpdateCustomTransform(sprite, hitBoxSize, hitBoxOffset, Origins::BC);
    sf::FloatRect targetBounds = hitBox.GetGlobalBounds();

    sf::Vector2f targetPosCenter = {
        targetBounds.left + targetBounds.width * 0.5f,
        targetBounds.top + targetBounds.height * 0.5f
    };

    // KHI: Window
    constexpr int GRID_SIZE = 52;
    const sf::FloatRect worldBounds(0.f, 0.f, 15 * GRID_SIZE, 13 * GRID_SIZE);

    if (!worldBounds.contains(targetPosCenter))
    {
        return true;
    }

    // KHI: Block
    auto gameObjects = curScene->FindGameObjects("Block");
    for (auto* obj : gameObjects)
    {
        Block* block = dynamic_cast<Block*>(obj);
        if (!block || block == this || !block->GetActive())
        {
            continue;
        }

        sf::FloatRect blockBounds = block->GetHitBox().GetGlobalBounds();
        if (blockBounds.contains(targetPosCenter))
        {
            return true;
        }
    }

    // KHI: Player
    auto players = curScene->FindGameObjects("Player");
    for (auto* obj : players)
    {
        Player* player = dynamic_cast<Player*>(obj);

        sf::FloatRect playerBounds = player->GetHitBox().GetGlobalBounds();
        if (playerBounds.contains(targetPosCenter))
        {
            return true;
        }
    }

    return false;
}