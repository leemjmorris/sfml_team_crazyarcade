#pragma once
#include "UiHud.h"

class Player;
class ResultPop :
    public UiHud
{
protected:
	sf::Sprite popUp;
	sf::Texture tex;

	sf::Text resultText;
	sf::Font font;

	std::vector<Player*> resultPlayers;
	std::vector<sf::Text> resultTexts;
public:
	ResultPop(const std::string& name);
	~ResultPop();

	SortingLayers sortingLayer = SortingLayers::UI;
	int sortingOrder = 0;

	void SetPlayerList(std::vector<Player*> players) { resultPlayers = players; };
	void SetResult();

	const std::string GetName() const { return name; }
	bool GetActive() const { return active; }
	void SetActive(bool a) { active = a; }

	sf::Vector2f GetPosition() const { return position; }
	void SetPosition(const sf::Vector2f& pos) { position = pos; }

	void Init();
	void Release();
	void Reset();

	void Update(float dt);
	void Draw(sf::RenderWindow& window);
};