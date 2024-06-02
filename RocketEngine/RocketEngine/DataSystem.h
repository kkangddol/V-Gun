#pragma once
#include "Singleton.h"
#include <string>
#include <vector>
#include <unordered_map>
#include "Data.h"

#include "../include/rapidjson/document.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/stringbuffer.h"
#include "../include/rapidjson/prettywriter.h" 

namespace RocketEngine
{
	class GameObject;
	class Scene;
}

namespace RocketCore
{
	class IComponent;
}

namespace RocketCore
{

	class DataSystem : public Singleton<DataSystem>
	{
		friend Singleton;
	private:
		DataSystem() = default;

	public:
		~DataSystem() = default;

	public:
		void Initialize();
		void Finalize();

	public:
		void SetAllDataToScene(std::string scenePath);
		void ExportSceneToJson(RocketEngine::Scene& scenedata);

	private:
		void GetAllDataFromJson(std::string filename);
		void MakeAllObjects();
		void CreateObjAndComponent(DataStruct& data);
		void SetObjOption(DataStruct& data);

		void SerializeComponents(RocketEngine::GameObject* obj, std::pair< const std::string, std::vector< RocketCore::IComponent* >>& comp, rapidjson::Document& doc, rapidjson::Value& objValue);

		/// ���� ���� ������Ʈ���� �ø��������
		// ��������� ����� Ʈ������ �ø�������� �Լ�
		void TransformSerialization(const RocketEngine::GameObject& data, rapidjson::Document& doc, rapidjson::Value& val);
		// ������ ������Ʈ�� ���� �ø������� ����
		void MeshRendererSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
		void SkinnedMeshRendererSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
		void SpriteRendererSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
		//void AnimationControllerSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);
		void TextBoxSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);

		void CapsuleColliderSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
		void BoxColliderSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
		void SphereColliderSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);

		void PlaneColliderSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
		void StaticBoxColliderSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
		void ButtonSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);

		//void AudioClipSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);
		void CameraSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);

		void DirectionalLightSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
		void PointLightSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);
		void SpotLightSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, rapidjson::Document& doc, rapidjson::Value& val);

		RocketEngine::Scene* currentScene;
		std::string sceneName;
		std::string currentFilePath;
		std::vector<DataStruct> serializeData;
		std::unordered_map<std::string, RocketEngine::GameObject*> objs;

		DataStruct* _FORDEBUG;
	};

}