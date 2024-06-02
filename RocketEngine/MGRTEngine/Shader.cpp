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
	//����Ʈ ���̴� ����.
	Shader::Shader()
	{
		assert(false); //���Ǹ� �ȵȴ�.
		///�Ͼ��, �ϴ� ����Ʈ �Ű����� ���� �Ϸ� + Material, InputLayout �޾Ƽ� ������ �� �ִ� ������ ����.
	}

	//Ŀ���� ���̴� ����, Effect/TechVariable���� ���õǾ ���´�.
	Shader::Shader(const std::string& _shaderName, const std::string& _fileRoute,
		const std::vector<std::string>& _techMap,
		const std::map<std::pair<std::string, eShaderVarType>, std::pair<int, int>>& _varMap)
	{
		this->m_ShaderName = _shaderName;
		ID3DX11Effect* mFX = CreateEffect(_fileRoute);
		this->m_Effect = mFX; //���������� ����.

		m_FXFilePath = _fileRoute;

		//��ũ���� �̸�, ���� ID3DX11TechniqueVariable�� ����!
		for (auto& it : _techMap)
		{
			ID3DX11EffectTechnique* tTech = mFX->GetTechniqueByName(it.c_str());
			assert(tTech->IsValid());
			m_FXTechMap.insert(std::make_pair(it, tTech));
		}

		////������, Effect�� � ������ Ȱ���ؾ� �ϴ��� �����ϴ� �ڵ�.
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

	//�Ƹ� �� �Լ��� (IASetVB,IB)�� (TechApply, DrawIndexed) ���̿� ��ġ�� ���� ��.
	void Shader::UpdateShader()
	{
		D3DX11_TECHNIQUE_DESC techDesc;
		m_MainTech->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			//IA�ܰ迡�� VB,IB Buffer ������ ���⼭ �̷������ ���� �ƴϴ�.
			//����� Shader�� Variable�� ���ư��鼭 ���� ���� �� �ְ�!

			//Memory�� ���� voidPtr, ���� SetRawValue�� ���� ������ Ȱ���� ����!
			for (const auto& [varString, eTuple] : m_FXVarSizeMap)
			{
				ID3DX11EffectVariable* tEffectVar = std::get<0>(eTuple);
				void* tShaderMem = std::get<3>(eTuple);
				int tSingleSize = std::get<2>(eTuple).first;
				int tElemCnt = std::get<2>(eTuple).second;
				//SetResource�� �۵������� �𸣰�����, �ϴ� �ٸ� �о߿����� ���� ���� �۵��ϴ� ���� Ȯ��.
				//�ϴ��� �б� ����! (�׷��Ƿ�, ShaderResourceView�� ũ��/������ ������ ���� �ʴ´�.
				///[TW] SetRawValue�� �ȵȴ�! �ϴ���, Matrix ���·� �ٲ㺸��. ����, ���� ������ ����.
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
					//�ӽ� �ڵ�.
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
						////[TW] 100���� �ϴ��� �ϵ��ڵ�.
						ID3DX11EffectMatrixVariable* tMatVar = tEffectVar->AsMatrix();
						BOOL res = tMatVar->IsValid();
						assert(res);
						//HR(tMatVar->SetMatrixArray(reinterpret_cast<const float*>(&tShaderMem),0,tElemCnt
						ByteBuffer* tByteBuffer = (ByteBuffer*)tShaderMem;

						////����Ʈ���� �׽��� ����.
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
						//float* tNewMem = static_cast<float*>(tShaderMem); //ShaderMem���� Float�� ������ N�� �����Ѵ�. (Custom���� ����)
						//HR(tScalarVar->SetFloatArray(tNewMem, 0, tElemCnt))

						///ScalarVariable ��ü�� �ν��� �ȵȴ�..! ��¿ �� ����, Ǯ� SetRawValue�� ����.
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
						int* tNewMem = static_cast<int*>(tShaderMem); //ShaderMem���� Float�� ������ N�� �����Ѵ�. (Custom���� ����)
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
						bool* tNewMem = static_cast<bool*>(tShaderMem); //ShaderMem���� Float�� ������ N�� �����Ѵ�. (Custom���� ����)
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
					//Ŀ���� ����ü, SetRawValue�� ��� �Ѵ�.
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

	//Void ������ : �̹� Variable Map�� ������ ũ�⸦ �����ϰ� ������, �޸� ������ �޾Ƶ� �����ϰ� ĳ���� ����!
	//Shader�� Map Tuple�� �Ϻ��ϰ� ���ư��� ���� ���, ������ �� ���̴�.
	void Shader::UpdateSingleVariable(const std::string& _varName, void* _dataMem)
	{
		try
		{
			//���� �ڵ带 ã��.
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
		//�� ����� �̹� FX ������ ���� ���� ��������,�ϴ��� �� ��Ĵ�� ����.
		std::ifstream fin(_fileRoute, std::ios::binary);

		fin.seekg(0, std::ios_base::end);
		int size = (int)fin.tellg();
		fin.seekg(0, std::ios_base::beg);
		std::vector<char> compiledShader(size);

		fin.read(&compiledShader[0], size);
		fin.close();

		//������� Effects ��ü.
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
