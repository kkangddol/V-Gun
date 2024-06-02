#pragma once
#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace RocketCore::Graphics
{
	class ISketchable abstract
	{
	public:
		virtual void SetData(const char* data) abstract;
		virtual void SetAlpha(float alpha) abstract;
		virtual void SetFillRatio(float ratio) abstract;
		virtual void Render(const RocketEngine::RMFLOAT3X3& rectTM) abstract;
		//path,text
	};
}
