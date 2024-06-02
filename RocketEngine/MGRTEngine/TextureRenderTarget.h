#pragma once
#include <d3d11.h>

namespace RocketCore::Graphics
{
	struct TextureRenderTarget
	{
		ID3D11Texture2D* _texture = nullptr;
		ID3D11RenderTargetView* _renderTargetView = nullptr;
		ID3D11ShaderResourceView* _shaderResourceView = nullptr;
		ID3D11DepthStencilView* _depthStencilView = nullptr; //이 파라미터는 Forward 이외에 쓰이지 않음.
	};
}
