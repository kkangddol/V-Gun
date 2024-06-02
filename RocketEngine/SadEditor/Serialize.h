#pragma once
#include <string>
#include <vector>

#include "../RocketMath/RocketMath.h"

// json�� ���� ���
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/stringbuffer.h"
#include "../include/rapidjson/prettywriter.h" 


// imgui�� ���� ���

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Image.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "imgui_internal.h"

#include "..\\RocketEngine\\GameObject.h"

#ifdef _DEBUG
#pragma comment(lib,"RocketEngine.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketEngine.lib")
#endif // _DEBUG

#include "DataTypes.h"

using namespace rapidjson;


/// <summary>
/// ���� ���� �ø��������� ��ø������� ���� Ŭ���� 
/// 23.07.31 ������
/// ����� �ø��������� ��ø������� �����ϴ� �Լ��� ������ �ִ�.
/// </summary>


namespace SAD
{
	class RocketEngine::Scene;

	class Serialize
	{
	public:
		Serialize();
		~Serialize();

	public:
		/// �ϳ��� ���� �ø��������
		void SerializeSceneByName(const RocketEngine::Scene& scenedata);
		void SerializeSceneByObjList(const std::vector<RocketEngine::GameObject*> objs);
		
		//void SerializeComponents(RocketEngine::GameObject* obj, const std::pair<std::string, RocketCore::IComponent*>& comp, Document& doc, Value& objValue);
		void SerializeComponents(RocketEngine::GameObject* obj, const std::pair<std::string, std::vector<RocketCore::IComponent*>>& comp, Document& doc, Value& objValue);

	private:
		/// ���� ���� ������Ʈ���� �ø��������

		// ��������� ����� Ʈ������ �ø�������� �Լ�
		void TransformSerialization(const RocketEngine::GameObject& data, Document& doc, Value& val);

		// ������ ������Ʈ�� ���� �ø������� ����
		void MeshRendererSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);
		void SkinnedMeshRendererSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);
		void SpriteRendererSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);
		void TextBoxSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);
		void CapsuleColliderSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);
		void BoxColliderSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);
		void SphereColliderSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);
		void PlaneColliderSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);
		void StaticBoxColliderSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);
		void ButtonSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);
		void CameraSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);
		void DirectionalLightSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);
		void PointLightSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);
		void SpotLightSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val);




	public:
		/// �� �����Ͱ� ����� ���� ���
		static std::string filepath;
		/// ���Ͽ��� �ϳ��� ����ü�� �����͸� �ٽ� �ε��ϴ� �Լ�
		void GetAllDataFromFile();

		// �� �̸��� ���� ����
		static std::string sceneName;
		// �ø�������� �����͸� ���� ���� ū ����ü
		static std::vector<DataStruct> serializeData;
	};
	
}

