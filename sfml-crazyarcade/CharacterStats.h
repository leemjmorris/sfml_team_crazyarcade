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
	// LSY: default speed (100.f) 
	{CharacterID::DAO,   {1,10,7,1,100.f,700.f}},
	{CharacterID::BAZZI, {1,6,1,7,100.f,900.f}},
};