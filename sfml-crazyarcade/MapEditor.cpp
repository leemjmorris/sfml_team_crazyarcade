#include "stdafx.h"
#include "MapEditor.h"

MapEditor::MapEditor() : Scene(SceneIds::MapEditor)
{
    currentLayer = LayerType::Background;
    tileOptionIndex = 0;
    blockRegistryIndex = 0;
    MouseScrollInput = ScrollInput::None;
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

    // LMJ: "Load block textures from registry"
    auto allBlocks = Block::GetAllBlocks();
    for (const auto& blockInfo : allBlocks)
    {
        texIds.push_back(blockInfo.textureId);
    }

    LoadTileSet();
    LoadBlockSet();

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

    // LMJ: Trying multiple path, for test purpose
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
        else
        {
            std::cout << "Failed to load: " << path << std::endl;
        }
    }

    if (!success)
    {
        std::cout << "All texture loading failed! Creating test texture..." << std::endl;
    }

    // LMJ: size check
    sf::Vector2u textureSize = tileMapTexture.getSize();
    std::cout << "Texture size: " << textureSize.x << "x" << textureSize.y << std::endl;

    // LMJ: testing for tile size
    float tile_width = textureSize.x / 5.0f;
    float tile_height = textureSize.y / 2.0f;

    std::cout << "Tile size: " << tile_width << "x" << tile_height << std::endl;

    TileOptions.clear();

    // LMJ: load tile
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

            // LMJ: adjust scale
            float scaleX = GRID_SIZE / tile_width;
            float scaleY = GRID_SIZE / tile_height;

            tile.setScale(sf::Vector2f(scaleX, scaleY));
            tile.setOrigin(sf::Vector2f(tile_width / 2, tile_height / 2));

            TileOptions.push_back(tile);
            std::cout << "Added tile " << (y * 5 + x) << std::endl;
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
    // LMJ: world view set
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

    // LMJ: "Draw placed blocks"
    for (Block* block : PlacedBlocks)
    {
        if (block && block->GetActive())
        {
            block->Draw(window);
        }
    }

    // LMJ: drawing tiles to mouse point
    DrawMapEditor(window);

    window.setView(uiView);
    DrawTilePreview(window);

    // LMJ: drawing scene
    Scene::Draw(window);
}

void MapEditor::HandleInput()
{
    HandleLayerSwitching();
    HandleScrollInput();

    sf::Vector2i mousePos = InputMgr::GetMousePosition();
    sf::Vector2f worldPos = FRAMEWORK.GetWindow().mapPixelToCoords(mousePos, worldView);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (currentLayer == LayerType::Background)
        {
            CreateTileAtPosition(worldPos);
            std::cout << "Left click at: " << worldPos.x << ", " << worldPos.y << std::endl;
        }
        else if (currentLayer == LayerType::Block)
        {
            CreateBlockAtPosition(worldPos);
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        if (currentLayer == LayerType::Background)
        {
            DeleteTileAtPosition(worldPos);
            std::cout << "Right click at: " << worldPos.x << ", " << worldPos.y << std::endl;
        }
        else if (currentLayer == LayerType::Block)
        {
            DeleteBlockAtPosition(worldPos);
        }
    }
}

void MapEditor::HandleLayerSwitching()
{
    // LMJ: "Tab key to switch layers"
    if (InputMgr::GetKeyDown(sf::Keyboard::Tab))
    {
        int currentLayerInt = static_cast<int>(currentLayer);
        currentLayerInt = (currentLayerInt + 1) % 3; // LMJ: "Cycle through 3 layers"
        currentLayer = static_cast<LayerType>(currentLayerInt);

        std::cout << "Layer switched to: " << currentLayerInt << std::endl;
    }
}

void MapEditor::HandleScrollInput()
{
    MouseScrollInput = ScrollInput::None;

    if (InputMgr::IsMouseWheelScrolled())
    {
        float delta = InputMgr::GetMouseWheelDelta();

        if (delta > 0) // LMJ: "Scroll up"
        {
            MouseScrollInput = ScrollInput::ScrollUp;

            if (currentLayer == LayerType::Background)
            {
                if (!TileOptions.empty())
                {
                    tileOptionIndex--;
                    if (tileOptionIndex < 0)
                    {
                        tileOptionIndex = TileOptions.size() - 1;
                    }
                    std::cout << "Tile index-- changed to: " << tileOptionIndex << std::endl;
                }
            }
            else if (currentLayer == LayerType::Block)
            {
                int maxIndex = Block::GetBlockRegistrySize();
                if (maxIndex > 0)
                {
                    blockRegistryIndex = (blockRegistryIndex - 1 + maxIndex) % maxIndex;
                    std::cout << "Block index-- changed to: " << blockRegistryIndex << std::endl;
                }
            }
        }
        else if (delta < 0) // LMJ: "Scroll down"
        {
            MouseScrollInput = ScrollInput::ScrollDown;

            if (currentLayer == LayerType::Background)
            {
                if (!TileOptions.empty())
                {
                    tileOptionIndex++;
                    if (tileOptionIndex >= TileOptions.size())
                    {
                        tileOptionIndex = 0;
                    }
                    std::cout << "Tile index++ changed to: " << tileOptionIndex << std::endl;
                }
            }
            else if (currentLayer == LayerType::Block)
            {
                int maxIndex = Block::GetBlockRegistrySize();
                if (maxIndex > 0)
                {
                    blockRegistryIndex = (blockRegistryIndex + 1) % maxIndex;
                    std::cout << "Block index++ changed to: " << blockRegistryIndex << std::endl;
                }
            }
        }
    }
}

void MapEditor::CreateTileAtPosition(const sf::Vector2f& position)
{
    if (TileOptions.empty() || tileOptionIndex >= TileOptions.size())
    {
        std::cout << "Cannot create tile - invalid state" << std::endl;
        return;
    }

    if (position.x < 0 || position.x >= GRID_WIDTH * GRID_SIZE || position.y < 0 || position.y >= GRID_HEIGHT * GRID_SIZE)
    {
        std::cout << "Put tiles on the grid" << std::endl;
        return;
    }

    std::cout << "Creating tile at position: " << position.x << ", " << position.y << std::endl;

    // LMJ: snap on grid
    int gridX = static_cast<int>(position.x / GRID_SIZE);
    int gridY = static_cast<int>(position.y / GRID_SIZE);
    sf::Vector2f tilePosition(gridX * GRID_SIZE + GRID_SIZE / 2.0f, gridY * GRID_SIZE + GRID_SIZE / 2.0f);

    // LMJ: get rid of the tile if its already there
    for (int i = 0; i < Tiles.size(); ++i)
    {
        sf::Vector2f tilePos = Tiles[i].getPosition();
        float distance = std::sqrt((tilePos.x - tilePosition.x) * (tilePos.x - tilePosition.x) +
            (tilePos.y - tilePosition.y) * (tilePos.y - tilePosition.y));
        if (distance < GRID_SIZE / 2)
        {
            Tiles.erase(Tiles.begin() + i);
            break;
        }
    }

    // LMJ: new tiles
    sf::Sprite tile = TileOptions[tileOptionIndex];
    tile.setPosition(tilePosition);
    Tiles.push_back(tile);

    std::cout << "Tile created. Total tiles: " << Tiles.size() << std::endl;
}

void MapEditor::CreateBlockAtPosition(const sf::Vector2f& position)
{
    sf::Vector2f gridPos = GetGridPosition(position);
    if (!IsValidGridPosition(gridPos))
        return;

    // LMJ: "Check if block already exists at this position"
    Block* existingBlock = GetBlockAtPosition(gridPos);
    if (existingBlock)
        return;

    // LMJ: "Create new block using registry index"
    Block* newBlock = Block::CreateBlockFromRegistry(blockRegistryIndex,
        gridPos * static_cast<float>(GRID_SIZE) + sf::Vector2f(GRID_SIZE / 2, GRID_SIZE / 2));

    if (newBlock)
    {
        PlacedBlocks.push_back(newBlock);
        newBlock->Init();
        newBlock->Reset();
        std::cout << "Block created at: " << gridPos.x << ", " << gridPos.y << std::endl;
    }
}

void MapEditor::DeleteTileAtPosition(const sf::Vector2f& position)
{
    // LMJ: snap on grid
    int gridX = static_cast<int>(position.x / GRID_SIZE);
    int gridY = static_cast<int>(position.y / GRID_SIZE);
    sf::Vector2f tilePosition(gridX * GRID_SIZE + GRID_SIZE / 2.0f, gridY * GRID_SIZE + GRID_SIZE / 2.0f);

    for (int i = 0; i < Tiles.size(); ++i)
    {
        sf::Vector2f tilePos = Tiles[i].getPosition();
        float distance = std::sqrt((tilePos.x - tilePosition.x) * (tilePos.x - tilePosition.x) +
            (tilePos.y - tilePosition.y) * (tilePos.y - tilePosition.y));
        if (distance < GRID_SIZE / 2)
        {
            Tiles.erase(Tiles.begin() + i);
            std::cout << "Tile deleted. Total tiles: " << Tiles.size() << std::endl;
            break;
        }
    }
}

void MapEditor::DeleteBlockAtPosition(const sf::Vector2f& position)
{
    sf::Vector2f gridPos = GetGridPosition(position);
    if (!IsValidGridPosition(gridPos))
        return;

    Block* blockToDelete = GetBlockAtPosition(gridPos);
    if (blockToDelete)
    {
        // LMJ: "Remove from vector and delete"
        auto it = std::find(PlacedBlocks.begin(), PlacedBlocks.end(), blockToDelete);
        if (it != PlacedBlocks.end())
        {
            delete* it;
            PlacedBlocks.erase(it);
            std::cout << "Block deleted" << std::endl;
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
    sf::Vector2f worldPos = gridPos * static_cast<float>(GRID_SIZE) + sf::Vector2f(GRID_SIZE / 2, GRID_SIZE / 2);

    for (Block* block : PlacedBlocks)
    {
        if (block && block->GetActive())
        {
            sf::Vector2f blockPos = block->GetPosition();
            // LMJ: "Check if positions match (with small tolerance)"
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
    {
        std::cout << "TileOptions is empty in DrawMapEditor!" << std::endl;
        return;
    }

    if (tileOptionIndex >= TileOptions.size())
    {
        std::cout << "Invalid tile index: " << tileOptionIndex << std::endl;
        tileOptionIndex = 0;
    }

    // LMJ: get mouse pos
    sf::Vector2i mousePos = InputMgr::GetMousePosition();
    sf::Vector2f worldPos = FRAMEWORK.GetWindow().mapPixelToCoords(mousePos, worldView);

    if (worldPos.x >= 0 && worldPos.x < GRID_WIDTH * GRID_SIZE && worldPos.y >= 0 && worldPos.y < GRID_HEIGHT * GRID_SIZE)
    {
        int gridX = static_cast<int>(worldPos.x / GRID_SIZE);
        int gridY = static_cast<int>(worldPos.y / GRID_SIZE);

        sf::Vector2f snappedPos(gridX * GRID_SIZE + GRID_SIZE / 2.f, gridY * GRID_SIZE + GRID_SIZE / 2.f);

        // LMJ: draw selected tile
        sf::Sprite currentTile = TileOptions[tileOptionIndex];
        currentTile.setPosition(snappedPos);
        currentTile.setColor(sf::Color(255, 255, 255, 150)); // LMJ: "Semi-transparent"
        window.draw(currentTile);

        sf::RectangleShape highlight;
        highlight.setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
        highlight.setPosition(gridX * GRID_SIZE, gridY * GRID_SIZE);
        highlight.setFillColor(sf::Color(255, 255, 0, 50));  // 반투명 노란색
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
    sf::Vector2f worldPos = FRAMEWORK.GetWindow().mapPixelToCoords(mousePos, worldView);
    sf::Vector2f gridPos = GetGridPosition(worldPos);

    if (IsValidGridPosition(gridPos))
    {
        int gridX = static_cast<int>(gridPos.x);
        int gridY = static_cast<int>(gridPos.y);

        sf::Sprite preview = BlockPreviewSprites[blockRegistryIndex];
        preview.setPosition(gridX * GRID_SIZE + GRID_SIZE / 2, gridY * GRID_SIZE + GRID_SIZE / 2);
        preview.setColor(sf::Color(255, 255, 255, 128)); // LMJ: "Semi-transparent"
        window.draw(preview);

        sf::RectangleShape highlight;
        highlight.setSize(sf::Vector2f(GRID_SIZE, GRID_SIZE));
        highlight.setPosition(gridX * GRID_SIZE, gridY * GRID_SIZE);
        highlight.setFillColor(sf::Color(0, 255, 0, 50));  // 반투명 초록색
        highlight.setOutlineColor(sf::Color::Green);
        highlight.setOutlineThickness(2.0f);
        window.draw(highlight);
    }
}

void MapEditor::DrawTilePreview(sf::RenderWindow& window)
{
    if (TileOptions.empty() || tileOptionIndex >= TileOptions.size())
        return;

    float previewSize = 120.0f;
    sf::Vector2f windowSize(800, 600);
    sf::Vector2f previewPos(windowSize.x - previewSize - 35, 40);

    sf::RectangleShape previewBg;
    previewBg.setSize(sf::Vector2f(previewSize, previewSize));
    previewBg.setPosition(previewPos);
    previewBg.setFillColor(sf::Color(50, 50, 50, 200));
    previewBg.setOutlineColor(sf::Color::White);
    previewBg.setOutlineThickness(2.0f);
    window.draw(previewBg);

    sf::Sprite previewTile = TileOptions[tileOptionIndex];

    sf::Vector2f previewCenter = previewPos + sf::Vector2f(previewSize / 2, previewSize / 2);
    previewTile.setPosition(previewCenter);

    sf::Vector2f currentScale = previewTile.getScale();
    previewTile.setScale(currentScale.x * 2.0f, currentScale.y * 2.0f);

    previewTile.setColor(sf::Color::White);

    window.draw(previewTile);

    sf::RectangleShape indexBg;
    indexBg.setSize(sf::Vector2f(30, 20));
    indexBg.setPosition(previewPos.x, previewPos.y - 25);
    indexBg.setFillColor(sf::Color::Black);
    indexBg.setOutlineColor(sf::Color::White);
    indexBg.setOutlineThickness(1.0f);
    window.draw(indexBg);

    for (int i = 0; i < (tileOptionIndex + 1) && i < 10; i++)
    {
        sf::RectangleShape dot;
        dot.setSize(sf::Vector2f(2, 2));
        dot.setPosition(previewPos.x + 3 + i * 3, previewPos.y - 20);
        dot.setFillColor(sf::Color::Yellow);
        window.draw(dot);
    }

    // LMJ: "Show current layer info"
    sf::RectangleShape layerBg;
    layerBg.setSize(sf::Vector2f(200, 30));
    layerBg.setPosition(10, 10);
    layerBg.setFillColor(sf::Color(0, 0, 0, 150));
    layerBg.setOutlineColor(sf::Color::White);
    layerBg.setOutlineThickness(1.0f);
    window.draw(layerBg);
}