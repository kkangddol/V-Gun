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
		virtual void SetFillRatio(float ratio) final; //Pivot�� ����Ʈ�� Left, FillMode�� 100�� �⺻.
		virtual void Render(const RocketEngine::RMFLOAT3X3& rectTM) override final;

	private:
		void AssignTextureToSelf();

		float _width = 0.0f;
		float _height = 0.0f;
		float _alpha = 1.0f;
		float _sortingLayer = 0.0f; //�ϴ��� ����Ʈ�� �����ϴµ�, ���߿� Text�� Layer�� �߿������ٸ�, �̰� ���� ���̴�!

		std::string _texturePath;
		Texture* _textureData = nullptr;

		//��� Sketchable �����̴�. �갡 �̰� ���ҽ� ���� �ǵ帮�� �ȵȴ�!
		DirectX::SpriteBatch* _spriteBatch = nullptr;

		float _fillRatio = 100.0f;
	};
}


