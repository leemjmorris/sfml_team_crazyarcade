#pragma once
#include <unordered_map>


enum class CharacterID
{
	BAZZI, DAO, CAPPI, MARID, NONE
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
	{CharacterID::DAO,   {1,10,1,7,150.f,500.f}},
	{CharacterID::BAZZI, {1,6,1,7,190.f,700.f}},
	{CharacterID::CAPPI, {1,6,2,7,170.f,600.f}},
	{CharacterID::MARID, {2,6,1,7,170.f,600.f}}
};