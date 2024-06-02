#include "MeshFormation.h"
#include "ByteBuffer.hpp"
#include "VertexStructHelper.h"
#include "AssetModelData.h"
#include "LowLevelDX11.h"
#include "Define3D.h"
#include <cassert>

namespace RocketCore::Graphics
{
	MeshFormation::MeshFormation(const AssetInputStruct& _inputStruct, AssetModelData* _modelData) :
		BaseFormation(_inputStruct, _modelData), m_IndexCount(0)
	{
		this->m_ByteBufferForVB = new ByteBuffer;
	}

	MeshFormation::~MeshFormation()
	{
		delete m_ByteBufferForVB;
	}

	void MeshFormation::AssignData(AssetModelData* const modelData)
	{
		//1. Formation의 String들을 싹 다 Enum으로 바꿔주어, Vector 안에 넣기.
		std::vector<eShaderVertexSemantic> tSemVec;
		tSemVec.reserve(_semanticNameVec.size());
		for (auto& it : _semanticNameVec)
		{
			eShaderVertexSemantic tVS = VertexStructHelper::GetInstance().VertexSemanticStringToEnum(it);
			tSemVec.push_back(tVS);
		}

		//2. Formation의 SingleBufferSize, Offset 활용, 실제 간격 사이 바이트 연산!
		std::vector<unsigned int> tIntervalVec;
		size_t byteIntervalSize = _offsetVec.size();
		tIntervalVec.resize(byteIntervalSize);
		for (unsigned int i = 0; i < byteIntervalSize; i++)
		{
			if (i == byteIntervalSize - 1)
			{
				tIntervalVec[i] = _singleBufferSize - _offsetVec[i];
			}
			else
			{
				tIntervalVec[i] = _offsetVec[i + 1] - _offsetVec[i];
			}
		}

		//3. 일단 모든 Vertex를 합친 Count를 저장.
		AssetSceneData* tScene = _modelData->_sceneData;
		//모든 Vertex를 채우기. 
		size_t tVertexCount = 0;
		for (size_t i = 0; i < tScene->m_NumMesh; i++)
		{
			size_t tSingleVerticeCnt = tScene->m_MeshList[i]->m_NumVertice;
			tVertexCount += tSingleVerticeCnt;
		}

		//4. Semantic & Interval에 따라, 값을 집어넣는다!
		for (size_t i = 0; i < tScene->m_NumMesh; i++)
		{
			for (size_t j = 0; j < tScene->m_MeshList[i]->m_NumVertice; j++)
			{
				for (size_t k = 0; k < byteIntervalSize; k++)
				{
					if (tSemVec[k] == eShaderVertexSemantic::_POSITION)
					{
						PutSingleBufferElement((void*)&(tScene->m_MeshList[i]->m_VerticeList[j]), tIntervalVec[k]);
					}
					else if (tSemVec[k] == eShaderVertexSemantic::_NORMAL)
					{
						//Normal 문제 때문에?
						PutSingleBufferElement((void*)&(tScene->m_MeshList[i]->m_NormalList[j]), tIntervalVec[k]);
					}
					else if (tSemVec[k] == eShaderVertexSemantic::_TANGENT)
					{
						PutSingleBufferElement((void*)&(tScene->m_MeshList[i]->m_TangentList[j]), tIntervalVec[k]);
					}
					else if (tSemVec[k] == eShaderVertexSemantic::_BINORMAL)
					{
						PutSingleBufferElement((void*)&(tScene->m_MeshList[i]->m_BiTangentList[j]), tIntervalVec[k]);
					}
					else if (tSemVec[k] == eShaderVertexSemantic::_TEXCOORD)
					{
						DirectX::SimpleMath::Vector3 it = tScene->m_MeshList[i]->m_TextureCoordList[j];
						DirectX::XMFLOAT2 tInput = DirectX::XMFLOAT2(it.x, it.y);
						PutSingleBufferElement((void*)&tInput, tIntervalVec[k]);
					}
					else
					{
						//일반 Mesh를 파싱하는 단계에서, 시맨틱에 다른게 오면 문제이다.
						assert(false);
					}
				}

			}
		}

		//5. 모든 Index Count를 센다.
		size_t tIndexCount = 0;
		for (auto& it : tScene->m_MeshList)
		{
			for (auto&& itt : it->m_FaceList)
			{
				tIndexCount += itt.m_NumIndice;
			}
		}

		this->m_IBVec.reserve(tIndexCount);

		for (auto& it : tScene->m_MeshList)
		{
			for (auto&& itt : it->m_FaceList)
			{
				for (int i = 0; i < itt.m_IndiceList.size(); i++)
				{
					this->m_IBVec.push_back(itt.m_IndiceList[i]);
				}
			}
		}

		//6. 실제로 VB, IB 만들기!
		D3D11_BUFFER_DESC tVBD;
		tVBD.Usage = D3D11_USAGE_IMMUTABLE;
		tVBD.ByteWidth = static_cast<UINT>(_singleBufferSize * tVertexCount);
		tVBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		tVBD.CPUAccessFlags = 0;
		tVBD.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = m_ByteBufferForVB->GetStartAddress();
		HR(LowLevelDX11::GetInstance()->m_d3dDevice->CreateBuffer(&tVBD, &vinitData, &m_VB));

		D3D11_BUFFER_DESC tIBD;
		tIBD.Usage = D3D11_USAGE_IMMUTABLE;
		tIBD.ByteWidth = static_cast<UINT>(sizeof(UINT) * tIndexCount);
		tIBD.BindFlags = D3D11_BIND_INDEX_BUFFER;
		tIBD.CPUAccessFlags = 0;
		tIBD.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = &(m_IBVec[0]);
		HR(LowLevelDX11::GetInstance()->m_d3dDevice->CreateBuffer(&tIBD, &iinitData, &(m_IB)));

		m_IndexCount = static_cast<UINT>(tIndexCount);
	}

	void MeshFormation::PutSingleBufferElement(void* _buffer, size_t _bufLenInBytes)
	{
		m_ByteBufferForVB->putBytes((uint8_t*)_buffer, (uint32_t)_bufLenInBytes);
	}

	void MeshFormation::ReadSingleBufferElement(void* _buffer, size_t _bufLenInBytes)
	{
		m_ByteBufferForVB->getBytes((uint8_t*)_buffer, (uint32_t)_bufLenInBytes);
	}

	


}