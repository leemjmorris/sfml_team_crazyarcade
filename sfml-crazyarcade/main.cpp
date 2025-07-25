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
//    // TestMap ���� �� �׽�Ʈ
//    TestMap testMap;
//
//    // ������ �׽�Ʈ ���� (FRAMEWORK.Do() ���)
//    sf::RenderWindow& window = FRAMEWORK.GetWindow(); // FRAMEWORK���� ������ ��������
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
//        // ������Ʈ �� ������
//        testMap.Update(0.016f); // 60FPS ����
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
