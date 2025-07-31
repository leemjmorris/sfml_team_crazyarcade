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
	resultTexts.clear();
	int num = 1;

	for (auto* player : resultPlayers)
	{
		sf::Text line;
		line.setFont(FONT_MGR.Get("assets/font/ARCADECLASSIC.TTF"));
		line.setCharacterSize(28);
		line.setFillColor(sf::Color::White);

		if (player->GetPlayerState() == AnimState::Win)
			line.setString(std::to_string(num) + ": Win");
		else if (player->GetPlayerState() == AnimState::Dead)
			line.setString(std::to_string(num) + ": Lose");
		else if(player->GetPlayerState()==AnimState::Draw)
			line.setString(std::to_string(num) + ": Draw");

		line.setPosition({ 77.f, 180.f + 26.f * (num - 1) });
		resultTexts.push_back(line);
		++num;
	}
}

void ResultPop::Init()
{
	FONT_MGR.Load("assets/font/ARCADECLASSIC.TTF");
	tex.loadFromFile("assets/ui/game/result.png");
	popUp.setTexture(tex);
	popUp.setPosition({ 38.f, 137.f });
	SetActive(false);
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

}

void ResultPop::Draw(sf::RenderWindow& window)
{
	if (GetActive())
	{
		window.draw(popUp);
		for (auto it : resultTexts)
		{
			window.draw(it);
		}
	}
}