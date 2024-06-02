#pragma once
#include "Singleton.h"

namespace RocketCore
{
	class EventSystem : public Singleton<EventSystem>
	{
		friend Singleton;
	private:
		EventSystem() = default;		// �̱����̱� ������ �ܺο��� ������ �� ������.
	};
}
