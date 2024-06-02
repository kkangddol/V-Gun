#pragma once
#include <iostream>
#include <string>

namespace RocketCore::Graphics
{
	struct AnimData
	{
		std::string name;
		bool isLoop = false;
		float duration = 5.0f;			//230822 ���� ������
	};
}