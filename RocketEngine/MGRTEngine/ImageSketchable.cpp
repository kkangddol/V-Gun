#include "ImageSketchable.h"
#include "Define3D.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "MathHelper.h"
#include <algorithm>
#include <limits>

namespace RocketCore::Graphics
{
	ImageSketchable::ImageSketchable(DirectX::SpriteBatch* sBatch, float width, float height, const char* _texturePath)
	{
		this->_spriteBatch = sBatch;
		this->_width = width;
		this->_height = height;
		this->_texturePath = _texturePath;

		this->_fillRatio = 100.0f;

		//Texture Path ���� ResourceManager ã��, ������ �����!
		AssignTextureToSelf();
	}

	ImageSketchable::~ImageSketchable()
	{
		//
	}

	void ImageSketchable::SetData(const char* data)
	{
		//�ߺ��ؼ� ����� �ȵȴ�..! 2D Texture�� ResourceManager�� ���ؼ� ã�ƾ� �� ���̴�!
		std::string tNewString = data;

		
		if (this->_texturePath.compare(tNewString) != 0)
		{
			//���� �ٸ��ٸ�, Texture ��ü!
			this->_texturePath = tNewString;
			this->_textureData = ResourceManager::GetInstance().FindTextureByFilePath(tNewString);
		
			if (_textureData == nullptr)
			{
				_textureData = ResourceManager::GetInstance().MakeTextureFromFilePath(_texturePath);
			}
		}
		assert(this->_textureData != nullptr);
	}

	void ImageSketchable::SetAlpha(float alpha)
	{
		this->_alpha = alpha;
	}

	void ImageSketchable::Render(const RocketEngine::RMFLOAT3X3& rectTM)
	{
		RocketEngine::RMFLOAT2 tTrans;
		float ttRotation;
		RocketEngine::RMFLOAT2 tScale;

		MathHelper::RMMatrix3x3Decompose(rectTM, tTrans, ttRotation, tScale);

		DirectX::XMFLOAT2 ttTrans = MathHelper::RMToXMFloat2(tTrans);
		DirectX::XMFLOAT2 ttScale = MathHelper::RMToXMFloat2(tScale);
		float ttScaleAverage = (tScale.x + tScale.y) / 2.0f;

		DirectX::XMFLOAT2 ttOrigin = DirectX::XMFLOAT2(_width / 2.0f, _height / 2.0f);
		//DirectX::XMFLOAT2 ttOrigin = DirectX::XMFLOAT2(0.0f,0.0f);

		//������ �׷��ִ� ����.
		DirectX::XMFLOAT4 ttFF = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, _alpha);
		//DirectX::XMFLOAT4 ttFF = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, _alpha);
		DirectX::XMVECTOR ttTintColor = DirectX::XMLoadFloat4(&ttFF);

		//SourceRect�� ������Ʈ �״�� �����Ѵ�����, ����� �� ���� ������?
		float tCorrectedRatio = std::clamp(_fillRatio, std::numeric_limits<float>::epsilon(), 100.0f); //���� DivideByZero������ ����.
		
		//���⿡ �ִ� ������ Fill ���� �뺯
		RECT tImageRect = { 0,0, static_cast<LONG>(_width * (tCorrectedRatio / 100.0f)), static_cast<LONG>(_height)};

		//�̹��� �򸮴� ���� ��ħ!
		_spriteBatch->Draw(_textureData->GetSRV(), ttTrans, &tImageRect, ttTintColor,
			ttRotation, ttOrigin, ttScaleAverage, DirectX::SpriteEffects::SpriteEffects_None, _sortingLayer);
		//_spriteBatch->Draw(_textureData->GetSRV(), ttTrans, &tImageRect, NULL,
		//	ttRotation, ttOrigin, ttScaleAverage, DirectX::SpriteEffects::SpriteEffects_None, _sortingLayer);
	}

	//Private, Image�� ResourceManager�� �����ϱ� ����!
	void ImageSketchable::AssignTextureToSelf()
	{
		Texture* tTexture = ResourceManager::GetInstance().FindTextureByFilePath(_texturePath);

		if (tTexture == nullptr)
		{
			tTexture = ResourceManager::GetInstance().MakeTextureFromFilePath(_texturePath);
		}
		this->_textureData = tTexture;
	}

	void ImageSketchable::SetFillRatio(float ratio)
	{
		this->_fillRatio = ratio;
	}

}