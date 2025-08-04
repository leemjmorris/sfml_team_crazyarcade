#include "stdafx.h"
#include "MapEditor.h"
#include <algorithm>
#include "libtinyfiledialogs-master/tinyfiledialogs.h"
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// LMJ: TileData JSON serialization implementation
json TileData::ToJson() const
{
    json j;
    j["tileOptionIndex"] = tileOptionIndex;
    j["gridX"] = gridX;
    j["gridY"] = gridY;
    j["rotation"] = rotation;
    j["positionX"] = worldPosition.x;
    j["positionY"] = worldPosition.y;

    return j;
}

// LMJ: Debug version of TileData::FromJson
TileData TileData::FromJson(const json& j)
{
    int tileOptionIndex = j.at("tileOptionIndex").get<int>();
    int gridX = j.at("gridX").get<int>();
    int gridY = j.at("gridY").get<int>();
    float rotation = j.value("rotation", 0.f);

    // LMJ: "Fix invalid tile option index for new tileset (0-69 only)"
    const int MAX_TILES = 70;
    if (tileOptionIndex >= MAX_TILES || tileOptionIndex < 0)
    {
        int originalIndex = tileOptionIndex;
        tileOptionIndex = tileOptionIndex % MAX_TILES;
        if (tileOptionIndex < 0) tileOptionIndex = 0;

        //std::cout << "WARNING: TileData index " << originalIndex << " fixed to " << tileOptionIndex << std::endl;
    }

    // LMJ: "Load worldPosition if available, fallback to grid calculation"
    sf::Vector2f worldPos;
    if (j.contains("positionX") && j.contains("positionY"))
    {
        worldPos.x = j.at("positionX").get<float>();
        worldPos.y = j.at("positionY").get<float>();
    }
    else
    {
        // LMJ: "Fallback for old save files without worldPosition"
        worldPos = Utils::GridToWorldPosition(gridX, gridY, 52); // LMJ: "Use MapEditor's GRID_SIZE"
    }

    // LMJ: "Use constructor with worldPosition"
    return TileData(tileOptionIndex, gridX, gridY, worldPos, rotation);
}

// LMJ: MapEditor constructor with proper member initialization
MapEditor::MapEditor()
    : Scene(SceneIds::MapEditor)
    , currentLayer(LayerType::Background)
    , tileOptionIndex(0)
    , blockRegistryIndex(0)
    , currentPropertyMode(PropertyMode::Destroyable)
    , selectedBlock(nullptr)
    , currentTileRotation(0.0f)
    , currentSpawnPlayerIndex(0)  // LMJ: Initialize spawn player index
{
    // LMJ: Initialize selection highlight visual properties
    selectionHighlight.setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
    selectionHighlight.setFillColor(sf::Color::Transparent);
    selectionHighlight.setOutlineColor(sf::Color::Yellow);
    selectionHighlight.setOutlineThickness(3.0f);
}

void MapEditor::Init()
{
    Scene::Init();

    // LMJ: Configure viewport settings with scaling for consistent visual appearance
    sf::Vector2f viewSize(800, 600);

    // LMJ: Calculate scale factor to make GRID_SIZE 52 appear like GRID_SIZE 40
    const float targetGridSize = 40.0f;
    const float actualGridSize = static_cast<float>(GRID_SIZE);
    const float scaleFactor = targetGridSize / actualGridSize;  // LMJ: 40/52 ≈ 0.769

    // LMJ: Apply scaling to worldView to make everything appear smaller
    worldView.setSize(viewSize / scaleFactor);  // LMJ: Zoom in by inverse scale factor
    worldView.setCenter((viewSize / scaleFactor) * 0.5f);

    // LMJ: UI view remains unscaled for consistent UI positioning
    uiView.setSize(viewSize);
    uiView.setCenter(viewSize * 0.5f);

    // LMJ: Load font for UI rendering (not in texIds)
    fontIds.push_back("assets/font/Daum_Regular.ttf");

    // LMJ: Load block textures from registry
    auto allBlocks = Block::GetAllBlocks();
    for (const auto& blockInfo : allBlocks)
    {
        texIds.push_back(blockInfo.textureId);
    }

    LoadTileSet();
    LoadBlockSet();

    // LMJ: Initialize grid visualization
    gridLines.setPrimitiveType(sf::Lines);
    gridLines.clear();

    const sf::Color gridColor(255, 255, 255, 100);

    // LMJ: Create vertical grid lines
    for (int x = 0; x <= GRID_WIDTH; ++x)
    {
        float xPos = x * GRID_SIZE;
        gridLines.append(sf::Vertex(sf::Vector2f(xPos, 0), gridColor));
        gridLines.append(sf::Vertex(sf::Vector2f(xPos, GRID_HEIGHT * GRID_SIZE), gridColor));
    }

    // LMJ: Create horizontal grid lines
    for (int y = 0; y <= GRID_HEIGHT; ++y)
    {
        float yPos = y * GRID_SIZE;
        gridLines.append(sf::Vertex(sf::Vector2f(0, yPos), gridColor));
        gridLines.append(sf::Vertex(sf::Vector2f(GRID_WIDTH * GRID_SIZE, yPos), gridColor));
    }
}

void MapEditor::Release()
{
    // LMJ: Clean up dynamically allocated blocks
    for (Block* block : PlacedBlocks)
    {
        if (block)
        {
            delete block;
        }
    }
    PlacedBlocks.clear();
    tileDatas.clear();

    Scene::Release();
}

void MapEditor::Enter()
{
    SOUND_MGR.StopBgm();

    Scene::Enter();
}

void MapEditor::Exit()
{
    Scene::Exit();
}

void MapEditor::LoadTileSet()
{
    // LMJ: Try multiple paths for tileset loading
    std::string tilesetPaths[] = { PATH_TILE_SHEET "tile_set.png" };

    bool textureLoaded = false;
    for (const auto& path : tilesetPaths)
    {
        if (tileMapTexture.loadFromFile(path))
        {
            textureLoaded = true;
            break;
        }
    }

    if (!textureLoaded)
    {
        //std::cerr << "Failed to load tileset texture" << std::endl;
        return;
    }

    // LMJ: Create tile sprites from loaded texture
    sf::Vector2u textureSize = tileMapTexture.getSize();
    const float tileWidth = textureSize.x / 10.0f;   // LMJ: 10 tiles per row
    const float tileHeight = textureSize.y / 7.0f;  // LMJ: 7 rows total

    TileOptions.clear();

    // LMJ: Generate sprites for 70 tiles in 10x7 grid layout
    for (int y = 0; y < 7; ++y)
    {
        for (int x = 0; x < 10; ++x)
        {
            sf::Sprite tile;
            tile.setTexture(tileMapTexture);
            tile.setTextureRect(sf::IntRect(
                static_cast<int>(x * tileWidth),
                static_cast<int>(y * tileHeight),
                static_cast<int>(tileWidth),
                static_cast<int>(tileHeight)
            ));

            // LMJ: Scale tiles to fit grid cell size
            float scaleX = GRID_SIZE / tileWidth;
            float scaleY = GRID_SIZE / tileHeight;
            tile.setScale(sf::Vector2f(scaleX, scaleY));
            tile.setOrigin(sf::Vector2f(tileWidth / 2, tileHeight / 2));

            TileOptions.push_back(tile);
        }
    }
}

void MapEditor::LoadBlockSet()
{
    BlockPreviewSprites.clear();
    BlockTextures.clear();

    sf::Texture blockSheetTexture;
    if (!blockSheetTexture.loadFromFile(PATH_BLOCK_SHEET "block_sheet.png"))
        return;

    BlockTextures.push_back(blockSheetTexture);

    sf::Vector2u textureSize = blockSheetTexture.getSize();
    const float blockWidth = textureSize.x / 37.0f;
    const float blockHeight = textureSize.y / 2.0f;

    // LMJ: "Create 74 blocks in same order as InitializeBlockRegistry"
    for (int i = 0; i < 74; ++i)
    {
        int row = i / 37;  // 0-36은 row=0, 37-73은 row=1
        int col = i % 37;  // 각 행에서의 열 위치

        sf::Sprite blockSprite;
        blockSprite.setTexture(BlockTextures[0]);

        blockSprite.setTextureRect(sf::IntRect(
            static_cast<int>(col * blockWidth),
            static_cast<int>(row * blockHeight),
            static_cast<int>(blockWidth),
            static_cast<int>(blockHeight)
        ));

        float scaleX = static_cast<float>(GRID_SIZE) / blockWidth;
        float scaleY = static_cast<float>(GRID_SIZE) / blockHeight;
        //blockSprite.setScale(scaleX, scaleY);
        blockSprite.setOrigin(blockWidth / 2, (blockHeight / 2) + 26.f);

        BlockPreviewSprites.push_back(blockSprite);
    }
}


void MapEditor::Update(float dt)
{
    Scene::Update(dt);
    HandleInput();

    // LMJ: Update all active placed blocks
    for (Block* block : PlacedBlocks)
    {
        if (block && block->GetActive())
        {
            block->Update(dt);
        }
    }
}

void MapEditor::Draw(sf::RenderWindow& window)
{
    // LMJ: Render world content with world view
    window.setView(worldView);

    window.draw(gridLines);

    // LMJ: Draw grid boundary outline
    sf::RectangleShape outline;
    outline.setSize(sf::Vector2f(GRID_WIDTH * GRID_SIZE, GRID_HEIGHT * GRID_SIZE));
    outline.setPosition(0, 0);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineColor(sf::Color::Yellow);
    outline.setOutlineThickness(3.f);
    window.draw(outline);

    // LMJ: Render all placed tiles
    for (const TileData& tileData : tileDatas)
    {
        if (tileData.tileOptionIndex < static_cast<int>(TileOptions.size()))
        {
            sf::Sprite tile = TileOptions[tileData.tileOptionIndex];
            tile.setPosition(
                tileData.gridX * GRID_SIZE + GRID_SIZE / 2.f,
                tileData.gridY * GRID_SIZE + GRID_SIZE / 2.f
            );
            tile.setRotation(tileData.rotation);
            window.draw(tile);
        }
    }

    // LMJ: Sort and render blocks for proper depth ordering
    {
        std::sort(PlacedBlocks.begin(), PlacedBlocks.end(), CompareBlockAxisY);
    }

    for (Block* block : PlacedBlocks)
    {
        if (block && block->GetActive())
        {
            block->Draw(window);
        }
    }

    // LMJ: Draw spawn points
    DrawSpawnPoints(window);

    DrawMapEditor(window);

    // LMJ: Draw selection highlight for block state layer
    if (currentLayer == LayerType::BlockState && selectedBlock != nullptr)
    {
        window.draw(selectionHighlight);
    }

    DrawBlockPropertyIndicators(window);

    // LMJ: Render UI elements with UI view
    window.setView(uiView);
    DrawSaveLoadInfo(window);
    DrawRightSideUI(window);

    Scene::Draw(window);
}

bool MapEditor::CompareBlockAxisY(const Block* a, const Block* b)
{
    return a->GetPosition().y < b->GetPosition().y;
}

void MapEditor::HandleInput()
{
    HandleLayerSwitching();
    HandleScrollInput();

    if (InputMgr::GetKeyDown(sf::Keyboard::F5))
    {
        SCENE_MGR.ChangeScene(SceneIds::Intro);
        return;
    }

    if (currentLayer == LayerType::Background)
    {
        HandleTileRotation();
    }

    if (currentLayer == LayerType::BlockState)
    {
        HandleLayer2Input();
    }

    if (currentLayer == LayerType::SpawnPoint)  // LMJ: Handle spawn point layer
    {
        HandleSpawnPointInput();
    }

    // LMJ: Handle mouse input for object placement
    sf::Vector2i mousePos = InputMgr::GetMousePosition();
    sf::Vector2f worldPos = ScreenToWorld(mousePos);
    sf::Vector2f gridPos = GetGridPosition(worldPos);

    if (InputMgr::GetMouseButton(sf::Mouse::Left))
    {
        if (IsValidGridPosition(gridPos))
        {
            if (currentLayer == LayerType::Background)
            {
                CreateTileAtPosition(gridPos);
            }
            else if (currentLayer == LayerType::Block)
            {
                CreateBlockAtPosition(gridPos);
            }
            else if (currentLayer == LayerType::SpawnPoint)  // LMJ: Create spawn point
            {
                CreateSpawnPointAtPosition(gridPos);
            }
        }
    }

    if (InputMgr::GetMouseButton(sf::Mouse::Right))
    {
        if (IsValidGridPosition(gridPos))
        {
            if (currentLayer == LayerType::Background)
            {
                DeleteTileAtPosition(gridPos);
            }
            else if (currentLayer == LayerType::Block)
            {
                DeleteBlockAtPosition(gridPos);
            }
            else if (currentLayer == LayerType::SpawnPoint)  // LMJ: Delete spawn point
            {
                DeleteSpawnPointAtPosition(gridPos);
            }
        }
    }

    // LMJ: File operations with error handling
    if (InputMgr::GetKey(sf::Keyboard::LControl) && InputMgr::GetKeyDown(sf::Keyboard::S))
    {
        const char* filters[] = { "*.json" };
        const char* filename = tinyfd_saveFileDialog(
            "맵 파일 저장",
            "map.json",
            1, filters, nullptr
        );

        if (filename)
        {
            SaveMapToJson(filename);
        }
    }

    if (InputMgr::GetKey(sf::Keyboard::LControl) && InputMgr::GetKeyDown(sf::Keyboard::O))
    {
        const char* filters[] = { "*.json" };
        const char* filename = tinyfd_openFileDialog(
            "맵 파일 열기",
            "",
            1, filters, nullptr, 0
        );

        if (filename)
        {
            LoadMapFromJson(filename);
        }
    }
}

void MapEditor::HandleTileRotation()
{
    if (InputMgr::GetKeyDown(sf::Keyboard::Q))
    {
        currentTileRotation -= 90.0f;
        if (currentTileRotation < 0.0f)
            currentTileRotation += 360.0f;
    }
    else if (InputMgr::GetKeyDown(sf::Keyboard::E))
    {
        currentTileRotation += 90.0f;
        if (currentTileRotation >= 360.0f)
            currentTileRotation -= 360.0f;
    }
}

void MapEditor::HandleLayer2Input()
{
    sf::Vector2i mousePos = InputMgr::GetMousePosition();
    sf::Vector2f worldPos = ScreenToWorld(mousePos);
    sf::Vector2f gridPos = GetGridPosition(worldPos);

    if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
    {
        if (IsValidGridPosition(gridPos))
        {
            SelectBlockAtPosition(gridPos);
        }
    }

    // LMJ: Property mode selection with number keys
    if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
        currentPropertyMode = PropertyMode::Destroyable;
    else if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
        currentPropertyMode = PropertyMode::Hidable;
    else if (InputMgr::GetKeyDown(sf::Keyboard::Num3))
        currentPropertyMode = PropertyMode::Movable;
    else if (InputMgr::GetKeyDown(sf::Keyboard::Num4))
        currentPropertyMode = PropertyMode::SpawnItem;

    if (selectedBlock != nullptr)
    {
        if (InputMgr::GetKeyDown(sf::Keyboard::Space))
        {
            ToggleSelectedBlockProperty(currentPropertyMode);
        }

        if (InputMgr::GetKeyDown(sf::Keyboard::T))
        {
            ModifySelectedBlockProperty(currentPropertyMode, true);
        }

        if (InputMgr::GetKeyDown(sf::Keyboard::F))
        {
            ModifySelectedBlockProperty(currentPropertyMode, false);
        }

        // LMJ: Block property copy/paste functionality
        static sf::Vector2f copySourcePos = sf::Vector2f(-1, -1);

        if (InputMgr::GetKeyDown(sf::Keyboard::C))
        {
            copySourcePos = selectedBlock->GetPosition();
        }

        if (InputMgr::GetKeyDown(sf::Keyboard::V) && copySourcePos.x >= 0)
        {
            CopyBlockProperties(copySourcePos, selectedBlock->GetPosition());
        }
    }
}

void MapEditor::HandleLayerSwitching()
{
    if (InputMgr::GetKeyDown(sf::Keyboard::Tab))
    {
        int currentLayerInt = static_cast<int>(currentLayer);
        currentLayerInt = (currentLayerInt + 1) % 4;
        currentLayer = static_cast<LayerType>(currentLayerInt);

        if (currentLayer != LayerType::BlockState)
        {
            selectedBlock = nullptr;
        }
    }
}

void MapEditor::HandleScrollInput()
{
    if (InputMgr::IsMouseWheelScrolled())
    {
        float delta = InputMgr::GetMouseWheelDelta();

        if (currentLayer == LayerType::Background)
        {
            if (!TileOptions.empty())
            {
                if (delta > 0)
                {
                    tileOptionIndex = (tileOptionIndex - 1 + TileOptions.size()) % TileOptions.size();
                }
                else
                {
                    tileOptionIndex = (tileOptionIndex + 1) % TileOptions.size();
                }
            }
        }
        else if (currentLayer == LayerType::Block)
        {
            int maxIndex = Block::GetBlockRegistrySize();
            if (maxIndex > 0)
            {
                if (delta > 0)
                {
                    blockRegistryIndex = (blockRegistryIndex - 1 + maxIndex) % maxIndex;
                }
                else
                {
                    blockRegistryIndex = (blockRegistryIndex + 1) % maxIndex;
                }
            }
        }
    }
}

void MapEditor::HandleSpawnPointInput()
{
    // LMJ: Switch between player 1 and player 2 with number keys
    if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
    {
        currentSpawnPlayerIndex = 0;  // Player 1
    }
    else if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
    {
        currentSpawnPlayerIndex = 1;  // Player 2
    }
    else if (InputMgr::GetKeyDown(sf::Keyboard::Num3))
    {
        currentSpawnPlayerIndex = 2;  // Player 3
    }
    else if (InputMgr::GetKeyDown(sf::Keyboard::Num4))
    {
        currentSpawnPlayerIndex = 3;  // Player 4
	}
}

void MapEditor::SelectBlockAtPosition(const sf::Vector2f& gridPos)
{
    Block* block = GetBlockAtPosition(gridPos);
    if (block != nullptr)
    {
        selectedBlock = block;
        sf::Vector2f worldPos = gridPos * static_cast<float>(GRID_SIZE);
        selectionHighlight.setPosition(worldPos);
    }
    else
    {
        selectedBlock = nullptr;
    }
}

void MapEditor::ModifySelectedBlockProperty(PropertyMode mode, bool value)
{
    if (selectedBlock == nullptr) return;

    switch (mode)
    {
    case PropertyMode::Destroyable:
        selectedBlock->SetDestroyable(value);
        break;
    case PropertyMode::Hidable:
        selectedBlock->SetHidable(value);
        break;
    case PropertyMode::Movable:
        selectedBlock->SetMovable(value);
        break;
    case PropertyMode::SpawnItem:
        selectedBlock->SetCanSpawnItem(value);
        break;
    }
}

void MapEditor::ToggleSelectedBlockProperty(PropertyMode mode)
{
    if (selectedBlock == nullptr) return;

    bool currentValue = false;
    switch (mode)
    {
    case PropertyMode::Destroyable:
        currentValue = selectedBlock->IsDestroyable();
        break;
    case PropertyMode::Hidable:
        currentValue = selectedBlock->IsHidable();
        break;
    case PropertyMode::Movable:
        currentValue = selectedBlock->IsMovable();
        break;
    case PropertyMode::SpawnItem:
        currentValue = selectedBlock->CanSpawnItem();
        break;
    }

    ModifySelectedBlockProperty(mode, !currentValue);
}

void MapEditor::CopyBlockProperties(const sf::Vector2f& sourcePos, const sf::Vector2f& targetPos)
{
    Block* sourceBlock = nullptr;
    Block* targetBlock = nullptr;

    // LMJ: Find source and target blocks by position
    for (Block* block : PlacedBlocks)
    {
        if (block && block->GetActive())
        {
            sf::Vector2f blockPos = block->GetPosition();
            if (std::abs(blockPos.x - sourcePos.x) < 1.0f && std::abs(blockPos.y - sourcePos.y) < 1.0f)
            {
                sourceBlock = block;
            }
            if (std::abs(blockPos.x - targetPos.x) < 1.0f && std::abs(blockPos.y - targetPos.y) < 1.0f)
            {
                targetBlock = block;
            }
        }
    }

    if (sourceBlock == nullptr || targetBlock == nullptr) return;

    // LMJ: Copy all properties from source to target
    targetBlock->SetBlockProperties(
        sourceBlock->IsDestroyable(),
        sourceBlock->IsHidable(),
        sourceBlock->IsMovable(),
        sourceBlock->CanSpawnItem()
    );
}

void MapEditor::CreateTileAtPosition(const sf::Vector2f& gridPos)
{
    if (TileOptions.empty() || !IsValidGridPosition(gridPos))
        return;

    int gx = static_cast<int>(gridPos.x);
    int gy = static_cast<int>(gridPos.y);

    // LMJ: Remove existing tile at position if any
    for (auto it = tileDatas.begin(); it != tileDatas.end(); ++it)
    {
        if (it->gridX == gx && it->gridY == gy)
        {
            tileDatas.erase(it);
            break;
        }
    }

    // LMJ: Add new tile at position
    tileDatas.emplace_back(tileOptionIndex, gx, gy, currentTileRotation);
}

void MapEditor::CreateBlockAtPosition(const sf::Vector2f& gridPos)
{
    if (!IsValidGridPosition(gridPos))
        return;

    // LMJ: Check if block already exists at this position
    Block* existingBlock = GetBlockAtPosition(gridPos);
    if (existingBlock)
        return;

    // LMJ: Calculate snapped position at grid center
    sf::Vector2f snappedPosition(
        gridPos.x * GRID_SIZE + GRID_SIZE / 2.0f,
        gridPos.y * GRID_SIZE + GRID_SIZE
    );

    // LMJ: Create new block using registry system
    Block* newBlock = Block::CreateBlockFromRegistry(blockRegistryIndex, snappedPosition);

    if (newBlock)
    {
        ApplyBlockDefaultScale(newBlock);

        newBlock->SetPosition(snappedPosition);
        PlacedBlocks.push_back(newBlock);
        newBlock->Init();
        newBlock->Reset();
    }
}

void MapEditor::DeleteTileAtPosition(const sf::Vector2f& gridPos)
{
    if (!IsValidGridPosition(gridPos))
        return;

    int gx = static_cast<int>(gridPos.x);
    int gy = static_cast<int>(gridPos.y);

    for (auto it = tileDatas.begin(); it != tileDatas.end(); ++it)
    {
        if (it->gridX == gx && it->gridY == gy)
        {
            tileDatas.erase(it);
            break;
        }
    }
}

void MapEditor::DeleteBlockAtPosition(const sf::Vector2f& gridPos)
{
    if (!IsValidGridPosition(gridPos))
        return;

    Block* blockToDelete = GetBlockAtPosition(gridPos);
    if (blockToDelete)
    {
        auto it = std::find(PlacedBlocks.begin(), PlacedBlocks.end(), blockToDelete);
        if (it != PlacedBlocks.end())
        {
            delete* it;
            PlacedBlocks.erase(it);

            // LMJ: Clear selection if deleted block was selected
            if (selectedBlock == blockToDelete)
            {
                selectedBlock = nullptr;
            }
        }
    }
}

sf::Vector2f MapEditor::GetGridPosition(const sf::Vector2f& mousePos)
{
    return sf::Vector2f(
        std::floor(mousePos.x / GRID_SIZE),
        std::floor(mousePos.y / GRID_SIZE)
    );
}

bool MapEditor::IsValidGridPosition(const sf::Vector2f& gridPos)
{
    return gridPos.x >= 0 && gridPos.x < GRID_WIDTH &&
        gridPos.y >= 0 && gridPos.y < GRID_HEIGHT;
}

Block* MapEditor::GetBlockAtPosition(const sf::Vector2f& gridPos)
{
    sf::Vector2f worldPos = sf::Vector2f(
        gridPos.x * GRID_SIZE + GRID_SIZE / 2,
        gridPos.y * GRID_SIZE + GRID_SIZE
    );

    for (Block* block : PlacedBlocks)
    {
        if (block && block->GetActive())
        {
            sf::Vector2f blockPos = block->GetPosition();
            if (std::abs(blockPos.x - worldPos.x) < 1.0f &&
                std::abs(blockPos.y - worldPos.y) < 1.0f)
            {
                return block;
            }
        }
    }

    return nullptr;
}

void MapEditor::ApplyBlockDefaultScale(Block* block)
{
    sf::Sprite& s = block->GetSprite();

    // LMJ: "Same scaling as tile preview (no extra scaling)"
    sf::IntRect rect = s.getTextureRect();
    float scaleX = static_cast<float>(GRID_SIZE) / rect.width;
    float scaleY = static_cast<float>(GRID_SIZE) / rect.height;
    //s.setScale(scaleX * 1.3f, scaleY * 1.3f);  // 1.3배만 적용
    Utils::SetOrigin(s, Origins::MC);
}

void MapEditor::DrawMapEditor(sf::RenderWindow& window)
{
    if (currentLayer == LayerType::Background)
    {
        DrawTilePreviewAtMouse(window);
    }
    else if (currentLayer == LayerType::Block)
    {
        DrawBlockPreview(window);
    }
    else if (currentLayer == LayerType::SpawnPoint)  // LMJ: Draw spawn point preview
    {
        DrawSpawnPointPreview(window);
    }
}

void MapEditor::DrawSpawnPoints(sf::RenderWindow& window)
{
    for (const auto& spawnPoint : spawnPoints)
    {
        sf::CircleShape point(25.0f);
        point.setFillColor(GetSpawnPointColor(spawnPoint.playerIndex));
        point.setOutlineColor(sf::Color::White);
        point.setOutlineThickness(2.0f);
        Utils::SetOrigin(point, Origins::MC);
        point.setPosition(spawnPoint.position);
        window.draw(point);

        // LMJ: Draw player number text
        sf::Text playerText;
        if (FONT_MGR.Exists("assets/font/Daum_Regular.ttf"))
        {
            playerText.setFont(FONT_MGR.Get("assets/font/Daum_Regular.ttf"));
            playerText.setCharacterSize(20);
            playerText.setFillColor(sf::Color::White);
            playerText.setString(std::to_string(spawnPoint.playerIndex + 1));
            Utils::SetOrigin(playerText, Origins::MC);
            playerText.setPosition(spawnPoint.position);
            window.draw(playerText);
        }
    }
}

sf::Color MapEditor::GetSpawnPointColor(int playerIndex) const
{
    switch (playerIndex)
    {
    case 0: return sf::Color::Cyan;    // Player 1
    case 1: return sf::Color::Magenta; // Player 2
	case 2: return sf::Color::Yellow;  // Player 3
	case 3: return sf::Color::Green;   // Player 4
    default: return sf::Color::White;
    }
}

void MapEditor::DrawTilePreviewAtMouse(sf::RenderWindow& window)
{
    if (TileOptions.empty())
        return;

    sf::Vector2i mousePos = InputMgr::GetMousePosition();
    sf::Vector2f worldPos = ScreenToWorld(mousePos);
    sf::Vector2f gridPos = GetGridPosition(worldPos);

    if (IsValidGridPosition(gridPos))
    {
        sf::Vector2f snappedPos(
            gridPos.x * GRID_SIZE + GRID_SIZE / 2.f,
            gridPos.y * GRID_SIZE + GRID_SIZE / 2.f
        );

        sf::Sprite currentTile = TileOptions[tileOptionIndex];
        currentTile.setPosition(snappedPos);
        currentTile.setRotation(currentTileRotation);
        currentTile.setColor(sf::Color(255, 255, 255, 150));
        window.draw(currentTile);

        sf::RectangleShape highlight;
        highlight.setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
        highlight.setPosition(gridPos.x * GRID_SIZE, gridPos.y * GRID_SIZE);
        highlight.setFillColor(sf::Color(255, 255, 0, 50));
        highlight.setOutlineColor(sf::Color::Yellow);
        highlight.setOutlineThickness(2.0f);
        window.draw(highlight);
    }
}

void MapEditor::DrawBlockPreview(sf::RenderWindow& window)
{
    if (BlockTextures.empty() || blockRegistryIndex >= static_cast<int>(BlockPreviewSprites.size()))
        return;

    sf::Vector2i mousePos = InputMgr::GetMousePosition();
    sf::Vector2f worldPos = ScreenToWorld(mousePos);
    sf::Vector2f gridPos = GetGridPosition(worldPos);

    if (IsValidGridPosition(gridPos))
    {
        // LMJ: "Snap preview to grid center like placed blocks"
        sf::Vector2f snappedPos(
            gridPos.x * GRID_SIZE + GRID_SIZE / 2.0f,
            gridPos.y * GRID_SIZE + GRID_SIZE / 2.0f
        );

        // LMJ: "Create preview sprite using existing BlockPreviewSprites"
        sf::Sprite preview = BlockPreviewSprites[blockRegistryIndex];
        preview.setPosition(snappedPos);
        //preview.setScale(preview.getScale().x/* * 1.3f*/, preview.getScale().y/* * 1.3f*/); // LMJ: "Apply same scale as placed blocks"
        preview.setColor(sf::Color(255, 255, 255, 180));

        window.draw(preview);

        // LMJ: "Draw grid highlight for block placement"
        sf::RectangleShape highlight;
        highlight.setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
        highlight.setPosition(gridPos.x * GRID_SIZE, gridPos.y * GRID_SIZE);
        highlight.setFillColor(sf::Color(0, 255, 0, 50));
        highlight.setOutlineColor(sf::Color::Green);
        highlight.setOutlineThickness(2.0f);
        window.draw(highlight);
    }
}

void MapEditor::DrawSpawnPointPreview(sf::RenderWindow& window)
{
    sf::Vector2i mousePos = InputMgr::GetMousePosition();
    sf::Vector2f worldPos = ScreenToWorld(mousePos);
    sf::Vector2f gridPos = GetGridPosition(worldPos);

    if (IsValidGridPosition(gridPos))
    {
        sf::Vector2f snappedPos(
            gridPos.x * GRID_SIZE + GRID_SIZE / 2.0f,
            gridPos.y * GRID_SIZE + GRID_SIZE / 2.0f
        );

        // LMJ: Draw spawn point preview circle
        sf::CircleShape preview(20.0f);
        preview.setFillColor(sf::Color(GetSpawnPointColor(currentSpawnPlayerIndex).r,
            GetSpawnPointColor(currentSpawnPlayerIndex).g,
            GetSpawnPointColor(currentSpawnPlayerIndex).b, 150));
        preview.setOutlineColor(GetSpawnPointColor(currentSpawnPlayerIndex));
        preview.setOutlineThickness(3.0f);
        Utils::SetOrigin(preview, Origins::MC);
        preview.setPosition(snappedPos);
        window.draw(preview);

        // LMJ: Draw grid highlight
        sf::RectangleShape highlight;
        highlight.setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
        highlight.setPosition(gridPos.x * GRID_SIZE, gridPos.y * GRID_SIZE);
        highlight.setFillColor(sf::Color(255, 0, 255, 50));
        highlight.setOutlineColor(sf::Color::Magenta);
        highlight.setOutlineThickness(2.0f);
        window.draw(highlight);
    }
}

void MapEditor::DrawBlockPropertyIndicators(sf::RenderWindow& window)
{
    if (currentLayer != LayerType::BlockState) return;

    const float indicatorSize = 8.0f;
    const float yOffset = 26.f;

    for (Block* block : PlacedBlocks)
    {
        if (block == nullptr || !block->GetActive()) continue;

        sf::Vector2f blockPos = block->GetPosition();

        // LMJ: Draw property indicators in block corners
        if (block->IsDestroyable())
        {
            sf::RectangleShape indicator;
            indicator.setSize(sf::Vector2f(indicatorSize, indicatorSize));
            indicator.setPosition(blockPos.x - GRID_SIZE / 2, blockPos.y - GRID_SIZE / 2 - yOffset);
            indicator.setFillColor(GetPropertyColor(PropertyMode::Destroyable, true));
            window.draw(indicator);
        }

        if (block->IsHidable())
        {
            sf::RectangleShape indicator;
            indicator.setSize(sf::Vector2f(indicatorSize, indicatorSize));
            indicator.setPosition(blockPos.x + GRID_SIZE / 2 - indicatorSize, blockPos.y - GRID_SIZE / 2 - yOffset);
            indicator.setFillColor(GetPropertyColor(PropertyMode::Hidable, true));
            window.draw(indicator);
        }

        if (block->IsMovable())
        {
            sf::RectangleShape indicator;
            indicator.setSize(sf::Vector2f(indicatorSize, indicatorSize));
            indicator.setPosition(blockPos.x - GRID_SIZE / 2, blockPos.y + GRID_SIZE / 2 - indicatorSize - yOffset);
            indicator.setFillColor(GetPropertyColor(PropertyMode::Movable, true));
            window.draw(indicator);
        }

        if (block->CanSpawnItem())
        {
            sf::RectangleShape indicator;
            indicator.setSize(sf::Vector2f(indicatorSize, indicatorSize));
            indicator.setPosition(blockPos.x + GRID_SIZE / 2 - indicatorSize, blockPos.y + GRID_SIZE / 2 - indicatorSize - yOffset);
            indicator.setFillColor(GetPropertyColor(PropertyMode::SpawnItem, true));
            window.draw(indicator);
        }
    }
}

void MapEditor::DrawRightSideUI(sf::RenderWindow& window)
{
    // LMJ: "UI positioning"
    const float rightPanelX = GRID_WIDTH * 40.0f + 20;
    const float rightPanelWidth = 160;

    // LMJ: "Draw UI background panel"
    sf::RectangleShape uiBackground;
    uiBackground.setSize(sf::Vector2f(rightPanelWidth, 580));
    uiBackground.setPosition(rightPanelX, 10);
    uiBackground.setFillColor(sf::Color(20, 20, 20, 200));
    uiBackground.setOutlineColor(sf::Color::White);
    uiBackground.setOutlineThickness(2);
    window.draw(uiBackground);

    // LMJ: "Draw preview section background"
    const float previewSize = 80;
    sf::RectangleShape previewBg;
    previewBg.setSize(sf::Vector2f(previewSize, previewSize));
    previewBg.setPosition(rightPanelX + 40, 20);
    previewBg.setFillColor(sf::Color(50, 50, 50, 180));
    previewBg.setOutlineColor(sf::Color::White);
    previewBg.setOutlineThickness(2);
    window.draw(previewBg);

    // LMJ: "Draw preview content based on current layer"
    if (currentLayer == LayerType::Background && tileOptionIndex < TileOptions.size())
    {
        sf::Sprite currentTile = TileOptions[tileOptionIndex];
        currentTile.setPosition(rightPanelX + 80, 60);
        currentTile.setRotation(currentTileRotation);
        currentTile.setScale(1.3f, 1.3f);
        currentTile.setColor(sf::Color::White);
        window.draw(currentTile);
    }
    else if (currentLayer == LayerType::Block && blockRegistryIndex < Block::GetBlockRegistrySize())
    {
        // LMJ: "Draw block preview from sprite sheet"
        if (!BlockTextures.empty() && blockRegistryIndex < static_cast<int>(BlockPreviewSprites.size()))
        {
            sf::Sprite blockPreview = BlockPreviewSprites[blockRegistryIndex];
            blockPreview.setPosition(rightPanelX + 80, 60);
            blockPreview.setScale(1.2f, 1.2f); // LMJ: "Adjust scale for UI preview"
            blockPreview.setColor(sf::Color::White);
            window.draw(blockPreview);
        }
    }
    else if (currentLayer == LayerType::SpawnPoint)
    {
        sf::CircleShape spawnPreview(30.0f);
        spawnPreview.setFillColor(GetSpawnPointColor(currentSpawnPlayerIndex));
        spawnPreview.setOutlineColor(sf::Color::White);
        spawnPreview.setOutlineThickness(2.0f);
        Utils::SetOrigin(spawnPreview, Origins::MC);
        spawnPreview.setPosition(rightPanelX + 80, 60);
        window.draw(spawnPreview);

        if (FONT_MGR.Exists("assets/font/Daum_Regular.ttf"))
        {
            sf::Text playerText;
            playerText.setFont(FONT_MGR.Get("assets/font/Daum_Regular.ttf"));
            playerText.setCharacterSize(20);
            playerText.setFillColor(sf::Color::White);
            playerText.setString(std::to_string(currentSpawnPlayerIndex + 1));  // 1-4
            Utils::SetOrigin(playerText, Origins::MC);
            playerText.setPosition(rightPanelX + 80, 60);
            window.draw(playerText);
        }
    }

    // LMJ: "Draw information sections"
    DrawLayerInfo(window, rightPanelX, 110);
    DrawControlsInfo(window, rightPanelX, 190);

    if (currentLayer == LayerType::BlockState)
    {
        DrawLayer2Info(window, rightPanelX, 320);
    }
    else if (currentLayer == LayerType::SpawnPoint)  // LMJ: "Draw spawn point info"
    {
        DrawSpawnPointInfo(window, rightPanelX, 320);
    }
}

void MapEditor::DrawSaveLoadInfo(sf::RenderWindow& window)
{
    sf::Text Text;
    Utils::SetOrigin(Text, Origins::BL);
    Text.setFont(FONT_MGR.Get("assets/font/Daum_Regular.ttf"));
    Text.setCharacterSize(25);
    Text.setFillColor(sf::Color::Magenta);
    Text.setString(L"저장: LCtrl + S\n불러오기: LCtrl + O");

    sf::View currentView = window.getView();
    sf::Vector2f viewSize = currentView.getSize();
    sf::Vector2f viewCenter = currentView.getCenter();

    const float margin = 70.f;
    sf::Vector2f textPos;
    textPos.x = viewCenter.x - viewSize.x * 0.5f + 10.f;
    textPos.y = viewCenter.y + viewSize.y * 0.5f - margin;

    Text.setPosition(textPos);

    window.draw(Text);
}

void MapEditor::DrawLayerInfo(sf::RenderWindow& window, float x, float y)
{
    if (!FONT_MGR.Exists("assets/font/Daum_Regular.ttf"))
        return;

    sf::Text layerText;
    layerText.setFont(FONT_MGR.Get("assets/font/Daum_Regular.ttf"));
    layerText.setCharacterSize(13);
    layerText.setFillColor(sf::Color::White);
    layerText.setPosition(x + 10, y);

    std::wstring layerInfo = L"레이어: ";
    switch (currentLayer)
    {
    case LayerType::Background:
        layerInfo += L"배경\n타일: " + std::to_wstring(tileOptionIndex + 1) + L"/" + std::to_wstring(TileOptions.size());
        layerInfo += L"\n회전: " + std::to_wstring(static_cast<int>(currentTileRotation)) + L"°";
        break;
    case LayerType::Block:
        layerInfo += L"블록\n블록: " + std::to_wstring(blockRegistryIndex + 1) + L"/" + std::to_wstring(Block::GetBlockRegistrySize());
        break;
    case LayerType::BlockState:
        layerInfo += L"속성\n모드: " + GetPropertyModeString(currentPropertyMode);
        break;
    case LayerType::SpawnPoint:
        layerInfo += L"스폰\n플레이어: " + std::to_wstring(currentSpawnPlayerIndex + 1) + L"/4";  // LMJ: 4P 표시
        break;
    }

    layerText.setString(layerInfo);
    window.draw(layerText);
}

void MapEditor::DrawControlsInfo(sf::RenderWindow& window, float x, float y)
{
    if (!FONT_MGR.Exists("assets/font/Daum_Regular.ttf"))
        return;

    sf::Text controlsText;
    controlsText.setFont(FONT_MGR.Get("assets/font/Daum_Regular.ttf"));
    controlsText.setCharacterSize(11);
    controlsText.setFillColor(sf::Color::Cyan);
    controlsText.setPosition(x + 10, y);

    std::wstring controls;
    if (currentLayer == LayerType::Background)
    {
        controls = L"=== 조작법 ===\n";
        controls += L"Tab: 레이어 전환\n";
        controls += L"마우스 휠: 타일 변경\n";
        controls += L"Q / E: 타일 회전\n";
        controls += L"좌 클릭: 배치\n";
        controls += L"우 클릭: 삭제";
    }
    else if (currentLayer == LayerType::Block)
    {
        controls = L"=== 조작법 ===\n";
        controls += L"Tab: 레이어 전환\n";
        controls += L"마우스 휠: 블록 변경\n";
        controls += L"좌 클릭: 배치\n";
        controls += L"우 클릭: 삭제";
    }
    else if (currentLayer == LayerType::BlockState)
    {
        controls = L"=== 레이어 2 ===\n";
        controls += L"Tab: 레이어 전환\n";
        controls += L"좌 클릭: 블록 선택\n";
        controls += L"1 ~ 4: 속성 선택\n";
        controls += L"스페이스: 토글\n";
        controls += L"T / F: 참/거짓\n";
        controls += L"C / V: 복사/붙여넣기";
    }
    else if (currentLayer == LayerType::SpawnPoint)
    {
        controls = L"=== 스폰 포인트 ===\n";
        controls += L"Tab: 레이어 전환\n";
        controls += L"1~4: 플레이어 선택\n";        // LMJ: 1-4키로 확장
        controls += L"좌 클릭: 스폰 포인트 배치\n";
        controls += L"우 클릭: 스폰 포인트 삭제\n";
        controls += L"※ 플레이어당 1개만 가능";
    }

    controlsText.setString(controls);
    window.draw(controlsText);
}

void MapEditor::DrawLayer2Info(sf::RenderWindow& window, float x, float y)
{
    if (!FONT_MGR.Exists("assets/font/Daum_Regular.ttf"))
        return;

    sf::Text propertyText;
    propertyText.setFont(FONT_MGR.Get("assets/font/Daum_Regular.ttf"));
    propertyText.setCharacterSize(11);
    propertyText.setPosition(x + 10, y);
    propertyText.setString(L"=== 속성 모드 ===");
    propertyText.setFillColor(sf::Color::White);
    window.draw(propertyText);

    // LMJ: Draw property mode options with visual indicators
    std::vector<std::wstring> modeNames = { L"1:파괴 설정", L"2:은신 설정", L"3:이동 설정", L"4:아이템 소환 설정" };
    std::vector<PropertyMode> modes = { PropertyMode::Destroyable, PropertyMode::Hidable, PropertyMode::Movable, PropertyMode::SpawnItem };

    for (int i = 0; i < 4; ++i)
    {
        float modeY = y + 20 + i * 16;

        sf::RectangleShape colorIndicator;
        colorIndicator.setSize(sf::Vector2f(12, 12));
        colorIndicator.setPosition(x + 10, modeY);
        colorIndicator.setFillColor(GetPropertyColor(modes[i], true));
        colorIndicator.setOutlineColor(currentPropertyMode == modes[i] ? sf::Color::White : sf::Color::Black);
        colorIndicator.setOutlineThickness(currentPropertyMode == modes[i] ? 2 : 1);
        window.draw(colorIndicator);

        sf::Text modeText;
        modeText.setFont(FONT_MGR.Get("assets/font/Daum_Regular.ttf"));
        modeText.setCharacterSize(10);
        modeText.setFillColor(currentPropertyMode == modes[i] ? sf::Color::Yellow : sf::Color::White);
        modeText.setPosition(x + 25, modeY);
        modeText.setString(modeNames[i]);
        window.draw(modeText);
    }

    // LMJ: Display selected block information
    if (selectedBlock != nullptr)
    {
        sf::Text selectedText;
        selectedText.setFont(FONT_MGR.Get("assets/font/Daum_Regular.ttf"));
        selectedText.setCharacterSize(10);
        selectedText.setFillColor(sf::Color::Yellow);
        selectedText.setPosition(x + 10, y + 90);

        std::wstring selectedInfo = L"=== 선택된 블록 ===\n";
        selectedInfo += L"파괴: " + std::wstring(selectedBlock->IsDestroyable() ? L"켜짐" : L"꺼짐") + L"\n";
        selectedInfo += L"은신: " + std::wstring(selectedBlock->IsHidable() ? L"켜짐" : L"꺼짐") + L"\n";
        selectedInfo += L"이동: " + std::wstring(selectedBlock->IsMovable() ? L"켜짐" : L"꺼짐") + L"\n";
        selectedInfo += L"아이템 소환: " + std::wstring(selectedBlock->CanSpawnItem() ? L"켜짐" : L"꺼짐");

        selectedText.setString(selectedInfo);
        window.draw(selectedText);
    }
}

void MapEditor::DrawSpawnPointInfo(sf::RenderWindow& window, float x, float y)
{
    if (!FONT_MGR.Exists("assets/font/Daum_Regular.ttf"))
        return;

    sf::Text spawnInfoText;
    spawnInfoText.setFont(FONT_MGR.Get("assets/font/Daum_Regular.ttf"));
    spawnInfoText.setCharacterSize(11);
    spawnInfoText.setFillColor(sf::Color::White);
    spawnInfoText.setPosition(x + 10, y);
    spawnInfoText.setString(L"=== 스폰 포인트 ===");
    window.draw(spawnInfoText);

    // LMJ: Show current spawn points for all 4 players
    std::wstring spawnInfo = L"배치된 스폰 포인트:\n";
    bool hasPlayer[4] = { false, false, false, false };  // LMJ: 4P 추적

    for (const auto& spawn : spawnPoints)
    {
        if (spawn.playerIndex >= 0 && spawn.playerIndex < 4)
        {
            hasPlayer[spawn.playerIndex] = true;
        }
    }

    // LMJ: 각 플레이어별 상태 표시
    spawnInfo += L"플레이어 1: " + std::wstring(hasPlayer[0] ? L"배치됨" : L"미배치") + L"\n";
    spawnInfo += L"플레이어 2: " + std::wstring(hasPlayer[1] ? L"배치됨" : L"미배치") + L"\n";
    spawnInfo += L"플레이어 3: " + std::wstring(hasPlayer[2] ? L"배치됨" : L"미배치") + L"\n";
    spawnInfo += L"플레이어 4: " + std::wstring(hasPlayer[3] ? L"배치됨" : L"미배치") + L"\n\n";
    spawnInfo += L"현재 선택: 플레이어 " + std::to_wstring(currentSpawnPlayerIndex + 1);

    sf::Text detailText;
    detailText.setFont(FONT_MGR.Get("assets/font/Daum_Regular.ttf"));
    detailText.setCharacterSize(10);
    detailText.setFillColor(sf::Color::Yellow);
    detailText.setPosition(x + 10, y + 20);
    detailText.setString(spawnInfo);
    window.draw(detailText);
}

std::wstring MapEditor::GetPropertyModeString(PropertyMode mode) const
{
    switch (mode)
    {
    case PropertyMode::Destroyable: return L"파괴 가능";
    case PropertyMode::Hidable: return L"은신 가능";
    case PropertyMode::Movable: return L"이동 가능";
    case PropertyMode::SpawnItem: return L"아이템 생성";
    default: return L"알 수 없음";
    }
}

sf::Color MapEditor::GetPropertyColor(PropertyMode mode, bool enabled) const
{
    if (!enabled) return sf::Color(64, 64, 64);

    switch (mode)
    {
    case PropertyMode::Destroyable: return sf::Color::Red;
    case PropertyMode::Hidable: return sf::Color::Blue;
    case PropertyMode::Movable: return sf::Color::Green;
    case PropertyMode::SpawnItem: return sf::Color::Yellow;
    default: return sf::Color::White;
    }
}

void MapEditor::CreateSpawnPointAtPosition(const sf::Vector2f& gridPos)
{
    if (!IsValidGridPosition(gridPos))
        return;

    sf::Vector2f worldPos = sf::Vector2f(
        gridPos.x * GRID_SIZE + GRID_SIZE / 2.0f,
        gridPos.y * GRID_SIZE + GRID_SIZE / 2.0f
    );

    // LMJ: Remove existing spawn point for this player
    for (auto it = spawnPoints.begin(); it != spawnPoints.end(); ++it)
    {
        if (it->playerIndex == currentSpawnPlayerIndex)
        {
            spawnPoints.erase(it);
            break;
        }
    }

    // LMJ: Add new spawn point
    spawnPoints.emplace_back(currentSpawnPlayerIndex, worldPos);
}

void MapEditor::DeleteSpawnPointAtPosition(const sf::Vector2f& gridPos)
{
    if (!IsValidGridPosition(gridPos))
        return;

    SpawnPointData* spawnPoint = GetSpawnPointAtPosition(gridPos);
    if (spawnPoint)
    {
        auto it = std::find_if(spawnPoints.begin(), spawnPoints.end(),
            [spawnPoint](const SpawnPointData& sp) {
                return &sp == spawnPoint;
            });

        if (it != spawnPoints.end())
        {
            spawnPoints.erase(it);
        }
    }
}

SpawnPointData* MapEditor::GetSpawnPointAtPosition(const sf::Vector2f& gridPos)
{
    sf::Vector2f worldPos = sf::Vector2f(
        gridPos.x * GRID_SIZE + GRID_SIZE / 2.0f,
        gridPos.y * GRID_SIZE + GRID_SIZE / 2.0f
    );

    for (auto& spawnPoint : spawnPoints)
    {
        if (std::abs(spawnPoint.position.x - worldPos.x) < 1.0f &&
            std::abs(spawnPoint.position.y - worldPos.y) < 1.0f)
        {
            return &spawnPoint;
        }
    }

    return nullptr;
}

void MapEditor::SaveMapToJson(const std::string& filename) const
{
    try
    {
        json jMap;

        // LMJ: Save tile data to JSON array
        jMap["tiles"] = json::array();
        for (const auto& tile : tileDatas)
        {
            jMap["tiles"].push_back(tile.ToJson());
        }

        // LMJ: Save block data to JSON array
        jMap["blocks"] = json::array();
        for (const Block* block : PlacedBlocks)
        {
            if (block && block->GetActive())
            {
                int registryIndex = block->GetRegistryIndex();
                jMap["blocks"].push_back(Block::ToJson(block, registryIndex));
            }
        }

        // LMJ: Save spawn points to JSON array
        jMap["spawnPoints"] = json::array();
        for (const auto& spawnPoint : spawnPoints)
        {
            jMap["spawnPoints"].push_back(spawnPoint.ToJson());
        }

        // LMJ: Write to file with proper error handling
        std::ofstream ofs(filename);
        if (ofs.is_open())
        {
            ofs << jMap.dump(4); // LMJ: Pretty print with 4-space indentation
            ofs.close();
        }
    }
    catch (const std::exception& e)
    {
        // Error handling
    }
}

void MapEditor::LoadMapFromJson(const std::string& filename)
{
    std::ifstream ifs(filename);
    if (!ifs.is_open())
    {
        return;
    }

    try
    {
        json jMap;
        ifs >> jMap;
        ifs.close();

        // LMJ: Clear existing map data before loading
        tileDatas.clear();
        for (Block* block : PlacedBlocks)
        {
            delete block;
        }
        PlacedBlocks.clear();
        spawnPoints.clear();  // LMJ: Clear spawn points
        selectedBlock = nullptr;

        // LMJ: Load tile data from JSON
        if (jMap.contains("tiles") && jMap["tiles"].is_array())
        {
            for (const auto& jTile : jMap["tiles"])
            {
                tileDatas.push_back(TileData::FromJson(jTile));
            }
        }

        // LMJ: Load block data from JSON
        if (jMap.contains("blocks") && jMap["blocks"].is_array())
        {
            for (const auto& jBlock : jMap["blocks"])
            {
                Block* block = Block::FromJson(jBlock);
                if (!block) continue;

                PlacedBlocks.push_back(block);
                block->Init();
                ApplyBlockDefaultScale(block);
                block->Reset();
            }
        }

        // LMJ: Load spawn points from JSON
        if (jMap.contains("spawnPoints") && jMap["spawnPoints"].is_array())
        {
            for (const auto& jSpawn : jMap["spawnPoints"])
            {
                spawnPoints.push_back(SpawnPointData::FromJson(jSpawn));
            }
        }
    }
    catch (const std::exception& e)
    {
        // Error handling
    }
}