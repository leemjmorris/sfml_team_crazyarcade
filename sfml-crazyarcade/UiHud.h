#pragma once
class UiHud
{
protected:
	std::string name;
	bool active = true;

	sf::Vector2f position;

public:
	UiHud(const std::string& name);
	UiHud(const std::string& name, bool b);
	~UiHud();

	SortingLayers sortingLayer = SortingLayers::UI;
	int sortingOrder = 0;

	const std::string GetName() const { return name; }
	bool GetActive() const { return active; }
	virtual void SetActive(bool a) { active = a; }

	sf::Vector2f GetPosition() const { return position; }
	virtual void SetPosition(const sf::Vector2f& pos) { position = pos; }

	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Reset() = 0;

	virtual void Update(float dt) = 0;
	virtual void Draw(sf::RenderWindow& window) = 0;
};

