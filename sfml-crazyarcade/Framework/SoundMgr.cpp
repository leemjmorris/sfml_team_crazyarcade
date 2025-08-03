#include "stdafx.h"
#include "SoundMgr.h"

void SoundMgr::Init(int totalChannels)
{
	for (int i = 0; i < totalChannels; ++i)
	{
		sf::Sound* sound = new sf::Sound();
		sound->setVolume(sfxVolume);

		// LMJ: Enhanced settings for speaker compatibility
		sound->setPosition(0.f, 0.f, 0.f);     // Center position
		sound->setRelativeToListener(true);     // Relative to listener
		sound->setMinDistance(1.f);             // Minimum distance
		sound->setAttenuation(0.f);             // No distance attenuation

		waiting.push_back(sound);
	}

	// LMJ: Set BGM settings for speaker compatibility
	bgm.setVolume(bgmVolume);
	bgm.setPosition(0.f, 0.f, 0.f);
	bgm.setRelativeToListener(true);
	bgm.setMinDistance(1.f);
	bgm.setAttenuation(0.f);
}

void SoundMgr::Release()
{
	for (auto sound : waiting)
	{
		delete sound;
	}
	waiting.clear();
	for (auto sound : playing)
	{
		delete sound;
	}
	playing.clear();
}

void SoundMgr::Update(float dt)
{
	auto it = playing.begin();
	while (it != playing.end())
	{
		if ((*it)->getStatus() == sf::Sound::Stopped)
		{
			waiting.push_back(*it);
			it = playing.erase(it); // LMJ: Use return value of erase()
		}
		else
		{
			++it;
		}
	}
}

void SoundMgr::PlayBgm(std::string id, bool loop)
{
	PlayBgm(SOUNDBUFFER_MGR.Get(id), loop);
}

void SoundMgr::PlayBgm(sf::SoundBuffer& buffer, bool loop)
{
	bgm.stop();
	bgm.setLoop(loop);
	bgm.setBuffer(buffer);

	// LMJ: Enhanced BGM settings for speaker compatibility
	bgm.setVolume(bgmVolume);
	bgm.setPosition(0.f, 0.f, 0.f);
	bgm.setRelativeToListener(true);
	bgm.setMinDistance(1.f);
	bgm.setAttenuation(0.f);

	bgm.play();
}

void SoundMgr::StopBgm()
{
	bgm.stop();
}

void SoundMgr::PlaySfx(std::string id, bool loop)
{
	PlaySfx(SOUNDBUFFER_MGR.Get(id), loop);
}

void SoundMgr::PlaySfx(sf::SoundBuffer& buffer, bool loop)
{
	sf::Sound* sound = nullptr;

	if (waiting.empty())
	{
		sound = playing.front();
		playing.pop_front();
		sound->stop();
	}
	else
	{
		sound = waiting.front();
		waiting.pop_front();
	}

	sound->setBuffer(buffer);
	sound->setLoop(loop);

	// LMJ: Enhanced SFX settings for speaker compatibility
	sound->setVolume(sfxVolume);
	sound->setPitch(1.f);                   // Normal pitch
	sound->setPosition(0.f, 0.f, 0.f);     // Center position
	sound->setRelativeToListener(true);     // Relative to listener
	sound->setMinDistance(1.f);             // Minimum distance
	sound->setAttenuation(0.f);             // No distance attenuation

	sound->play();
	playing.push_back(sound);
}

void SoundMgr::SetSfxVolume(float v)
{
	sfxVolume = v;
	for (auto sound : playing)
	{
		sound->setVolume(sfxVolume);
	}

	// LMJ: Also update waiting sounds
	for (auto sound : waiting)
	{
		sound->setVolume(sfxVolume);
	}
}

void SoundMgr::StopAllSfx()
{
	for (auto sound : playing)
	{
		sound->stop();
		waiting.push_back(sound);
	}
	playing.clear();
}