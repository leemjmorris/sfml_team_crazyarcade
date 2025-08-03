#include "stdafx.h"
#include "Utils.h"
#include "Block.h"
#include "Scene.h"
#include "SpriteGo.h"
#include "MapEditor.h" // LMJ: "For TileData struct"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::random_device Utils::rd;
std::mt19937 Utils::gen;
std::vector<sf::Vector2f> Utils::PlayerSpawnPoints;
const float Utils::PI = acosf(-1.f);

//int Utils::CollBlockLayer[13][15];

void Utils::Init()
{
    gen.seed(rd());
}

float Utils::RandomValue()
{
    std::uniform_real_distribution<float> dist(0.f, 1.f);
    return dist(gen);
}

int Utils::RandomRange(int min, int maxExclude)
{
    if (min >= maxExclude)
    {
        return min;
    }
    std::uniform_int_distribution<int> dist(min, maxExclude - 1);
    return dist(gen);
}

float Utils::RandomRange(float min, float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

sf::Vector2f Utils::RandomOnUnitCircle()
{
    sf::Vector2f point;
    do
    {
        point = RandomInUnitCircle();
    } while (SqrMagnitude(point) < std::numeric_limits<float>::epsilon());

    return GetNormal(point);
}

sf::Vector2f Utils::RandomInUnitCircle()
{
    sf::Vector2f point;
    do
    {
        point = RandomPointInRect(sf::FloatRect(-1.f, -1.f, 2.f, 2.f));
    } while (SqrMagnitude(point) > 1.f);

    return point;
}

sf::Color Utils::RandomColor(bool opaque)
{
    sf::Uint8 r = (sf::Uint8)RandomRange(0, 256);
    sf::Uint8 g = (sf::Uint8)RandomRange(0, 256);
    sf::Uint8 b = (sf::Uint8)RandomRange(0, 256);
    sf::Uint8 a = opaque ? 255 : (sf::Uint8)RandomRange(0, 256);
    return sf::Color(r, g, b, a);
}

sf::Vector2f Utils::RandomPointInRect(const sf::FloatRect& rect)
{
    return sf::Vector2f(
        RandomRange(rect.left, rect.left + rect.width),
        RandomRange(rect.top, rect.top + rect.height));
}


sf::Vector2f Utils::SetOrigin(sf::Transformable& obj, Origins preset, const sf::FloatRect rect)
{
    sf::Vector2f newOrigin(rect.width, rect.height);
    newOrigin.x *= ((int)preset % 3) * 0.5f;
    newOrigin.y *= ((int)preset / 3) * 0.5f;
    obj.setOrigin(newOrigin);
    return newOrigin;
}

sf::Vector2f Utils::SetOrigin(sf::Shape& obj, Origins preset)
{
    return SetOrigin(obj, preset, obj.getLocalBounds());
}

sf::Vector2f Utils::SetOrigin(sf::Text& obj, Origins preset)
{
    return SetOrigin(obj, preset, obj.getLocalBounds());
}

sf::Vector2f Utils::SetOrigin(sf::Sprite& obj, Origins preset)
{
    return SetOrigin(obj, preset, obj.getLocalBounds());
}

float Utils::Clamp(float value, float min, float max)
{
    if (value < min)
        return min;
    else if (value > max)
        return max;
    return value;
}

float Utils::Clamp01(float value)
{
    return Clamp(value, 0.f, 1.f);
}

float Utils::Magnitude(const sf::Vector2f& vec)
{
    return std::sqrtf(SqrMagnitude(vec));
}

float Utils::SqrMagnitude(const sf::Vector2f& vec)
{
    return vec.x * vec.x + vec.y * vec.y;
}

sf::Vector2f Utils::GetNormal(const sf::Vector2f& vec)
{
    float mag = Utils::Magnitude(vec);
    if (mag < std::numeric_limits<float>::epsilon())
    {
        return { 0.f, 0.f };
    }
    return vec / mag;
}

void Utils::Normalize(sf::Vector2f& vec)
{
    float mag = Utils::Magnitude(vec);
    if (mag > std::numeric_limits<float>::epsilon())
    {
        vec /= mag;
    }
}

float Utils::Distance(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
    return Utils::Magnitude(p1 - p2);
}

float Utils::Lerp(float min, float max, float t, bool clamp)
{
    if (clamp)
    {
        t = Utils::Clamp01(t);
    }
    return min + (max - min) * t;
}

sf::Vector2f Utils::Lerp(const sf::Vector2f& min, const sf::Vector2f& max, float t, bool clamp)
{
    if (clamp)
    {
        t = Utils::Clamp01(t);
    }
    return min + (max - min) * t;
}

sf::Color Utils::Lerp(const sf::Color& min, const sf::Color& max, float t, bool clamp)
{
    if (clamp)
    {
        t = Utils::Clamp01(t);
    }

    return sf::Color(
        (sf::Uint8)std::round(Clamp(Lerp((float)min.r, (float)max.r, t, clamp), 0.f, 255.f)),
        (sf::Uint8)std::round(Clamp(Lerp((float)min.g, (float)max.g, t, clamp), 0.f, 255.f)),
        (sf::Uint8)std::round(Clamp(Lerp((float)min.b, (float)max.b, t, clamp), 0.f, 255.f)),
        (sf::Uint8)std::round(Clamp(Lerp((float)min.a, (float)max.a, t, clamp), 0.f, 255.f))
    );
}

float Utils::RadianToDegree(float radian)
{
    return radian * (180.f / PI);
}

float Utils::DegreeToRadian(float degree)
{
    return degree * (PI / 180.f);
}

float Utils::AngleRadian(const sf::Vector2f& vec)
{
    return std::atan2(vec.y, vec.x);
}

float Utils::Angle(const sf::Vector2f& vec)
{
    return RadianToDegree(AngleRadian(vec));
}

float Utils::Dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return a.x * b.x + a.y * b.y;
}

bool Utils::CheckCollision(const sf::Sprite& shapeA, const sf::Sprite& shapeB)
{
    if (!shapeA.getGlobalBounds().intersects(shapeB.getGlobalBounds()))
        return false;

    auto pointsA = GetShapePoints(shapeA);
    auto pointsB = GetShapePoints(shapeB);
    return PolygonsIntersect(pointsA, shapeA.getTransform(), pointsB, shapeB.getTransform());
}


bool Utils::CheckCollision(const sf::RectangleShape& shapeA, const sf::RectangleShape& shapeB)
{
    if (!shapeA.getGlobalBounds().intersects(shapeB.getGlobalBounds()))
        return false;

    auto pointsA = GetShapePoints(shapeA);
    auto pointsB = GetShapePoints(shapeB);
    return PolygonsIntersect(pointsA, shapeA.getTransform(), pointsB, shapeB.getTransform());
}

bool Utils::CheckCircleCollision(const sf::Vector2f& centerA, float radiusA, const sf::Vector2f& centerB, float radiusB)
{
    float distanceSqr = SqrMagnitude(centerA - centerB);
    float radiusSum = radiusA + radiusB;
    return distanceSqr <= radiusSum * radiusSum;
}

bool Utils::PointInTransformBounds(const sf::Transformable& transformable, const sf::FloatRect& localBounds, const sf::Vector2f& point)
{
    sf::Transform inverse = transformable.getInverseTransform();
    sf::Vector2f localPoint = inverse.transformPoint(point);
    return localBounds.contains(localPoint);
}

std::vector<sf::Vector2f> Utils::GetShapePoints(const sf::RectangleShape& shape)
{
    sf::FloatRect localBounds = shape.getLocalBounds();
    return GetRectanglePointsFromBounds(localBounds);
}

std::vector<sf::Vector2f> Utils::GetShapePoints(const sf::Sprite& shape)
{
    sf::FloatRect localBounds = shape.getLocalBounds();
    return GetRectanglePointsFromBounds(localBounds);
}

std::vector<sf::Vector2f> Utils::GetRectanglePointsFromBounds(const sf::FloatRect& localBounds)
{
    std::vector<sf::Vector2f> points(4);
    points[0] = sf::Vector2f(localBounds.left, localBounds.top);
    points[1] = sf::Vector2f(localBounds.left + localBounds.width, localBounds.top);
    points[2] = sf::Vector2f(localBounds.left + localBounds.width, localBounds.top + localBounds.height);
    points[3] = sf::Vector2f(localBounds.left, localBounds.top + localBounds.height);
    return points;
}

bool Utils::PolygonsIntersect(const std::vector<sf::Vector2f>& polygonA, const sf::Transform& transformA, const std::vector<sf::Vector2f>& polygonB, const sf::Transform& transformB)
{
    std::vector<sf::Vector2f> axes;
    axes.reserve(polygonA.size() + polygonB.size());

    int countA = polygonA.size();
    for (int i = 0; i < countA; ++i)
    {
        sf::Vector2f p1 = transformA.transformPoint(polygonA[i]);
        sf::Vector2f p2 = transformA.transformPoint(polygonA[(i + 1) % countA]);
        sf::Vector2f edge = p2 - p1;

        if (SqrMagnitude(edge) < std::numeric_limits<float>::epsilon())
            continue;

        sf::Vector2f normal(-edge.y, edge.x);

        bool isDuplicate = false;
        for (const auto& axis : axes)
        {
            float cross = normal.x * axis.y - normal.y * axis.x;
            if (std::abs(cross) < std::numeric_limits<float>::epsilon())
            {
                isDuplicate = true;
                break;
            }
        }

        if (!isDuplicate)
            axes.push_back(normal);
    }

    int countB = polygonB.size();
    for (int i = 0; i < countB; ++i)
    {
        sf::Vector2f p1 = transformB.transformPoint(polygonB[i]);
        sf::Vector2f p2 = transformB.transformPoint(polygonB[(i + 1) % countB]);
        sf::Vector2f edge = p2 - p1;

        if (SqrMagnitude(edge) < std::numeric_limits<float>::epsilon())
            continue;

        sf::Vector2f normal(-edge.y, edge.x);

        bool isDuplicate = false;
        for (const auto& axis : axes)
        {
            float cross = normal.x * axis.y - normal.y * axis.x;
            if (std::abs(cross) < std::numeric_limits<float>::epsilon())
            {
                isDuplicate = true;
                break;
            }
        }

        if (!isDuplicate)
            axes.push_back(normal);
    }

    for (const auto& axis : axes)
    {
        float minA = std::numeric_limits<float>::max();
        float maxA = std::numeric_limits<float>::lowest();
        for (const auto& point : polygonA)
        {
            sf::Vector2f transformedPoint = transformA.transformPoint(point);
            float projection = Dot(axis, transformedPoint);
            minA = std::min(minA, projection);
            maxA = std::max(maxA, projection);
        }

        float minB = std::numeric_limits<float>::max();
        float maxB = std::numeric_limits<float>::lowest();
        for (const auto& point : polygonB)
        {
            sf::Vector2f transformedPoint = transformB.transformPoint(point);
            float projection = Dot(axis, transformedPoint);
            minB = std::min(minB, projection);
            maxB = std::max(maxB, projection);
        }

        if (maxA < minB || maxB < minA)
        {
            return false;
        }
    }
    return true;
}

// LMJ: "Map loading utilities implementation"
bool Utils::LoadMapFromJson(Scene* scene, const std::string& filename)
{
    if (!scene)
    {
        std::cerr << "Scene is null!" << std::endl;
        return false;
    }

    // LMJ: Construct full file path
    std::string fullPath = PATH_MAP_JSON + filename;

    // LMJ: Open and parse JSON file
    std::ifstream ifs(fullPath);
    if (!ifs.is_open())
    {
        std::cerr << "Failed to open map file: " << fullPath << std::endl;
        return false;
    }

    json jMap;
    try
    {
        ifs >> jMap;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
        ifs.close();
        return false;
    }
    ifs.close();

    // LMJ: Load tiles if they exist
    if (jMap.contains("tiles"))
    {
        LoadTilesFromJson(scene, &jMap["tiles"]);
    }

    // LMJ: Load blocks if they exist
    if (jMap.contains("blocks"))
    {
        LoadBlocksFromJson(scene, &jMap["blocks"]);
    }

    // LMJ: Load spawn points if they exist
    if (jMap.contains("spawnPoints"))
    {
        LoadSpawnPointsFromJson(scene, &jMap["spawnPoints"]);
    }

    return true;
}

void Utils::LoadTilesFromJson(Scene* scene, const void* tilesJsonPtr)
{
    const json& tilesJson = *static_cast<const json*>(tilesJsonPtr);

    ////std::cout << "LoadTilesFromJson: Processing " << tilesJson.size() << " tiles" << std::endl;

    // LMJ: Load tiles using TileData::FromJson which now handles worldPosition correctly
    for (const auto& jt : tilesJson)
    {
        // LMJ: Create TileData from JSON (now includes worldPosition)
        TileData tileData = TileData::FromJson(jt);

        // LMJ: Debug output to check loaded values
        ////std::cout << "Tile - Index: " << tileData.tileOptionIndex
            //// ", Grid: (" << tileData.gridX << "," << tileData.gridY << ")"
            ////<< ", WorldPos: (" << tileData.worldPosition.x << "," << tileData.worldPosition.y << ")"
            ////<< ", Rotation: " << tileData.rotation << std::endl;

        // LMJ: Use the worldPosition from TileData instead of grid calculation
        sf::Sprite* tileSprite = CreateTileSprite(tileData.tileOptionIndex, tileData.worldPosition, tileData.rotation);

        if (tileSprite)
        {
            ////std::cout << "Created tile sprite at position: (" << tileData.worldPosition.x << "," << tileData.worldPosition.y << ")" << std::endl;

            // LMJ: Add tile sprite to scene as SpriteGo
            SpriteGo* spriteGo = new SpriteGo();
            spriteGo->GetSprite() = *tileSprite;
            spriteGo->SetPosition(tileData.worldPosition); // LMJ: Use saved worldPosition
            spriteGo->SetScale({ 1.f, 1.f });
            spriteGo->SetOrigin(Origins::TL);
            spriteGo->sortingLayer = SortingLayers::Background;
            scene->AddGameObject(spriteGo);

            delete tileSprite; // LMJ: Clean up temporary sprite
        }
        else
        {
            ////std::cout << "ERROR: Failed to create tile sprite for index " << tileData.tileOptionIndex << std::endl;
        }
    }
    ////std::cout << "LoadTilesFromJson: Finished processing tiles" << std::endl;
}

void Utils::LoadBlocksFromJson(Scene* scene, const void* blocksJsonPtr)
{
    const json& blocksJson = *static_cast<const json*>(blocksJsonPtr);

    // LMJ: "Create Block objects using sprite sheet"
    for (const auto& jb : blocksJson)
    {
        Block* block = Block::FromJson(jb);
        if (block)
        {
            // LMJ: "Initialize block same as MapEditor"
            block->Init();
            block->SetScale({ 1.f, 1.f }); // LMJ: "Same scale as MapEditor"
            block->Reset(); // LMJ: "This will set up sprite sheet texture and rect"
            block->sortingLayer = SortingLayers::Default;

            scene->AddGameObject(block);
        }
    }
}

void Utils::LoadSpawnPointsFromJson(Scene* scene, const void* spawnPointsJsonPtr)
{
    const json& spawnPointsJson = *static_cast<const json*>(spawnPointsJsonPtr);

    // LMJ: Clear existing spawn points
    PlayerSpawnPoints.clear();
    PlayerSpawnPoints.resize(4, sf::Vector2f(-1, -1)); // LMJ: Initialize with invalid positions

    // LMJ: Load spawn points from JSON
    for (const auto& jSpawn : spawnPointsJson)
    {
        int playerIndex = jSpawn.at("playerIndex").get<int>();
        float x = jSpawn.at("positionX").get<float>();
        float y = jSpawn.at("positionY").get<float>();

        if (playerIndex >= 0 && playerIndex < 4)
        {
            PlayerSpawnPoints[playerIndex] = sf::Vector2f(x, y + 26.f); // LMJ: Change Player Spawn Position inside the Grid.
        }
    }
}

sf::Vector2f Utils::GridToWorldPosition(int gridX, int gridY, int gridSize)
{
    return sf::Vector2f(static_cast<float>(gridX * gridSize), static_cast<float>(gridY * gridSize));
}

void Utils::ClearMapObjects(Scene* scene)
{
    // LMJ: Not using this part. Maybe for future, I dont know.
}

sf::Vector2f Utils::GetPlayerSpawnPoint(int playerIndex)
{
    if (playerIndex < 0 || playerIndex >= static_cast<int>(PlayerSpawnPoints.size()))
    {
        return GetDefaultSpawnPoint(playerIndex);
    }

    sf::Vector2f spawnPoint = PlayerSpawnPoints[playerIndex];

    // LMJ: Check if spawn point is valid (not -1, -1)
    if (spawnPoint.x < 0 || spawnPoint.y < 0)
    {
        return GetDefaultSpawnPoint(playerIndex);
    }

    return spawnPoint;
}

bool Utils::HasValidSpawnPoint(int playerIndex)
{
    if (playerIndex < 0 || playerIndex >= static_cast<int>(PlayerSpawnPoints.size()))
    {
        return false;
    }

    sf::Vector2f spawnPoint = PlayerSpawnPoints[playerIndex];
    return (spawnPoint.x >= 0 && spawnPoint.y >= 0);
}

void Utils::SetPlayerSpawnPoint(int playerIndex, const sf::Vector2f& position)
{
    if (playerIndex < 0)
        return;

    // LMJ: Resize vector if necessary
    if (playerIndex >= static_cast<int>(PlayerSpawnPoints.size()))
    {
        PlayerSpawnPoints.resize(playerIndex + 1, sf::Vector2f(-1, -1));
    }

    PlayerSpawnPoints[playerIndex] = position;
}

sf::Vector2f Utils::GetDefaultSpawnPoint(int playerIndex)
{
    // LMJ: Default spawn positions for fallback
    switch (playerIndex)
    {
    case 0: return sf::Vector2f(234.f, 260.f);  // Player 1 default
    case 1: return sf::Vector2f(546.f, 468.f);  // Player 2 default
    case 2: return sf::Vector2f(120.f, 400.f); // Player 3 default
    case 3: return sf::Vector2f(400.f, 300.f);  // Player 4 default
    default: return sf::Vector2f(400.f, 300.f); // Generic default
    }
}

bool Utils::HasTrueOverlap(const sf::FloatRect& a, const sf::FloatRect& b, float threshold)
{
    sf::FloatRect i;
    if (a.intersects(b, i))
    {
        return (i.width >= threshold && i.height >= threshold);
    }
    return false;
}

sf::Sprite* Utils::CreateTileSprite(int tileOptionIndex, const sf::Vector2f& position, float rotation)
{
    // LMJ: "Use forest_tile_set.png like in MapEditor"
    std::string textureFile = PATH_TILE_SHEET "tile_set.png";

    // LMJ: "Map tile option index to texture coordinates (same as MapEditor)"
    sf::IntRect textureRect;
    const int TILE_SIZE = 52; // LMJ: "Same as MapEditor"
    const int TILES_PER_ROW = 10; // LMJ: "10 tiles per row in tileset"
    const int TOTAL_ROWS = 7;
    const int MAX_TILES = 70;

    if (tileOptionIndex < 0 || tileOptionIndex >= MAX_TILES)
    {
        int originalIndex = tileOptionIndex;
        tileOptionIndex = tileOptionIndex % MAX_TILES; // LMJ: "Wrap to 0-69 range"
        if (tileOptionIndex < 0) tileOptionIndex = 0;
    }

    // LMJ: "Calculate texture coordinates from index"
    int row = tileOptionIndex / TILES_PER_ROW;
    int col = tileOptionIndex % TILES_PER_ROW;

    textureRect.left = col * TILE_SIZE;
    textureRect.top = row * TILE_SIZE;
    textureRect.width = TILE_SIZE;
    textureRect.height = TILE_SIZE;

    sf::Sprite* sprite = new sf::Sprite();
    sprite->setTexture(TEXTURE_MGR.Get(textureFile));
    sprite->setTextureRect(textureRect); // LMJ: "Set the specific tile area"
    sprite->setPosition(position);
    sprite->setRotation(rotation);

    return sprite;
}