#pragma once
#include <memory>
#include <string>
#include <SpriteFont.h>
#include <DirectXMath.h>
#include <tchar.h>
#include <array>
#include "../RocketGraphicsInterface/ISketchable.h"
#include "BaseSketchable.h"

namespace RocketCore::Graphics
{
	//D3D Init �� ���� �Ŀ� ȣ��Ǿ��.
	class TextSketchable : public BaseSketchable
	{
	public:
		enum {MAXIMUM_TEXTBUFFER_COUNT = 1024};

		TextSketchable(DirectX::SpriteBatch* sBatch, DirectX::SpriteFont* sfont, float width, float height, DirectX::XMFLOAT4 txtCol);
		~TextSketchable();

		virtual void SetData(const char* data) final;
		virtual void SetAlpha(float alpha) final;
		virtual void Render(const RocketEngine::RMFLOAT3X3& rectTM) final override;
		virtual void SetFillRatio(float ratio) final; //����.

	private:
		//void RecordToWCHAR(TCHAR* text, ...);

		float _sortingLayer = 0.0f; //�ϴ��� ����Ʈ�� �����ϴµ�, ���߿� Text�� Layer�� �߿������ٸ�, �̰� ���� ���̴�!

		float _width = 0.0f;
		float _height = 0.0f;
		//��� Sketchable �����̴�. �갡 �̰� ���ҽ� ���� �ǵ帮�� �ȵȴ�!
		DirectX::SpriteBatch* _spriteBatch = nullptr;
		DirectX::SpriteFont* _spriteFont = nullptr;
		TCHAR _wBuffer[MAXIMUM_TEXTBUFFER_COUNT];
		DirectX::XMFLOAT4 _textColor;

	};
}


