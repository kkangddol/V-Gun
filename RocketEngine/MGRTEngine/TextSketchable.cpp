#include "TextSketchable.h"
#include "LowLevelDX11.h"
#include "MathHelper.h"
#include <cmath>
#include <atlconv.h>
#include <windows.h>
#include <stringapiset.h>
#include <algorithm>

namespace RocketCore::Graphics
{
	TextSketchable::TextSketchable(DirectX::SpriteBatch* sBatch, DirectX::SpriteFont* sfont, float width, float height, DirectX::XMFLOAT4 txtCol)
	{
		this->_spriteBatch = sBatch;
		this->_spriteFont = sfont;
		this->_textColor = txtCol;
		this->_width = width;
		this->_height = height;
	}

	TextSketchable::~TextSketchable()
	{
		//
	}

	void TextSketchable::SetData(const char* data)
	{
		for (int i = 0; i < MAXIMUM_TEXTBUFFER_COUNT; i++)
		{
			_wBuffer[i] = '\0';
		}
		
		std::string tStr = data;
		int _tStringSize = min(tStr.size(), MAXIMUM_TEXTBUFFER_COUNT);
		MultiByteToWideChar(CP_ACP, 0, data, strlen(data) + 1, _wBuffer, _tStringSize);
		assert(true);
	}

	void TextSketchable::SetAlpha(float alpha)
	{
		this->_textColor.w = alpha;
	}

	void TextSketchable::Render(const RocketEngine::RMFLOAT3X3& rectTM)
	{
		RocketEngine::RMFLOAT2 tTrans;
		float ttRotation;
		RocketEngine::RMFLOAT2 tScale;

		MathHelper::RMMatrix3x3Decompose(rectTM, tTrans, ttRotation, tScale);

		DirectX::XMFLOAT2 ttTrans = MathHelper::RMToXMFloat2(tTrans);
		DirectX::XMFLOAT2 ttScale = MathHelper::RMToXMFloat2(tScale);
		float ttScaleAverage = (tScale.x + tScale.y) / 2.0f;

		DirectX::XMFLOAT2 ttOrigin = DirectX::XMFLOAT2(_width / 3.0f, _height / 6.0f);

		DirectX::XMVECTOR ttColor = DirectX::XMLoadFloat4(&_textColor);

		_spriteFont->DrawString(_spriteBatch, _wBuffer, ttTrans, ttColor, ttRotation, ttOrigin,
			ttScaleAverage, DirectX::SpriteEffects_None, _sortingLayer);
	}

	void TextSketchable::SetFillRatio(float ratio)
	{
		//
	}

}