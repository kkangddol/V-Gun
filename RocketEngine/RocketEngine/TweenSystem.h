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
		TweenSystem();		// 싱글턴이기 때문에 외부에서 생성할 수 없도록.

	public:
		void Initialize();
		void Finalize();

	//private:
		// 사용하고 있는 닷트윈 객체의 벡터
		std::vector<RocketEngine::Tween*> _dotweens;
		// 사용이 끝나서 지우기 위한 닷트윈 객체의 벡터
		std::vector<RocketEngine::Tween*> _deleteObj;

	public:
		RocketEngine::Tween& CreateTween();
		void Update();
	};
}
