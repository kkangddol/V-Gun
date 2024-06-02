#pragma once
#include "..\\RocketMath\\RocketMath.h"
#include "MaterialData.h"
#include "AnimData.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace RocketCore::Graphics
{
	class IRenderable abstract
	{
	public:
		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM) abstract;
 		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM, const LegacyMaterialData& m) abstract; 
 		virtual void Render(const RocketEngine::RMFLOAT4X4& worldTM, const PBRMaterialData& m) abstract;
		virtual void PlayAnim(AnimData animData) abstract;
	};
}
