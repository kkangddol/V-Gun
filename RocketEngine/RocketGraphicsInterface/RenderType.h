#pragma once

namespace RocketCore::Graphics
{
	////포워드 렌더링인지, 디퍼드 렌더링인지 정하기.
	//enum class eRenderType
	//{
	//	FORWARD,
	//	DEFERRED
	//};

	///[TW] 후에 eRenderType은 이렇게 바뀌어야 합니다.
	//포워드 렌더링인지, 디퍼드 렌더링인지 정하기.
	enum class eRenderType
	{
		FORWARD,
		DEFERRED_PHONG,
		DEFERRED_PBR,
		DEFERRED_PBR_SKINNED
	};
}