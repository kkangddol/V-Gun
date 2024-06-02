#include "Component.h"
#include <typeinfo>

namespace RocketEngine
{
// 	Component::Component(GameObject* owner)
// 		: gameObject(owner)
// 	{
// 
// 	}

	void Component::SetRestart()
	{
		_isStarted = false;
	}

}
