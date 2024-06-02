#include "VertexStructHelper.h"
#include <d3d11.h>
#include <cstring>
#include "RenderPipeline.h"
#include "LowLevelDX11.h"
#include <tchar.h>
#include <comdef.h> 

namespace RocketCore::Graphics
{
	void VertexStructHelper::SetInputClassification(const std::string& _ic)
	{
		m_ICMode = InputClassification()[_ic];
	}

	void VertexStructHelper::InitVertexLayout()
	{
		//�����ؾ� �Ѵ�.
		m_CurrentByteOffset = 0;

		//�� �̻� m_IEDescVec�� �����͸� ���� �ʴ´�! (Delete �ʿ� ����)

		//���͵� ���� ��� Ŭ����. (����)
		m_IEDescVec.clear();
		m_SemanticNameVec.clear();
	}

	void VertexStructHelper::InsertSingleAlign(const InputLayoutTuple& _ilTuple)
	{
		const auto& [semName, semIndex, dxgiFormatStr] = _ilTuple;
		DXGI_FORMAT tFormat = VertexEnumMap()[dxgiFormatStr].first;
		int tOffset = VertexEnumMap()[dxgiFormatStr].second;

		D3D11_INPUT_ELEMENT_DESC tDesc;

		//Semantic Name�� �޸� ���� ������� ����, �ٸ� ������� ����� ����.
		tDesc.SemanticIndex = semIndex;
		tDesc.Format = tFormat;
		tDesc.InputSlot = 0; //�ϴ� �⺻����.
		tDesc.AlignedByteOffset = m_CurrentByteOffset; //���� �Ϳ� Alignment �߰�.
		tDesc.InputSlotClass = (D3D11_INPUT_CLASSIFICATION)m_ICMode;
		tDesc.InstanceDataStepRate = 0; //�ϴ� �⺻����.

		//��������� Offset ���.
		m_CurrentByteOffset += tOffset;
		//���Ϳ� �߰�.
		m_IEDescVec.push_back(tDesc);
		m_SemanticNameVec.push_back(semName);
	}

	AssetInputStruct VertexStructHelper::FinishInputLayout(ID3DX11EffectTechnique* _tech)
	{
		ID3D11InputLayout* tLayout = nullptr;

		//���ϰ� ���� ���� ���� ����ü.
		AssetInputStruct tInputStruct;

		//�迭�� �ű�.
		D3D11_INPUT_ELEMENT_DESC* descArray = new D3D11_INPUT_ELEMENT_DESC[m_IEDescVec.size()];
		
		//������ ��ü�� �����̶� std::copy�� �Ұ�.
		for (int i = 0; i < m_IEDescVec.size(); i++)
		{
			descArray[i] = m_IEDescVec[i];
		}

		//SemanticName �ٽ� �־��ֱ�.
		for (int i = 0; i < m_IEDescVec.size(); i++)
		{
			descArray[i].SemanticName = m_SemanticNameVec[i].c_str();

			//InputStruct�� ���� ������ ����.
			tInputStruct.m_SemanticNameVector.push_back(m_SemanticNameVec[i]);
		}

		//PASS_DESC ������.
		D3DX11_PASS_DESC passDesc;
		_tech->GetPassByIndex(0)->GetDesc(&passDesc);

		//InputLayout ����.
		LowLevelDX11::GetInstance()->m_d3dDevice.Get()->
			CreateInputLayout(descArray, m_IEDescVec.size(), passDesc.pIAInputSignature,
				passDesc.IAInputSignatureSize, &tLayout);

		//DirectXMesh�� ComputeInputLayout Ȱ��, �̸� ������ �ѱ��.
		uint32_t offsets[D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT];
		ZeroMemory(offsets, _countof(offsets));
		uint32_t strides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
		ZeroMemory(strides, _countof(strides));
		ComputeInputLayout(descArray, m_IEDescVec.size(), offsets, strides);

		tInputStruct.m_Layout = tLayout;
		tInputStruct.m_InputLayoutCount = m_IEDescVec.size();
		tInputStruct.m_InputLayoutSize = passDesc.IAInputSignatureSize;
		memcpy(tInputStruct.m_Offsets, offsets, sizeof(offsets));
		memcpy(tInputStruct.m_Strides, strides, sizeof(strides));

		//�޸� ����.
		delete[] descArray;

		return tInputStruct;
	}

	RocketCore::Graphics::eShaderVertexSemantic VertexStructHelper::VertexSemanticStringToEnum(const std::string& _semNameStr)
	{
		//Color�� ���� �ʴ´�. (�ϴ���)
		if (_semNameStr == "BINORMAL")
		{
			return eShaderVertexSemantic::_BINORMAL;
		}
		else if (_semNameStr == "BLENDINDICES")
		{
			return eShaderVertexSemantic::_BLENDINDICES;
		}
		else if (_semNameStr == "BLENDWEIGHT")
		{
			return eShaderVertexSemantic::_BLENDWEIGHT;
		}
		else if (_semNameStr == "NORMAL")
		{
			return eShaderVertexSemantic::_NORMAL;
		}
		else if (_semNameStr == "POSITION")
		{
			return eShaderVertexSemantic::_POSITION;
		}
		else if (_semNameStr == "TANGENT")
		{
			return eShaderVertexSemantic::_TANGENT;
		}
		else if (_semNameStr == "TEXCOORD")
		{
			return eShaderVertexSemantic::_TEXCOORD;
		}
		else
		{
			//MGRT_ERROR("No Such Semantic Known : ", _semNameStr);
			assert(false);
			return eShaderVertexSemantic::_ERROR;
		}
	}

}
