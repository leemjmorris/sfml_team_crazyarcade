#include "stdafx.h"
#include "TestMap.h"

int main()
{
    FRAMEWORK.Init(800, 600, "CRAZY ARCADE");
    FRAMEWORK.Do();
    FRAMEWORK.Release();

    return 0;
}

#pragma region TestMap Region

//int main()
//{
//    FRAMEWORK.Init(800, 600, "CRAZY ARCADE");
//
//    // TestMap 생성 및 테스트
//    TestMap testMap;
//
//    // 간단한 테스트 루프 (FRAMEWORK.Do() 대신)
//    sf::RenderWindow& window = FRAMEWORK.GetWindow(); // FRAMEWORK에서 윈도우 가져오기
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        // 업데이트 및 렌더링
//        testMap.Update(0.016f); // 60FPS 가정
//
//        window.clear();
//        testMap.Draw(window);
//        window.display();
//    }
//
//    FRAMEWORK.Release();
//    return 0;
//}

#pragma endregion
