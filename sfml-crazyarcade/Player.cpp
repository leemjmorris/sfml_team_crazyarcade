#include "stdafx.h"
#include "Player.h"
#include "WaterBalloon.h"

Player::Player(const std::string& name, CharacterID id, int index, const std::string& resultName)
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
	rName(resultName),
	obj(nullptr)
{
	SetOrigin(Origins::BC);
	const auto& stats = CharacterTable.at(charId);
	animator.SetTarget(&sprite);
	curSpeed = stats.intiPlayerSpeed;
	balloonCapacity = stats.initBombCount;
	maxBalloonCount = stats.maxBombCount;
	maxBalloonLength = stats.maxbombLength;

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
	case 2:
		vAxis = Axis::None;
		hAxis = Axis::None;
		installWaterBomb = sf::Keyboard::KeyCount;
		break;
	case 3:
		vAxis = Axis::None;
		hAxis = Axis::None;
		installWaterBomb = sf::Keyboard::KeyCount;
		break;
	}
}

Player::~Player()
{
}

void Player::PlayerEvent(float dt)
{
	// LMJ: Player 1 - keyboard bomb input only
	if (playerIndex == 0 && InputMgr::GetKeyDown(installWaterBomb))
	{
		CheckInstallWaterballoon();
	}


	if ((playerIndex == 0 || playerIndex == 1) && InputMgr::GetKeyDown(installWaterBomb))
	{
		CheckInstallWaterballoon();
	}

	// LMJ: Gamepad bomb input for players 3 and 4
	if (playerIndex == 2 || playerIndex == 3)
	{
		int gamepadIndex = playerIndex - 2; // LMJ: Convert to gamepad index (0 or 1)
		if (InputMgr::GetGamepadBombButton(gamepadIndex))
		{
			CheckInstallWaterballoon();
		}
	}
}

void Player::OnBalloonExploded()
{
	if (activeBalloons > 0) --activeBalloons;
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
		//std::cout << "all waterballoon is installed" << std::endl;
		return false;
	}

	WaterBalloon* b = WaterBalloon::Spawn("bomb", { GetPosition().x, GetPosition().y - 10.f }, GetWaterBalloonLength(), this);
	if (!b) {
		//std::cout << "cannot install position\n";
		return false;
	}

	SOUND_MGR.PlaySfx("sounds/Set_WaterBomb.mp3");

	passThroughBombs.insert(b);
	++activeBalloons;
	return true;
}

bool Player::HandleBubbleDeath(AnimState s)
{
	SOUND_MGR.PlaySfx("sounds/Kill_WaterBomb.wav");

	animState = s;
	animator.Play(convertAniStr("animation/", "_die.csv"));
	return true;
}

void Player::AddSpeed(float s)
{
	SOUND_MGR.PlaySfx("sounds/Eat_Item.mp3");

	const auto& stats = CharacterTable.at(charId);
	if (curSpeed >= stats.maxPlayerSpeed)
	{
		curSpeed = stats.maxPlayerSpeed;
	}
	curSpeed += s;
}

void Player::AddWaterBalloonCount(int b)
{
	SOUND_MGR.PlaySfx("sounds/Eat_Item.mp3");

	balloonCapacity = Utils::Clamp(balloonCapacity + b, 1, maxBalloonCount);
}

void Player::AddWaterBalloonLength(int l)
{
	SOUND_MGR.PlaySfx("sounds/Eat_Item.mp3");

	activeWaterBalloonLength += l;
}

//====================================GAME OVER==========================================
void Player::SetPlayerState(AnimState s)
{
	animState = s;
}

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
	animator.Play(convertAniStr("animation/", "_run.csv"));
}

void Player::PlayMoveAnimation()
{
	const std::string clipId = animator.GetCurrentClipId();
	if (animState == AnimState::Live)
	{
		if (dir.x != 0 && clipId != "Run")
			animator.Play(convertAniStr("animation/", "_run.csv"));
		else if (dir.y < 0 && clipId != "Up")
			animator.Play(convertAniStr("animation/", "_up.csv"));
		else if (dir.y > 0 && clipId != "Down")
			animator.Play(convertAniStr("animation/", "_down.csv"));
		else if (dir == sf::Vector2f(0.f, 0.f)) {
			if (charId == CharacterID::BAZZI)
			{
				if (clipId == "Run") animator.Play(convertAniStr("animation/", "_run.csv"));
				else if (clipId == "Up") animator.Play(convertAniStr("animation/", "_up.csv"));
				else if (clipId == "Down") animator.Play(convertAniStr("animation/", "_down.csv"));
			}
			else {
				if (clipId == "Run") animator.Play(convertAniStr("animation/", "_idleSide.csv"));
				else if (clipId == "Up") animator.Play(convertAniStr("animation/", "_idleUp.csv"));
				else if (clipId == "Down") animator.Play(convertAniStr("animation/", "_idle.csv"));
			}
		}
	}
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

	if (needsHiddenStateCheck)
	{
		CheckHiddenState();
		needsHiddenStateCheck = false;
	}

	// LSY: if player is trapped, then stop moving
	if (animState == AnimState::Trapped)
	{
		dieTimer += dt;
		//std::cout << "TrappedTimer: " << dieTimer << std::endl;
		if (dieTimer > 3.7f)
		{
			animState = AnimState::Dead;
			dieTimer = 0.f;
			animator.Play(convertAniStr("animation/", "_die.csv"));
			std::cout << "TrappedTimer is finished: AnimeState::Dead" << std::endl;
		}
	}
	if (gameOverStarted && animState != AnimState::Win)
	{
		winTimer += dt;
		if (winTimer > 1.f)
		{
			animState = AnimState::Win;
			winTimer = 0.f;
			gameOverStarted = false;
			//animator.Play("animation/bazzi_win.csv");
			animator.Play(convertAniStr("animation/", "_win.csv"));
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
	if (animState == AnimState::Trapped || animState == AnimState::Dead)
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
				//animator.Play("animation/bazzi_trap.csv", true);
				animator.Play(convertAniStr("animation/", "_trap.csv"), true);
				break;
			}
		}
	}
}

void Player::SetEnter(bool t) 
{
	animator.Play(convertAniStr("animation/", "_ready.csv"), true);
	if (charId == CharacterID::BAZZI) {
		animator.PlayQueue("animation/bazzi_ready2.csv");
		animator.PlayQueue("animation/bazzi_ready2.csv");
		animator.PlayQueue("animation/bazzi_ready2.csv");
		animator.PlayQueue("animation/bazzi_ready2.csv");
		animator.PlayQueue("animation/bazzi_ready2.csv");
	}
	else {
		SetScale({ 1.2f, 1.2f });
	}

	animState = AnimState::Ready;
	isStart = t;
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

const std::string Player::convertAniStr(const std::string& s, const std::string& ss)
{
	switch (charId) {
		case CharacterID::BAZZI: return s + "bazzi" + ss;
		case CharacterID::DAO:   return s + "dao" + ss;
		case CharacterID::CAPPI: return s + "cappy" + ss;
		case CharacterID::MARID: return s + "marid" + ss;
		default:                 return s + "bazzi" + ss;
	}
}

bool Player::CollectObstacleRects(std::vector<sf::FloatRect>& outRects)
{
	Scene* cur = SCENE_MGR.GetCurrentScene();

	for (auto* obj : cur->FindGameObjects("Block"))
	{
		Block* blk = dynamic_cast<Block*>(obj);
		if (blk && blk->GetActive() && !blk->IsHidable())
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

		// LMJ: 1P keyboard, 2P gamepad (for testing), 3P, 4P gamepad
		if (playerIndex == 0 || playerIndex == 1)
		{
			// LMJ: Player 1 - keyboard input only
			dir = InputMgr::GetPriorityDirection(hAxis, vAxis, playerIndex);
		}
		else if (playerIndex == 2 || playerIndex == 3)
		{
			// LMJ: Player 3, 4 - gamepad input
			int gamepadIndex = playerIndex - 2;
			
			sf::Vector2f gamepadDir = InputMgr::GetGamepadDirection(gamepadIndex);
			if (gamepadDir.x != 0.f || gamepadDir.y != 0.f)
			{
				dir = gamepadDir;
			}
			else
			{
				dir = { 0.f, 0.f };
			}
		}

		sf::Vector2f currentPos = GetPosition();
		sf::Vector2f tempPos = currentPos;

		const float correction = slidePixelsPerSecond * dt; // KHI: Distance for making player slide during collision (slide correction offset)

		// KHI: Get Player Center
		sf::FloatRect bounds = hitBox.rect.getGlobalBounds();
		sf::Vector2f playerCenter = { bounds.left + bounds.width * 0.5f, bounds.top + bounds.height * 0.5f };

		sf::FloatRect collidedBounds;
		bool collX = false;
		bool collY = false;
		Block* collidedBlockX = nullptr;
		Block* collidedBlockY = nullptr;

		// KHI: Move X
		sf::Vector2f tryX = currentPos + sf::Vector2f(dir.x * curSpeed * dt, 0.f);
		sprite.setPosition(tryX);

		sf::Vector2f hbOffset = playerHitBoxOffset2;
		if (sprite.getScale().x < 0.f) hbOffset.x = -hbOffset.x;

		hitBox.UpdateNoScale(sprite.getPosition(), playerHitBoxSize, hbOffset, Origins::BC);
		//hitBox.UpdateCustomTransform(sprite, playerHitBoxSize, playerHitBoxOffset, Origins::BC);
		//size_t collidedX = GetCollidedTileInfo(collidedBounds);
		size_t collidedX = GetCollidedObstacleInfo(collidedBounds);

		if (collidedX == 0)
		{
			tempPos.x = tryX.x;
		}
		else if (collidedX == 1)
		{
			collX = true;
			collidedBlockX = GetCollidedBlock();

			if (collidedBlockX != nullptr && !collidedBlockX->IsHidable())
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

		}

		// KHI: Move Y
		sf::Vector2f tryY = tempPos + sf::Vector2f(0.f, dir.y * curSpeed * dt);
		sprite.setPosition(sf::Vector2f(tempPos.x, tryY.y));
		hitBox.UpdateNoScale(sprite.getPosition(), playerHitBoxSize, playerHitBoxOffset2, Origins::BC);

		size_t collidedY = GetCollidedObstacleInfo(collidedBounds);

		if (collidedY == 0 && !slidePlayer)
		{
			tempPos.y = tryY.y;
		}
		else if (collidedY == 1)
		{
			collY = true;
			collidedBlockY = GetCollidedBlock();

			if (collidedBlockY != nullptr && !collidedBlockY->IsHidable())
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
		}
		slidePlayer = false;

		bool hasInput = (InputMgr::GetAxisRaw(vAxis) != 0 || InputMgr::GetAxisRaw(hAxis) != 0);
		bool collided = collX || collY;
		Block* targetBlock = (collidedBlockX != nullptr) ? collidedBlockX : collidedBlockY;

		if (collided && hasInput)
		{
			pushedCount += dt;

			if (pushedCount >= pushCount)
			{
				if (targetBlock && targetBlock->IsMovable() && !targetBlock->GetIsMoving())
				{
					targetBlock->PushBlock(dir);
				}
				pushedCount = 0.f;
			}
		}
		else
		{
			pushedCount = 0.f;
		}

		SetPosition(tempPos);
		float tempSpeed = GetSpeed();
		if (animState == AnimState::Live)
		{
			if (charId == CharacterID::BAZZI) {
				SetScale({ dir.x < 0 ? -1.f : dir.x > 0 ? 1.f : sprite.getScale().x, 1.f });
			}
			else {
				const float scaleX = (dir.x < 0 ? -1.f : dir.x > 0 ? 1.f : sprite.getScale().x < 0 ? -1.f : 1.f) * (1.2f);
				const float scaleY = 1.2f;
				SetScale({ scaleX, scaleY });
			}
		}

		if (animState == AnimState::Trapped)
		{
			curSpeed = 20.f;
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
		if (block)
		{
			sf::FloatRect blockBounds = block->GetHitBox().GetGlobalBounds();
			if (hitBox.rect.getGlobalBounds().intersects(blockBounds))
			{
				if (block && block->GetActive() && !block->IsHidable())
				{
					outTileBounds = blockBounds;
					cnt++;
				}
			}
		}
	}
	return cnt;
}

Block* Player::GetCollidedBlock()
{
	Scene* curScene = SCENE_MGR.GetCurrentScene();
	auto gameObjects = curScene->FindGameObjects("Block");

	sf::FloatRect playerBounds = hitBox.rect.getGlobalBounds();

	for (auto* obj : gameObjects)
	{
		Block* block = dynamic_cast<Block*>(obj);
		if (!block || !block->GetActive() || block->IsHidable())
		{
			continue;
		}

		if (playerBounds.intersects(block->GetHitBox().GetGlobalBounds()))
		{
			return block;
		}
	}

	return nullptr;
}

void Player::CheckHiddenState()
{
	Scene* scene = SCENE_MGR.GetCurrentScene();
	auto blocks = scene->FindGameObjects("Block");

	bool isHidden = false;
	sf::FloatRect playerBounds = hitBox.rect.getGlobalBounds();
	sf::Vector2f playerCenter = {
		playerBounds.left + playerBounds.width * 0.5f,
		playerBounds.top + playerBounds.height * 0.5f
	};

	for (auto* obj : blocks)
	{
		Block* block = dynamic_cast<Block*>(obj);
		if (!block || !block->GetActive() || !block->IsHidable())
			continue;

		if (block->GetHitBox().GetGlobalBounds().contains(playerCenter))
		{
			isHidden = true;
			break;
		}
	}

	if (isHidden)
	{
		SetSpriteColor(sf::Color(255, 255, 255, 0));
	}
	else
	{
		SetSpriteColor(sf::Color(255, 255, 255, 255));
	}
}