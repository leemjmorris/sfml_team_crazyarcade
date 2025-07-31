#include "stdafx.h"
#include "ResultPop.h"
#include "Player.h"

ResultPop::ResultPop(const std::string& name)
	:UiHud(name)
{
}

ResultPop::~ResultPop()
{
}

void ResultPop::SetResult()
{
	int num = 1;
	for (auto player : resultPlayers)
	{
		if (player->GetPlayerState() == AnimState::Win)
		{
			resultText.setString(std::to_string(num) + ": win");
			resultTexts.push_back(resultText);
		}
		else if (player->GetPlayerState() == AnimState::Dead)
		{
			resultText.setString(std::to_string(num) + ": Lose");
			resultTexts.push_back(resultText);
		}
		else if (player->GetPlayerState() == AnimState::Live)
		{
			resultText.setString(std::to_string(num) + ": Draw");
			resultTexts.push_back(resultText);
		}
			num++;
	}
}

void ResultPop::Init()
{
	tex.loadFromFile("assets/ui/game/result.png");
	popUp.setTexture(tex);
}

void ResultPop::Release()
{

}

void ResultPop::Reset()
{
	resultText.setFont(FONT_MGR.Get("ARCADECLASSIC.TTF"));

	int arrayY = 1;
	for (auto it : resultTexts)
	{
		it.setPosition({ 77.f, 180.f + (26.f * arrayY) });
		arrayY++;
	}
}

void ResultPop::Update(float dt)
{
	SetResult();
}

void ResultPop::Draw(sf::RenderWindow& window)
{
	if(GetActive())
	window.draw(resultText);
}