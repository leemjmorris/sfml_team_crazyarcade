#include "stdafx.h"
#include "MapEditor.h"

MapEditor::MapEditor() : Scene(SceneIds::MapEditor)
{
    currentLayer = LayerType::Background;
    tileOptionIndex = 0;
    blockRegistryIndex = 0;
    currentPropertyMode = PropertyMode::Destroyable;
    MouseScrollInput = ScrollInput::None;
    selectedBlock = nullptr;

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

    std::cout << "MapEditor::Init() called" << std::endl;

    // LMJ: "Add font to texture loading"
    texIds.push_back("assets/font/Daum_Regular.ttf");
    fontIds.push_back("assets/font/Daum_Regular.ttf");

    // LMJ: "Load block textures from registry"
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
    // LMJ: "Clean up placed blocks"
    for (Block* block : PlacedBlocks)
    {
        if (block)
        {
            delete block;
        }
    }
    PlacedBlocks.clear();

    Scene::Release();
    std::cout << "MapEditor::Release() called" << std::endl;
}

void MapEditor::Enter()
{
    Scene::Enter();
    std::cout << "MapEditor::Enter() called" << std::endl;
}

void MapEditor::Exit()
{
    Scene::Exit();
    std::cout << "MapEditor::Exit() called" << std::endl;
}

void MapEditor::LoadTileSet()
{
    std::cout << "Loading tileset..." << std::endl;

    bool success = false;
    std::string paths[] = {
        "assets/map/forest/tile/forest_tile_set.png",
        "../assets/map/forest/tile/forest_tile_set.png",
        "assets/map/forest/tile_1.bmp",
        "assets/map/forest/tile_2.bmp"
    };

    for (const auto& path : paths)
    {
        if (tileMapTexture.loadFromFile(path))
        {
            std::cout << "Loaded texture from: " << path << std::endl;
            success = true;
            break;
        }
    }

    if (!success)
    {
        std::cout << "All texture loading failed!" << std::endl;
        return;
    }

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
    // LMJ: "Create preview sprites for all blocks in registry"
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

    // LMJ: "Update placed blocks"
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
    // LMJ: "Set world view and draw main content"
    window.setView(worldView);

    // LMJ: "Draw grid"
    window.draw(gridLines);

    // LMJ: "Draw grid outline"
    sf::RectangleShape outline;
    outline.setSize(sf::Vector2f(GRID_WIDTH * GRID_SIZE, GRID_HEIGHT * GRID_SIZE));
    outline.setPosition(0, 0);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineColor(sf::Color::Yellow);
    outline.setOutlineThickness(3.f);
    window.draw(outline);

    // LMJ: "Draw tiles"
    for (const sf::Sprite& tile : Tiles)
    {
        window.draw(tile);
    }

    // LMJ: "Draw placed blocks"
    for (Block* block : PlacedBlocks)
    {
        if (block && block->GetActive())
        {
            block->Draw(window);
        }
    }

    // LMJ: "Draw preview at mouse position"
    DrawMapEditor(window);

    // LMJ: "Draw Layer 2 selection highlight"
    if (currentLayer == LayerType::BlockState && selectedBlock != nullptr)
    {
        window.draw(selectionHighlight);
    }

    // LMJ: "Draw block property indicators"
    DrawBlockPropertyIndicators(window);

    // LMJ: "Switch to UI view for UI elements"
    window.setView(uiView);

    // LMJ: "Draw right-side UI"
    DrawRightSideUI(window);

    // LMJ: "Draw base scene"
    Scene::Draw(window);
}

void MapEditor::HandleInput()
{
    HandleLayerSwitching();
    HandleScrollInput();

    if (currentLayer == LayerType::BlockState)
    {
        HandleLayer2Input();
        return;
    }

    // LMJ: "Regular input handling for Layer 0 and 1"
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

void MapEditor::HandleLayer2Input()
{
    sf::Vector2i mousePos = InputMgr::GetMousePosition();
    sf::Vector2f worldPos = ScreenToWorld(mousePos);
    sf::Vector2f gridPos = GetGridPosition(worldPos);

    // LMJ: "Block selection with left click"
    if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
    {
        if (IsValidGridPosition(gridPos))
        {
            SelectBlockAtPosition(gridPos);
        }
    }

    // LMJ: "Property mode switching with number keys"
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
        // LMJ: "Toggle current property with Space"
        if (InputMgr::GetKeyDown(sf::Keyboard::Space))
        {
            ToggleSelectedBlockProperty(currentPropertyMode);
        }

        // LMJ: "Set property to true with T key"
        if (InputMgr::GetKeyDown(sf::Keyboard::T))
        {
            ModifySelectedBlockProperty(currentPropertyMode, true);
        }

        // LMJ: "Set property to false with F key"
        if (InputMgr::GetKeyDown(sf::Keyboard::F))
        {
            ModifySelectedBlockProperty(currentPropertyMode, false);
        }

        // LMJ: "Copy properties with C key (source) and V key (target)"
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

        // LMJ: "Reset selection when switching layers"
        if (currentLayer != LayerType::BlockState)
        {
            selectedBlock = nullptr;
        }

        std::cout << "Layer switched to: " << currentLayerInt << std::endl;
    }
}

void MapEditor::HandleScrollInput()
{
    MouseScrollInput = ScrollInput::None;

    if (InputMgr::IsMouseWheelScrolled())
    {
        float delta = InputMgr::GetMouseWheelDelta();

        if (delta > 0)
        {
            MouseScrollInput = ScrollInput::ScrollUp;
            if (currentLayer == LayerType::Background)
            {
                if (!TileOptions.empty())
                {
                    tileOptionIndex = (tileOptionIndex - 1 + TileOptions.size()) % TileOptions.size();
                }
            }
            else if (currentLayer == LayerType::Block)
            {
                int maxIndex = Block::GetBlockRegistrySize();
                if (maxIndex > 0)
                {
                    blockRegistryIndex = (blockRegistryIndex - 1 + maxIndex) % maxIndex;
                }
            }
        }
        else if (delta < 0)
        {
            MouseScrollInput = ScrollInput::ScrollDown;
            if (currentLayer == LayerType::Background)
            {
                if (!TileOptions.empty())
                {
                    tileOptionIndex = (tileOptionIndex + 1) % TileOptions.size();
                }
            }
            else if (currentLayer == LayerType::Block)
            {
                int maxIndex = Block::GetBlockRegistrySize();
                if (maxIndex > 0)
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

    // LMJ: "Find blocks at positions"
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

    // LMJ: "Copy all properties from source to target"
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

    // LMJ: "Remove existing tile at position"
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

    // LMJ: "Add new tile"
    sf::Sprite tile = TileOptions[tileOptionIndex];
    tile.setPosition(tilePosition);
    Tiles.push_back(tile);
}

void MapEditor::CreateBlockAtPosition(const sf::Vector2f& gridPos)
{
    if (!IsValidGridPosition(gridPos))
        return;

    // LMJ: "Check if block already exists at this position"
    Block* existingBlock = GetBlockAtPosition(gridPos);
    if (existingBlock)
        return;

    // LMJ: "Create new block using registry index"
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

    // LMJ: "Draw small colored squares on blocks to show their properties"
    for (Block* block : PlacedBlocks)
    {
        if (block == nullptr || !block->GetActive()) continue;

        sf::Vector2f blockPos = block->GetPosition();
        float indicatorSize = 8.0f;

        // LMJ: "Destroyable indicator (top-left)"
        if (block->IsDestroyable())
        {
            sf::RectangleShape indicator;
            indicator.setSize(sf::Vector2f(indicatorSize, indicatorSize));
            indicator.setPosition(blockPos.x - GRID_SIZE / 2, blockPos.y - GRID_SIZE / 2);
            indicator.setFillColor(GetPropertyColor(PropertyMode::Destroyable, true));
            window.draw(indicator);
        }

        // LMJ: "Hidable indicator (top-right)"
        if (block->IsHidable())
        {
            sf::RectangleShape indicator;
            indicator.setSize(sf::Vector2f(indicatorSize, indicatorSize));
            indicator.setPosition(blockPos.x + GRID_SIZE / 2 - indicatorSize, blockPos.y - GRID_SIZE / 2);
            indicator.setFillColor(GetPropertyColor(PropertyMode::Hidable, true));
            window.draw(indicator);
        }

        // LMJ: "Movable indicator (bottom-left)"
        if (block->IsMovable())
        {
            sf::RectangleShape indicator;
            indicator.setSize(sf::Vector2f(indicatorSize, indicatorSize));
            indicator.setPosition(blockPos.x - GRID_SIZE / 2, blockPos.y + GRID_SIZE / 2 - indicatorSize);
            indicator.setFillColor(GetPropertyColor(PropertyMode::Movable, true));
            window.draw(indicator);
        }

        // LMJ: "SpawnItem indicator (bottom-right)"
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
    float rightPanelX = GRID_WIDTH * GRID_SIZE + 20; // LMJ: "Start right of grid"
    float rightPanelWidth = 160; // LMJ: "Reduced width to fit in 800px window"

    // LMJ: "Main UI background"
    sf::RectangleShape uiBackground;
    uiBackground.setSize(sf::Vector2f(rightPanelWidth, 580));
    uiBackground.setPosition(rightPanelX, 10);
    uiBackground.setFillColor(sf::Color(20, 20, 20, 200));
    uiBackground.setOutlineColor(sf::Color::White);
    uiBackground.setOutlineThickness(2);
    window.draw(uiBackground);

    // LMJ: "Preview window - smaller size"
    float previewSize = 80;
    sf::RectangleShape previewBg;
    previewBg.setSize(sf::Vector2f(previewSize, previewSize));
    previewBg.setPosition(rightPanelX + 40, 20); // LMJ: "Centered in smaller panel"
    previewBg.setFillColor(sf::Color(50, 50, 50, 180));
    previewBg.setOutlineColor(sf::Color::White);
    previewBg.setOutlineThickness(2);
    window.draw(previewBg);

    // LMJ: "Draw preview content"
    if (currentLayer == LayerType::Background && tileOptionIndex < TileOptions.size())
    {
        sf::Sprite currentTile = TileOptions[tileOptionIndex];
        currentTile.setPosition(rightPanelX + 80, 60); // LMJ: "Center in preview"
        currentTile.setScale(1.2f, 1.2f); // LMJ: "Slightly smaller scale"
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

    // LMJ: "Layer information"
    DrawLayerInfo(window, rightPanelX, 110);

    // LMJ: "Controls information"
    DrawControlsInfo(window, rightPanelX, 190);

    // LMJ: "Layer 2 specific UI"
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
    layerText.setCharacterSize(13); // LMJ: "Smaller font size"
    layerText.setFillColor(sf::Color::White);
    layerText.setPosition(x + 10, y);

    std::string layerInfo = "Layer: ";
    switch (currentLayer)
    {
    case LayerType::Background:
        layerInfo += "Background\nTile: " + std::to_string(tileOptionIndex + 1) + "/" + std::to_string(TileOptions.size());
        break;
    case LayerType::Block:
        layerInfo += "Block\nBlock: " + std::to_string(blockRegistryIndex + 1) + "/" + std::to_string(Block::GetBlockRegistrySize());
        break;
    case LayerType::BlockState:
        layerInfo += "Properties\nMode: " + GetPropertyModeString(currentPropertyMode);
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
    controlsText.setCharacterSize(11); // LMJ: "Smaller font to fit"
    controlsText.setFillColor(sf::Color::Cyan);
    controlsText.setPosition(x + 10, y);

    std::string controls;
    if (currentLayer == LayerType::Background || currentLayer == LayerType::Block)
    {
        controls = "=== Controls ===\n";
        controls += "Tab: Switch Layer\n";
        controls += "Wheel: Change\n";
        controls += "L-Click: Place\n";
        controls += "R-Click: Delete";
    }
    else if (currentLayer == LayerType::BlockState)
    {
        controls = "=== Layer 2 ===\n";
        controls += "Tab: Switch Layer\n";
        controls += "L-Click: Select\n";
        controls += "1-4: Property Mode\n";
        controls += "Space: Toggle\n";
        controls += "T/F: True/False\n";
        controls += "C/V: Copy/Paste";
    }

    controlsText.setString(controls);
    window.draw(controlsText);
}

void MapEditor::DrawLayer2Info(sf::RenderWindow& window, float x, float y)
{
    if (!FONT_MGR.Exists("assets/font/Daum_Regular.ttf"))
        return;

    // LMJ: "Property modes header"
    sf::Text propertyText;
    propertyText.setFont(FONT_MGR.Get("assets/font/Daum_Regular.ttf"));
    propertyText.setCharacterSize(11);
    propertyText.setPosition(x + 10, y);
    propertyText.setString("=== Properties ===");
    propertyText.setFillColor(sf::Color::White);
    window.draw(propertyText);

    // LMJ: "Draw property mode indicators - compact"
    std::vector<std::string> modeNames = { "1:Dest", "2:Hide", "3:Move", "4:Item" };
    std::vector<PropertyMode> modes = { PropertyMode::Destroyable, PropertyMode::Hidable, PropertyMode::Movable, PropertyMode::SpawnItem };

    for (int i = 0; i < 4; ++i)
    {
        float modeY = y + 20 + i * 16; // LMJ: "Reduced spacing"

        // LMJ: "Color indicator - smaller"
        sf::RectangleShape colorIndicator;
        colorIndicator.setSize(sf::Vector2f(12, 12));
        colorIndicator.setPosition(x + 10, modeY);
        colorIndicator.setFillColor(GetPropertyColor(modes[i], true));
        colorIndicator.setOutlineColor(currentPropertyMode == modes[i] ? sf::Color::White : sf::Color::Black);
        colorIndicator.setOutlineThickness(currentPropertyMode == modes[i] ? 2 : 1);
        window.draw(colorIndicator);

        // LMJ: "Mode name - smaller font"
        sf::Text modeText;
        modeText.setFont(FONT_MGR.Get("assets/font/Daum_Regular.ttf"));
        modeText.setCharacterSize(10);
        modeText.setFillColor(currentPropertyMode == modes[i] ? sf::Color::Yellow : sf::Color::White);
        modeText.setPosition(x + 25, modeY);
        modeText.setString(modeNames[i]);
        window.draw(modeText);
    }

    // LMJ: "Selected block properties - compact"
    if (selectedBlock != nullptr)
    {
        sf::Text selectedText;
        selectedText.setFont(FONT_MGR.Get("assets/font/Daum_Regular.ttf"));
        selectedText.setCharacterSize(10);
        selectedText.setFillColor(sf::Color::Yellow);
        selectedText.setPosition(x + 10, y + 90);

        std::string selectedInfo = "=== Selected ===\n";
        selectedInfo += "Dest: " + std::string(selectedBlock->IsDestroyable() ? "ON" : "OFF") + "\n";
        selectedInfo += "Hide: " + std::string(selectedBlock->IsHidable() ? "ON" : "OFF") + "\n";
        selectedInfo += "Move: " + std::string(selectedBlock->IsMovable() ? "ON" : "OFF") + "\n";
        selectedInfo += "Item: " + std::string(selectedBlock->CanSpawnItem() ? "ON" : "OFF");

        selectedText.setString(selectedInfo);
        window.draw(selectedText);
    }
}

std::string MapEditor::GetPropertyModeString(PropertyMode mode) const
{
    switch (mode)
    {
    case PropertyMode::Destroyable: return "Destroyable";
    case PropertyMode::Hidable: return "Hidable";
    case PropertyMode::Movable: return "Movable";
    case PropertyMode::SpawnItem: return "Spawn Item";
    default: return "Unknown";
    }
}

sf::Color MapEditor::GetPropertyColor(PropertyMode mode, bool enabled) const
{
    if (!enabled) return sf::Color(64, 64, 64); // LMJ: "Dark gray for disabled"

    switch (mode)
    {
    case PropertyMode::Destroyable: return sf::Color::Red;
    case PropertyMode::Hidable: return sf::Color::Blue;
    case PropertyMode::Movable: return sf::Color::Green;
    case PropertyMode::SpawnItem: return sf::Color::Yellow;
    default: return sf::Color::White;
    }
}