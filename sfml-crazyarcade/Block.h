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

struct BlockInfo
{
    std::string textureId;
    BlockType type;
    std::string displayName;

    BlockInfo(const std::string& texId, BlockType blockType, const std::string& name = "")
        : textureId(texId), type(blockType), displayName(name) {}
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

    void SetBlockType(BlockType type);
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

    // LMJ: "Block registry management"
    static void InitializeBlockRegistry();
    static void RegisterBlock(const std::string& textureId, BlockType type, const std::string& displayName = "");
    static std::vector<BlockInfo> GetBlocksByType(BlockType type);
    static std::vector<BlockInfo> GetAllBlocks();
    static BlockInfo GetBlockInfo(int registryIndex);
    static int GetBlockRegistrySize();

    // LMJ: "Legacy method - kept for compatibility but deprecated"
    static std::string GetTextureId(BlockType type);

    // LMJ: "Factory methods - enhanced"
    static Block* CreateBlock(BlockType type, const sf::Vector2f& position);
    static Block* CreateBlockFromRegistry(int registryIndex, const sf::Vector2f& position);
};