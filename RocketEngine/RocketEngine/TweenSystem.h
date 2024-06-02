#pragma once
#include "Singleton.h"
#include <vector>

namespace RocketEngine
{
	class Tween;
}

namespace RocketCore
{
	class TweenSystem : public Singleton<TweenSystem>
	{
		friend Singleton;
	private:
		TweenSystem();		// �̱����̱� ������ �ܺο��� ������ �� ������.

	public:
		void Initialize();
		void Finalize();

	//private:
		// ����ϰ� �ִ� ��Ʈ�� ��ü�� ����
		std::vector<RocketEngine::Tween*> _dotweens;
		// ����� ������ ����� ���� ��Ʈ�� ��ü�� ����
		std::vector<RocketEngine::Tween*> _deleteObj;

	public:
		RocketEngine::Tween& CreateTween();
		void Update();
	};
}
