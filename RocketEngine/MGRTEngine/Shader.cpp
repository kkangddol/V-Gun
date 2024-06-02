#include "Shader.h"
#include "RenderPipeline.h"
#include "Define3D.h"
#include "TypedefGraphicHelper.h"
#include "ByteBuffer.hpp"
#include "LowLevelDX11.h"
#include "RenderAnimation.h"

#include <fstream>
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <functional>

namespace RocketCore::Graphics
{
	//디폴트 셰이더 생성.
	Shader::Shader()
	{
		assert(false); //사용되면 안된다.
		///일어나서, 일단 디폴트 매개변수 세팅 완료 + Material, InputLayout 받아서 세팅할 수 있는 형식을 보자.
	}

	//커스텀 셰이더 설정, Effect/TechVariable들이 세팅되어서 들어온다.
	Shader::Shader(const std::string& _shaderName, const std::string& _fileRoute,
		const std::vector<std::string>& _techMap,
		const std::map<std::pair<std::string, eShaderVarType>, std::pair<int, int>>& _varMap)
	{
		this->m_ShaderName = _shaderName;
		ID3DX11Effect* mFX = CreateEffect(_fileRoute);
		this->m_Effect = mFX; //내부적으로 저장.

		m_FXFilePath = _fileRoute;

		//테크닉의 이름, 각각 ID3DX11TechniqueVariable에 투입!
		for (auto& it : _techMap)
		{
			ID3DX11EffectTechnique* tTech = mFX->GetTechniqueByName(it.c_str());
			assert(tTech->IsValid());
			m_FXTechMap.insert(std::make_pair(it, tTech));
		}

		////이제는, Effect의 어떤 변수를 활용해야 하는지 설정하는 코드.
		for (const auto& [varNameType, Size_Cnt] : _varMap)
		{
			ID3DX11EffectVariable* tVar = mFX->GetVariableByName(varNameType.first.c_str());
			assert(tVar->IsValid());
			m_FXVarSizeMap.insert(std::make_pair(varNameType.first, std::make_tuple(tVar, varNameType.second, Size_Cnt, nullptr)));
		}
	}


	void Shader::SetShaderFileName(std::string _shaderName)
	{
		m_ShaderName = _shaderName;
	}

	std::string Shader::GetShaderFileName()
	{
		return m_ShaderName;
	}

	void Shader::SetMainTechnique(const std::string& _techStr)
	{
		auto it = m_FXTechMap.find(_techStr);
		if (it == m_FXTechMap.end())
		{
			//MGRT_ERROR("No Technique Found : ", _techStr);
			assert(false);
		}
		m_MainTech = it->second;
	}

	ID3DX11EffectTechnique* Shader::GetMainTechnique()
	{
		return m_MainTech;
	}

	//아마 이 함수가 (IASetVB,IB)와 (TechApply, DrawIndexed) 사이에 위치해 있을 듯.
	void Shader::UpdateShader()
	{
		D3DX11_TECHNIQUE_DESC techDesc;
		m_MainTech->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			//IA단계에서 VB,IB Buffer 세팅은 여기서 이루어지는 것이 아니다.
			//여기는 Shader의 Variable만 돌아가면서 값을 넣을 수 있게!

			//Memory가 들어온 voidPtr, 이제 SetRawValue와 내부 정보를 활용해 실행!
			for (const auto& [varString, eTuple] : m_FXVarSizeMap)
			{
				ID3DX11EffectVariable* tEffectVar = std::get<0>(eTuple);
				void* tShaderMem = std::get<3>(eTuple);
				int tSingleSize = std::get<2>(eTuple).first;
				int tElemCnt = std::get<2>(eTuple).second;
				//SetResource가 작동할지는 모르겠으나, 일단 다른 분야에서는 문제 없이 작동하는 것을 확인.
				//일단은 분기 설정! (그러므로, ShaderResourceView는 크기/개수의 영향을 받지 않는다.
				///[TW] SetRawValue가 안된다! 일단은, Matrix 형태로 바꿔보자. 또한, 지금 여러개 힘듬.
				if (std::get<1>(eTuple) == eShaderVarType::_SHADER_RESOURCE)
				{
					ID3DX11EffectShaderResourceVariable* tSRVar = tEffectVar->AsShaderResource();
					BOOL res = tSRVar->IsValid();
					assert(res);
					ID3D11ShaderResourceView* tNewMem = static_cast<ID3D11ShaderResourceView*>(tShaderMem);
					HR(tSRVar->SetResource(tNewMem));
				}
				else if (std::get<1>(eTuple) == eShaderVarType::_MATRIX)
				{
					//임시 코드.
					if (tElemCnt == 1)
					{
						ID3DX11EffectMatrixVariable* tMatVar = tEffectVar->AsMatrix();
						BOOL res = tMatVar->IsValid();
						assert(res);
						XMFLOAT4X4* tNewMem = static_cast<XMFLOAT4X4*>(tShaderMem);
						XMMATRIX tMatrix = DirectX::XMLoadFloat4x4(tNewMem);
						HR(tMatVar->SetMatrix(reinterpret_cast<const float*>(&tMatrix)));
					}
					else
					{
						////[TW] 100으로 일단은 하드코딩.
						ID3DX11EffectMatrixVariable* tMatVar = tEffectVar->AsMatrix();
						BOOL res = tMatVar->IsValid();
						assert(res);
						//HR(tMatVar->SetMatrixArray(reinterpret_cast<const float*>(&tShaderMem),0,tElemCnt
						ByteBuffer* tByteBuffer = (ByteBuffer*)tShaderMem;

						////바이트버퍼 테스팅 위해.
						std::vector<DirectX::XMFLOAT4X4> tTemp;
						for (int i = 0; i < RenderAnimation::MAXIMUM_BONE_COUNT; i++)
						{
							DirectX::XMFLOAT4X4 tFF;
							tByteBuffer->getBytes((uint8_t*)&tFF, sizeof(DirectX::XMFLOAT4X4));
							tTemp.push_back(tFF);
						}
						assert(true);
						HR(tMatVar->SetMatrixArray(reinterpret_cast<const float*>(&tTemp[0]), 0, 100));
					}
				}
				else if (std::get<1>(eTuple) == eShaderVarType::_SCALAR_FLOAT)
				{
					if (tElemCnt == 1)
					{
						ID3DX11EffectScalarVariable* tScalarVar = tEffectVar->AsScalar();
						BOOL res = tScalarVar->IsValid();
						assert(res);
						float* tNewMem = static_cast<float*>(tShaderMem);
						float ttNewMem = *tNewMem;
						HR(tScalarVar->SetFloat(ttNewMem));
					}
					else
					{
						//ID3DX11EffectScalarVariable* tScalarVar = tEffectVar->AsScalar();
						//BOOL res = tScalarVar->IsValid();
						//assert(res);
						//float* tNewMem = static_cast<float*>(tShaderMem); //ShaderMem에는 Float의 개수를 N개 보관한다. (Custom에서 세팅)
						//HR(tScalarVar->SetFloatArray(tNewMem, 0, tElemCnt))

						///ScalarVariable 자체로 인식이 안된다..! 어쩔 수 없이, 풀어서 SetRawValue를 쓰자.
						ID3DX11EffectVariable* tCustomVar = tEffectVar;
						BOOL res = tCustomVar->IsValid();
						assert(res);
						tCustomVar->SetRawValue(tShaderMem, 0, sizeof(float) * tElemCnt);
					}
				}
				else if (std::get<1>(eTuple) == eShaderVarType::_SCALAR_INT)
				{
					if (tElemCnt == 1)
					{
						ID3DX11EffectScalarVariable* tScalarVar = tEffectVar->AsScalar();
						BOOL res = tScalarVar->IsValid();
						assert(res);
						int* tNewMem = static_cast<int*>(tShaderMem);
						int ttNewMem = *tNewMem;
						HR(tScalarVar->SetInt(ttNewMem));
					}
					else
					{
						ID3DX11EffectScalarVariable* tScalarVar = tEffectVar->AsScalar();
						BOOL res = tScalarVar->IsValid();
						assert(res);
						int* tNewMem = static_cast<int*>(tShaderMem); //ShaderMem에는 Float의 개수를 N개 보관한다. (Custom에서 세팅)
						HR(tScalarVar->SetIntArray(tNewMem, 0, tElemCnt));
					}
				}
				else if (std::get<1>(eTuple) == eShaderVarType::_SCALAR_BOOL)
				{
					if (tElemCnt == 1)
					{
						ID3DX11EffectScalarVariable* tScalarVar = tEffectVar->AsScalar();
						BOOL res = tScalarVar->IsValid();
						assert(res);
						bool* tNewMem = static_cast<bool*>(tShaderMem);
						bool ttNewMem = *tNewMem;
						HR(tScalarVar->SetBool(ttNewMem));
					}
					else
					{
						ID3DX11EffectScalarVariable* tScalarVar = tEffectVar->AsScalar();
						BOOL res = tScalarVar->IsValid();
						assert(res);
						bool* tNewMem = static_cast<bool*>(tShaderMem); //ShaderMem에는 Float의 개수를 N개 보관한다. (Custom에서 세팅)
						HR(tScalarVar->SetBoolArray(tNewMem, 0, tElemCnt));
					}
				}
				else if (std::get<1>(eTuple) == eShaderVarType::_RENDER_TARGET_VIEW)
				{
					ID3DX11EffectRenderTargetViewVariable* tSRVar = tEffectVar->AsRenderTargetView();
					BOOL res = tSRVar->IsValid();
					assert(res);
					ID3D11RenderTargetView* tNewMem = static_cast<ID3D11RenderTargetView*>(tShaderMem);
					HR(tSRVar->SetRenderTarget(tNewMem));
				}
				else if (std::get<1>(eTuple) == eShaderVarType::_DEPTH_STENCIL_VIEW)
				{
					ID3DX11EffectDepthStencilViewVariable* tSRVar = tEffectVar->AsDepthStencilView();
					BOOL res = tSRVar->IsValid();
					assert(res);
					ID3D11DepthStencilView* tNewMem = static_cast<ID3D11DepthStencilView*>(tShaderMem);
					HR(tSRVar->SetDepthStencil(tNewMem));
				}
				else if (std::get<1>(eTuple) == eShaderVarType::_USER_DEFINED_TYPE)
				{
					//커스텀 구조체, SetRawValue를 써야 한다.
					ID3DX11EffectVariable* tCustomVar = tEffectVar;
					BOOL res = tCustomVar->IsValid();
					assert(res);
					tCustomVar->SetRawValue(tShaderMem, 0, tSingleSize * tElemCnt);
				}
				else
				{
					//HR(tEffectVar->SetRawValue(tShaderMem, 0, tSingleSize * tElemCnt));
				}
			}

		}
	}

	//Void 포인터 : 이미 Variable Map에 정보의 크기를 저장하고 있으니, 메모리 단위로 받아도 안전하게 캐스팅 가능!
	//Shader의 Map Tuple이 완벽하게 돌아가지 않을 경우, 오류가 날 것이다.
	void Shader::UpdateSingleVariable(const std::string& _varName, void* _dataMem)
	{
		try
		{
			//같은 코드를 찾기.
			EffectsTuple& val = m_FXVarSizeMap.at(_varName);
			std::get<3>(val) = _dataMem;
		}
		catch (std::exception& _error)
		{
			assert(false);
			abort();
		}
	}



	ID3DX11Effect* Shader::CreateEffect(const std::string& _fileRoute)
	{
		//이 방식은 이미 FX 파일이 있을 경우는 괜찮지만,일단은 이 방식대로 ㄱㄱ.
		std::ifstream fin(_fileRoute, std::ios::binary);

		fin.seekg(0, std::ios_base::end);
		int size = (int)fin.tellg();
		fin.seekg(0, std::ios_base::beg);
		std::vector<char> compiledShader(size);

		fin.read(&compiledShader[0], size);
		fin.close();

		//만들어질 Effects 객체.
		ID3DX11Effect* mFX;
		HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
			0, LowLevelDX11::GetInstance()->m_d3dDevice.Get(), &mFX));
		return mFX;
	}

	bool Shader::CheckShaderDataExist()
	{
		for (const auto& [varString, eTuple] : m_FXVarSizeMap)
		{
			if (std::get<3>(eTuple) == nullptr)
			{
				return false;
			}
		}
		return true;
	}

	ID3DX11Effect* Shader::GetEffect()
	{
		return m_Effect;
	}

}
