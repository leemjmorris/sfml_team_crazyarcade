#include "stdafx.h"
#include "ResultPop.h"
#include "Player.h"

ResultPop::ResultPop(const std::string& name)
    :UiHud(name)
{
}

void ResultPop::Init()
{
    FONT_MGR.Load("assets/font/ARCADECLASSIC.TTF");

    bgTex.loadFromFile("assets/ui/game/result.png");
    bgSpr.setTexture(bgTex);
    bgSpr.setPosition({ 38.f, 137.f });

    lineBgTex.loadFromFile("assets/ui/game/winBox.png");

    showWin = false;
    SetActive(false);
}

void ResultPop::SetResult(const std::vector<Player*>& players)
{
    resultTexts.clear();
    lineBgs.clear();
    lineNums.clear();

    constexpr float firstY = 180.f;
    constexpr float lineGap = 26.f;

    int idx = 0;
    for (auto* p : players)
    {
        sf::Sprite bg(lineBgTex);
        bg.setPosition({ 50.f, firstY + lineGap * idx });
        lineBgs.push_back(std::move(bg));

        sf::Text num;
        num.setFont(FONT_MGR.Get("assets/font/ARCADECLASSIC.TTF"));
        num.setCharacterSize(18);
        num.setFillColor(sf::Color::Black);
        num.setString(std::to_string(idx + 1));  // 1P, 2P ¡¦

        num.setPosition({ 60.f, firstY + lineGap * idx - 2.f });
        lineNums.push_back(std::move(num));

        sf::Text res;
        res.setFont(FONT_MGR.Get("assets/font/ARCADECLASSIC.TTF"));
        res.setCharacterSize(26);
        res.setFillColor(sf::Color::White);

        switch (p->GetPlayerState())
        {
        case AnimState::Win: res.setString("WIN    " + p->GetName()); break;
        case AnimState::Dead: res.setString("LOSE    " + p->GetName()); break;
        case AnimState::Draw: res.setString("DRAW"); break;
        default:              res.setString("--");   break;
        }

        res.setPosition({ 90.f, firstY + lineGap * idx - 6.f });
        resultTexts.push_back(std::move(res));

        ++idx;
    }
}

void ResultPop::SetWinner(int winner)
{
    // ex) assets/ui/game/1p Win.png, 2p Win.png ...
    std::string file = "assets/ui/game/" + std::to_string(winner) + "p Win.png";
    if (!winTex.loadFromFile(file))
        return;

    winSpr.setTexture(winTex);

    sf::FloatRect w = winSpr.getLocalBounds();
    winSpr.setOrigin(w.width * 0.5f, 0.f);

    sf::FloatRect b = bgSpr.getGlobalBounds();
    winSpr.setPosition(b.left + b.width * 0.5f, b.top - w.height - 8.f);

    showWin = true;
}

void ResultPop::Release()
{
}

void ResultPop::Reset()
{
    if (showWin)
    {
        sf::FloatRect w = winSpr.getLocalBounds();
        sf::FloatRect b = bgSpr.getGlobalBounds();
        winSpr.setPosition(b.left + b.width * 0.5f, b.top - w.height - 8.f);
    }
}

void ResultPop::Draw(sf::RenderWindow& window)
{
    if (!GetActive()) return;

    window.draw(bgSpr);

    if (showWin)
        window.draw(winSpr);

    for (size_t i = 0; i < lineBgs.size(); ++i)
    {
        window.draw(lineBgs[i]);
        window.draw(lineNums[i]);
        window.draw(resultTexts[i]);
    }
}