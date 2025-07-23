#include "stdafx.h"
#include "WaterSplashPool.h"
#include "WaterSplash.h"

std::vector<WaterSplash*> WaterSplashPool::allObjects;
std::queue<WaterSplash*> WaterSplashPool::readyObjects;
int WaterSplashPool::initPoolSize = 100;
Scene* WaterSplashPool::curScene = nullptr;

WaterSplashPool::WaterSplashPool()
{
	std::cout << "Created WaterSplash ObjectPool" << std::endl;
}

void WaterSplashPool::Init()
{
    CreateObject(initPoolSize);
}

WaterSplash* WaterSplashPool::GetFromPool()
{
    // KHI: Create a new one if no available object exists
    if (readyObjects.empty())
    {
        CreateObject(20);
    }

    WaterSplash* splash = readyObjects.front();
    readyObjects.pop();
    splash->SetActive(true);
    return splash;
}

void WaterSplashPool::ReturnToPool(WaterSplash* splashObj)
{
    splashObj->SetActive(false);
    readyObjects.push(splashObj);
}

void WaterSplashPool::CreateObject(int amount)
{
    for (int i = 0; i < amount; ++i)
    {
        WaterSplash* obj = new WaterSplash();
        obj->Init();
        obj->Reset();
        obj->SetActive(false);
        allObjects.push_back(obj);
        readyObjects.push(obj);

        if (curScene != nullptr)
        {
            curScene->AddGameObject(obj);
        }
        else
        {
            std::cout << "WaterSplashPool: 현재 씬 설정해주세요 ㅜ.ㅜ" << std::endl;
        }
    }
}