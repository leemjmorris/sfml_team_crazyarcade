#include "stdafx.h"
#include "MapEditor.h"

MapEditor::MapEditor()
    : Scene(SceneIds::MapEditor)
    , tileOptionIndex(0)
    , MouseScrollInput(None)
{
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

    LoadTileSet();
}

void MapEditor::Release()
{
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
        // LMJ: Red rect for Test purpose
        sf::Image testImage;
        testImage.create(260, 104, sf::Color::Red);
        tileMapTexture.loadFromImage(testImage);
        std::cout << "Created red test texture" << std::endl;
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
            tile.setScale(sf::Vector2f(2.0f, 2.0f));
            tile.setOrigin(sf::Vector2f(tile_width / 2, tile_height / 2));

            TileOptions.push_back(tile);
            std::cout << "Added tile " << (y * 5 + x) << std::endl;
        }
    }

    std::cout << "Total tiles loaded: " << TileOptions.size() << std::endl;
}

void MapEditor::Update(float dt)
{
    Scene::Update(dt);

    std::cout << "MapEditor::Update() called - TileOptions size: " << TileOptions.size() << std::endl;

    HandleInput();
}

void MapEditor::Draw(sf::RenderWindow& window)
{
    std::cout << "MapEditor::Draw() called" << std::endl;

    // LMJ: world view set
    window.setView(worldView);

    for (const sf::Sprite& tile : Tiles)
    {
        window.draw(tile);
    }

    // LMJ: drawing tiles to mouse point
    DrawMapEditor(window);

    // LMJ: drawing scene
    Scene::Draw(window);
}

void MapEditor::HandleInput()
{
    if (TileOptions.empty())
    {
        std::cout << "TileOptions is empty in HandleInput!" << std::endl;
        return;
    }

    static float lastScrollTime = 0;
    float currentTime = FRAMEWORK.GetTime();
           
    if (InputMgr::IsMouseWheelUp())
    {
        tileOptionIndex--;
        if (tileOptionIndex < 0)
        {
            tileOptionIndex = TileOptions.size() - 1;
        }
        std::cout << "Tile index-- changed to: " << tileOptionIndex << std::endl;
    }
    else if (InputMgr::IsMouseWheelDown())
    {
        tileOptionIndex++;
        if (tileOptionIndex >= TileOptions.size())
        {
            tileOptionIndex = 0;
        }
        std::cout << "Tile index++ changed to: " << tileOptionIndex << std::endl;
    }

    if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
    {
        sf::Vector2i mousePos = InputMgr::GetMousePosition();
        sf::Vector2f worldPos = FRAMEWORK.GetWindow().mapPixelToCoords(mousePos, worldView);
        CreateTileAtPosition(worldPos);
        std::cout << "Left click at: " << worldPos.x << ", " << worldPos.y << std::endl;
    }

    if (InputMgr::GetMouseButtonDown(sf::Mouse::Right))
    {
        sf::Vector2i mousePos = InputMgr::GetMousePosition();
        sf::Vector2f worldPos = FRAMEWORK.GetWindow().mapPixelToCoords(mousePos, worldView);
        DeleteTileAtPosition(worldPos);
        std::cout << "Right click at: " << worldPos.x << ", " << worldPos.y << std::endl;
    }
}

void MapEditor::CreateTileAtPosition(const sf::Vector2f& position)
{
    if (TileOptions.empty() || tileOptionIndex >= TileOptions.size())
    {
        std::cout << "Cannot create tile - invalid state" << std::endl;
        return;
    }

    std::cout << "Creating tile at position: " << position.x << ", " << position.y << std::endl;

    // LMJ: snap on grid
    int gridSize = 40;
    int gridX = static_cast<int>(position.x / gridSize);
    int gridY = static_cast<int>(position.y / gridSize);
    sf::Vector2f tilePosition(gridX * gridSize + gridSize / 2.0f, gridY * gridSize + gridSize / 2.0f);

    // LMJ: get rid of the tile if its already there
    for (int i = 0; i < Tiles.size(); ++i)
    {
        sf::Vector2f tilePos = Tiles[i].getPosition();
        float distance = std::sqrt((tilePos.x - tilePosition.x) * (tilePos.x - tilePosition.x) +
            (tilePos.y - tilePosition.y) * (tilePos.y - tilePosition.y));
        if (distance < gridSize / 2)
        {
            Tiles.erase(Tiles.begin() + i);
            break;
        }
    }

    // LMJ: new tiles
    sf::Sprite tile = TileOptions[tileOptionIndex];
    tile.setPosition(tilePosition);
    tile.setColor(sf::Color::White);  // LMJ: set in transperant
    Tiles.push_back(tile);

    std::cout << "Tile created. Total tiles: " << Tiles.size() << std::endl;
}

void MapEditor::DeleteTileAtPosition(const sf::Vector2f& position)
{
    // LMJ: snap on grid
    int gridSize = 40;
    int gridX = static_cast<int>(position.x / gridSize);
    int gridY = static_cast<int>(position.y / gridSize);
    sf::Vector2f tilePosition(gridX * gridSize + gridSize / 2.0f, gridY * gridSize + gridSize / 2.0f);

    for (int i = 0; i < Tiles.size(); ++i)
    {
        sf::Vector2f tilePos = Tiles[i].getPosition();
        float distance = std::sqrt((tilePos.x - tilePosition.x) * (tilePos.x - tilePosition.x) +
            (tilePos.y - tilePosition.y) * (tilePos.y - tilePosition.y));
        if (distance < gridSize / 2)
        {
            Tiles.erase(Tiles.begin() + i);
            std::cout << "Tile deleted. Total tiles: " << Tiles.size() << std::endl;
            break;
        }
    }
}

void MapEditor::DrawMapEditor(sf::RenderWindow& window)
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

    // LMJ: draw selected tile
    sf::Sprite currentTile = TileOptions[tileOptionIndex];
    currentTile.setPosition(worldPos);

    // LMJ: set color into transperant
    currentTile.setColor(sf::Color(255, 255, 255, 150));

    window.draw(currentTile);

    // 디버깅: 마우스 위치에 작은 원 그리기
    //sf::CircleShape debugCircle(5);
    //debugCircle.setFillColor(sf::Color::Yellow);
    //debugCircle.setPosition(worldPos.x - 5, worldPos.y - 5);
    //window.draw(debugCircle);
}