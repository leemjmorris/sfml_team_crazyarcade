#pragma once

class WaterSplash;
class Scene;

class WaterSplashPool
{
protected:
	static int initPoolSize;

	static std::vector<WaterSplash*> allObjects;
	static std::queue<WaterSplash*> readyObjects;

	static Scene* curScene;

public:
	WaterSplashPool();
	~WaterSplashPool() = default;

	static void Init();
	static void SetCurScene(Scene* scene) { curScene = scene; }
	static WaterSplash* GetFromPool();
	static void ReturnToPool(WaterSplash* splashObj);
	static void CreateObject(int amount);
};