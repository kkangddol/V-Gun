#pragma once
#include <d3d11.h>
#include "TextureRenderTarget.h"

namespace RocketCore::Graphics
{
	class Shader;

	class ScreenBuffer
	{
	public:
		enum { DEFERRED_PHONG_BUFFER_COUNT = 4,
			   DEFERRED_PBR_BUFFER_COUNT = 5 };
	public:
		ScreenBuffer(Shader* belongShader);

		Shader* _belongShader = nullptr;

		unsigned int _indexCount = 0;
		ID3D11Buffer* _vertexBuffer = nullptr;
		ID3D11Buffer* _indexBuffer = nullptr;

		//Phong, PBR �� �����޴� �ֵ��� ��׵��� �� �� ���̴�. ����Ʈ ����!
		TextureRenderTarget _defTextureRenderTarget;
	};
}


