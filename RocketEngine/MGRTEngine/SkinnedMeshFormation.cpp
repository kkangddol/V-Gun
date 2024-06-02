#include "SkinnedMeshFormation.h"
#include "Transform.h"
#include "Define3D.h"
#include "ByteBuffer.hpp"
#include "VertexStructHelper.h"
#include "AssetModelData.h"
#include "AssetDataDefine.h"
#include "RenderUsageMesh.h"
#include "RenderUsageVertexBone.h"
#include "RenderUsageBoneInfo.h"
#include "AssetDataDefine.h"
#include "AnimationHandler.h"
#include "LowLevelDX11.h"
#include <cassert>
#include <map>

///예상 시나리오:
///이제는 실제 ASSIMP랑 연동해서 값을 가져오는 일만 남았다.
///1. Parsing한 정보를 버텍스 버퍼에 대입.  (PutSingleBufferElement, 단, 얕은 복사 조심)
///2. stride를 m_SingleBufferSize에 넣는다.
///3. 일반 Rendering 과정.

namespace RocketCore::Graphics
{
	//들어왔을 때는 이미 만들어진 시점.
	SkinnedMeshFormation::SkinnedMeshFormation(const AssetInputStruct& _inputStruct, AssetModelData* _modelData) :
		 BaseFormation(_inputStruct, _modelData)
	{
		this->m_ByteBufferForVB	  = new ByteBuffer;
		this->m_ByteBufferForBones = new ByteBuffer;

	}

	SkinnedMeshFormation::~SkinnedMeshFormation()
	{
		//
	}
	

	//순서대로 Vertex 버퍼에 값을 넣는 함수, 반복적으로 호출된다.
	//Ex. XMFLOAT3, XMFLOAT2, XMFLOAT4 순서대로라면,
	//전달인자에 차례로 동일하게 들어와 호출되어야 한다!
	//나중에는, 일괄적으로 InputSizeList에 의해 처리되는 것이 목표.


	void SkinnedMeshFormation::AssignData(AssetModelData* const modelData)
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

		//3. AssetModelData에서 RenderUsageNode로 필요한 정보를 연동한다.
		std::vector<unsigned int> tIndiceList;
		m_VertexBoneList.clear();
		tIndiceList.clear();

		InitAnimDataFromScene(m_VertexBoneList, tIndiceList, modelData);

		//5. Semantic & Interval에 따라, VB,IB의 버퍼에에, Vertex 값을 집어넣는다!
		AssetSceneData* tScene = _modelData->_sceneData;

		//모든 Vertex를 채우기. 
		size_t tVertexCount = 0;
		for (size_t i = 0; i < tScene->m_NumMesh; i++)
		{
			size_t tSingleVerticeCnt = tScene->m_MeshList[i]->m_NumVertice;
			tVertexCount += tSingleVerticeCnt;
		}


		//지금까지 Bone Index/Weight Binding을 위해, 인덱스 카운팅 도입.
		UINT tTotalElapsedVertexCount = 0;

		int tNowBlendIndexIDX = 0;
		int tNowBlendWeightIDX = 0;

		//4. Semantic & Interval에 따라, 값을 집어넣는다!
		for (size_t i = 0; i < tScene->m_NumMesh; i++) // Mesh 숫자.
		{
			tTotalElapsedVertexCount += m_MeshEntries[i].BaseVertex;
			for (size_t j = 0; j < tScene->m_MeshList[i]->m_NumVertice; j++) //Vertice에 해당.
			{
				//새로운 Vertex 시작할 때마다 읽고 있는 Blending 관련 Index가 리셋되어야 한다.
				tNowBlendIndexIDX = 0;
				tNowBlendWeightIDX = 0;
				for (size_t k = 0; k < byteIntervalSize; k++)
				{
					if (tSemVec[k] == eShaderVertexSemantic::_POSITION)
					{
						PutSingleBufferElement((void*)&(tScene->m_MeshList[i]->m_VerticeList[j]), tIntervalVec[k]);
					}
					else if (tSemVec[k] == eShaderVertexSemantic::_NORMAL)
					{
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
					else if (tSemVec[k] == eShaderVertexSemantic::_BLENDINDICES) //다 따로 둔다고 생각하자!
					{
						//여기서는 tVertexBoneList가 전체 들어갈 VertexBuffer의 인덱스와 같아야 한다.
						//Mesh를 기준으로 현재까지의 VertexBuffer를 역추적할까?
						//설정했던 VertexBone 정보는 전체 VB/IB 기준,
						//이를 기준으로 다시 Mesh 기준으로 For문을 다시 돌릴 수 있는 방법이 있어야!
						//MeshEntries가 이 역할을 해준다.
						UINT tBlendIndice = m_VertexBoneList.at(j + tTotalElapsedVertexCount).IDs[tNowBlendIndexIDX];
						PutSingleBufferElement((void*)&tBlendIndice, tIntervalVec[k]);
						tNowBlendIndexIDX++;
						assert(tNowBlendIndexIDX <= 4);
					}
					else if (tSemVec[k] == eShaderVertexSemantic::_BLENDWEIGHT) //다 따로 둔다고 생각하자!
					{
						float tBlendWeight = m_VertexBoneList.at(j + tTotalElapsedVertexCount).Weights[tNowBlendWeightIDX];
						PutSingleBufferElement((void*)&tBlendWeight, tIntervalVec[k]);
						tNowBlendWeightIDX++;
						assert(tNowBlendWeightIDX <= 4);
					}
					else
					{
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

		//Formation에서, Vertex Buffer, Index Buffer 연동 끝난 상태!
		return;
	}

	void SkinnedMeshFormation::InitAnimDataFromScene(std::vector<RenderUsageVertexBone>& outBone, std::vector<unsigned int>& outIndice, AssetModelData* const modelData)
	{
		m_MeshEntries.resize(modelData->_sceneData->m_NumMesh);

		this->_modelData = modelData;

		//예제코드에서는 여기에 tVertices가 있었다.
		///Mesh들이 있으니, 이는 나중에 데이터가 들어올 때 정도까지 보관하고 있는 방식 채택!

		unsigned int tNumVertices = 0;
		unsigned int tNumIndices = 0;

		for (unsigned int i = 0; i < m_MeshEntries.size(); i++)
		{
			// Total mesh indices. 
			m_MeshEntries[i].NumIndices = modelData->_sceneData->m_MeshList[i]->m_NumFace * AssetFaceData::DEFAULT_INDEX_COUNT;

			// Set the base vertex of this mesh (initial vertex for this mesh within the vertices array) to the current total vertices. 
			m_MeshEntries[i].BaseVertex = tNumVertices;

			// Set the base index of this mesh (initial index for this mesh within the indices array) to the current total indices. 
			m_MeshEntries[i].BaseIndex = tNumIndices;

			// Increment total vertices and indices. 
			tNumVertices += modelData->_sceneData->m_MeshList[i]->m_NumVertice;
			tNumIndices += m_MeshEntries[i].NumIndices;
		}

		outBone.resize(tNumVertices);
		outIndice.reserve(tNumIndices);

		for (unsigned int i = 0; i < m_MeshEntries.size(); i++)
		{
			AssetMeshData* aMesh = modelData->_sceneData->m_MeshList[i];
			InitRenderMesh(i, aMesh, outIndice, outBone);
		}
		assert(true);
	}

	void SkinnedMeshFormation::InitRenderMesh(unsigned int index, AssetMeshData* meshData,
		std::vector<unsigned int>& indicesList, std::vector<RenderUsageVertexBone>& vBonesList)
	{
		//Vertex 로드는 인덱스가 변하지 않기 때문에, 
		// 제대로 적용됨
		if (meshData->m_MeshHasBones)
		{
			LoadRenderBones(index, meshData, vBonesList);
		}

		for (unsigned int i = 0; i < meshData->m_NumFace; i++)
		{
			AssetFaceData tFace = meshData->m_FaceList[i];
			assert(tFace.m_NumIndice == 3);
			indicesList.push_back(tFace.m_IndiceList[0]);
			indicesList.push_back(tFace.m_IndiceList[1]);
			indicesList.push_back(tFace.m_IndiceList[2]);
		}

		//InitMesh까지 했다.
	}

	void SkinnedMeshFormation::LoadRenderBones(unsigned int meshIndex, 
		AssetMeshData* mesh, std::vector<RenderUsageVertexBone>& vBonesList)
	{
		for (unsigned int i = 0; i < mesh->m_NumBone; i++) {

			unsigned int BoneIndex = 0;

			// Obtain the bone name.
			std::string BoneName(mesh->m_BoneList[i]->m_Name);

			// If bone isn't already in the map. 
			if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) 
			{
				// Set the bone ID to be the current total number of bones. 
				BoneIndex = _formationNumBone;

				// Increment total bones. 
				_formationNumBone++;

				// Push new bone info into bones vector. 
				RenderUsageBoneInfo tBi;
				m_RenderBoneInfo.push_back(tBi);
			}
			else {
				// Bone ID is already in map. 
				BoneIndex = m_BoneMapping[BoneName];
			}

			m_BoneMapping[BoneName] = BoneIndex;

			// Obtains the offset matrix which transforms the bone from mesh space into bone space. 
			m_RenderBoneInfo[BoneIndex].BoneOffset = mesh->m_BoneList[i]->m_InverseBindPoseMatrix;

			// Iterate over all the affected vertices by this bone i.e weights. 
			for (unsigned int j = 0; j < mesh->m_BoneList[i]->m_NumWeight; j++) {

				// Obtain an index to the affected vertex within the array of vertices.
				unsigned int VertexID = m_MeshEntries[meshIndex].BaseVertex + mesh->m_BoneList[i]->m_WeightList[j].m_VertexID;
				// The value of how much this bone influences the vertex. 
				float Weight = mesh->m_BoneList[i]->m_WeightList[j].m_Weight;

				// Insert bone data for particular vertex ID. A maximum of 4 bones can influence the same vertex. 
				vBonesList[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}
		assert(mesh);
	}


	void SkinnedMeshFormation::PutSingleBufferElement(void* _buffer, size_t _bufLenInBytes)
	{
		m_ByteBufferForVB->putBytes((uint8_t*)_buffer, (uint32_t)_bufLenInBytes);
	}

	void SkinnedMeshFormation::ReadSingleBufferElement(void* _buffer, size_t _bufLenInBytes)
	{
		m_ByteBufferForVB->getBytes((uint8_t*)_buffer, (uint32_t)_bufLenInBytes);
	}

	

}







