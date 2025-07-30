#include "stdafx.h"
#include "lobbyUi.h"
#include "Button.h"

lobbyUi::lobbyUi(const std::string& name)
	: UiHud(name)
{

}

lobbyUi::~lobbyUi()
{
	delete characterArea1;
	delete characterArea2;
	delete characterArea3;
	delete choiceCharacter0;
	delete choiceCharacter1;
	delete choiceCharacter2;
	delete choiceCharacter3;
	delete choiceCharacter4;
	delete choiceCharacter5;
	delete choiceCharacter6;
	delete choiceCharacter7;
	delete choiceCharacter8;

	delete choiceColorRed;
	delete choiceColorYellow;
	delete choiceColorOrange;
	delete choiceColorGreen;
	delete choiceColorSkyBlue;
	delete choiceColorBlue;
	delete choiceColorPurple;
	delete choiceColorMagenta;

	delete mapPop;
	delete gameStartButton;

	delete exitButton;
}

void lobbyUi::Init()
{
	characterArea1 = new Button("characterArea1");
	characterArea2 = new Button("characterArea2");
	characterArea3 = new Button("characterArea3");

	choiceCharacter0 = new Button("choiceCharacter0");
	choiceCharacter1 = new Button("choiceCharacter1");
	choiceCharacter2 = new Button("choiceCharacter2");
	choiceCharacter3 = new Button("choiceCharacter3");
	choiceCharacter4 = new Button("choiceCharacter4");
	choiceCharacter5 = new Button("choiceCharacter5");
	choiceCharacter6 = new Button("choiceCharacter6");
	choiceCharacter7 = new Button("choiceCharacter7");
	choiceCharacter8 = new Button("choiceCharacter8");

	choiceColorRed		= new Button("choiceColorRed");
	choiceColorYellow	= new Button("choiceColorYellow");
	choiceColorOrange	= new Button("choiceColorOrange");
	choiceColorGreen	= new Button("choiceColorGreen");
	choiceColorSkyBlue  = new Button("choiceColorSkyBlue");
	choiceColorBlue		= new Button("choiceColorBlue");
	choiceColorPurple   = new Button("choiceColorPurple");
	choiceColorMagenta  = new Button("choiceColorMagenta");

	mapPop = new Button("mapPop");
	gameStartButton = new Button("gameStartButton");

	exitButton = new Button("exitButton");

	characterArea1->SetButton("assets/ui/lobby/Button_Space_UnHover.png", { 132.f, 93.f, 100.f, 100.f });
	characterArea2->SetButton("assets/ui/lobby/Button_Space_UnHover.png", { 238.f, 93.f, 100.f, 100.f });
	characterArea3->SetButton("assets/ui/lobby/Button_Space_UnHover.png", { 344.f, 93.f, 100.f, 100.f });

	choiceCharacter0->SetButton("assets/ui/lobby/CharatorSelect_Button_Pick 6.png", { 496.f, 84.f, 64.f, 42.f });
	choiceCharacter1->SetButton("assets/ui/lobby/CharatorSelect_Button_Pick 2.png", { 564.f, 84.f, 64.f, 42.f });
	choiceCharacter2->SetButton("assets/ui/lobby/CharatorSelect_Button_Pick 2.png",{ 632.f, 84.f, 64.f, 42.f });
	choiceCharacter3->SetButton("assets/ui/lobby/CharatorSelect_Button_Pick 2.png",{ 700.f, 84.f, 64.f, 42.f });
	choiceCharacter4->SetButton("assets/ui/lobby/CharatorSelect_Button_Pick 2.png",{ 496.f, 84.f, 64.f, 42.f });
	choiceCharacter5->SetButton("assets/ui/lobby/CharatorSelect_Button_Pick 2.png",{ 564.f, 84.f, 64.f, 42.f });
	choiceCharacter6->SetButton("assets/ui/lobby/CharatorSelect_Button_Pick 2.png",{ 632.f, 84.f, 64.f, 42.f });
	choiceCharacter7->SetButton("assets/ui/lobby/CharatorSelect_Button_Pick 2.png",{ 700.f, 84.f, 64.f, 42.f });
	choiceCharacter8->SetButton("assets/ui/lobby/CharatorSelect_Button_Pick 2.png",{ 496.f, 84.f, 64.f, 42.f });

	choiceColorRed->SetButton("assets/ui/lobby/Button_ColorChoice_Red_Pick.png", { 488.f, 282.f, 33.f, 35.f });
	choiceColorYellow->SetButton("assets/ui/lobby/Button_ColorChoice_Yellow_Pick.png", { 524.f, 282.f, 33.f, 35.f });
	choiceColorOrange->SetButton("assets/ui/lobby/Button_ColorChoice_Orange_Pick.png", { 560.f, 282.f, 33.f, 35.f });
	choiceColorGreen->SetButton("assets/ui/lobby/Button_ColorChoice_Green_Pick.png", { 596.f, 282.f, 33.f, 35.f });
	choiceColorSkyBlue->SetButton("assets/ui/lobby/Button_ColorChoice_Skyblue_Pick.png", { 632.f, 282.f, 33.f, 35.f });
	choiceColorBlue->SetButton("assets/ui/lobby/Button_ColorChoice_Blue_Pick.png", { 668.f, 282.f, 33.f, 35.f });
	choiceColorPurple->SetButton("assets/ui/lobby/Button_ColorChoice_Purple_Pick.png", { 704.f, 282.f, 33.f, 35.f });
	choiceColorMagenta->SetButton("assets/ui/lobby/Button_ColorChoice_Pink_Pick.png", { 740.f, 282.f, 33.f, 35.f });

	mapPop->SetButton("assets/ui/lobby/choiceMap.png", { 647.f, 429.f, 121.f, 46.f });
	gameStartButton->SetButton("assets/ui/lobby/Button_GameStart_Down.png", { 538.f, 495.f, 191.f, 54.f });

	characterArea1->Reset();
	characterArea2->Reset();
	characterArea3->Reset();

	choiceColorRed	  ->Reset();
	choiceColorYellow ->Reset();
	choiceColorOrange ->Reset();
	choiceColorGreen  ->Reset();
	choiceColorSkyBlue->Reset();
	choiceColorBlue	  ->Reset();
	choiceColorPurple ->Reset();
	choiceColorMagenta->Reset();

	choiceCharacter0 ->Reset();
	choiceCharacter1 ->Reset();
	choiceCharacter2 ->Reset();
	choiceCharacter3 ->Reset();
	choiceCharacter4 ->Reset();
	choiceCharacter5 ->Reset();
	choiceCharacter6 ->Reset();
	choiceCharacter7 ->Reset();
	choiceCharacter8 ->Reset();

	mapPop->Reset();
	gameStartButton->Reset();
	exitButton->Reset();
}

void lobbyUi::Release()
{

}

void lobbyUi::Reset()
{

}

void lobbyUi::Update(float dt)
{

}

void lobbyUi::Draw(sf::RenderWindow& window)
{
	characterArea1->Draw(window);
	characterArea2->Draw(window);
	characterArea3->Draw(window);

	choiceCharacter0->Draw(window);
	choiceCharacter1->Draw(window);
	choiceCharacter2->Draw(window);
	choiceCharacter3->Draw(window);
	choiceCharacter4->Draw(window);
	choiceCharacter5->Draw(window);
	choiceCharacter6->Draw(window);
	choiceCharacter7->Draw(window);
	choiceCharacter8->Draw(window);

	choiceColorRed	  ->Draw(window);
	choiceColorYellow ->Draw(window);
	choiceColorOrange ->Draw(window);
	choiceColorGreen  ->Draw(window);
	choiceColorSkyBlue->Draw(window);
	choiceColorBlue	  ->Draw(window);
	choiceColorPurple ->Draw(window);
	choiceColorMagenta->Draw(window);

	mapPop->Draw(window);
	gameStartButton->Draw(window);
	exitButton->Draw(window);
}
