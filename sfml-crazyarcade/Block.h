#pragma once
#include "SpriteGo.h"
#include "ColorMaskShader.h"
#include "HitBox.h"   // KHI
#include "Animator.h" // KHI
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Scene;

enum class BlockType
{
    None = -1,
    SoftBlock,
    HardBlock,
    HideBlock,
    UncoverBlock,
    PushBlock,
    FixBlock,
    Custom,      // LMJ: "For blocks using BlockInfo properties"
    Count,
};

// LMJ: "Updated BlockInfo struct with sprite sheet support"
struct BlockInfo
{
    std::string textureId;
    std::string displayName;
    bool isDestroyable;
    bool isHidable;
    bool isMovable;
    bool canSpawnItem;
    int blockIndex; // LMJ: "Index in sprite sheet (0-5)"

    BlockInfo(const std::string& texId, const std::string& name,
        bool destroyable, bool hidable, bool movable, bool spawnItem, int index = 0)
        : textureId(texId), displayName(name), isDestroyable(destroyable),
        isHidable(hidable), isMovable(movable), canSpawnItem(spawnItem), blockIndex(index)
    {
    }
};

class Block : public SpriteGo
{
protected:
    BlockType blockType;
    bool isDestroyable;
    bool isHidable;
    bool isMovable;
    bool canSpawnItem;
    float itemSpawnProbability;
    int registryIndex; // LMJ: "Index in block registry"
    int blockIndex;    // LMJ: "Index in sprite sheet (0-5)"

    // LMJ: "Static registry for block management"
    static std::vector<BlockInfo> blockRegistry;
    static bool registryInitialized;

    // KHI: about HitBox
    HitBox hitBox;
    sf::Vector2f hitBoxSize = { 52.f, 52.f };
    sf::Vector2f hitBoxOffset = { 0.f, 0.f };

    Animator animator; // KHI
    bool hasAnimStarted = false; // KHI

public:
    // LMJ: "Static default item spawn probability"
    static float defaultItemSpawnProbability;

    Block(const std::string& name = "Block");
    ~Block() = default;

    // LMJ: "GameObject interface"
    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;

    // LMJ: "Block type and properties"
    void SetBlockType(BlockType type);
    BlockType GetBlockType() const { return blockType; }

    void SetBlockProperties(const BlockInfo& info);
    void SetBlockProperties(bool destroyable, bool hidable, bool movable, bool spawnItem);

    // LMJ: "Property getters and setters"
    bool IsDestroyable() const { return isDestroyable; }
    bool IsHidable() const { return isHidable; }
    bool IsMovable() const { return isMovable; }
    bool CanSpawnItem() const { return canSpawnItem; }

    void SetDestroyable(bool value) { isDestroyable = value; }
    void SetHidable(bool value) { isHidable = value; }
    void SetMovable(bool value) { isMovable = value; }
    void SetCanSpawnItem(bool value) { canSpawnItem = value; }

    float GetItemSpawnProbability() const { return itemSpawnProbability; }
    void SetItemSpawnProbability(float prob) { itemSpawnProbability = prob; }

    // LMJ: "Registry index management"
    int GetRegistryIndex() const { return registryIndex; }
    void SetRegistryIndex(int index) { registryIndex = index; }

    // LMJ: "Sprite sheet index management"
    int GetBlockIndex() const { return blockIndex; }
    void SetBlockIndex(int index) { blockIndex = index; }

    // LMJ: "Block actions"
    void DestroyBlock(Scene* scene);
    bool ShouldSpawnItem() const;

    // LMJ: "Static registry management"
    static void InitializeBlockRegistry();
    static void RegisterBlockFromSheet(int sheetIndex, const std::string& displayName,
        bool destroyable, bool hidable, bool movable, bool spawnItem);
    static void RegisterBlock(const std::string& textureId, const std::string& displayName,
        bool destroyable, bool hidable, bool movable, bool spawnItem);
    static void RegisterBlock(const std::string& textureId, BlockType type, const std::string& displayName);

    static std::vector<BlockInfo> GetBlocksByType(BlockType type);
    static std::vector<BlockInfo> GetAllBlocks();
    static BlockInfo GetBlockInfo(int registryIndex);
    static int GetBlockRegistrySize();

    // LMJ: "Legacy support"
    static std::string GetTextureId(BlockType type);

    // LMJ: "Block creation methods"
    static Block* CreateBlock(BlockType type, const sf::Vector2f& position);
    static Block* CreateBlockFromRegistry(int registryIndex, const sf::Vector2f& position);
    static Block* CreateBlockWithProperties(const std::string& textureId, const sf::Vector2f& position,
        bool destroyable, bool hidable, bool movable, bool spawnItem);

    // LMJ: "Sprite sheet utility method"
    static sf::IntRect GetBlockTextureRect(int blockIndex);

    // LMJ: "JSON serialization methods"
    static json ToJson(const Block* block, int registryIndex);
    static Block* FromJson(const json& j);

    // KHI --------
    void PlayExitAnim();

    sf::Vector2f originPos = { 0.f, 0.f };
    sf::Vector2f targetPos = { 0.f, 0.f };
    bool isMoving = false;
    void PushBlock(sf::Vector2f dir);
    void Movement(float dt);
    bool IsBlockedAtTarget();

    const HitBox& GetHitBox() const
    {
        return hitBox;
    }

    public:
    bool GetIsMoving() const { return isMoving; }
};