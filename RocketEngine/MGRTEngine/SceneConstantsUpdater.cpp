#include "SceneConstantsUpdater.h"
#include "LowLevelDX11.h"
#include "MathHelper.h"
#include <utility>
#include <functional>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <d3dcompiler.h>
#include <d3dcommon.h>
#include "d3dx11effect.h"
#include <istream>
#include <numeric>  
#include "Define3D.h"

namespace RocketCore::Graphics
{
	RocketCore::Graphics::SceneConstantsUpdater* SceneConstantsUpdater::instance = nullptr;

	void SceneConstantsUpdater::Initialize()
	{
		_tempDirLightCount = 0;
		_tempPointLightCount = 0;
		_tempSpotLightCount = 0;
	}

	///현재 SetRenderData에서 한 프레임당 한번씩 호출됨. 
	void SceneConstantsUpdater::PlaceShaderConstants(RenderConstantData* rcData)
	{
		this->_renderConstantData = rcData;

		//일단 들어온 데이터 관리.
		// 0. 일단, 미리 처리는 되겠으나 추가적인 버퍼 클리어 (인덱스)
		_tempPointIndexList.clear();
		_tempSpotIndexList.clear();
		
		//1. Intensity를 기준으로 Directional Light를 정렬한다. 내림차순.
		//Directional Light는 순서 상관없이 구조체 자체를 정렬한다.
		std::sort(rcData->dirLightList.begin(), rcData->dirLightList.end(),
			[](DirectionalLightData& itA, DirectionalLightData& itB)
			{ return itA.intensity > itB.intensity; });

		//Point Light, Spot Light은 위치를 기반으로 정렬되는 것이다.
		//따로 매 오브젝트 렌더 시작마다, 인덱스를 보는 순서를 바꿀 것이다.
		
		assert(rcData->dirLightList.size() <= MAXIMUM_LIGHT_CALL_COUNT);
		assert(rcData->pointLightList.size() <= MAXIMUM_LIGHT_CALL_COUNT);
		assert(rcData->spotLightList.size() <= MAXIMUM_LIGHT_CALL_COUNT);

		_tempDirLightCount = rcData->dirLightList.size();
		_tempPointLightCount = rcData->pointLightList.size();
		_tempSpotLightCount = rcData->spotLightList.size();
	}

	///현재 매 오브젝트가 렌더시작되기 전마다 얘가 호출됨. 
	void SceneConstantsUpdater::AdjustShaderConstantsPerObject(DirectX::XMFLOAT3 objPosition)
	{
		//Position을 기준으로 현재 들어있는 인덱스 정렬.
		//비어있는 곳이 있어도, 앞이 비어 있을 일은 없다.

		//Point Light Sorting.
		_tempPointIndexList.clear();
		_tempSpotIndexList.clear();

		//가장 가까운 것을 기준으로 소팅, 나머지는 싹 다 뒤에 밀어줘야.
		//있는 크기만큼 넣고, 뒤에 빈 값을 넣어주면 된다.
		_tempPointIndexList.resize(_tempPointLightCount);
		_tempSpotIndexList.resize(_tempSpotLightCount);

		//람다 활용을 위한 Uniform 참조.
		auto& _tPListRef = _renderConstantData->pointLightList;
		auto& _tSListRef = _renderConstantData->spotLightList;

		//Point Light.
		std::iota(_tempPointIndexList.begin(), _tempPointIndexList.end(), 0);
		//인덱스 소팅 : ObjPosition <-> _tempPointLightList 사이 거리값 기준으로 정렬.
		std::stable_sort(_tempPointIndexList.begin(), _tempPointIndexList.end(),
			[&_tPListRef, objPosition](UINT a, UINT b)
			{return pow(_tPListRef[a].position.x - objPosition.x, 2) + pow(_tPListRef[a].position.y - objPosition.y, 2) + pow(_tPListRef[a].position.z - objPosition.z, 2) >
			pow(_tPListRef[b].position.x - objPosition.x, 2) + pow(_tPListRef[b].position.y - objPosition.y, 2) + pow(_tPListRef[b].position.z - objPosition.z, 2); });

		//Spot Light.
		std::iota(_tempSpotIndexList.begin(), _tempSpotIndexList.end(), 0);
		//인덱스 소팅 : ObjPosition <-> _tempSpotLightList 사이 거리값 기준으로 정렬.
		std::stable_sort(_tempSpotIndexList.begin(), _tempSpotIndexList.end(),
			[&_tSListRef, objPosition](UINT a, UINT b)
			{return pow(_tSListRef[a].position.x - objPosition.x, 2) + pow(_tSListRef[a].position.y - objPosition.y, 2) + pow(_tSListRef[a].position.z - objPosition.z, 2) >
			pow(_tSListRef[b].position.x - objPosition.x, 2) + pow(_tSListRef[b].position.y - objPosition.y, 2) + pow(_tSListRef[b].position.z - objPosition.z, 2); });

		//다시 벡터 리사이징,=> 뒤에는 메모리 채우기용 디폴트 값이 들어가게 된다.
		_tempPointIndexList.resize(MAXIMUM_LIGHT_CALL_COUNT);
		_tempSpotIndexList.resize(MAXIMUM_LIGHT_CALL_COUNT);
	}

	///CPU의 라이트 호출부에서 활용될 용도. 
	unsigned int SceneConstantsUpdater::GetDirLightCount()
	{
		return _tempDirLightCount;
	}

	unsigned int SceneConstantsUpdater::GetPointLightCount()
	{
		return _tempPointLightCount;
	}

	unsigned int SceneConstantsUpdater::GetSpotLightCount()
	{
		return _tempSpotLightCount;
	}

	unsigned int SceneConstantsUpdater::GetDirLightBrightIndex(unsigned int numIndex)
	{
		return numIndex;
	}

	unsigned int SceneConstantsUpdater::GetPointLightBrightIndex(unsigned int numIndex)
	{
		return _tempPointIndexList[numIndex];
	}

	unsigned int SceneConstantsUpdater::GetSpotLightBrightIndex(unsigned int numIndex)
	{
		return _tempSpotIndexList[numIndex];
	}

	RocketCore::Graphics::RenderConstantData* SceneConstantsUpdater::GetRenderConstantData()
	{
		return this->_renderConstantData;
	}



}
