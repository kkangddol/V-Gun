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

	///���� SetRenderData���� �� �����Ӵ� �ѹ��� ȣ���. 
	void SceneConstantsUpdater::PlaceShaderConstants(RenderConstantData* rcData)
	{
		this->_renderConstantData = rcData;

		//�ϴ� ���� ������ ����.
		// 0. �ϴ�, �̸� ó���� �ǰ����� �߰����� ���� Ŭ���� (�ε���)
		_tempPointIndexList.clear();
		_tempSpotIndexList.clear();
		
		//1. Intensity�� �������� Directional Light�� �����Ѵ�. ��������.
		//Directional Light�� ���� ������� ����ü ��ü�� �����Ѵ�.
		std::sort(rcData->dirLightList.begin(), rcData->dirLightList.end(),
			[](DirectionalLightData& itA, DirectionalLightData& itB)
			{ return itA.intensity > itB.intensity; });

		//Point Light, Spot Light�� ��ġ�� ������� ���ĵǴ� ���̴�.
		//���� �� ������Ʈ ���� ���۸���, �ε����� ���� ������ �ٲ� ���̴�.
		
		assert(rcData->dirLightList.size() <= MAXIMUM_LIGHT_CALL_COUNT);
		assert(rcData->pointLightList.size() <= MAXIMUM_LIGHT_CALL_COUNT);
		assert(rcData->spotLightList.size() <= MAXIMUM_LIGHT_CALL_COUNT);

		_tempDirLightCount = rcData->dirLightList.size();
		_tempPointLightCount = rcData->pointLightList.size();
		_tempSpotLightCount = rcData->spotLightList.size();
	}

	///���� �� ������Ʈ�� �������۵Ǳ� ������ �갡 ȣ���. 
	void SceneConstantsUpdater::AdjustShaderConstantsPerObject(DirectX::XMFLOAT3 objPosition)
	{
		//Position�� �������� ���� ����ִ� �ε��� ����.
		//����ִ� ���� �־, ���� ��� ���� ���� ����.

		//Point Light Sorting.
		_tempPointIndexList.clear();
		_tempSpotIndexList.clear();

		//���� ����� ���� �������� ����, �������� �� �� �ڿ� �о����.
		//�ִ� ũ�⸸ŭ �ְ�, �ڿ� �� ���� �־��ָ� �ȴ�.
		_tempPointIndexList.resize(_tempPointLightCount);
		_tempSpotIndexList.resize(_tempSpotLightCount);

		//���� Ȱ���� ���� Uniform ����.
		auto& _tPListRef = _renderConstantData->pointLightList;
		auto& _tSListRef = _renderConstantData->spotLightList;

		//Point Light.
		std::iota(_tempPointIndexList.begin(), _tempPointIndexList.end(), 0);
		//�ε��� ���� : ObjPosition <-> _tempPointLightList ���� �Ÿ��� �������� ����.
		std::stable_sort(_tempPointIndexList.begin(), _tempPointIndexList.end(),
			[&_tPListRef, objPosition](UINT a, UINT b)
			{return pow(_tPListRef[a].position.x - objPosition.x, 2) + pow(_tPListRef[a].position.y - objPosition.y, 2) + pow(_tPListRef[a].position.z - objPosition.z, 2) >
			pow(_tPListRef[b].position.x - objPosition.x, 2) + pow(_tPListRef[b].position.y - objPosition.y, 2) + pow(_tPListRef[b].position.z - objPosition.z, 2); });

		//Spot Light.
		std::iota(_tempSpotIndexList.begin(), _tempSpotIndexList.end(), 0);
		//�ε��� ���� : ObjPosition <-> _tempSpotLightList ���� �Ÿ��� �������� ����.
		std::stable_sort(_tempSpotIndexList.begin(), _tempSpotIndexList.end(),
			[&_tSListRef, objPosition](UINT a, UINT b)
			{return pow(_tSListRef[a].position.x - objPosition.x, 2) + pow(_tSListRef[a].position.y - objPosition.y, 2) + pow(_tSListRef[a].position.z - objPosition.z, 2) >
			pow(_tSListRef[b].position.x - objPosition.x, 2) + pow(_tSListRef[b].position.y - objPosition.y, 2) + pow(_tSListRef[b].position.z - objPosition.z, 2); });

		//�ٽ� ���� ������¡,=> �ڿ��� �޸� ä���� ����Ʈ ���� ���� �ȴ�.
		_tempPointIndexList.resize(MAXIMUM_LIGHT_CALL_COUNT);
		_tempSpotIndexList.resize(MAXIMUM_LIGHT_CALL_COUNT);
	}

	///CPU�� ����Ʈ ȣ��ο��� Ȱ��� �뵵. 
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
