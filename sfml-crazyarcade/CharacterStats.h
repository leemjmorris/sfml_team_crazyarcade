#pragma once
#include <unordered_map>


enum class CharacterID
{
	BAZZI, DAO
};


struct PlayerStats
{
	int initBombCount;
	int maxBombCount;
	int initbombLength;
	int maxbombLength;
	float intiPlayerSpeed;
	float maxPlayerSpeed;
};

const std::unordered_map<CharacterID, PlayerStats> CharacterTable =
{
	// LSY: Initial Water Bomb / Maximum Water Bomb / Initial Water Flow / Maximum Water Flow / Initial Speed / Maximum Speed
	{CharacterID::DAO,   {1,10,7,1,5.f,7.f}},
	{CharacterID::BAZZI, {1,6,1,7,5.f,9.f}},
};


