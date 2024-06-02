#pragma once
#pragma once
#include "Singleton.h"
#include <vector>

namespace RocketCore
{
	class UIRenderer;
}

namespace RocketCore
{
	/// <summary>
	/// ���� ��Ŀ�� �� ��ü ������ ���
	/// 
	/// 23.08.11 ������ �����.
	/// </summary>
	class UISystem : public Singleton<UISystem>
	{
		friend Singleton;
	private:
		UISystem();		// �̱����̱� ������ �ܺο��� ������ �� ������.

	public:
		void Initialize();
		void Finalize();

	public:
		void CheckFocusCurrentScene();

	public:
		void AddUICurrentScene(UIRenderer* ui);
	};
}
