#include "stdafx.h"
#include "SceneDemo.h"

SceneDemo::SceneDemo()
	: Scene(SceneIds::Demo)
{
}

void SceneDemo::Init()
{

}

void SceneDemo::Enter()
{
	std::cout << "===================" << std::endl;
	std::cout << "     SceneDemo" << std::endl;
	std::cout << "===================" << std::endl;
}

void SceneDemo::Update(float dt)
{

}

void SceneDemo::Draw(sf::RenderWindow& window)
{

}