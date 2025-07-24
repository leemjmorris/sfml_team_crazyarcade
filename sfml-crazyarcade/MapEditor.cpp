#include "stdafx.h"
#include "MapEditor.h"

MapEditor::MapEditor() : Scene(SceneIds::MapEditor)
{
    currentLayer = LayerType::Background;
    tileOptionIndex = 0;
    blockRegistryIndex = 0;
    currentPropertyMode = PropertyMode::Destroyable;
    selectedBlock = nullptr;
    currentTileRotation = 0.0f;

    // LMJ: "Initialize selection highlight"
    selectionHighlight.setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
    selectionHighlight.setFillColor(sf::Color::Transparent);
    selectionHighlight.setOutlineColor(sf::Color::Yellow);
    selectionHighlight.setOutlineThickness(3.0f);
}

void MapEditor::Init()
{
    Scene::Init();

    sf::Vector2f viewSize(800, 600);
    worldView.setSize(viewSize);
    worldView.setCenter(viewSize * 0.5f);
    uiView.setSize(viewSize);
    uiView.setCenter(viewSize * 0.5f);

    // LMJ: "Load font separately - NOT in texIds"
    fontIds.push_back("assets/font/Daum_Regular.ttf");

    // LMJ: "Load block textures only"
    auto allBlocks = Block::GetAllBlocks();
    for (const auto& blockInfo : allBlocks)
    {
        texIds.push_back(blockInfo.textureId);
    }

    LoadTileSet();
    LoadBlockSet();

    // LMJ: "Initialize grid lines"
    gridLines.setPrimitiveType(sf::Lines);
    gridLines.clear();

    sf::Color gridColor(255, 255, 255, 100);
    for (int x = 0; x <= GRID_WIDTH; ++x)
    {
        float xPos = x * GRID_SIZE;
        gridLines.append(sf::Vertex(sf::Vector2f(xPos, 0), gridColor));
        gridLines.append(sf::Vertex(sf::Vector2f(xPos, GRID_HEIGHT * GRID_SIZE), gridColor));
    }
    for (int y = 0; y <= GRID_HEIGHT; ++y)
    {
        float yPos = y * GRID_SIZE;
        gridLines.append(sf::Vertex(sf::Vector2f(0, yPos), gridColor));
        gridLines.append(sf::Vertex(sf::Vector2f(GRID_WIDTH * GRID_SIZE, yPos), gridColor));
    }
}

void MapEditor::Release()
{
    for (Block* block : PlacedBlocks)
    {
        if (block)
        {
            delete block;
        }
    }
    PlacedBlocks.clear();
    Scene::Release();
}

void MapEditor::Enter()
{
    Scene::Enter();
}

void MapEditor::Exit()
{
    Scene::Exit();
}

void MapEditor::LoadTileSet()
{
    std::string paths[] = {
        "assets/map/forest/tile/forest_tile_set.png",
        "../assets/map/forest/tile/forest_tile_set.png",
        "assets/map/forest/tile_1.bmp",
        "assets/map/forest/tile_2.bmp"
    };

    bool success = false;
    for (const auto& path : paths)
    {
        if (tileMapTexture.loadFromFile(path))
        {
            success = true;
            break;
        }
    }

    if (!success) return;

    sf::Vector2u textureSize = tileMapTexture.getSize();
    float tile_width = textureSize.x / 5.0f;
    float tile_height = textureSize.y / 2.0f;

    TileOptions.clear();
    for (int y = 0; y < 2; ++y)
    {
        for (int x = 0; x < 5; ++x)
        {
            sf::Sprite tile;
            tile.setTexture(tileMapTexture);
            tile.setTextureRect(sf::IntRect(
                static_cast<int>(x * tile_width),
                static_cast<int>(y * tile_height),
                static_cast<int>(tile_width),
                static_cast<int>(tile_height)
            ));

            float scaleX = GRID_SIZE / tile_width;
            float scaleY = GRID_SIZE / tile_height;
            tile.setScale(sf::Vector2f(scaleX, scaleY));
            tile.setOrigin(sf::Vector2f(tile_width / 2, tile_height / 2));

            TileOptions.push_back(tile);
        }
    }
}

void MapEditor::LoadBlockSet()
{
    BlockPreviewSprites.clear();

    auto allBlocks = Block::GetAllBlocks();
    for (const auto& blockInfo : allBlocks)
    {
        if (TEXTURE_MGR.Exists(blockInfo.textureId))
        {
            sf::Sprite blockSprite;
            blockSprite.setTexture(TEXTURE_MGR.Get(blockInfo.textureId));
            Utils::SetOrigin(blockSprite, Origins::MC);
            BlockPreviewSprites.push_back(blockSprite);
        }
    }
}

void MapEditor::Update(float dt)
{
    Scene::Update(dt);
    HandleInput();

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
    // LMJ: "Draw world content"
    window.setView(worldView);

    window.draw(gridLines);

    sf::RectangleShape outline;
    outline.setSize(sf::Vector2f(GRID_WIDTH * GRID_SIZE, GRID_HEIGHT * GRID_SIZE));
    outline.setPosition(0, 0);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineColor(sf::Color::Yellow);
    outline.setOutlineThickness(3.f);
    window.draw(outline);

    for (const sf::Sprite& tile : Tiles)
    {
        window.draw(tile);
    }

    for (Block* block : PlacedBlocks)
    {
        if (block && block->GetActive())
        {
            block->Draw(window);
        }
    }

    DrawMapEditor(window);

    if (currentLayer == LayerType::BlockState && selectedBlock != nullptr)
    {
        window.draw(selectionHighlight);
    }

    DrawBlockPropertyIndicators(window);

    // LMJ: "Draw UI content"
    window.setView(uiView);
    DrawRightSideUI(window);
    Scene::Draw(window);
}

void MapEditor::HandleInput()
{
    HandleLayerSwitching();
    HandleScrollInput();

    if (currentLayer == LayerType::Background)
    {
        HandleTileRotation();
    }

    if (currentLayer == LayerType::BlockState)
    {
        HandleLayer2Input();
        return;
    }

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
        currentLayerInt = (currentLayerInt + 1) % 3;
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

    sf::Vector2f tilePosition(gridPos.x * GRID_SIZE + GRID_SIZE / 2.0f, gridPos.y * GRID_SIZE + GRID_SIZE / 2.0f);

    for (auto it = Tiles.begin(); it != Tiles.end(); ++it)
    {
        sf::Vector2f tilePos = it->getPosition();
        if (std::abs(tilePos.x - tilePosition.x) < GRID_SIZE / 2 &&
            std::abs(tilePos.y - tilePosition.y) < GRID_SIZE / 2)
        {
            Tiles.erase(it);
            break;
        }
    }

    sf::Sprite tile = TileOptions[tileOptionIndex];
    tile.setPosition(tilePosition);
    tile.setRotation(currentTileRotation);
    Tiles.push_back(tile);
}

void MapEditor::CreateBlockAtPosition(const sf::Vector2f& gridPos)
{
    if (!IsValidGridPosition(gridPos))
        return;

    Block* existingBlock = GetBlockAtPosition(gridPos);
    if (existingBlock)
        return;

    Block* newBlock = Block::CreateBlockFromRegistry(blockRegistryIndex,
        sf::Vector2f(gridPos.x * GRID_SIZE + GRID_SIZE / 2, gridPos.y * GRID_SIZE + GRID_SIZE / 2));

    if (newBlock)
    {
        PlacedBlocks.push_back(newBlock);
        newBlock->Init();
        newBlock->Reset();
    }
}

void MapEditor::DeleteTileAtPosition(const sf::Vector2f& gridPos)
{
    if (!IsValidGridPosition(gridPos))
        return;

    sf::Vector2f tilePosition(gridPos.x * GRID_SIZE + GRID_SIZE / 2.0f, gridPos.y * GRID_SIZE + GRID_SIZE / 2.0f);

    for (auto it = Tiles.begin(); it != Tiles.end(); ++it)
    {
        sf::Vector2f tilePos = it->getPosition();
        if (std::abs(tilePos.x - tilePosition.x) < GRID_SIZE / 2 &&
            std::abs(tilePos.y - tilePosition.y) < GRID_SIZE / 2)
        {
            Tiles.erase(it);
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
    sf::Vector2f worldPos = sf::Vector2f(gridPos.x * GRID_SIZE + GRID_SIZE / 2, gridPos.y * GRID_SIZE + GRID_SIZE / 2);

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
        sf::Vector2f snappedPos(gridPos.x * GRID_SIZE + GRID_SIZE / 2.f, gridPos.y * GRID_SIZE + GRID_SIZE / 2.f);

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
    if (BlockPreviewSprites.empty() || blockRegistryIndex >= static_cast<int>(BlockPreviewSprites.size()))
        return;

    sf::Vector2i mousePos = InputMgr::GetMousePosition();
    sf::Vector2f worldPos = ScreenToWorld(mousePos);
    sf::Vector2f gridPos = GetGridPosition(worldPos);

    if (IsValidGridPosition(gridPos))
    {
        sf::Sprite preview = BlockPreviewSprites[blockRegistryIndex];
        preview.setPosition(gridPos.x * GRID_SIZE + GRID_SIZE / 2, gridPos.y * GRID_SIZE + GRID_SIZE / 2);
        preview.setColor(sf::Color(255, 255, 255, 128));
        window.draw(preview);

        sf::RectangleShape highlight;
        highlight.setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
        highlight.setPosition(gridPos.x * GRID_SIZE, gridPos.y * GRID_SIZE);
        highlight.setFillColor(sf::Color(0, 255, 0, 50));
        highlight.setOutlineColor(sf::Color::Green);
        highlight.setOutlineThickness(2.0f);
        window.draw(highlight);
    }
}

void MapEditor::DrawBlockPropertyIndicators(sf::RenderWindow& window)
{
    if (currentLayer != LayerType::BlockState) return;

    for (Block* block : PlacedBlocks)
    {
        if (block == nullptr || !block->GetActive()) continue;

        sf::Vector2f blockPos = block->GetPosition();
        float indicatorSize = 8.0f;

        if (block->IsDestroyable())
        {
            sf::RectangleShape indicator;
            indicator.setSize(sf::Vector2f(indicatorSize, indicatorSize));
            indicator.setPosition(blockPos.x - GRID_SIZE / 2, blockPos.y - GRID_SIZE / 2);
            indicator.setFillColor(GetPropertyColor(PropertyMode::Destroyable, true));
            window.draw(indicator);
        }

        if (block->IsHidable())
        {
            sf::RectangleShape indicator;
            indicator.setSize(sf::Vector2f(indicatorSize, indicatorSize));
            indicator.setPosition(blockPos.x + GRID_SIZE / 2 - indicatorSize, blockPos.y - GRID_SIZE / 2);
            indicator.setFillColor(GetPropertyColor(PropertyMode::Hidable, true));
            window.draw(indicator);
        }

        if (block->IsMovable())
        {
            sf::RectangleShape indicator;
            indicator.setSize(sf::Vector2f(indicatorSize, indicatorSize));
            indicator.setPosition(blockPos.x - GRID_SIZE / 2, blockPos.y + GRID_SIZE / 2 - indicatorSize);
            indicator.setFillColor(GetPropertyColor(PropertyMode::Movable, true));
            window.draw(indicator);
        }

        if (block->CanSpawnItem())
        {
            sf::RectangleShape indicator;
            indicator.setSize(sf::Vector2f(indicatorSize, indicatorSize));
            indicator.setPosition(blockPos.x + GRID_SIZE / 2 - indicatorSize, blockPos.y + GRID_SIZE / 2 - indicatorSize);
            indicator.setFillColor(GetPropertyColor(PropertyMode::SpawnItem, true));
            window.draw(indicator);
        }
    }
}

void MapEditor::DrawRightSideUI(sf::RenderWindow& window)
{
    float rightPanelX = GRID_WIDTH * GRID_SIZE + 20;
    float rightPanelWidth = 160;

    sf::RectangleShape uiBackground;
    uiBackground.setSize(sf::Vector2f(rightPanelWidth, 580));
    uiBackground.setPosition(rightPanelX, 10);
    uiBackground.setFillColor(sf::Color(20, 20, 20, 200));
    uiBackground.setOutlineColor(sf::Color::White);
    uiBackground.setOutlineThickness(2);
    window.draw(uiBackground);

    float previewSize = 80;
    sf::RectangleShape previewBg;
    previewBg.setSize(sf::Vector2f(previewSize, previewSize));
    previewBg.setPosition(rightPanelX + 40, 20);
    previewBg.setFillColor(sf::Color(50, 50, 50, 180));
    previewBg.setOutlineColor(sf::Color::White);
    previewBg.setOutlineThickness(2);
    window.draw(previewBg);

    // LMJ: "Draw preview content with rotation"
    if (currentLayer == LayerType::Background && tileOptionIndex < TileOptions.size())
    {
        sf::Sprite currentTile = TileOptions[tileOptionIndex];
        currentTile.setPosition(rightPanelX + 80, 60);
        currentTile.setRotation(currentTileRotation);
        currentTile.setScale(1.2f, 1.2f);
        currentTile.setColor(sf::Color::White);
        window.draw(currentTile);
    }
    else if (currentLayer == LayerType::Block && blockRegistryIndex < Block::GetBlockRegistrySize())
    {
        BlockInfo blockInfo = Block::GetBlockInfo(blockRegistryIndex);
        if (!blockInfo.textureId.empty() && TEXTURE_MGR.Exists(blockInfo.textureId))
        {
            sf::Sprite blockPreview;
            blockPreview.setTexture(TEXTURE_MGR.Get(blockInfo.textureId));
            Utils::SetOrigin(blockPreview, Origins::MC);
            blockPreview.setPosition(rightPanelX + 80, 60);
            blockPreview.setColor(sf::Color::White);
            window.draw(blockPreview);
        }
    }

    DrawLayerInfo(window, rightPanelX, 110);
    DrawControlsInfo(window, rightPanelX, 190);

    if (currentLayer == LayerType::BlockState)
    {
        DrawLayer2Info(window, rightPanelX, 320);
    }
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