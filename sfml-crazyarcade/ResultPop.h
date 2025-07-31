#pragma once
#include "UiHud.h"

class Player;
class ResultPop : public UiHud
{
public:
    ResultPop(const std::string& name);
    ~ResultPop() = default;

    void Init() override;
    void Reset() override;
    void Update(float dt) override {}
    void Draw(sf::RenderWindow& window) override;
    void Release() override;

    void SetResult(const std::vector<Player*>& players);
    void SetWinner(int winnerIndex);          // 1 ¡æ 1P, 2 ¡æ 2P ¡¦

private:
    sf::Texture bgTex;
    sf::Sprite  bgSpr;

    sf::Texture winTex;
    sf::Sprite  winSpr;
    bool        showWin = false;

    sf::Texture lineBgTex;
    std::vector<sf::Sprite> lineBgs;

    std::vector<sf::Text> lineNums;

    std::vector<sf::Text> resultTexts;
};