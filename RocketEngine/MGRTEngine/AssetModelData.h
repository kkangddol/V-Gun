//���� Model�� ���õ� ��Ҹ� Ȯ���ϰ� �����ϰ�, 
//���ҽ� ������ ������ Model Data ������ Class.

///FBX ������ ��� �ִ� ��� ������ ������ ��� ������ ���·� ��� �ִ� Ŭ����.
///������Ʈ���� ��� �ִ� Formation�� ���ڵǴ� ���ҽ��� ȿ�������� Ȱ���ϸ鼭,
///�Ź� ��� ������ �Ľ����� �ʾƵ� �ǰ� ������ ��.
///��� ��Ұ� �Ź� Formation�� ���� ���� �ƴϴ�.

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

		//��� ������ ����Ǵ� ���.
		AssetSceneData* _sceneData = nullptr;
	private:
		unsigned int _handleID;
	};
}


