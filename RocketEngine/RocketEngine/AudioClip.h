#pragma once
#include "DLLExporter.h"
#include "Component.h"
#include "eSoundGroup.h"
#include <unordered_map>
#include <string>

namespace RocketCore
{
	class SoundSystem;
}

namespace RocketEngine
{
	struct AudioStatus
	{
		std::string path;
		bool isPlaying;
	};

	class GameObject;

	/// <summary>
	/// User�� ���� �� ��밡���� �Լ��� ������ ���� class.
	/// 
	/// </summary>
	class ROCKET_API AudioClip final : public Component
	{
	public:
		AudioClip(GameObject* owner);

	protected:
		virtual void Start() override;

		/// �������.
	public:
		// 2D Sound �Լ�.
		void PlayOnce(std::string audioName);		// �ѹ��� ���
		void PlayRepeat(std::string audioName);		// �ݺ� ���

		// 3D Sound �Լ�.
		void Play3DOnce(std::string audioName);		// �ѹ��� ���
		void Play3DRepeat(std::string audioName);	// �ݺ� ���

		// ���� �Լ�.
		void Allplay();								// ��� Sound ���
		void PlayEnd(std::string audioName);		// Sound ����
		void PlayAllEnd();							// Sound All ����

		/// Sound ����
		// ���� Sound
		void PlayMute(std::string audioName);
		void PlayPaused(std::string audioName);
		void SoundVolumeSet(std::string audioName);
		
		bool IsSoundPlaying(std::string audioName);

		// Group ���� �Լ�
		void PlayGroupMute(std::string audioName);			// Sound OnOff
		void PlayGroupAllMute();							// Sound All OnOff
		void PlayGroupPaused(eSoundGroup soundGroupName);	// Sound �Ͻ�����
		void PlayGroupAllPaused();							// Sound All �Ͻ�����
		void SoundGroupVolumeSet(eSoundGroup soundGroupName);	// Sound ���� ����
		void SoundGroupAllVolumeSet();						// Sound All ���� ����

		/// ����� �߰� ����.
	public:
		// Audio ����
		void AddAudio(std::string audioName, std::string audioPath, eSoundGroup group);		// 2D Sound �߰�
		void Add3DAudio(std::string audioName, std::string audioPath, eSoundGroup group,
			float minDistance, float maxDistance);		// 3D Sound �߰� 

		/// ����� ��� �ʿ��� ������.
	public:
		float GetVolume();
		std::unordered_map<std::string, std::string>& GetAudios();

	private:
		RocketCore::SoundSystem& _soundSystem;							// FMOD System

		std::unordered_map<std::string, std::string> _audioMap;			// audio�̸�, audioPath
		float _volume = 1.f;													// Sound ���� ũ��
	};
}

// Play(�����ؼ�) -> ���� ���� ���� �� �־�� ��
// �Ͻ�����
// ����
// ��������
// �������� �׷�����? -> �ý��ۿ� ����Ҷ�