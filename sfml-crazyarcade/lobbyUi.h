#pragma once
#include "UiHud.h"
#include "CharacterStats.h"

enum class PlayerColor { Red, Yellow, Orange, Green, SkyBlue, Blue, Purple, Magenta };

struct PlayerSelectInfo
{
	CharacterID characterId;
	PlayerColor color;
};

class Button;
class lobbyUi :
	public UiHud
{
protected:
	Button* characterArea1 = nullptr;
	Button* characterArea2 = nullptr;
	Button* characterArea3 = nullptr;

	Button* choiceCharacter0 = nullptr;
	Button* choiceCharacter1 = nullptr;
	Button* choiceCharacter2 = nullptr;
	Button* choiceCharacter3 = nullptr;
	Button* choiceCharacter4 = nullptr;
	Button* choiceCharacter5 = nullptr;
	Button* choiceCharacter6 = nullptr;
	Button* choiceCharacter7 = nullptr;
	Button* choiceCharacter8 = nullptr;

	Button* choiceColorRed = nullptr;
	Button* choiceColorYellow = nullptr;
	Button* choiceColorOrange = nullptr;
	Button* choiceColorGreen = nullptr;
	Button* choiceColorSkyBlue = nullptr;
	Button* choiceColorBlue = nullptr;
	Button* choiceColorPurple = nullptr;
	Button* choiceColorMagenta = nullptr;

	Button* mapPop = nullptr;
	Button* gameStartButton = nullptr;

	Button* exitButton = nullptr;

	int numRooms = 0;
	int numColors = 0;


	//sf::FloatRect characterArea1 = { 132.f, 93.f, 100.f, 100.f };
	//sf::FloatRect characterArea2 = { 238.f, 93.f, 100.f, 100.f };
	//sf::FloatRect characterArea3 = { 344.f, 93.f, 100.f, 100.f };

	//sf::FloatRect choiceCharacter0 = { 496.f, 93.f, 64.f, 42.f };
	//sf::FloatRect choiceCharacter1 = { 564.f, 93.f, 64.f, 42.f };
	//sf::FloatRect choiceCharacter2 = { 632.f, 93.f, 64.f, 42.f };
	//sf::FloatRect choiceCharacter3 = { 700.f, 93.f, 64.f, 42.f };
	//sf::FloatRect choiceCharacter4 = { 496.f, 93.f, 64.f, 42.f };
	//sf::FloatRect choiceCharacter5 = { 564.f, 93.f, 64.f, 42.f };
	//sf::FloatRect choiceCharacter6 = { 632.f, 93.f, 64.f, 42.f };
	//sf::FloatRect choiceCharacter7 = { 700.f, 93.f, 64.f, 42.f };
	//sf::FloatRect choiceCharacter8 = { 496.f, 93.f, 64.f, 42.f };
 //  
	//sf::FloatRect choiceColorRed 		= { 488.f, 282.f, 33.f, 35.f };
	//sf::FloatRect choiceColorYellow 	= { 524.f, 282.f, 33.f, 35.f };
	//sf::FloatRect choiceColorOrange 	= { 560.f, 282.f, 33.f, 35.f };
	//sf::FloatRect choiceColorGreen		= { 596.f, 282.f, 33.f, 35.f };
	//sf::FloatRect choiceColorSkyBlue 	= { 632.f, 282.f, 33.f, 35.f };
	//sf::FloatRect choiceColorBlue 		= { 668.f, 282.f, 33.f, 35.f };
	//sf::FloatRect choiceColorPurple 	= { 704.f, 282.f, 33.f, 35.f };
	//sf::FloatRect choiceColorMagenta 	= { 740.f, 282.f, 33.f, 35.f };

	//sf::FloatRect mapPop			= { 647.f, 429.f, 121.f, 46.f };
	//sf::FloatRect gameStartButton   = { 538.f, 495.f, 191.f, 54.f };
	//
	//sf::FloatRect exitButton		= { 749.f, 573.f, 30.f, 22.f };

public:
	lobbyUi(const std::string& name);
	~lobbyUi();

	SortingLayers sortingLayer = SortingLayers::UI;
	int sortingOrder = 0;

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