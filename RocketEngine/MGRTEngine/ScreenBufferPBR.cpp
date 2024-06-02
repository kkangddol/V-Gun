#include "ScreenBufferPBR.h"
#include "Shader.h"
#include <d3d11.h>
#include "GeometryGenerator.h"
#include "LowLevelDX11.h"

namespace RocketCore::Graphics
{
	ScreenBufferPBR::ScreenBufferPBR(Shader* belongShader) : ScreenBuffer(belongShader)
	{
		//
	}

	void ScreenBufferPBR::Initialize(ID3DX11EffectTechnique* quadTech)
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

	void ScreenBufferPBR::AssignRequiredDeferredPBRVariables(const std::string& albedoGVal, const std::string& normalGVal, const std::string& positionGVal, const std::string& armGVal, const std::string& depthGVal)
	{
		_albedoGBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(albedoGVal.c_str())->AsShaderResource();
		assert(_albedoGBufferEffectVar->IsValid());

		_normalGBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(normalGVal.c_str())->AsShaderResource();
		assert(_normalGBufferEffectVar->IsValid());

		_positionGBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(positionGVal.c_str())->AsShaderResource();
		assert(_positionGBufferEffectVar->IsValid());

		_armGBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(armGVal.c_str())->AsShaderResource();
		assert(_armGBufferEffectVar->IsValid());

		_depthGBufferEffectVar = this->_belongShader->GetEffect()->GetVariableByName(depthGVal.c_str())->AsShaderResource();
		assert(_depthGBufferEffectVar->IsValid());
	}

	void ScreenBufferPBR::UseQuadLayout()
	{
		LowLevelDX11::GetInstance()->m_d3dMainContext->IASetInputLayout(_quadLayout);
	}

}