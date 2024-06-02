#pragma once
//갑자기 디퍼드를 추가하면서, 구조가 무너졌다.. 디퍼드 한정..
//Phong으로 디퍼드 렌더링 현재 담당, 곧 PBR 담당 역시 할 예정.

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

	//이런 하부 클래스 : G-Buffer를 제외한 작업을 여기서 하지 말자.
	class ScreenBufferPhong : public ScreenBuffer
	{
	public:
		ScreenBufferPhong(Shader* belongShader);
		void Initialize(ID3DX11EffectTechnique* quadTech);
		void AssignRequiredDeferredPhongVariables(
			const std::string& diffuseGVal, 
			const std::string& normalGVal, const std::string& positionGVal, const std::string& depthGVal);
		void UseQuadLayout(); //
		
		//Phong 한정. 
		ID3DX11EffectShaderResourceVariable* _diffuseGBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _normalGBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _positionGBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _depthGBufferEffectVar = nullptr;

		//원래는 PBR <-> Phong 다른 특징을 가지고 있어 다른 클래스가 되어야 하지만,
		//일괄적으로 보관해기 위해서 이처럼 활용.
		union
		{
			TextureRenderTarget _trtPhongGBuffers[ScreenBuffer::DEFERRED_PHONG_BUFFER_COUNT]; //이제 Depth Buffer 추가. 
			struct
			{
				TextureRenderTarget _phongDiffuseGBuffer;
				TextureRenderTarget _phongNormalGBuffer;
				TextureRenderTarget _phongPositionGBuffer;
				TextureRenderTarget _phongDepthGBuffer;
			};
		};
		
	private:
		ID3D11InputLayout* _quadLayout = nullptr; //LightingPS를 위해 존재한다.
		ID3DX11EffectTechnique* _quadTech = nullptr; //Quad에 렌더하기 위해서 존재.
		//PBR 추가되면 TBA..
	};
}


