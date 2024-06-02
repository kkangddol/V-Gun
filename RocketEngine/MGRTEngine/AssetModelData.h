//실제 Model에 관련된 요소만 확실하게 저장하고, 
//리소스 관리가 가능한 Model Data 관리용 Class.

///FBX 파일이 들고 있는 모든 정보를 실제로 사용 가능한 형태로 들고 있는 클래스.
///오브젝트들이 들고 있는 Formation에 투자되는 리소스를 효율적으로 활용하면서,
///매번 모든 파일을 파싱하지 않아도 되게 세팅할 것.
///모든 요소가 매번 Formation에 들어가는 것이 아니다.

#pragma once
#include <string>
#include <memory>
#include "AssetDataDefine.h"
#include "TypedefGraphicHelper.h"
#include "../RocketGraphicsInterface/ModelData.h"

namespace RocketCore::Graphics
{
	class AssetModelData
	{
	public:
		AssetModelData(unsigned int _handleID);
		~AssetModelData();

		std::string _directory;
		std::string _fileName;
		std::string _extension;
		eModelMeshType _meshType;

		//모든 정보가 저장되는 장소.
		AssetSceneData* _sceneData = nullptr;
	private:
		unsigned int _handleID;
	};
}


