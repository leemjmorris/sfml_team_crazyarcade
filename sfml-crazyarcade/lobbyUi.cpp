#include "stdafx.h"
#include "lobbyUi.h"
#include "Button.h"
#include "SceneDemo.h"

lobbyUi::lobbyUi(const std::string& name)
	: UiHud(name)
{

}

lobbyUi::~lobbyUi()
{
	delete characterArea0;
	delete characterArea1;
	delete characterArea2;
	delete characterArea3;
	delete choiceCharacter0;
	delete choiceCharacter1;
	delete choiceCharacter2;
	delete choiceCharacter3;
	//delete choiceCharacter4;
	//delete choiceCharacter5;
	//delete choiceCharacter6;
	//delete choiceCharacter7;
	//delete choiceCharacter8;

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
	slotB.loadFromFile("assets/ui/lobby/slotB.png");
	slotD.loadFromFile("assets/ui/lobby/slotD.png");
	slotC.loadFromFile("assets/ui/lobby/slotC.png");
	slotM.loadFromFile("assets/ui/lobby/slotM.png");
	portraitTex[CharacterID::BAZZI] = &slotB;
	portraitTex[CharacterID::DAO] = &slotD;
	portraitTex[CharacterID::CAPPI] = &slotC;
	portraitTex[CharacterID::MARID] = &slotM;

	characterArea0 = new Button("characterArea0");
	characterArea1 = new Button("characterArea1");
	characterArea2 = new Button("characterArea2");
	characterArea3 = new Button("characterArea3");


	choiceCharacter0 = new Button("choiceCharacter0");
	choiceCharacter1 = new Button("choiceCharacter1");
	choiceCharacter2 = new Button("choiceCharacter2");
	choiceCharacter3 = new Button("choiceCharacter3");

	//choiceCharacter4 = new Button("choiceCharacter4");
	//choiceCharacter5 = new Button("choiceCharacter5");
	//choiceCharacter6 = new Button("choiceCharacter6");
	//choiceCharacter7 = new Button("choiceCharacter7");
	//choiceCharacter8 = new Button("choiceCharacter8");

	//Button* charBtns[] = {
	//	choiceCharacter0, choiceCharacter1, choiceCharacter2, choiceCharacter3
	//};

	choiceColorRed = new Button("choiceColorRed");
	choiceColorYellow = new Button("choiceColorYellow");
	choiceColorOrange = new Button("choiceColorOrange");
	choiceColorGreen = new Button("choiceColorGreen");
	choiceColorSkyBlue = new Button("choiceColorSkyBlue");
	choiceColorBlue = new Button("choiceColorBlue");
	choiceColorPurple = new Button("choiceColorPurple");
	choiceColorMagenta = new Button("choiceColorMagenta");

	Button* colorBtns[] = {
	choiceColorRed, choiceColorYellow, choiceColorOrange, choiceColorGreen,
	choiceColorSkyBlue, choiceColorBlue, choiceColorPurple, choiceColorMagenta
	};

	areaBtns = { characterArea0, characterArea1, characterArea2, characterArea3 };
	charBtns = { choiceCharacter0, choiceCharacter1, choiceCharacter2, choiceCharacter3 };

	//for (Button* btn : charBtns)
	//{
	//	btn->SetActive(false);
	//	btn->SetOnClick([=]
	//		{
	//			//btn->SetActive(!btn->GetActive());
	//			if (!btn->GetActive()) {
	//				if (numChars < numRooms + 1) {
	//					btn->SetActive(true);
	//					++numChars;
	//				}
	//			}
	//			else {
	//				//btn->SetActive(false);
	//				//--numChars;
	//			}
	//		});
	//}

	for (Button* btn : colorBtns)
	{
		btn->SetActive(false);
		btn->SetOnClick([=]
			{
				if (!btn->GetActive()) {
					if (numColors < numRooms + 1) {
						btn->SetActive(true);
						++numColors;
					}
				}
				else {
					btn->SetActive(false);
					--numColors;
				}
			});
	}


	mapPop = new Button("mapPop");
	mapPop->SetActive(false);
	mapPop->SetOnClick([=] { mapPop->SetActive(!mapPop->GetActive()); });

	gameStartButton = new Button("gameStartButton");
	gameStartButton->SetActive(false);
	gameStartButton->SetOnClick([=] {
		gameStartButton->SetActive(!gameStartButton->GetActive());
		if (gameStartButton->GetActive()) 
			exitButton->SetActive(false);
		//	SCENE_MGR.ChangeScene(SceneIds::Demo);
		});

	exitButton = new Button("exitButton");
	exitButton->SetActive(false);
	exitButton->SetOnClick([=] { exitButton->SetActive(!exitButton->GetActive()); });

	characterArea0->SetButton("assets/ui/lobby/Button_Space_UnHover.png", { 26.f, 93.f, 100.f, 100.f });
	characterArea1->SetButton("assets/ui/lobby/Button_Space_UnHover.png", { 132.f, 93.f, 100.f, 100.f });
	characterArea2->SetButton("assets/ui/lobby/Button_Space_UnHover.png", { 238.f, 93.f, 100.f, 100.f });
	characterArea3->SetButton("assets/ui/lobby/Button_Space_UnHover.png", { 344.f, 93.f, 100.f, 100.f });

	choiceCharacter0->SetButton("assets/ui/lobby/Button_CharatorSelect_Bazzi_Pick.png", { 496.f, 81.f, 64.f, 42.f });
	choiceCharacter1->SetButton("assets/ui/lobby/Button_CharatorSelect_Dao_Pick.png", { 564.f, 81.f, 64.f, 42.f });
	choiceCharacter2->SetButton("assets/ui/lobby/Button_CharatorSelect_Kephi_Pick.png", { 632.f, 81.f, 64.f, 42.f });
	choiceCharacter3->SetButton("assets/ui/lobby/Button_CharatorSelect_Marid_Pick.png", { 700.f, 81.f, 64.f, 42.f });
	/*choiceCharacter4->SetButton("assets/ui/lobby/CharatorSelect_Button_Pick 2.png", { 496.f, 84.f, 64.f, 42.f });
	choiceCharacter5->SetButton("assets/ui/lobby/CharatorSelect_Button_Pick 2.png", { 564.f, 84.f, 64.f, 42.f });
	choiceCharacter6->SetButton("assets/ui/lobby/CharatorSelect_Button_Pick 2.png", { 632.f, 84.f, 64.f, 42.f });
	choiceCharacter7->SetButton("assets/ui/lobby/CharatorSelect_Button_Pick 2.png", { 700.f, 84.f, 64.f, 42.f });
	choiceCharacter8->SetButton("assets/ui/lobby/CharatorSelect_Button_Pick 2.png", { 496.f, 84.f, 64.f, 42.f });*/

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

	characterArea0->Reset();
	characterArea1->Reset();
	characterArea2->Reset();
	characterArea3->Reset();

	choiceColorRed->Reset();
	choiceColorYellow->Reset();
	choiceColorOrange->Reset();
	choiceColorGreen->Reset();
	choiceColorSkyBlue->Reset();
	choiceColorBlue->Reset();
	choiceColorPurple->Reset();
	choiceColorMagenta->Reset();

	choiceCharacter0->Reset();
	choiceCharacter1->Reset();
	choiceCharacter2->Reset();
	choiceCharacter3->Reset();
	/*choiceCharacter4->Reset();
	choiceCharacter5->Reset();
	choiceCharacter6->Reset();
	choiceCharacter7->Reset();
	choiceCharacter8->Reset();*/

	mapPop->Reset();
	gameStartButton->Reset();
	exitButton->Reset();

	/*auto charIdOfBtn = [=](Button* b) -> CharacterID {
		if (b == choiceCharacter0) return CharacterID::BAZZI;
		if (b == choiceCharacter1) return CharacterID::DAO;
		if (b == choiceCharacter2) return CharacterID::CAPPI; 
		if (b == choiceCharacter3) return CharacterID::MARID;
		return CharacterID::BAZZI;
		};

	auto btnOfChar = [&](CharacterID id) -> Button* {
		switch (id) {
		case CharacterID::BAZZI: return choiceCharacter0;
		case CharacterID::DAO:   return choiceCharacter1;
		case CharacterID::CAPPI: return choiceCharacter2;
		case CharacterID::MARID: return choiceCharacter3;
		default:                 return nullptr;
		}
		};

	auto RefreshCharButtons = [&]() {
		auto used = [&](CharacterID id) {
			for (int i = 0; i < (int)areaBtns.size(); ++i) {
				if (!areaBtns[i]->GetActive()) continue;
				if (chosenCharByArea[i] != CharacterID::NONE && chosenCharByArea[i] == id) return true;
			}
			return false;
			};

		for (Button* b : charBtns) {
			if (!b) continue;
			CharacterID id = charIdOfBtn(b);
			b->SetActive(used(id));  
		}
	};*/

	for (Button* btn : { characterArea1, characterArea2, characterArea3 })
	{
		btn->SetActive(false);
		btn->SetOnClick([=]
			{
				bool willOn = !btn->GetActive();
				btn->SetActive(willOn);
				numRooms += willOn ? +1 : -1;

				// index Ã£±â
				auto indexOfArea = [&](Button* b) {
					for (int i = 0; i < (int)areaBtns.size(); ++i)
						if (areaBtns[i] == b) return i;
					return -1;
					};
				int idx = indexOfArea(btn);

				if (willOn && idx >= 0) {
					chosenCharByArea[idx] = CharacterID::BAZZI;    
					areaDirty[idx] = true;                         
				}
				else if (!willOn && idx >= 0) {
					chosenCharByArea[idx] = CharacterID::NONE;
					if (focusedArea == idx) focusedArea = -1;
				}
				/*if (btn->GetActive()) {
					btn->SetActive(false);
					--numRooms;
				}
				else {
					btn->SetActive(true);
					++numRooms;
				}*/

				//int maxChars = numRooms + 1;
				//if (numChars > maxChars) {
				//	int needToTurnOff = numChars - maxChars;
				//	for (int i = int(std::size(charBtns)) - 1; i >= 0; --i) 
				//	{ 
				//		Button* cBtn = charBtns[i];
				//		if (cBtn->GetActive()) {
				//			cBtn->SetActive(false);
				//			--numChars;
				//			if (--needToTurnOff == 0) break;
				//		}
				//	}
				//}

				int maxColors = numRooms + 1;
				if (numColors > maxColors) {
					int needToTurnOff = numColors - maxColors;
					for (Button* cBtn : colorBtns) {
						if (cBtn->GetActive()) {
							cBtn->SetActive(false);
							--numColors;
							if (--needToTurnOff == 0) break;
						}
					}
				}
				RefreshCharButtons();
			});
	}


	for (Button* btn : charBtns)
	{
		btn->SetActive(false); 
		btn->SetOnClick([this, btn]
			{
				if (focusedArea < 0 || focusedArea >= (int)areaBtns.size() || !areaBtns[focusedArea]->GetActive()) {
					std::cout << "Please Right Click first" << std::endl;
					return;
				}

				CharacterID id = CharIdOfButton(btn);
				chosenCharByArea[focusedArea] = id;

				RefreshCharButtons();
				areaDirty[focusedArea] = true;

				std::string cname;
				switch (id) {
				case CharacterID::BAZZI: cname = "BAZZI"; break;
				case CharacterID::DAO:   cname = "DAO"; break;
				case CharacterID::CAPPI: cname = "CAPPI"; break;
				case CharacterID::MARID: cname = "MARID"; break;
				case CharacterID::NONE:  cname = "NONE"; break;
				default:                 cname = "UNKNOWN"; break;
				}
				std::cout << "[Select] area=" << focusedArea
					<< " -> " << cname << std::endl;

			});
	}

	for (int i = 0; i < 4; ++i) {
		areaPortraits[i].setColor(sf::Color(255, 255, 255, 0)); 
		areaPortraits[i].setOrigin(50.f, 50.f);             

		sf::FloatRect box = areaBtns[i]->getBox().getGlobalBounds();
		areaPortraits[i].setPosition(box.left + box.width * 0.5f,
			box.top + box.height * 0.5f);
	}


	//for (Button* btn : charBtns)
	//{
	//	btn->SetActive(false);
	//	btn->SetOnClick([=]
	//		{
	//			if (!btn->GetActive()) {
	//				if (numChars < numRooms + 1) {
	//					btn->SetActive(true);
	//					++numChars;
	//				}
	//			}
	//			else {
	//				btn->SetActive(false);
	//				--numChars;
	//			}

	//			if (focusedArea >= 0 && focusedArea < (int)areaBtns.size() && areaBtns[focusedArea]->GetActive()) {
	//				CharacterID id = charIdOfBtn(btn);
	//				chosenCharByArea[focusedArea] = id;

	//				//UI UPDATE
	//			}
	//			else {
	//				std::cout << "Please Right Click first" << std::endl;
	//			}
	//		});
	//}

}

void lobbyUi::Release()
{

}

void lobbyUi::Reset()
{
	characterArea0->SetActive(true);
	if (chosenCharByArea[0] == CharacterID::NONE) {
		chosenCharByArea[0] = CharacterID::BAZZI;
		areaDirty[0] = true;
		RefreshCharButtons();
	}
}

void lobbyUi::Update(float dt)
{
	if (gameStartButton->GetActive()) {
		startButtonTimer += dt;
		if (startButtonTimer > 0.3f) {
			startButtonTimer = 0.f;
			gameStartButton->SetActive(false);
			if (numRooms == 0) 
			{
				std::cout << "Do not play 1p" << std::endl;
				return;
			}
			auto* demo = dynamic_cast<SceneDemo*>(SCENE_MGR.GetScene(SceneIds::Demo));
			if (demo) 
			{
				LobbyConfig conf;
				conf.roomCount = numRooms;

				for (int i = 0; i < (int)areaBtns.size(); ++i) {
					if (!areaBtns[i]->GetActive()) continue;
					if (chosenCharByArea[i] != CharacterID::NONE)
						conf.chars.push_back(chosenCharByArea[i]);
					else
						conf.chars.push_back(CharacterID::BAZZI);
				}
				demo->SetLobbyConfig(conf);
			}
			SCENE_MGR.ChangeScene(SceneIds::Demo);
		}
	}

	if (InputMgr::GetMouseButton(sf::Mouse::Right)) {
		sf::Vector2f mp = (sf::Vector2f)InputMgr::GetMousePosition();
		for (int i = 0; i < (int)areaBtns.size(); ++i) {
			if (areaBtns[i]->GetActive()                    
				&& areaBtns[i]->getBox().getGlobalBounds().contains(mp)) {
				focusedArea = i;          
				break;
			}
		}
	}

	for (int i = 0; i < 4; ++i) {
		if (areaDirty[i]) {
			ApplySelectionToArea(i);
			areaDirty[i] = false;
		}
	}

	characterArea0->Update(dt);
	characterArea1->Update(dt);
	characterArea2->Update(dt);
	characterArea3->Update(dt);

	choiceCharacter0->Update(dt);
	choiceCharacter1->Update(dt);
	choiceCharacter2->Update(dt);
	choiceCharacter3->Update(dt);
	/*choiceCharacter4->Update(dt);
	choiceCharacter6->Update(dt);
	choiceCharacter8->Update(dt);
	choiceCharacter5->Update(dt);
	choiceCharacter7->Update(dt);*/


	choiceColorRed->Update(dt);		
	choiceColorOrange->Update(dt);	
	choiceColorSkyBlue->Update(dt); 
	choiceColorPurple->Update(dt);	

	choiceColorYellow->Update(dt);
	choiceColorGreen->Update(dt);
	choiceColorBlue->Update(dt);
	choiceColorMagenta->Update(dt);

	mapPop->Update(dt);
	gameStartButton->Update(dt);
	exitButton->Update(dt);
}

void lobbyUi::Draw(sf::RenderWindow& window)
{
	characterArea0->Draw(window);
	characterArea1->Draw(window);
	characterArea2->Draw(window);
	characterArea3->Draw(window);

	choiceCharacter0->Draw(window);
	choiceCharacter1->Draw(window);
	choiceCharacter2->Draw(window);
	choiceCharacter3->Draw(window);
	//choiceCharacter4->Draw(window);
	//choiceCharacter5->Draw(window);
	//choiceCharacter6->Draw(window);
	//choiceCharacter7->Draw(window);
	//choiceCharacter8->Draw(window);

	choiceColorRed->Draw(window);
	choiceColorYellow->Draw(window);
	choiceColorOrange->Draw(window);
	choiceColorGreen->Draw(window);
	choiceColorSkyBlue->Draw(window);
	choiceColorBlue->Draw(window);
	choiceColorPurple->Draw(window);
	choiceColorMagenta->Draw(window);

	mapPop->Draw(window);
	gameStartButton->Draw(window);
	exitButton->Draw(window);

	for (int i = 0; i < 4; ++i) {
		if (areaBtns[i]->GetActive())
			window.draw(areaPortraits[i]);
	}
}

CharacterID lobbyUi::CharIdOfButton(Button* b) const
{
	if (b == choiceCharacter0) return CharacterID::BAZZI;
	if (b == choiceCharacter1) return CharacterID::DAO;
	if (b == choiceCharacter2) return CharacterID::CAPPI;
	if (b == choiceCharacter3) return CharacterID::MARID;
	return CharacterID::BAZZI;
}

void lobbyUi::RefreshCharButtons()
{
	auto used = [&](CharacterID id) {
		for (int i = 0; i < (int)areaBtns.size(); ++i) {
			if (!areaBtns[i] || !areaBtns[i]->GetActive()) continue;
			if (chosenCharByArea[i] != CharacterID::NONE && chosenCharByArea[i] == id)
				return true;
		}
		return false;
		};

	for (Button* b : charBtns) {
		if (!b) continue;
		CharacterID id = CharIdOfButton(b);
		b->SetActive(used(id));
	}
}

void lobbyUi::ApplySelectionToArea(int areaIndex)
{
	if (areaIndex < 0 || areaIndex >= (int)areaPortraits.size()) return;

	CharacterID id = chosenCharByArea[areaIndex];
	if (id == CharacterID::NONE) {
		areaPortraits[areaIndex].setColor(sf::Color(255, 255, 255, 0));
		return;
	}

	std::cout << "ApplySelectionToArea: i=" << areaIndex
		<< " id=" << (int)chosenCharByArea[areaIndex] << std::endl;
	auto tex = portraitTex[chosenCharByArea[areaIndex]];
	std::cout << "tex size = " << tex->getSize().x << "," << tex->getSize().y << std::endl;
	std::cout << "sprite pos = " << areaPortraits[areaIndex].getPosition().x
		<< "," << areaPortraits[areaIndex].getPosition().y << std::endl;

    areaPortraits[areaIndex].setTexture(*portraitTex[id], true);

	areaPortraits[areaIndex].setColor(sf::Color::White);
}
