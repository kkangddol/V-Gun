#pragma once
//PBR Deferred!

#include "TextureRenderTarget.h"
#include <string>
#include "ScreenBuffer.h"

struct ID3DX11EffectVariable;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectShaderResourceVariable;

struct ID3D11InputLayout;
struct ID3D11DeviceContext;

namespace RocketCore::Graphics
{
	class Shader;

	class ScreenBufferSkinnedPBR : public ScreenBuffer
	{
	public:
		ScreenBufferSkinnedPBR(Shader* belongShader);
		void Initialize(ID3DX11EffectTechnique* quadTech);
		void AssignRequiredDeferredPBRVariables(
			const std::string& albedoGVal,
			const std::string& normalGVal, const std::string& positionGVal,
			const std::string& armGVal, const std::string& depthGVal);
		void UseQuadLayout(); //

		//Deferred PBR 한정. 
		ID3DX11EffectShaderResourceVariable* _albedoGBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _normalGBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _positionGBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _armGBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _depthGBufferEffectVar = nullptr;

		union
		{
			TextureRenderTarget _trtPbrGBuffers[ScreenBuffer::DEFERRED_PBR_BUFFER_COUNT]; //이제 Depth Buffer 추가. 
			struct
			{
				TextureRenderTarget _pbrAlbedoGBuffer;
				TextureRenderTarget _pbrNormalGBuffer;
				TextureRenderTarget _pbrPositionGBuffer;
				TextureRenderTarget _pbrArmGBuffer;
				TextureRenderTarget _pbrDepthGBuffer;
			};
		};


	private:
		ID3D11InputLayout* _quadLayout = nullptr; //LightingPS를 위해 존재한다.
		ID3DX11EffectTechnique* _quadTech = nullptr; //Quad에 렌더하기 위해서 존재.
	};
}


