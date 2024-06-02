#pragma once
#include <memory>
#include <string>
#include <SpriteBatch.h>
#include <DirectXMath.h>

#include "BaseSketchable.h"


namespace RocketCore::Graphics
{
	class Texture;

	class ImageSketchable : public BaseSketchable
	{
	public:
		ImageSketchable(DirectX::SpriteBatch* sBatch, float width, float height, const char* _texturePath);
		~ImageSketchable();

		// == Data Path.
		virtual void SetData(const char* data) final;
		virtual void SetAlpha(float alpha) final;
		virtual void SetFillRatio(float ratio) final; //Pivot은 디폴트가 Left, FillMode는 100이 기본.
		virtual void Render(const RocketEngine::RMFLOAT3X3& rectTM) override final;

	private:
		void AssignTextureToSelf();

		float _width = 0.0f;
		float _height = 0.0f;
		float _alpha = 1.0f;
		float _sortingLayer = 0.0f; //일단은 디폴트로 설정하는데, 나중에 Text의 Layer가 중요해진다면, 이가 쓰일 것이다!

		std::string _texturePath;
		Texture* _textureData = nullptr;

		//모든 Sketchable 공용이다. 얘가 이거 리소스 관리 건드리면 안된다!
		DirectX::SpriteBatch* _spriteBatch = nullptr;

		float _fillRatio = 100.0f;
	};
}


