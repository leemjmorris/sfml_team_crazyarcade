#pragma once
#include "SpriteGo.h"

// LMJ: "Updated BlockInfo with individual properties"
struct BlockInfo
{
    std::string textureId;
    std::string displayName;

    // LMJ: "Individual properties instead of single type"
    bool isDestroyable;
    bool isHidable;
    bool isMovable;
    bool canSpawnItem;

    BlockInfo(const std::string& texId, const std::string& name = "",
        bool destroyable = false, bool hidable = false,
        bool movable = false, bool spawnItem = false)
        : textureId(texId), displayName(name),
        isDestroyable(destroyable), isHidable(hidable),
        isMovable(movable), canSpawnItem(spawnItem) {
    }
};

// LMJ: "Keep BlockType enum for backward compatibility and categorization"
enum class BlockType
{
    None = -1,
    SoftBlock,
    HardBlock,
    HideBlock,
    UncoverBlock,
    PushBlock,
    FixBlock,
    Custom, // LMJ: "For blocks with custom property combinations"
    Count
};

class Scene;
class Item;

class Block : public SpriteGo
{
protected:
    BlockType blockType;
    bool isDestroyable;
    bool isHidable;
    bool isMovable;

    bool canSpawnItem;
    float itemSpawnProbability;
    static float defaultItemSpawnProbability;

    static std::vector<BlockInfo> blockRegistry;
    static bool registryInitialized;

public:
    Block(const std::string& name = "Block");
    ~Block() override = default;

    // LMJ: "Updated methods for property system"
    void SetBlockType(BlockType type);
    void SetBlockProperties(const BlockInfo& info);
    void SetBlockProperties(bool destroyable, bool hidable, bool movable, bool spawnItem);

    BlockType GetBlockType() const { return blockType; }

    bool IsDestroyable() const { return isDestroyable; }
    void SetDestroyable(bool destroyable) { isDestroyable = destroyable; }

    bool IsHidable() const { return isHidable; }
    void SetHidable(bool hidable) { isHidable = hidable; }

    bool IsMovable() const { return isMovable; }
    void SetMovable(bool movable) { isMovable = movable; }

    // LMJ: ================= Item related methods =================
    bool CanSpawnItem() const { return canSpawnItem; }
    void SetCanSpawnItem(bool canSpawn) { canSpawnItem = canSpawn; }

    float GetItemSpawnProbability() const { return itemSpawnProbability; }
    void SetItemSpawnProbability(float probability) { itemSpawnProbability = probability; }

    static void SetDefaultItemSpawnProbability(float probability) { defaultItemSpawnProbability = probability; }
    static float GetDefaultItemSpawnProbability() { return defaultItemSpawnProbability; }
    // LMJ: =========================================================

    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;

    // LMJ: "Block destruction with item spawning"
    void DestroyBlock(Scene* scene);

    // LMJ: "Check if item should spawn based on probability"
    bool ShouldSpawnItem() const;

    // LMJ: "Updated block registry management with property system"
    static void InitializeBlockRegistry();
    static void RegisterBlock(const std::string& textureId, const std::string& displayName,
        bool destroyable, bool hidable, bool movable, bool spawnItem);
    static void RegisterBlock(const std::string& textureId, BlockType type, const std::string& displayName = "");
    static std::vector<BlockInfo> GetBlocksByType(BlockType type);
    static std::vector<BlockInfo> GetAllBlocks();
    static BlockInfo GetBlockInfo(int registryIndex);
    static int GetBlockRegistrySize();

    // LMJ: "Legacy method - kept for compatibility but deprecated"
    static std::string GetTextureId(BlockType type);

    // LMJ: "Factory methods - enhanced with property system"
    static Block* CreateBlock(BlockType type, const sf::Vector2f& position);
    static Block* CreateBlockFromRegistry(int registryIndex, const sf::Vector2f& position);
    static Block* CreateBlockWithProperties(const std::string& textureId, const sf::Vector2f& position,
        bool destroyable, bool hidable, bool movable, bool spawnItem);
};