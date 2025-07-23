#include "stdafx.h"
#include "InputMgr.h"

std::list<int> InputMgr::downKeys;
std::list<int> InputMgr::heldKeys;
std::list<int> InputMgr::upKeys;

std::unordered_map<Axis, AxisInfo> InputMgr::axisInfoMap;

sf::Vector2i InputMgr::mousePosition;

float InputMgr::mouseWheelDelta = 0.0f;
bool InputMgr::mouseWheelScrolled = false;

void InputMgr::Init()
{
	AxisInfo infoH_1p;
	AxisInfo infoH_2p;
	infoH_1p.axis = Axis::Horizontal_1p;
	infoH_2p.axis = Axis::Horizontal_2p;
	infoH_1p.positives.push_back(sf::Keyboard::D);
	infoH_2p.positives.push_back(sf::Keyboard::Right);
	infoH_1p.negatives.push_back(sf::Keyboard::A);
	infoH_2p.negatives.push_back(sf::Keyboard::Left);
	axisInfoMap.insert({ Axis::Horizontal_1p , infoH_1p });
	axisInfoMap.insert({ Axis::Horizontal_2p , infoH_2p });
	AxisInfo infoV_1p;
	AxisInfo infoV_2p;
	infoV_1p.axis = Axis::Vertical_1p;
	infoV_2p.axis = Axis::Vertical_2p;
	infoV_1p.positives.push_back(sf::Keyboard::S);
	infoV_2p.positives.push_back(sf::Keyboard::Down);
	infoV_1p.negatives.push_back(sf::Keyboard::W);
	infoV_2p.negatives.push_back(sf::Keyboard::Up);
	axisInfoMap.insert({ Axis::Vertical_1p , infoV_1p });
	axisInfoMap.insert({ Axis::Vertical_2p , infoV_2p});
}

void InputMgr::Clear()
{
	downKeys.clear();
	upKeys.clear();
	mouseWheelDelta = 0.f;
	mouseWheelScrolled = false;
}

void InputMgr::UpdateEvent(const sf::Event& ev)
{
	switch (ev.type)
	{
	case sf::Event::KeyPressed:
		if (!Contains(heldKeys, ev.key.code))
		{
			downKeys.push_back(ev.key.code);
			heldKeys.push_back(ev.key.code);
		}
		break;
	case sf::Event::KeyReleased:
		Remove(heldKeys, ev.key.code);
		upKeys.push_back(ev.key.code);
		break;
	case sf::Event::MouseButtonPressed:
	{
		int code = sf::Keyboard::KeyCount + ev.mouseButton.button;
		if (!Contains(heldKeys, code))
		{
			downKeys.push_back(code);
			heldKeys.push_back(code);
		}
	}
	break;
	case sf::Event::MouseButtonReleased:
	{
		int code = sf::Keyboard::KeyCount + ev.mouseButton.button;
		Remove(heldKeys, code);
		upKeys.push_back(code);
	}
	break;
	case sf::Event::MouseWheelScrolled:
		mouseWheelDelta = ev.mouseWheelScroll.delta;
		mouseWheelScrolled = true;
		break;	
	}
}

void InputMgr::Update(float dt)
{
	mousePosition = sf::Mouse::getPosition(FRAMEWORK.GetWindow());

	for (auto& pair : axisInfoMap)
	{
		AxisInfo& axisInfo = pair.second;
		float raw = GetAxisRaw(axisInfo.axis); // -1, 0, 1
		float dir = raw;
		if (raw == 0.f && axisInfo.value != 0.f)
		{
			dir = axisInfo.value > 0.f ? -1.f : 1.f;
		}
		axisInfo.value += dir * axisInfo.sensi * dt;
		axisInfo.value = Utils::Clamp(axisInfo.value, -1.f, 1.f);

		float stopThreshold = std::abs(dir * axisInfo.sensi * dt);
		if (raw == 0.f && std::abs(axisInfo.value) < stopThreshold)
		{
			axisInfo.value = 0.f;
		}
	}

}

bool InputMgr::GetKeyDown(sf::Keyboard::Key key)
{
	return Contains(downKeys, key);
}

bool InputMgr::GetKeyUp(sf::Keyboard::Key key)
{
	return Contains(upKeys, key);
}

bool InputMgr::GetKey(sf::Keyboard::Key key)
{
	return Contains(heldKeys, key);
}

bool InputMgr::Contains(const std::list<int>& list, int key)
{
	return std::find(list.begin(), list.end(), key) != list.end();
}

void InputMgr::Remove(std::list<int>& list, int key)
{
	list.remove(key);
}

float InputMgr::GetAxisRaw(Axis axis)
{
	auto findIt = axisInfoMap.find(axis);
	if (findIt == axisInfoMap.end())
		return 0.f;

	const AxisInfo& axisInfo = findIt->second;
	auto it = heldKeys.rbegin();
	while (it != heldKeys.rend())
	{
		int code = *it;
		if (Contains(axisInfo.positives, code))
		{
			return 1.f;
		}
		if (Contains(axisInfo.negatives, code))
		{
			return -1.f;
		}
		++it;
	}
	return 0.0f;
}

float InputMgr::GetAxis(Axis axis)
{
	auto findIt = axisInfoMap.find(axis);
	if (findIt == axisInfoMap.end())
	{
		return 0.0f;
	}
	return findIt->second.value;
}

bool InputMgr::GetMouseButtonDown(sf::Mouse::Button key)
{
	return Contains(downKeys, sf::Keyboard::KeyCount + key);;
}

bool InputMgr::GetMouseButtonUp(sf::Mouse::Button key)
{
	return Contains(upKeys, sf::Keyboard::KeyCount + key);;
}

bool InputMgr::GetMouseButton(sf::Mouse::Button key)
{
	return Contains(heldKeys, sf::Keyboard::KeyCount + key);;
}

sf::Vector2f InputMgr::GetPriorityDirection()
{
	for (auto it = heldKeys.rbegin(); it != heldKeys.rend(); ++it)
	{
		int code = *it;
		if (code == sf::Keyboard::W || code == sf::Keyboard::Up)
			return { 0.f, -1.f };
		if (code == sf::Keyboard::S || code == sf::Keyboard::Down)
			return { 0.f, 1.f };
		if (code == sf::Keyboard::A || code == sf::Keyboard::Left)
			return { -1.f, 0.f };
		if (code == sf::Keyboard::D || code == sf::Keyboard::Right)
			return { 1.f, 0.f };
	}
	return { 0.f, 0.f };
}

sf::Vector2i InputMgr::GetMousePosition()
{
	return mousePosition;
}

float InputMgr::GetMouseWheelDelta()
{
	return mouseWheelDelta;
}

bool InputMgr::IsMouseWheelScrolled()
{
	return mouseWheelScrolled;
}

bool InputMgr::IsMouseWheelUp()
{
	return mouseWheelScrolled && mouseWheelDelta > 0;
}

bool InputMgr::IsMouseWheelDown()
{
	return mouseWheelScrolled && mouseWheelDelta < 0;
}

