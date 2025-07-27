#pragma once

// LMJ: File Path Organizer Added
#define PATH_MAP_FOREST_TILE "assets/map/forest/tile/"
#define PATH_MAP_FOREST_BLOCK "assets/map/forest/block/"
#define PATH_MAP_TOWN "assets/map/town/"
#define PATH_MAP_JSON "map/" // LMJ: "JSON map files directory"
#define PATH_FONT "assets/font/"
#define PATH_ITEM "assets/item/"
#define PATH_BOMB "assets/bomb/"
#define PATH_UI_GAME_OVER "assets/game_over/"
#define PATH_UI_GAME_START "assets/game_start/"
#define PATH_PLAYER_ICON "assets/player/"
#define PATH_PLAYER_BAZZI "assets/player/bazzi/"
#define PATH_PLAYER_KAPPI "assets/player/kappi/"
#define PATH_PLAYER_WOONI "assets/player/wooni/"
#define PATH_SOUND "assets/sound/"
#define PATH_ANI_WAVE "assets/wave/"
#define PATH_ASSETS "assets/"

// LMJ: "Map loading convenience macros"
#define LOAD_MAP(scene, filename) Utils::LoadMapFromJson(scene, filename)
#define CLEAR_MAP(scene) Utils::ClearMapObjects(scene)

// STL
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <fstream> // LMJ: "Added for JSON file loading"

// SFML
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// FRAMEWORK
#include "Defines.h"
#include "Variables.h"
#include "Singleton.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "GameObject.h"
#include "Utils.h"
#include "Scene.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "Framework.h"
#include "MapEditor.h"
#include "ColorMaskShader.h"
#include <nlohmann/json.hpp>