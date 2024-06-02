#include "ScreenBufferPhong.h"
#include "Shader.h"
#include <d3d11.h>
#include "GeometryGenerator.h"
#include "LowLevelDX11.h"

namespace RocketCore::Graphics
{
	//
	ScreenBufferPhong::ScreenBufferPhong(Shader* belongShader) : ScreenBuffer(belongShader)
	{
		
	}

	//Phong일 경우는 이렇게 실행.
	void ScreenBufferPhong::AssignRequiredDeferredPhongVariables(const std::string& diffuseGVal, const std::string& normalGVal, const std::string& positionGVal, const std::string& depthGVal)
	{
		_diffuseGBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(diffuseGVal.c_str())->AsShaderResource();
		assert(_diffuseGBufferEffectVar->IsValid());

		_normalGBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(normalGVal.c_str())->AsShaderResource();
		assert(_normalGBufferEffectVar->IsValid());

		_positionGBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(positionGVal.c_str())->AsShaderResource();
		assert(_positionGBufferEffectVar->IsValid());

		_depthGBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(depthGVal.c_str())->AsShaderResource();
		assert(_depthGBufferEffectVar->IsValid());
	}

	void ScreenBufferPhong::Initialize(ID3DX11EffectTechnique* quadTech)
	{
		this->_quadTech = quadTech;

		const D3D11_INPUT_ELEMENT_DESC tPosNormalTex[3] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		D3DX11_PASS_DESC passDesc;
		quadTech->GetPassByIndex(0)->GetDesc(&passDesc);
		HR(LowLevelDX11::GetInstance()->m_d3dDevice.Get()->CreateInputLayout(tPosNormalTex, 3, passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize, &_quadLayout));
	}

	void ScreenBufferPhong::UseQuadLayout()
	{
		LowLevelDX11::GetInstance()->m_d3dMainContext->IASetInputLayout(_quadLayout);
	}

	

}
