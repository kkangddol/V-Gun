#pragma once

namespace RocketCore::Graphics
{
	////������ ����������, ���۵� ���������� ���ϱ�.
	//enum class eRenderType
	//{
	//	FORWARD,
	//	DEFERRED
	//};

	///[TW] �Ŀ� eRenderType�� �̷��� �ٲ��� �մϴ�.
	//������ ����������, ���۵� ���������� ���ϱ�.
	enum class eRenderType
	{
		FORWARD,
		DEFERRED_PHONG,
		DEFERRED_PBR,
		DEFERRED_PBR_SKINNED
	};
}