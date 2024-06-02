#pragma once
//���ڱ� ���۵带 �߰��ϸ鼭, ������ ��������.. ���۵� ����..
//Phong���� ���۵� ������ ���� ���, �� PBR ��� ���� �� ����.

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

	//�̷� �Ϻ� Ŭ���� : G-Buffer�� ������ �۾��� ���⼭ ���� ����.
	class ScreenBufferPhong : public ScreenBuffer
	{
	public:
		ScreenBufferPhong(Shader* belongShader);
		void Initialize(ID3DX11EffectTechnique* quadTech);
		void AssignRequiredDeferredPhongVariables(
			const std::string& diffuseGVal, 
			const std::string& normalGVal, const std::string& positionGVal, const std::string& depthGVal);
		void UseQuadLayout(); //
		
		//Phong ����. 
		ID3DX11EffectShaderResourceVariable* _diffuseGBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _normalGBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _positionGBufferEffectVar = nullptr;
		ID3DX11EffectShaderResourceVariable* _depthGBufferEffectVar = nullptr;

		//������ PBR <-> Phong �ٸ� Ư¡�� ������ �־� �ٸ� Ŭ������ �Ǿ�� ������,
		//�ϰ������� �����ر� ���ؼ� ��ó�� Ȱ��.
		union
		{
			TextureRenderTarget _trtPhongGBuffers[ScreenBuffer::DEFERRED_PHONG_BUFFER_COUNT]; //���� Depth Buffer �߰�. 
			struct
			{
				TextureRenderTarget _phongDiffuseGBuffer;
				TextureRenderTarget _phongNormalGBuffer;
				TextureRenderTarget _phongPositionGBuffer;
				TextureRenderTarget _phongDepthGBuffer;
			};
		};
		
	private:
		ID3D11InputLayout* _quadLayout = nullptr; //LightingPS�� ���� �����Ѵ�.
		ID3DX11EffectTechnique* _quadTech = nullptr; //Quad�� �����ϱ� ���ؼ� ����.
		//PBR �߰��Ǹ� TBA..
	};
}


