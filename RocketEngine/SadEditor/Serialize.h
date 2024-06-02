#pragma once
#include <string>
#include <vector>

#include "../RocketMath/RocketMath.h"

// json에 대한 헤더
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/stringbuffer.h"
#include "../include/rapidjson/prettywriter.h" 


// imgui에 대한 헤더

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
/// 게임 씬의 시리얼라이즈와 디시리얼라이즈를 위한 클래스 
/// 23.07.31 오수안
/// 멤버로 시리얼라이즈와 디시리얼라이즈를 수행하는 함수를 가지고 있다.
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
		/// 하나의 씬을 시리얼라이즈
		void SerializeSceneByName(const RocketEngine::Scene& scenedata);
		void SerializeSceneByObjList(const std::vector<RocketEngine::GameObject*> objs);
		
		//void SerializeComponents(RocketEngine::GameObject* obj, const std::pair<std::string, RocketCore::IComponent*>& comp, Document& doc, Value& objValue);
		void SerializeComponents(RocketEngine::GameObject* obj, const std::pair<std::string, std::vector<RocketCore::IComponent*>>& comp, Document& doc, Value& objValue);

	private:
		/// 씬이 가진 컴포넌트들을 시리얼라이즈

		// 재귀적으로 수행될 트랜스폼 시리얼라이즈 함수
		void TransformSerialization(const RocketEngine::GameObject& data, Document& doc, Value& val);

		// 나머지 컴포넌트에 대한 시리얼라이즈를 수행
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
		/// 씬 데이터가 저장될 파일 경로
		static std::string filepath;
		/// 파일에서 하나의 구조체로 데이터를 다시 로드하는 함수
		void GetAllDataFromFile();

		// 씬 이름을 가질 변수
		static std::string sceneName;
		// 시리얼라이즈 데이터를 가질 가장 큰 구조체
		static std::vector<DataStruct> serializeData;
	};
	
}

