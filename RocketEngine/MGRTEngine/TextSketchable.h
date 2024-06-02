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
	//D3D Init 다 끝난 후에 호출되어야.
	class TextSketchable : public BaseSketchable
	{
	public:
		enum {MAXIMUM_TEXTBUFFER_COUNT = 1024};

		TextSketchable(DirectX::SpriteBatch* sBatch, DirectX::SpriteFont* sfont, float width, float height, DirectX::XMFLOAT4 txtCol);
		~TextSketchable();

		virtual void SetData(const char* data) final;
		virtual void SetAlpha(float alpha) final;
		virtual void Render(const RocketEngine::RMFLOAT3X3& rectTM) final override;
		virtual void SetFillRatio(float ratio) final; //깡통.

	private:
		//void RecordToWCHAR(TCHAR* text, ...);

		float _sortingLayer = 0.0f; //일단은 디폴트로 설정하는데, 나중에 Text의 Layer가 중요해진다면, 이가 쓰일 것이다!

		float _width = 0.0f;
		float _height = 0.0f;
		//모든 Sketchable 공용이다. 얘가 이거 리소스 관리 건드리면 안된다!
		DirectX::SpriteBatch* _spriteBatch = nullptr;
		DirectX::SpriteFont* _spriteFont = nullptr;
		TCHAR _wBuffer[MAXIMUM_TEXTBUFFER_COUNT];
		DirectX::XMFLOAT4 _textColor;

	};
}


