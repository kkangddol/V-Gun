#pragma once

#include "../RocketGraphicsInterface/ISketchable.h"

namespace RocketCore::Graphics
{
	class BaseSketchable : public ISketchable
	{
	public:
		virtual void SetData(const char* data) override;
		virtual void SetAlpha(float alpha) override;
		virtual void SetFillRatio(float ratio) override;
		virtual void Render(const RocketEngine::RMFLOAT3X3& rectTM) override;

		//path,text
	};
};


