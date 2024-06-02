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
		//리셋해야 한다.
		m_CurrentByteOffset = 0;

		//더 이상 m_IEDescVec에 포인터를 담지 않는다! (Delete 필요 없음)

		//벡터들 내부 요소 클리어. (리셋)
		m_IEDescVec.clear();
		m_SemanticNameVec.clear();
	}

	void VertexStructHelper::InsertSingleAlign(const InputLayoutTuple& _ilTuple)
	{
		const auto& [semName, semIndex, dxgiFormatStr] = _ilTuple;
		DXGI_FORMAT tFormat = VertexEnumMap()[dxgiFormatStr].first;
		int tOffset = VertexEnumMap()[dxgiFormatStr].second;

		D3D11_INPUT_ELEMENT_DESC tDesc;

		//Semantic Name은 메모리 관리 방식으로 인해, 다른 방식으로 기록할 예정.
		tDesc.SemanticIndex = semIndex;
		tDesc.Format = tFormat;
		tDesc.InputSlot = 0; //일단 기본으로.
		tDesc.AlignedByteOffset = m_CurrentByteOffset; //기존 것에 Alignment 추가.
		tDesc.InputSlotClass = (D3D11_INPUT_CLASSIFICATION)m_ICMode;
		tDesc.InstanceDataStepRate = 0; //일단 기본으로.

		//현재까지의 Offset 기록.
		m_CurrentByteOffset += tOffset;
		//벡터에 추가.
		m_IEDescVec.push_back(tDesc);
		m_SemanticNameVec.push_back(semName);
	}

	AssetInputStruct VertexStructHelper::FinishInputLayout(ID3DX11EffectTechnique* _tech)
	{
		ID3D11InputLayout* tLayout = nullptr;

		//리턴값 편한 전달 위한 구조체.
		AssetInputStruct tInputStruct;

		//배열로 옮김.
		D3D11_INPUT_ELEMENT_DESC* descArray = new D3D11_INPUT_ELEMENT_DESC[m_IEDescVec.size()];
		
		//포인터 자체의 보관이라 std::copy는 불가.
		for (int i = 0; i < m_IEDescVec.size(); i++)
		{
			descArray[i] = m_IEDescVec[i];
		}

		//SemanticName 다시 넣어주기.
		for (int i = 0; i < m_IEDescVec.size(); i++)
		{
			descArray[i].SemanticName = m_SemanticNameVec[i].c_str();

			//InputStruct에 값을 빼내기 위해.
			tInputStruct.m_SemanticNameVector.push_back(m_SemanticNameVec[i]);
		}

		//PASS_DESC 가져옴.
		D3DX11_PASS_DESC passDesc;
		_tech->GetPassByIndex(0)->GetDesc(&passDesc);

		//InputLayout 만듬.
		LowLevelDX11::GetInstance()->m_d3dDevice.Get()->
			CreateInputLayout(descArray, m_IEDescVec.size(), passDesc.pIAInputSignature,
				passDesc.IAInputSignatureSize, &tLayout);

		//DirectXMesh의 ComputeInputLayout 활용, 미리 데이터 넘기기.
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

		//메모리 해제.
		delete[] descArray;

		return tInputStruct;
	}

	RocketCore::Graphics::eShaderVertexSemantic VertexStructHelper::VertexSemanticStringToEnum(const std::string& _semNameStr)
	{
		//Color는 쓰지 않는다. (일단은)
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
