#pragma once
#include "eSoundGroup.h"
#include <string>

#include "..\\SoundLib\\inc\\fmod.hpp"
#include "..\\SoundLib\\inc\\fmod_common.h"

namespace RocketEngine
{
	class Transform;
}

namespace RocketCore
{
	/// <summary>
	/// Sound ��ü�� �ʿ��� ����.
	/// </summary>
	struct RocketAudio
	{
		std::string path;				// Sound ���
		RocketEngine::Transform* transform;	// Sound ��ǥ
		FMOD::Sound* sound;					// Sound �����̳�
		FMOD::Channel* channel;				// Sound ä���ּ�
		eSoundGroup group;					// Sound �ش� �׷�
		bool isPlaying;						// Sound ��� ��?

		// 23.08.15 ������ �ø������� ���� ���� �߰�

		bool is3DSound;
	};
}