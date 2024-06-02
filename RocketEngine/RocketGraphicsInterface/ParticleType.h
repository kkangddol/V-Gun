#pragma once

namespace RocketCore::Graphics
{
	enum class eParticleType
	{
		FLASH, // Muzzle Flash : 총 쏘면 총구 근처가 반짝이는 것.
		HIT	   // 총을 맞은 부위에 사각형 파티클 몇개 확산.
	};
}
