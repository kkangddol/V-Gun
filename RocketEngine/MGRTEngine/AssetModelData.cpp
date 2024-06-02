#include "AssetModelData.h"
#include "ResourceManager.h"
#include <cassert>

namespace RocketCore::Graphics
{
	AssetModelData::AssetModelData(unsigned int _handleID) :
		_handleID(_handleID)
	{
		_sceneData = new AssetSceneData;
		_meshType = eModelMeshType::NONE;
	}

	AssetModelData::~AssetModelData()
	{
		delete _sceneData;
	}
}