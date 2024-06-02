#include "RocketCoreAPI.h"
#include "SoundSystem.h"

namespace RocketCore
{
	std::unordered_map<std::string, RocketAudio>& GetSoundMapFromSoundSystem()
	{
		return SoundSystem::Instance().GetSoundMap();
	}

}