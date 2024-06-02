#pragma once
//Precombined ScreenQuad를 담당할 예정.

#include "TextureRenderTarget.h"
#include <string>
#include "ScreenBuffer.h"

struct ID3DX11EffectVariable;
struct ID3DX11EffectScalarVariable;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectShaderResourceVariable;
struct ID3DX11EffectDepthStencilViewVariable;
struct ID3DX11EffectRenderTargetViewVariable;

struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace RocketCore::Graphics
{
	class Shader;

	class ScreenBufferPrecombined : public ScreenBuffer
	{
	public:
		ID3DX11EffectScalarVariable* _gNearClipEffectVar = nullptr;
		ID3DX11EffectScalarVariable* _gFarClipEffectVar = nullptr;

		ID3DX11EffectShaderResourceVariable* _gForward_QuadBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _gForward_DepthBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _gDeferredPhong_QuadBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _gDeferredPhong_DepthBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _gDeferredPBR_QuadBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _gDeferredPBR_DepthBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _gDeferredPBR_Skinned_QuadBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _gDeferredPBR_Skinned_DepthBufferEffectVar = nullptr;

		void AssignRequiredPrecombinedBufferVariables(const std::string& nearClip, const std::string& farClip, 
													  const std::string& forwardQuadBuf, const std::string& forwardDepthBuf,
													  const std::string& deferredPhongQuadBuf, const std::string& deferredPhongDepthBuf,
													  const std::string& deferredPBRQuadBuf, const std::string& deferredPBRDepthBuf,
													  const std::string& deferredPBRSkinnedQuadBuf, const std::string& deferredPBRSkinnedDepthBuf);

	};
}

