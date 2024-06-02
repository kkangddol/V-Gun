#pragma once

namespace RocketCore::Graphics
{
	struct AnimDesc
	{
		std::string name;
		std::string animPath;
		std::string ownerModelPath; 
		std::string objName; //[TW] 실제 게임 엔진에서의 게임오브젝트 이름.
	};
}