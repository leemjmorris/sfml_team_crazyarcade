#include "stdafx.h"
#include "Player.h"
#include "WaterBalloon.h"

Player::Player(const std::string& name, CharacterID id, int index)
	: GameObject(name),
	charId(id),
	curSpeed(100.f),
	activeWaterBalloonLength(1),
	velocity({ 1.f, 1.f }),
	dir({ 1.f, 1.f }),
	playerIndex(index),
	isStart(false),
	dieTimer(0.f),
	readyTimer(0.f),
	activeBalloons(0),
	animState(AnimState::Normal),
	hAxis(Axis::Horizontal_1p),
	vAxis(Axis::Vertical_1p),
	installWaterBomb(sf::Keyboard::Unknown),
	obj(nullptr)
{
	const auto& stats = CharacterTable.at(charId);

	curSpeed = stats.intiPlayerSpeed;
	balloonCapacity = stats.initBombCount;
	maxBalloonCount = stats.maxBombCount;
	maxBalloonLength = stats.maxbombLength;
}

Player::~Player()
{
}

void Player::PlayerEvent(float dt)
{
	if (InputMgr::GetKeyDown(installWaterBomb))
	{
		CheckInstallWaterballoon();
	}
	//if (isAlive)
	//{
	//	isTrapped = false;
	//	animState = AnimState::Live;
	//	animator.Play("animation/bazzi_live.csv");
	//}
}

void Player::OnBalloonExploded()
{
	if (activeBalloons > 0) --activeBalloons;
	//ClearFootBomb(footBomb);
}

bool Player::CanPlaceBalloon() const
{
	return activeBalloons < balloonCapacity;
}

bool Player::CheckInstallWaterballoon()
{
	if (animState != AnimState::Live)
		return false;

	if (!CanPlaceBalloon()) {
		std::cout << "all waterballoon is installed" << std::endl;
		return false;
	}

	WaterBalloon* b = WaterBalloon::Spawn("bomb", { GetPosition().x, GetPosition().y - 10.f }, GetWaterBalloonLength(), this);
	if (!b) {
		std::cout << "cannot install position\n";
		return false;
	}
	std::cout << "Spawned bomb ptr = " << b << '\n';
	if (playerIndex == 0)
		std::cout << "Player 1 activeBalloons:" << activeBalloons << ", balloonCapacity: " << balloonCapacity << std::endl;
	else
		std::cout << "Player 2  activeBalloons:" << activeBalloons << ", balloonCapacity: " << balloonCapacity << std::endl;

	passThroughBombs.insert(b);
	++activeBalloons;
	return true;
}

bool Player::HandleBubbleDeath(AnimState s)
{
	animState = s;
	animator.Play("animation/bazzi_die.csv");
	return true;
}

void Player::AddSpeed(float s)
{
	curSpeed += s;
}

void Player::AddWaterBalloonCount(int b)
{
	balloonCapacity = Utils::Clamp(balloonCapacity + b, 1, maxBalloonCount);
}

void Player::AddWaterBalloonLength(int l)
{
	activeWaterBalloonLength += l;
}

//====================================GAME OVER==========================================
void Player::SetGameOver(bool t, bool l, float dt)
{
	isAnotherDead = t;
	isDraw = l;
	gameOverStarted = true;
	if (!gameOverStarted)
	{
		gameOverStarted = true;
		winTimer = 0.f;
	}
}

void Player::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	sprite.setPosition(pos);
}

void Player::SetRotation(float rot)
{
	GameObject::SetRotation(rot);
	sprite.setRotation(rot);
}

void Player::SetScale(const sf::Vector2f& s)
{
	GameObject::SetScale(s);
	sprite.setScale(s);
}

void Player::SetOrigin(const sf::Vector2f& o)
{
	GameObject::SetOrigin(o);
	sprite.setOrigin(o);
}

void Player::SetOrigin(Origins preset)
{
	GameObject::SetOrigin(preset);
	if (preset != Origins::Custom)
	{
		Utils::SetOrigin(sprite, preset);
	}
}

void Player::Init()
{
	//std::cout << "[Init balloonCount]" << curWaterBalloonCount << ", [Init balloonLength]" << curWaterBalloonLength << ", [Init Speed]" << curSpeed << std::endl;
	SetOrigin(Origins::BC);
	animator.SetTarget(&sprite);

	switch (playerIndex)
	{
	case 0:
		vAxis = Axis::Vertical_1p;
		hAxis = Axis::Horizontal_1p;
		installWaterBomb = sf::Keyboard::LShift;
		break;
	case 1:
		vAxis = Axis::Vertical_2p;
		hAxis = Axis::Horizontal_2p;
		installWaterBomb = sf::Keyboard::RShift;
		break;
	}
}

void Player::Release()
{

}

void Player::Reset()
{
	gameOverStarted = false;
	atBalloon = false;
	sortingLayer = SortingLayers::Foreground;
	animState = AnimState::Normal;
	sortingOrder = 0;
	curSpeed = CharacterTable.at(charId).intiPlayerSpeed;
	balloonCapacity = CharacterTable.at(charId).initBombCount;
	activeBalloons = 0;
	activeWaterBalloonLength = 1;
	animator.Play("animation/bazzi_run.csv");
}

void Player::Update(float dt)
{
	SetOrigin(Origins::BC);
	if (isStart)
	{
		readyTimer += dt;

		dir = { 0.f, 0.f }; // LSY: stop moving when ready
		//std::cout << readyTimer << std::endl;
		if (readyTimer > 1.0f) // LSY: 1.f is the time to wait for the player to enter the game
		{
			animState = AnimState::Live;
			readyTimer = 0.0f;
			isStart = false;
		}
	}
	else
	{
		RefreshPassThroughSet();
		Movement(dt);
	}
	animator.Update(dt);
	PlayerEvent(dt);

	CheckCollWithSplash();

	// LSY: if player is trapped, then stop moving
	if (animState == AnimState::Trapped)
	{
		dieTimer += dt;
		//std::cout << "TrappedTimer: " << dieTimer << std::endl;
		if (dieTimer > 3.7f)
		{
			animState = AnimState::Dead;
			dieTimer = 0.f;
			animator.Play("animation/bazzi_die.csv");
			std::cout << "TrappedTimer is finished: AnimeState::Dead" << std::endl;
		}
	}
	if (gameOverStarted && animState != AnimState::Win)
	{
		winTimer += dt;
		if (winTimer > 1.f)
		{
			winTimer = 0.f;
			gameOverStarted = false;
			animator.Play("animation/bazzi_win.csv");
			animState = AnimState::Win;
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	hitBox.Draw(window);
}

inline sf::Vector2i ToGrid(const sf::Vector2f& worldPos)
{
	return { int(worldPos.x / 52), int(worldPos.y / 52) };
}

void Player::CheckCollWithSplash()
{
	if (animState == AnimState::Trapped)
		return;

	auto waterSplashes = SCENE_MGR.GetCurrentScene()->FindGameObjects("WaterSplash");
	for (auto* obj : waterSplashes)
	{
		WaterSplash* splashObj = dynamic_cast<WaterSplash*>(obj);

		if (splashObj && splashObj->GetActive())
		{
			sf::FloatRect rect(splashObj->GetGlobalBounds()); // left, top, width, height

			if (rect.contains({ GetPosition().x, GetPosition().y - 20.f }))
			{
				animState = AnimState::Trapped;
				animator.Play("animation/bazzi_trap.csv", true);
				break;
			}
		}
	}
}


void Player::RefreshPassThroughSet()
{
	sf::FloatRect me = hitBox.rect.getGlobalBounds();

	for (auto it = passThroughBombs.begin(); it != passThroughBombs.end(); )
	{
		WaterBalloon* wb = *it;


		bool stillOverlap = wb->GetActive() &&
			wb->GetGlobalBounds().intersects(me);

		if (!stillOverlap)
			it = passThroughBombs.erase(it);
		else
			++it;
	}
}

bool Player::CollectObstacleRects(std::vector<sf::FloatRect>& outRects)
{
	Scene* cur = SCENE_MGR.GetCurrentScene();

	for (auto* obj : cur->FindGameObjects("Block"))
	{
		Block* blk = dynamic_cast<Block*>(obj);
		if (blk && blk->GetActive() && blk->IsDestroyable())
			outRects.push_back(blk->GetHitBox().GetGlobalBounds());
	}

	for (auto* obj : cur->FindGameObjects("bomb"))
	{
		auto* wb = dynamic_cast<WaterBalloon*>(obj);
		if (!wb || !wb->GetActive()) continue;

		if (passThroughBombs.count(wb)) continue;
		outRects.push_back(wb->GetGlobalBounds());

	}
	return !outRects.empty();
}

size_t Player::GetCollidedObstacleInfo(sf::FloatRect& outBounds)
{
	std::vector<sf::FloatRect> rects;
	CollectObstacleRects(rects);

	size_t cnt = 0;
	for (auto& r : rects)
	{
		if (hitBox.rect.getGlobalBounds().intersects(r))
		{
			outBounds = r;
			++cnt;
		}
	}
	return cnt;
}

// KHI
void Player::Movement(float dt)
{
	if (animState == AnimState::Win || animState == AnimState::Dead)
		return;


	if (animState == AnimState::Live || animState == AnimState::Trapped)
	{
		PlayMoveAnimation();
		dir = InputMgr::GetPriorityDirection(hAxis, vAxis, playerIndex);

		sf::Vector2f currentPos = GetPosition();
		sf::Vector2f tempPos = currentPos;

		const float correction = slidePixelsPerSecond * dt; // KHI: Distance to nudge the player during collision (slide correction offset)

		// KHI: Get Player Center
		sf::FloatRect bounds = hitBox.rect.getGlobalBounds();
		sf::Vector2f playerCenter = {
			bounds.left + bounds.width * 0.5f,
			bounds.top + bounds.height * 0.5f
		};

		sf::FloatRect collidedBounds;

		// KHI: Move X
		sf::Vector2f tryX = currentPos + sf::Vector2f(dir.x * curSpeed * dt, 0.f);
		sprite.setPosition(tryX);
		hitBox.UpdateCustomTransform(sprite, playerHitBoxSize, playerHitBoxOffset, Origins::BC);
		//size_t collidedX = GetCollidedTileInfo(collidedBounds);
		size_t collidedX = GetCollidedObstacleInfo(collidedBounds);

		if (collidedX == 0)
		{
			tempPos.x = tryX.x;
		}
		else if (collidedX == 1)
		{
			float third = tileSize / 3.f;
			float upper = collidedBounds.top + third * 0.2;
			float lower = collidedBounds.top + third * 2.8;

			if (playerCenter.y < upper)
			{
				tempPos.y -= correction;
			}
			else if (playerCenter.y > lower)
			{
				tempPos.y += correction;
			}
			slidePlayer = true;
		}

		if (collidedX && (InputMgr::GetAxisRaw(vAxis) != 0 || InputMgr::GetAxisRaw(hAxis) != 0))
		{
			pushedCount += dt;
			std::cout << pushedCount << std::endl;
		}

		// KHI: Move Y
		sf::Vector2f tryY = tempPos + sf::Vector2f(0.f, dir.y * curSpeed * dt);
		sprite.setPosition(sf::Vector2f(tempPos.x, tryY.y));
		hitBox.UpdateCustomTransform(sprite, playerHitBoxSize, playerHitBoxOffset, Origins::BC);
		//size_t collidedY = GetCollidedTileInfo(collidedBounds);
		size_t collidedY = GetCollidedObstacleInfo(collidedBounds);

		if (collidedY == 0 && !slidePlayer)
		{
			tempPos.y = tryY.y;
		}
		else if (collidedY == 1)
		{
			float third = tileSize / 3.f;
			float left = collidedBounds.left + third * 0.2;
			float right = collidedBounds.left + third * 2.8;

			if (playerCenter.x < left)
			{
				tempPos.x -= correction;
			}
			else if (playerCenter.x > right)
			{
				tempPos.x += correction;
			}
			slidePlayer = true;
		}
		slidePlayer = false;

		if (collidedY && (InputMgr::GetAxisRaw(vAxis) != 0 || InputMgr::GetAxisRaw(hAxis) != 0))
		{
			pushedCount += dt;
			std::cout << pushedCount << std::endl;
		}

		SetPosition(tempPos);
		float tempSpeed = GetSpeed();
		if (animState == AnimState::Live)
			SetScale({ dir.x < 0 ? -1.f : dir.x > 0 ? 1.f : sprite.getScale().x, 1.f });

		if (animState == AnimState::Trapped)
		{
			curSpeed = 10.f;
		}
		else
		{
			curSpeed = tempSpeed;
		}
	}
}

// LSY: hange return value : bool -> size_t for counting

size_t Player::GetCollidedTileInfo(sf::FloatRect& outTileBounds)
{
	Scene* curScene = SCENE_MGR.GetCurrentScene();
	Block* block = dynamic_cast<Block*>(obj);
	auto gameObjects = curScene->FindGameObjects("Block");
	size_t cnt = 0;
	for (auto* obj : gameObjects)
	{
		Block* block = dynamic_cast<Block*>(obj);
		if (block && block->IsDestroyable())
		{
			sf::FloatRect blockBounds = block->GetHitBox().GetGlobalBounds();
			if (hitBox.rect.getGlobalBounds().intersects(blockBounds))
			{
				if (block && block->GetActive() && block->IsDestroyable())
				{
					outTileBounds = blockBounds;
					cnt++;
				}
			}
		}
	}
	return cnt;
}

//  //KHI(ver)
//bool Player::GetCollidedTileInfo(sf::FloatRect& outTileBounds)
// {
//Scene* curScene = SCENE_MGR.GetCurrentScene();
//auto gameObjects = curScene->FindGameObjects("Block");
//
//for (auto* obj : gameObjects)
//{
//	Block* block = dynamic_cast<Block*>(obj);
//	if (block && block->IsDestroyable())
//	{
//		sf::FloatRect blockBounds = block->GetHitBox().GetGlobalBounds();
//		if (hitBox.rect.getGlobalBounds().intersects(blockBounds))
//		{
//			if (block && block->GetActive() && block->IsDestroyable())
//			{
//				outTileBounds = blockBounds;
//				return true;
//			}
//		}
// 	 } 
//}
// return false; 
//}