#include "Serialize.h"
//#include "../RocketEngine/Scene.h"
//#include "../RocketEngine/Transform.h"
//#include "../RocketEngine/Light.h"
#include "../RocketEngine/RocketAPI.h"
#include "../RocketEngine/RocketCoreAPI.h"

#include "../RocketEngine/DirectionalLight.h"
#include "../RocketEngine/AudioClip.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\BUILDTEST\\RocketEngine.lib")
//#define ENGINE_PATH L"..\\x64\\Debug\\RocketEngine.dll"
#else
#pragma comment(lib,"..\\x64\\Release\\RocketEngine.lib")
//#define ENGINE_PATH L"Engine\\RocketEngine.dll" 
#endif // _DEBUG

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>

namespace RocketEngine
{
	class Light;
}

namespace SAD
{
	std::string Serialize::sceneName;
	std::vector<DataStruct> Serialize::serializeData;
	std::string Serialize::filepath = "";

	Serialize::Serialize()
	{

	}

	Serialize::~Serialize()
	{

	}

	void Serialize::SerializeSceneByName(const RocketEngine::Scene& scenedata)
	{
		Document doc;
		doc.SetArray();

		// 씬에 관한 데이터
		Value sceneValue(kObjectType);

		// 씬 이름
		Value sceneName;
		sceneName.SetString(scenedata._sceneName.c_str(), strlen(scenedata._sceneName.c_str()), doc.GetAllocator());
		sceneValue.AddMember(StringRef("__scene_name__"), sceneName, doc.GetAllocator());

		doc.PushBack(sceneValue, doc.GetAllocator());

		for (const auto& i : scenedata._originalList)
		{
			if (i->transform.GetParent() == nullptr)
			{
				Value objValue(kObjectType);

				Value nameValue;
				nameValue.SetString(i->objName.c_str(), strlen(i->objName.c_str()), doc.GetAllocator());
				objValue.AddMember(StringRef("__object_name__"), nameValue, doc.GetAllocator());

				//std::unordered_map<std::string, RocketCore::IComponent*> components = i->GetAllComponents();
				std::unordered_map<std::string, std::vector< RocketCore::IComponent*>>& components = i->GetAllComponents();
				for (auto& c : components)
				{
					SerializeComponents(i, c, doc, objValue);
				}
				doc.PushBack(objValue, doc.GetAllocator());
			}
		}

		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		doc.Accept(writer);


		// 여기서부터 파일을 생성하기 위한 코드
		std::string wordAdd = "../SceneData/";
		std::string wordAdd2 = ".json";

		filepath = wordAdd + scenedata._sceneName.c_str() + wordAdd2;
		std::ofstream outputFile(filepath);


		if (outputFile.is_open())
		{
			outputFile << buffer.GetString();
			outputFile.close();
		}
		else
		{
			assert(outputFile.is_open());
		}

	}

	void Serialize::SerializeSceneByObjList(const std::vector<RocketEngine::GameObject*> objs)
	{
		Document doc;
		doc.SetArray();

		// 씬에 관한 데이터
		Value sceneValue(kObjectType);

		for (const auto& i : objs)
		{
			if (i->transform.GetParent() == nullptr)
			{
				if (i->objName == "EditorCamera")
				{
					continue;
				}

				Value objValue(kObjectType);

				Value nameValue;
				nameValue.SetString(i->objName.c_str(), strlen(i->objName.c_str()), doc.GetAllocator());
				objValue.AddMember(StringRef("__object_name__"), nameValue, doc.GetAllocator());

				std::unordered_map<std::string, std::vector<RocketCore::IComponent*>>& components = i->GetAllComponents();
				for (auto& c : components)
				{
					SerializeComponents(i, c, doc, objValue);
				}
				doc.PushBack(objValue, doc.GetAllocator());
			}
		}

		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		doc.Accept(writer);


		// 여기서부터 파일을 생성하기 위한 코드
		std::string wordAdd = "../SceneData/";
		std::string wordAdd2 = ".json";

		std::ofstream outputFile(filepath);

		if (outputFile.is_open())
		{
			outputFile << buffer.GetString();
			outputFile.close();
		}
		else
		{
			assert(outputFile.is_open());
		}
	}

	void Serialize::SerializeComponents(RocketEngine::GameObject* obj, const std::pair<std::string, std::vector<RocketCore::IComponent*>>& comp, Document& doc, Value& objValue)
	{
		auto& componentName = comp.first;

		if (componentName == "class RocketEngine::MeshRenderer")
		{
			MeshRendererSerialization(comp, doc, objValue);
		}
		else if (componentName == "class RocketEngine::Transform")
		{
			TransformSerialization(*obj, doc, objValue);
		}
		/*else if (componentName == "class RocketEngine::SkinnedMeshRenderer")
		{
			SkinnedMeshRendererSerialization(comp, doc, objValue);
		}*/
		else if (componentName == "class RocketEngine::TextBox")
		{
			TextBoxSerialization(comp, doc, objValue);
		}
		else if (componentName == "class RocketEngine::SpriteRenderer")
		{
			SpriteRendererSerialization(comp, doc, objValue);
		}
		else if (componentName == "class RocketEngine::BoxCollider")
		{
			BoxColliderSerialization(comp, doc, objValue);
		}
		else if (componentName == "class RocketEngine::CapsuleCollider")
		{
			CapsuleColliderSerialization(comp, doc, objValue);
		}
		else if (componentName == "class RocketEngine::SphereCollider")
		{
			SphereColliderSerialization(comp, doc, objValue);
		}
		else if (componentName == "class RocketEngine::PlaneCollider")
		{
			PlaneColliderSerialization(comp, doc, objValue);
		}
		else if (componentName == "class RocketEngine::StaticBoxCollider")
		{
			StaticBoxColliderSerialization(comp, doc, objValue);
		}
		/*else if (comp.first == "class RocketEngine::AnimationController")
		{
			AnimationControllerSerialization(comp, doc, objValue);
		}*/
		else if (componentName == "class RocketEngine::Button")
		{
			ButtonSerialization(comp, doc, objValue);
		}
		/*else if (comp.first == "class RocketEngine::AudioClip")
		{
			AudioClipSerialization(comp, doc, objValue);
		}*/
		else if (componentName == "class RocketEngine::Camera")
		{
			CameraSerialization(comp, doc, objValue);
		}
		else if (componentName == "class RocketEngine::DirectionalLight")
		{
			DirectionalLightSerialization(comp, doc, objValue);
		}
		else if (componentName == "class RocketEngine::PointLight")
		{
			PointLightSerialization(comp, doc, objValue);
		}
		else if (componentName == "class RocketEngine::SpotLight")
		{
			SpotLightSerialization(comp, doc, objValue);
		}
		else if (componentName == "class RocketEngine::Raycast")
		{
			Value error;
			error.SetString(comp.first.c_str(), strlen(comp.first.c_str()), doc.GetAllocator());
			objValue.AddMember(StringRef("null"), error, doc.GetAllocator());
		}
		else
		{
			Value error;
			error.SetString(comp.first.c_str(), strlen(comp.first.c_str()), doc.GetAllocator());
			objValue.AddMember(StringRef("error type"), error, doc.GetAllocator());
		}
	}

	void Serialize::TransformSerialization(const RocketEngine::GameObject& data, Document& doc, Value& val)
	{
		Value transformComponent(kObjectType);

		Value componentName;
		componentName.SetString("Transform", strlen("Transform"), doc.GetAllocator());
		transformComponent.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

		// Serialize position
		Value positionValue(kObjectType);
		positionValue.AddMember("x", data.transform.GetLocalPosition().x, doc.GetAllocator());
		positionValue.AddMember("y", data.transform.GetLocalPosition().y, doc.GetAllocator());
		positionValue.AddMember("z", data.transform.GetLocalPosition().z, doc.GetAllocator());
		transformComponent.AddMember("position", positionValue, doc.GetAllocator());

		// Serialize rotation
		Value rotationValue(kObjectType);
		rotationValue.AddMember("w", data.transform.GetLocalRotation().w, doc.GetAllocator());
		rotationValue.AddMember("x", data.transform.GetLocalRotation().x, doc.GetAllocator());
		rotationValue.AddMember("y", data.transform.GetLocalRotation().y, doc.GetAllocator());
		rotationValue.AddMember("z", data.transform.GetLocalRotation().z, doc.GetAllocator());
		transformComponent.AddMember("rotation", rotationValue, doc.GetAllocator());

		// Serialize scale
		Value scaleValue(kObjectType);
		scaleValue.AddMember("x", data.transform.GetLocalScale().x, doc.GetAllocator());
		scaleValue.AddMember("y", data.transform.GetLocalScale().y, doc.GetAllocator());
		scaleValue.AddMember("z", data.transform.GetLocalScale().z, doc.GetAllocator());
		transformComponent.AddMember("scale", scaleValue, doc.GetAllocator());


		if (!data.transform.GetChildrenVec().empty())
		{
			for (auto i : data.transform.GetChildrenVec())
			{
				// 부모 오브젝트에 자식 이름을 넣고
				Value childValue;
				childValue.SetString(i->gameObject->objName.c_str(), strlen(i->gameObject->objName.c_str()), doc.GetAllocator());
				val.AddMember(StringRef("__child_name__"), childValue, doc.GetAllocator());

				Value objValue(kObjectType);

				Value nameValue;
				nameValue.SetString(i->gameObject->objName.c_str(), strlen(i->gameObject->objName.c_str()), doc.GetAllocator());
				objValue.AddMember(StringRef("__object_name__"), nameValue, doc.GetAllocator());

				Value parentValue;
				parentValue.SetString(i->GetParent()->gameObject->objName.c_str(), strlen(i->GetParent()->gameObject->objName.c_str()), doc.GetAllocator());
				objValue.AddMember(StringRef("__parent_name__"), parentValue, doc.GetAllocator());

				std::unordered_map<std::string, std::vector<RocketCore::IComponent*>>& components = i->gameObject->GetAllComponents();
				for (auto& c : components)
				{
					SerializeComponents(i->gameObject, c, doc, objValue);
				}
				doc.PushBack(objValue, doc.GetAllocator());
			}
		}

		val.AddMember(StringRef("Transform"), transformComponent, doc.GetAllocator());
	}

	void Serialize::MeshRendererSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	{


		for (const auto& element : data.second)
		{
			Value componentValue(kObjectType);

			Value componentName;
			componentName.SetString("MeshRenderer", strlen("MeshRenderer"), doc.GetAllocator());
			componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

			RocketEngine::MeshRenderer* comp = dynamic_cast<RocketEngine::MeshRenderer*>(element);

			componentName.SetString(comp->GetMeshPath().c_str(), strlen(comp->GetMeshPath().c_str()), doc.GetAllocator());
			componentValue.AddMember(StringRef("Mesh Path"), componentName, doc.GetAllocator());

			componentName.SetString(comp->GetShaderPath().c_str(), strlen(comp->GetShaderPath().c_str()), doc.GetAllocator());
			componentValue.AddMember(StringRef("Shader Path"), componentName, doc.GetAllocator());

			componentName.SetString(comp->GetDiffusePath().c_str(), strlen(comp->GetDiffusePath().c_str()), doc.GetAllocator());
			componentValue.AddMember(StringRef("Diffuse Path"), componentName, doc.GetAllocator());

			componentName.SetString(comp->GetNormalPath().c_str(), strlen(comp->GetNormalPath().c_str()), doc.GetAllocator());
			componentValue.AddMember(StringRef("Normal Path"), componentName, doc.GetAllocator());

			// renderType
			componentName.SetInt((int)comp->GetRenderType());
			componentValue.AddMember(StringRef("Render Type"), componentName, doc.GetAllocator());



			/// 렌더 타입에 따라 레거시, PBR 파싱을 분류한다

			// 포워드, 퐁 렌더링
			if ((int)comp->GetRenderType() == 0 || (int)comp->GetRenderType() == 1)
			{
				// ambient
				componentName.SetFloat(comp->GetLegacyAmbient().r);
				componentValue.AddMember(StringRef("ambient_r"), componentName, doc.GetAllocator());
				componentName.SetFloat(comp->GetLegacyAmbient().g);
				componentValue.AddMember(StringRef("ambient_g"), componentName, doc.GetAllocator());
				componentName.SetFloat(comp->GetLegacyAmbient().b);
				componentValue.AddMember(StringRef("ambient_b"), componentName, doc.GetAllocator());
				componentName.SetFloat(comp->GetLegacyAmbient().a);
				componentValue.AddMember(StringRef("ambient_a"), componentName, doc.GetAllocator());

				//diffuse
				componentName.SetFloat(comp->GetLegacyDiffuse().r);
				componentValue.AddMember(StringRef("diffuse_r"), componentName, doc.GetAllocator());
				componentName.SetFloat(comp->GetLegacyDiffuse().g);
				componentValue.AddMember(StringRef("diffuse_g"), componentName, doc.GetAllocator());
				componentName.SetFloat(comp->GetLegacyDiffuse().b);
				componentValue.AddMember(StringRef("diffuse_b"), componentName, doc.GetAllocator());
				componentName.SetFloat(comp->GetLegacyDiffuse().a);
				componentValue.AddMember(StringRef("diffuse_a"), componentName, doc.GetAllocator());

				//specular
				componentName.SetFloat(comp->GetLegacySpecular().r);
				componentValue.AddMember(StringRef("specular_r"), componentName, doc.GetAllocator());
				componentName.SetFloat(comp->GetLegacySpecular().g);
				componentValue.AddMember(StringRef("specular_g"), componentName, doc.GetAllocator());
				componentName.SetFloat(comp->GetLegacySpecular().b);
				componentValue.AddMember(StringRef("specular_b"), componentName, doc.GetAllocator());
				componentName.SetFloat(comp->GetLegacySpecular().a);
				componentValue.AddMember(StringRef("specular_a"), componentName, doc.GetAllocator());

				// reflect
				componentName.SetFloat(comp->GetLegacyReflect().r);
				componentValue.AddMember(StringRef("reflect_r"), componentName, doc.GetAllocator());
				componentName.SetFloat(comp->GetLegacyReflect().g);
				componentValue.AddMember(StringRef("reflect_g"), componentName, doc.GetAllocator());
				componentName.SetFloat(comp->GetLegacyReflect().b);
				componentValue.AddMember(StringRef("reflect_b"), componentName, doc.GetAllocator());
				componentName.SetFloat(comp->GetLegacyReflect().a);
				componentValue.AddMember(StringRef("reflect_a"), componentName, doc.GetAllocator());
			}

			// PBR
			if ((int)comp->GetRenderType() == 2)
			{
				// pbr_albedo
				componentName.SetFloat(comp->GetPBRAlbedo().r);
				componentValue.AddMember(StringRef("pbr_albedo_r"), componentName, doc.GetAllocator());
				componentName.SetFloat(comp->GetPBRAlbedo().g);
				componentValue.AddMember(StringRef("pbr_albedo_g"), componentName, doc.GetAllocator());
				componentName.SetFloat(comp->GetPBRAlbedo().b);
				componentValue.AddMember(StringRef("pbr_albedo_b"), componentName, doc.GetAllocator());
				componentName.SetFloat(comp->GetPBRAlbedo().a);
				componentValue.AddMember(StringRef("pbr_albedo_a"), componentName, doc.GetAllocator());

				// pbr_metalic
				componentName.SetFloat(comp->GetPBRMetallic());
				componentValue.AddMember(StringRef("pbr_metalic"), componentName, doc.GetAllocator());

				// pbr_roughness
				componentName.SetFloat(comp->GetPBRRoughness());
				componentValue.AddMember(StringRef("pbr_roughness"), componentName, doc.GetAllocator());

				// pbr_specular
				componentName.SetFloat(comp->GetPBRSpecular());
				componentValue.AddMember(StringRef("pbr_specular"), componentName, doc.GetAllocator());

				//pbr_emission
				componentName.SetFloat(comp->GetPBREmission());
				componentValue.AddMember(StringRef("pbr_emission"), componentName, doc.GetAllocator());

				// pbr_arm
				componentName.SetString(comp->GetARMPath().c_str(), strlen(comp->GetARMPath().c_str()), doc.GetAllocator());
				componentValue.AddMember(StringRef("pbr_arm"), componentName, doc.GetAllocator());
			}

			val.AddMember("Mesh Renderer", componentValue, doc.GetAllocator());
		}
	}

	//void Serialize::SkinnedMeshRendererSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	//{
	//	for (const auto& element : data.second)
	//	{
	//		Value componentValue(kObjectType);

	//		Value componentName;
	//		componentName.SetString("SkinnedMeshRenderer", strlen("SkinnedMeshRenderer"), doc.GetAllocator());
	//		componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

	//		RocketEngine::SkinnedMeshRenderer* comp = dynamic_cast<RocketEngine::SkinnedMeshRenderer*>(element);

	//		componentName.SetString(comp->GetMeshPath().c_str(), strlen(comp->GetMeshPath().c_str()), doc.GetAllocator());
	//		componentValue.AddMember(StringRef("Mesh Path"), componentName, doc.GetAllocator());

	//		componentName.SetString(comp->GetShaderPath().c_str(), strlen(comp->GetShaderPath().c_str()), doc.GetAllocator());
	//		componentValue.AddMember(StringRef("Shader Path"), componentName, doc.GetAllocator());

	//		componentName.SetString(comp->GetDiffusePath().c_str(), strlen(comp->GetDiffusePath().c_str()), doc.GetAllocator());
	//		componentValue.AddMember(StringRef("Diffuse Path"), componentName, doc.GetAllocator());

	//		componentName.SetString(comp->GetNormalPath().c_str(), strlen(comp->GetNormalPath().c_str()), doc.GetAllocator());
	//		componentValue.AddMember(StringRef("Normal Path"), componentName, doc.GetAllocator());

	//		// ambient
	//		componentName.SetFloat(comp->GetLegacyAmbient().r);
	//		componentValue.AddMember(StringRef("ambient_r"), componentName, doc.GetAllocator());
	//		componentName.SetFloat(comp->GetLegacyAmbient().g);
	//		componentValue.AddMember(StringRef("ambient_g"), componentName, doc.GetAllocator());
	//		componentName.SetFloat(comp->GetLegacyAmbient().b);
	//		componentValue.AddMember(StringRef("ambient_b"), componentName, doc.GetAllocator());
	//		componentName.SetFloat(comp->GetLegacyAmbient().a);
	//		componentValue.AddMember(StringRef("ambient_a"), componentName, doc.GetAllocator());

	//		//diffuse
	//		componentName.SetFloat(comp->GetLegacyDiffuse().r);
	//		componentValue.AddMember(StringRef("diffuse_r"), componentName, doc.GetAllocator());
	//		componentName.SetFloat(comp->GetLegacyDiffuse().g);
	//		componentValue.AddMember(StringRef("diffuse_g"), componentName, doc.GetAllocator());
	//		componentName.SetFloat(comp->GetLegacyDiffuse().b);
	//		componentValue.AddMember(StringRef("diffuse_b"), componentName, doc.GetAllocator());
	//		componentName.SetFloat(comp->GetLegacyDiffuse().a);
	//		componentValue.AddMember(StringRef("diffuse_a"), componentName, doc.GetAllocator());

	//		//specular
	//		componentName.SetFloat(comp->GetLegacySpecular().r);
	//		componentValue.AddMember(StringRef("specular_r"), componentName, doc.GetAllocator());
	//		componentName.SetFloat(comp->GetLegacySpecular().g);
	//		componentValue.AddMember(StringRef("specular_g"), componentName, doc.GetAllocator());
	//		componentName.SetFloat(comp->GetLegacySpecular().b);
	//		componentValue.AddMember(StringRef("specular_b"), componentName, doc.GetAllocator());
	//		componentName.SetFloat(comp->GetLegacySpecular().a);
	//		componentValue.AddMember(StringRef("specular_a"), componentName, doc.GetAllocator());

	//		// reflect
	//		componentName.SetFloat(comp->GetLegacyReflect().r);
	//		componentValue.AddMember(StringRef("reflect_r"), componentName, doc.GetAllocator());
	//		componentName.SetFloat(comp->GetLegacyReflect().g);
	//		componentValue.AddMember(StringRef("reflect_g"), componentName, doc.GetAllocator());
	//		componentName.SetFloat(comp->GetLegacyReflect().b);
	//		componentValue.AddMember(StringRef("reflect_b"), componentName, doc.GetAllocator());
	//		componentName.SetFloat(comp->GetLegacyReflect().a);
	//		componentValue.AddMember(StringRef("reflect_a"), componentName, doc.GetAllocator());

	//		// pbr_albedo
	//		componentName.SetFloat(comp->GetPBRAlbedo().r);
	//		componentValue.AddMember(StringRef("pbr_albedo_r"), componentName, doc.GetAllocator());
	//		componentName.SetFloat(comp->GetPBRAlbedo().g);
	//		componentValue.AddMember(StringRef("pbr_albedo_g"), componentName, doc.GetAllocator());
	//		componentName.SetFloat(comp->GetPBRAlbedo().b);
	//		componentValue.AddMember(StringRef("pbr_albedo_b"), componentName, doc.GetAllocator());
	//		componentName.SetFloat(comp->GetPBRAlbedo().a);
	//		componentValue.AddMember(StringRef("pbr_albedo_a"), componentName, doc.GetAllocator());

	//		// pbr_metalic
	//		componentName.SetFloat(comp->GetPBRMetallic());
	//		componentValue.AddMember(StringRef("pbr_metalic"), componentName, doc.GetAllocator());

	//		// pbr_roughness
	//		componentName.SetFloat(comp->GetPBRRoughness());
	//		componentValue.AddMember(StringRef("pbr_roughness"), componentName, doc.GetAllocator());

	//		// pbr_specular
	//		componentName.SetFloat(comp->GetPBRSpecular());
	//		componentValue.AddMember(StringRef("pbr_specular"), componentName, doc.GetAllocator());

	//		//pbr_emission
	//		componentName.SetFloat(comp->GetPBREmission());
	//		componentValue.AddMember(StringRef("pbr_emission"), componentName, doc.GetAllocator());

	//		val.AddMember("Skinned Mesh Renderer", componentValue, doc.GetAllocator());

	//		val.AddMember("Skinned Mesh Renderer", componentValue, doc.GetAllocator());
	//	}

	//}

	void Serialize::SpriteRendererSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	{
		for (const auto& element : data.second)
		{
			Value componentValue(kObjectType);

			Value componentName;
			componentName.SetString("SpriteRenderer", strlen("SpriteRenderer"), doc.GetAllocator());
			componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

			RocketEngine::SpriteRenderer* comp = dynamic_cast<RocketEngine::SpriteRenderer*>(element);

			componentName.SetString(comp->GetPath().c_str(), strlen(comp->GetPath().c_str()), doc.GetAllocator());
			componentValue.AddMember(StringRef("Sprite Path"), componentName, doc.GetAllocator());

			componentValue.AddMember("Width", comp->GetWidth(), doc.GetAllocator());
			componentValue.AddMember("Height", comp->GetHeight(), doc.GetAllocator());
			componentValue.AddMember("sort order", comp->GetSortOrder(), doc.GetAllocator());

			val.AddMember("Sprite Renderer", componentValue, doc.GetAllocator());
		}

	}

	//void Serialize::AnimationControllerSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	//{


	//	Value componentValue(kObjectType);

	//	Value componentName;
	//	componentName.SetString("AnimationController", strlen("AnimationController"), doc.GetAllocator());
	//	componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

	//	RocketEngine::AnimationController* comp = dynamic_cast<RocketEngine::AnimationController*>(data.second);

	//	for (const auto& s : comp->GetAnimator()->GetAllStates())
	//	{
	//		Value state(kObjectType);w

	//		Value stateName;
	//		stateName.SetString(s.first.c_str(), strlen(s.first.c_str()), doc.GetAllocator()); // 변수명 변경
	//		state.AddMember(StringRef("State Name"), stateName, doc.GetAllocator()); // 변수명 변경

	//		Value animDescName;
	//		animDescName.SetString(s.second->GetAnimDesc().name.c_str(), strlen(s.second->GetAnimDesc().name.c_str()), doc.GetAllocator()); // 변수명 변경
	//		state.AddMember(StringRef("AnimationDesc Name"), animDescName, doc.GetAllocator()); // 변수명 변경

	//		Value animPath;
	//		animPath.SetString(s.second->GetAnimDesc().animPath.c_str(), strlen(s.second->GetAnimDesc().animPath.c_str()), doc.GetAllocator()); // 변수명 변경
	//		state.AddMember(StringRef("Animation Path"), animPath, doc.GetAllocator()); // 변수명 변경

	//		Value ownerModelPath;
	//		ownerModelPath.SetString(s.second->GetAnimDesc().ownerModelPath.c_str(), strlen(s.second->GetAnimDesc().ownerModelPath.c_str()), doc.GetAllocator()); // 변수명 변경
	//		state.AddMember(StringRef("This Model Path"), ownerModelPath, doc.GetAllocator()); // 변수명 변경

	//		state.AddMember(StringRef("Is Animation Loop"), s.second->GetIsLoop(), doc.GetAllocator());

	//		componentValue.AddMember("State", state, doc.GetAllocator());
	//	}

		/*for (const auto& con : comp->GetAnimator()->GetAllCondition())
		{
			Value condition(kObjectType);

			Value conditionName;
			conditionName.SetString(con.first.c_str(), strlen(con.first.c_str()), doc.GetAllocator());
			condition.AddMember(StringRef("Condition Name"), conditionName, doc.GetAllocator());

			Value conditionBool;
			conditionBool.SetBool(con.second);
			condition.AddMember(StringRef("is Condition On"), conditionBool, doc.GetAllocator());

			componentValue.AddMember("Condition", condition, doc.GetAllocator());
		}

		val.AddMember("Animation Controller", componentValue, doc.GetAllocator());
	}*/

	void Serialize::TextBoxSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	{
		for (const auto& element : data.second)
		{
			Value componentValue(kObjectType);

			Value componentName;
			componentName.SetString("TextBox", strlen("TextBox"), doc.GetAllocator());
			componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

			RocketEngine::TextBox* comp = dynamic_cast<RocketEngine::TextBox*>(element);

			componentName.SetString(comp->GetText().c_str(), strlen(comp->GetText().c_str()), doc.GetAllocator());
			componentValue.AddMember(StringRef("text"), componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetFontSize());
			componentValue.AddMember("font size", componentName, doc.GetAllocator());

			{
				Value color(kObjectType);
				color.AddMember("r", comp->GetColor().r, doc.GetAllocator());
				color.AddMember("g", comp->GetColor().g, doc.GetAllocator());
				color.AddMember("b", comp->GetColor().b, doc.GetAllocator());
				color.AddMember("a", comp->GetColor().a, doc.GetAllocator());

				componentValue.AddMember("Color", color, doc.GetAllocator());
			}

			componentValue.AddMember("Width", comp->GetWidth(), doc.GetAllocator());
			componentValue.AddMember("Height", comp->GetHeight(), doc.GetAllocator());
			componentValue.AddMember("sort order", comp->GetSortOrder(), doc.GetAllocator());

			val.AddMember("Text Box", componentValue, doc.GetAllocator());
		}

	}

	void Serialize::CapsuleColliderSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	{
		for (const auto& element : data.second)
		{
			Value componentValue(kObjectType);

			Value componentName;
			componentName.SetString("CapsuleCollider", strlen("CapsuleCollider"), doc.GetAllocator());
			componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

			RocketEngine::CapsuleCollider* comp = dynamic_cast<RocketEngine::CapsuleCollider*>(element);

			componentName.SetFloat(comp->GetRadius());
			componentValue.AddMember("radius", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetHalfHeight());
			componentValue.AddMember("half height", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetPositionOffset().x);
			componentValue.AddMember("Position offset_x", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetPositionOffset().y);
			componentValue.AddMember("Position offset_y", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetPositionOffset().z);
			componentValue.AddMember("Position offset_z", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetRotationOffset().w);
			componentValue.AddMember("Rotation offset_w", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetRotationOffset().x);
			componentValue.AddMember("Rotation offset_x", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetRotationOffset().y);
			componentValue.AddMember("Rotation offset_y", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetRotationOffset().z);
			componentValue.AddMember("Rotation offset_z", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetScaleOffset().x);
			componentValue.AddMember("Scale offset_x", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetScaleOffset().y);
			componentValue.AddMember("Scale offset_y", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetScaleOffset().z);
			componentValue.AddMember("Scale offset_z", componentName, doc.GetAllocator());

			std::string test = comp->gameObject->objName;

			if (!comp->GetServants().empty())
			{
				for (auto& serv : comp->GetServants())
				{
					componentName.SetString(serv->gameObject->objName.c_str(), strlen(serv->gameObject->objName.c_str()), doc.GetAllocator());
					componentValue.AddMember("servant", componentName, doc.GetAllocator());
				}
			}

			val.AddMember("Capsule Collider", componentValue, doc.GetAllocator());
		}

	}

	void Serialize::BoxColliderSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	{
		for (const auto& element : data.second)
		{
			Value componentValue(kObjectType);

			Value componentName;
			componentName.SetString("BoxCollider", strlen("BoxCollider"), doc.GetAllocator());
			componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

			RocketEngine::BoxCollider* comp = dynamic_cast<RocketEngine::BoxCollider*>(element);

			componentName.SetFloat(comp->GetWidth());
			componentValue.AddMember("width", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetHeight());
			componentValue.AddMember("height", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetDepth());
			componentValue.AddMember("depth", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetPositionOffset().x);
			componentValue.AddMember("Position offset_x", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetPositionOffset().y);
			componentValue.AddMember("Position offset_y", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetPositionOffset().z);
			componentValue.AddMember("Position offset_z", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetRotationOffset().w);
			componentValue.AddMember("Rotation offset_w", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetRotationOffset().x);
			componentValue.AddMember("Rotation offset_x", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetRotationOffset().y);
			componentValue.AddMember("Rotation offset_y", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetRotationOffset().z);
			componentValue.AddMember("Rotation offset_z", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetScaleOffset().x);
			componentValue.AddMember("Scale offset_x", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetScaleOffset().y);
			componentValue.AddMember("Scale offset_y", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetScaleOffset().z);
			componentValue.AddMember("Scale offset_z", componentName, doc.GetAllocator());


			if (!comp->GetServants().empty())
			{
				for (auto& serv : comp->GetServants())
				{
					componentName.SetString(serv->gameObject->objName.c_str(), strlen(serv->gameObject->objName.c_str()), doc.GetAllocator());
					componentValue.AddMember("servant", componentName, doc.GetAllocator());
				}
			}


			val.AddMember("Box Collider", componentValue, doc.GetAllocator());
		}
	}

	void Serialize::SphereColliderSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	{
		for (const auto& element : data.second)
		{
			Value componentValue(kObjectType);

			Value componentName;
			componentName.SetString("SphereCollider", strlen("SphereCollider"), doc.GetAllocator());
			componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

			RocketEngine::SphereCollider* comp = dynamic_cast<RocketEngine::SphereCollider*>(element);

			componentName.SetFloat(comp->GetRadius());
			componentValue.AddMember("radius", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetPositionOffset().x);
			componentValue.AddMember("Position offset_x", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetPositionOffset().y);
			componentValue.AddMember("Position offset_y", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetPositionOffset().z);
			componentValue.AddMember("Position offset_z", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetRotationOffset().w);
			componentValue.AddMember("Rotation offset_w", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetRotationOffset().x);
			componentValue.AddMember("Rotation offset_x", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetRotationOffset().y);
			componentValue.AddMember("Rotation offset_y", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetRotationOffset().z);
			componentValue.AddMember("Rotation offset_z", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetScaleOffset().x);
			componentValue.AddMember("Scale offset_x", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetScaleOffset().y);
			componentValue.AddMember("Scale offset_y", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetScaleOffset().z);
			componentValue.AddMember("Scale offset_z", componentName, doc.GetAllocator());


			if (!comp->GetServants().empty())
			{
				for (auto& serv : comp->GetServants())
				{
					componentName.SetString(serv->gameObject->objName.c_str(), strlen(serv->gameObject->objName.c_str()), doc.GetAllocator());
					componentValue.AddMember("servant", componentName, doc.GetAllocator());
				}
			}

			val.AddMember("Sphere Collider", componentValue, doc.GetAllocator());
		}

	}

	void Serialize::PlaneColliderSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	{
		for (const auto& element : data.second)
		{

			Value componentValue(kObjectType);

			Value componentName;
			componentName.SetString("PlaneCollider", strlen("PlaneCollider"), doc.GetAllocator());
			componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

			RocketEngine::PlaneCollider* comp = dynamic_cast<RocketEngine::PlaneCollider*>(element);

			componentName.SetFloat(comp->GetPositionOffset().x);
			componentValue.AddMember("Position offset_x", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetPositionOffset().y);
			componentValue.AddMember("Position offset_y", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetPositionOffset().z);
			componentValue.AddMember("Position offset_z", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetRotationOffset().w);
			componentValue.AddMember("Rotation offset_w", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetRotationOffset().x);
			componentValue.AddMember("Rotation offset_x", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetRotationOffset().y);
			componentValue.AddMember("Rotation offset_y", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetRotationOffset().z);
			componentValue.AddMember("Rotation offset_z", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetScaleOffset().x);
			componentValue.AddMember("Scale offset_x", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetScaleOffset().y);
			componentValue.AddMember("Scale offset_y", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetScaleOffset().z);
			componentValue.AddMember("Scale offset_z", componentName, doc.GetAllocator());

			val.AddMember("Plane Collider", componentValue, doc.GetAllocator());
		}
	}

	void Serialize::StaticBoxColliderSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	{
		Value staticBoxGroup(kObjectType);

		for (const auto& element : data.second)
		{
			Value componentValue(kObjectType);

			Value componentName;
			componentName.SetString("StaticBoxCollider", strlen("StaticBoxCollider"), doc.GetAllocator());
			componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

			RocketEngine::StaticBoxCollider* comp = dynamic_cast<RocketEngine::StaticBoxCollider*>(element);

			componentName.SetFloat(comp->GetWidth());
			componentValue.AddMember("width", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetHeight());
			componentValue.AddMember("height", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetDepth());
			componentValue.AddMember("depth", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetPositionOffset().x);
			componentValue.AddMember("Position offset_x", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetPositionOffset().y);
			componentValue.AddMember("Position offset_y", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetPositionOffset().z);
			componentValue.AddMember("Position offset_z", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetRotationOffset().w);
			componentValue.AddMember("Rotation offset_w", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetRotationOffset().x);
			componentValue.AddMember("Rotation offset_x", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetRotationOffset().y);
			componentValue.AddMember("Rotation offset_y", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetRotationOffset().z);
			componentValue.AddMember("Rotation offset_z", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetScaleOffset().x);
			componentValue.AddMember("Scale offset_x", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetScaleOffset().y);
			componentValue.AddMember("Scale offset_y", componentName, doc.GetAllocator());
			componentName.SetFloat(comp->GetScaleOffset().z);
			componentValue.AddMember("Scale offset_z", componentName, doc.GetAllocator());

			staticBoxGroup.AddMember("One Collider", componentValue, doc.GetAllocator());

		}
		val.AddMember("Static Box Collider", staticBoxGroup, doc.GetAllocator());
	}

	void Serialize::ButtonSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	{
		for (const auto& element : data.second)
		{
			Value componentValue(kObjectType);

			Value componentName;
			componentName.SetString("Button", strlen("Button"), doc.GetAllocator());
			componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

			RocketEngine::Button* comp = dynamic_cast<RocketEngine::Button*>(element);

			componentName.SetFloat(comp->GetWidth());
			componentValue.AddMember("width", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetHeight());
			componentValue.AddMember("height", componentName, doc.GetAllocator());

			// 함수를 가져와야 하나? 어떻게?
			//componentName.SetString(comp->GetOnClickEvent())

			val.AddMember("Button", componentValue, doc.GetAllocator());
		}
	}

	/*void Serialize::AudioClipSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	{
		Value componentValue(kObjectType);

		Value componentName;
		componentName.SetString("AudioClip", strlen("AudioClip"), doc.GetAllocator());
		componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

		RocketEngine::AudioClip* comp = dynamic_cast<RocketEngine::AudioClip*>(data.second);

		auto& soundMap = RocketCore::GetSoundMapFromSoundSystem();


		for (const auto& audios : comp->GetAudios())
		{

			Value audio(kObjectType);

			Value audioName;
			audioName.SetString(audios.first.c_str(), strlen(audios.first.c_str()), doc.GetAllocator());
			audio.AddMember(StringRef("Audio name"), audioName, doc.GetAllocator());

			Value audioPath;
			audioName.SetString(audios.second.c_str(), strlen(audios.second.c_str()), doc.GetAllocator());
			audio.AddMember(StringRef("Audio path"), audioName, doc.GetAllocator());

			auto thisSound = soundMap.find(audios.second);

			Value is3D;
			is3D.SetBool(thisSound->second.is3DSound);
			audio.AddMember(StringRef("is3D"), is3D, doc.GetAllocator());

			Value soundGroup;
			if (thisSound->second.group == eSoundGroup::BackGoundMusic)
			{
				soundGroup.SetString("BackGround", strlen("BackGround"), doc.GetAllocator());
				audio.AddMember("sound group", soundGroup, doc.GetAllocator());
			}
			else if (thisSound->second.group == eSoundGroup::EffectSound)
			{
				soundGroup.SetString("Effect", strlen("Effect"), doc.GetAllocator());
				audio.AddMember("sound group", soundGroup, doc.GetAllocator());
			}

			componentValue.AddMember("Audio", audio, doc.GetAllocator());
		}

		val.AddMember("Audio Clip", componentValue, doc.GetAllocator());
	}*/

	void Serialize::CameraSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	{
		for (const auto& elements : data.second)
		{
			Value componentValue(kObjectType);

			Value componentName;
			componentName.SetString("Camera", strlen("Camera"), doc.GetAllocator());
			componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

			RocketEngine::Camera* comp = dynamic_cast<RocketEngine::Camera*>(elements);

			// 가까운 평면까지의 거리
			componentName.SetFloat(comp->GetFarZ());
			componentValue.AddMember("far_z", componentName, doc.GetAllocator());

			// 먼 평면까지의 거리
			componentName.SetFloat(comp->GetNearZ());
			componentValue.AddMember("near_z", componentName, doc.GetAllocator());

			// 가로세로 비율 
			componentName.SetFloat(comp->GetAspect());
			componentValue.AddMember("aspect", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetFovY());
			componentValue.AddMember("fov_y", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetNearWindowHeight());
			componentValue.AddMember("near_winHeight", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetNearWindowWidth());
			componentValue.AddMember("near_winWidth", componentName, doc.GetAllocator());
			{
				Value ViewMatrix(kObjectType);

				Value element;
				element.SetFloat(comp->GetViewMatrix()._11);
				ViewMatrix.AddMember("m_11", element, doc.GetAllocator());

				element.SetFloat(comp->GetViewMatrix()._12);
				ViewMatrix.AddMember("m_12", element, doc.GetAllocator());

				element.SetFloat(comp->GetViewMatrix()._13);
				ViewMatrix.AddMember("m_13", element, doc.GetAllocator());

				element.SetFloat(comp->GetViewMatrix()._14);
				ViewMatrix.AddMember("m_14", element, doc.GetAllocator());

				element.SetFloat(comp->GetViewMatrix()._21);
				ViewMatrix.AddMember("m_21", element, doc.GetAllocator());

				element.SetFloat(comp->GetViewMatrix()._22);
				ViewMatrix.AddMember("m_22", element, doc.GetAllocator());

				element.SetFloat(comp->GetViewMatrix()._23);
				ViewMatrix.AddMember("m_23", element, doc.GetAllocator());

				element.SetFloat(comp->GetViewMatrix()._24);
				ViewMatrix.AddMember("m_24", element, doc.GetAllocator());

				element.SetFloat(comp->GetViewMatrix()._31);
				ViewMatrix.AddMember("m_31", element, doc.GetAllocator());

				element.SetFloat(comp->GetViewMatrix()._32);
				ViewMatrix.AddMember("m_32", element, doc.GetAllocator());

				element.SetFloat(comp->GetViewMatrix()._33);
				ViewMatrix.AddMember("m_33", element, doc.GetAllocator());

				element.SetFloat(comp->GetViewMatrix()._34);
				ViewMatrix.AddMember("m_34", element, doc.GetAllocator());

				element.SetFloat(comp->GetViewMatrix()._41);
				ViewMatrix.AddMember("m_41", element, doc.GetAllocator());

				element.SetFloat(comp->GetViewMatrix()._42);
				ViewMatrix.AddMember("m_42", element, doc.GetAllocator());

				element.SetFloat(comp->GetViewMatrix()._43);
				ViewMatrix.AddMember("m_43", element, doc.GetAllocator());

				element.SetFloat(comp->GetViewMatrix()._44);
				ViewMatrix.AddMember("m_44", element, doc.GetAllocator());

				componentValue.AddMember("View Matrix", ViewMatrix, doc.GetAllocator());

				Value ProjMatrix(kObjectType);

				element.SetFloat(comp->GetProjMatrix()._11);
				ProjMatrix.AddMember("m_11", element, doc.GetAllocator());

				element.SetFloat(comp->GetProjMatrix()._12);
				ProjMatrix.AddMember("m_12", element, doc.GetAllocator());

				element.SetFloat(comp->GetProjMatrix()._13);
				ProjMatrix.AddMember("m_13", element, doc.GetAllocator());

				element.SetFloat(comp->GetProjMatrix()._14);
				ProjMatrix.AddMember("m_14", element, doc.GetAllocator());

				element.SetFloat(comp->GetProjMatrix()._21);
				ProjMatrix.AddMember("m_21", element, doc.GetAllocator());

				element.SetFloat(comp->GetProjMatrix()._22);
				ProjMatrix.AddMember("m_22", element, doc.GetAllocator());

				element.SetFloat(comp->GetProjMatrix()._23);
				ProjMatrix.AddMember("m_23", element, doc.GetAllocator());

				element.SetFloat(comp->GetProjMatrix()._24);
				ProjMatrix.AddMember("m_24", element, doc.GetAllocator());

				element.SetFloat(comp->GetProjMatrix()._31);
				ProjMatrix.AddMember("m_31", element, doc.GetAllocator());

				element.SetFloat(comp->GetProjMatrix()._32);
				ProjMatrix.AddMember("m_32", element, doc.GetAllocator());

				element.SetFloat(comp->GetProjMatrix()._33);
				ProjMatrix.AddMember("m_33", element, doc.GetAllocator());

				element.SetFloat(comp->GetProjMatrix()._34);
				ProjMatrix.AddMember("m_34", element, doc.GetAllocator());

				element.SetFloat(comp->GetProjMatrix()._41);
				ProjMatrix.AddMember("m_41", element, doc.GetAllocator());

				element.SetFloat(comp->GetProjMatrix()._42);
				ProjMatrix.AddMember("m_42", element, doc.GetAllocator());

				element.SetFloat(comp->GetProjMatrix()._43);
				ProjMatrix.AddMember("m_43", element, doc.GetAllocator());

				element.SetFloat(comp->GetProjMatrix()._44);
				ProjMatrix.AddMember("m_44", element, doc.GetAllocator());

				componentValue.AddMember("Proj Matrix", ProjMatrix, doc.GetAllocator());
			}

			val.AddMember("Camera", componentValue, doc.GetAllocator());
		}
	}

	void Serialize::DirectionalLightSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	{
		for (const auto& element : data.second)
		{
			Value componentValue(kObjectType);

			Value componentName;
			componentName.SetString("DirectionalLight", strlen("DirectionalLight"), doc.GetAllocator());
			componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

			RocketEngine::DirectionalLight* comp = dynamic_cast<RocketEngine::DirectionalLight*>(element);

			componentName.SetFloat(comp->GetIntensity());
			componentValue.AddMember("intensity", componentName, doc.GetAllocator());

			{
				Value ambient(kObjectType);

				Value val;
				val.SetFloat(comp->GetAmbient().r);
				ambient.AddMember("r", val, doc.GetAllocator());
				val.SetFloat(comp->GetAmbient().g);
				ambient.AddMember("g", val, doc.GetAllocator());
				val.SetFloat(comp->GetAmbient().b);
				ambient.AddMember("b", val, doc.GetAllocator());
				val.SetFloat(comp->GetAmbient().a);
				ambient.AddMember("a", val, doc.GetAllocator());

				componentValue.AddMember("ambient", ambient, doc.GetAllocator());
			}

			{
				Value diffuse(kObjectType);

				Value val;
				val.SetFloat(comp->GetDiffuse().r);
				diffuse.AddMember("r", val, doc.GetAllocator());
				val.SetFloat(comp->GetDiffuse().g);
				diffuse.AddMember("g", val, doc.GetAllocator());
				val.SetFloat(comp->GetDiffuse().b);
				diffuse.AddMember("b", val, doc.GetAllocator());
				val.SetFloat(comp->GetDiffuse().a);
				diffuse.AddMember("a", val, doc.GetAllocator());

				componentValue.AddMember("diffuse", diffuse, doc.GetAllocator());
			}

			{
				Value specular(kObjectType);

				Value val;
				val.SetFloat(comp->GetSpecular().r);
				specular.AddMember("r", val, doc.GetAllocator());
				val.SetFloat(comp->GetSpecular().g);
				specular.AddMember("g", val, doc.GetAllocator());
				val.SetFloat(comp->GetSpecular().b);
				specular.AddMember("b", val, doc.GetAllocator());
				val.SetFloat(comp->GetSpecular().a);
				specular.AddMember("a", val, doc.GetAllocator());

				componentValue.AddMember("specular", specular, doc.GetAllocator());
			}

			val.AddMember("Directional Light", componentValue, doc.GetAllocator());
		}
	}

	void Serialize::PointLightSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	{
		for (const auto& element : data.second)
		{
			Value componentValue(kObjectType);

			Value componentName;
			componentName.SetString("PointLight", strlen("PointLight"), doc.GetAllocator());
			componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

			RocketEngine::PointLight* comp = dynamic_cast<RocketEngine::PointLight*>(element);

			componentName.SetFloat(comp->GetRange());
			componentValue.AddMember("range", componentName, doc.GetAllocator());
			{
				Value attenuation(kObjectType);

				Value element;
				element.SetFloat(comp->GetAttenuation().x);
				attenuation.AddMember("attenu_x", componentName, doc.GetAllocator());

				element.SetFloat(comp->GetAttenuation().y);
				attenuation.AddMember("attenu_y", componentName, doc.GetAllocator());

				element.SetFloat(comp->GetAttenuation().z);
				attenuation.AddMember("attenu_z", componentName, doc.GetAllocator());

				componentValue.AddMember("attenuation", attenuation, doc.GetAllocator());
			}

			{
				Value ambient(kObjectType);

				Value val;
				val.SetFloat(comp->GetAmbient().r);
				ambient.AddMember("r", val, doc.GetAllocator());
				val.SetFloat(comp->GetAmbient().g);
				ambient.AddMember("g", val, doc.GetAllocator());
				val.SetFloat(comp->GetAmbient().b);
				ambient.AddMember("b", val, doc.GetAllocator());
				val.SetFloat(comp->GetAmbient().a);
				ambient.AddMember("a", val, doc.GetAllocator());

				componentValue.AddMember("ambient", ambient, doc.GetAllocator());
			}

			{
				Value diffuse(kObjectType);

				Value val;
				val.SetFloat(comp->GetDiffuse().r);
				diffuse.AddMember("r", val, doc.GetAllocator());
				val.SetFloat(comp->GetDiffuse().g);
				diffuse.AddMember("g", val, doc.GetAllocator());
				val.SetFloat(comp->GetDiffuse().b);
				diffuse.AddMember("b", val, doc.GetAllocator());
				val.SetFloat(comp->GetDiffuse().a);
				diffuse.AddMember("a", val, doc.GetAllocator());

				componentValue.AddMember("diffuse", diffuse, doc.GetAllocator());
			}

			{
				Value specular(kObjectType);

				Value val;
				val.SetFloat(comp->GetSpecular().r);
				specular.AddMember("r", val, doc.GetAllocator());
				val.SetFloat(comp->GetSpecular().g);
				specular.AddMember("g", val, doc.GetAllocator());
				val.SetFloat(comp->GetSpecular().b);
				specular.AddMember("b", val, doc.GetAllocator());
				val.SetFloat(comp->GetSpecular().a);
				specular.AddMember("a", val, doc.GetAllocator());

				componentValue.AddMember("specular", specular, doc.GetAllocator());
			}

			val.AddMember("Point Light", componentValue, doc.GetAllocator());
		}
	}

	void Serialize::SpotLightSerialization(const std::pair<std::string, std::vector<RocketCore::IComponent*>>& data, Document& doc, Value& val)
	{
		for (const auto& element : data.second)
		{
			Value componentValue(kObjectType);

			Value componentName;
			componentName.SetString("SpotLight", strlen("SpotLight"), doc.GetAllocator());
			componentValue.AddMember(StringRef("__component_type__"), componentName, doc.GetAllocator());

			RocketEngine::SpotLight* comp = dynamic_cast<RocketEngine::SpotLight*>(element);

			componentName.SetFloat(comp->GetRange());
			componentValue.AddMember("range", componentName, doc.GetAllocator());

			componentName.SetFloat(comp->GetSpot());
			componentValue.AddMember("spot", componentName, doc.GetAllocator());
			{
				Value attenuation(kObjectType);

				Value element;
				element.SetFloat(comp->GetAttenuation().x);
				attenuation.AddMember("attenu_x", componentName, doc.GetAllocator());

				element.SetFloat(comp->GetAttenuation().y);
				attenuation.AddMember("attenu_y", componentName, doc.GetAllocator());

				element.SetFloat(comp->GetAttenuation().z);
				attenuation.AddMember("attenu_z", componentName, doc.GetAllocator());

				componentValue.AddMember("attenuation", attenuation, doc.GetAllocator());
			}

			{
				Value ambient(kObjectType);

				Value val;
				val.SetFloat(comp->GetAmbient().r);
				ambient.AddMember("r", val, doc.GetAllocator());
				val.SetFloat(comp->GetAmbient().g);
				ambient.AddMember("g", val, doc.GetAllocator());
				val.SetFloat(comp->GetAmbient().b);
				ambient.AddMember("b", val, doc.GetAllocator());
				val.SetFloat(comp->GetAmbient().a);
				ambient.AddMember("a", val, doc.GetAllocator());

				componentValue.AddMember("ambient", ambient, doc.GetAllocator());
			}

			{
				Value diffuse(kObjectType);

				Value val;
				val.SetFloat(comp->GetDiffuse().r);
				diffuse.AddMember("r", val, doc.GetAllocator());
				val.SetFloat(comp->GetDiffuse().g);
				diffuse.AddMember("g", val, doc.GetAllocator());
				val.SetFloat(comp->GetDiffuse().b);
				diffuse.AddMember("b", val, doc.GetAllocator());
				val.SetFloat(comp->GetDiffuse().a);
				diffuse.AddMember("a", val, doc.GetAllocator());

				componentValue.AddMember("diffuse", diffuse, doc.GetAllocator());
			}

			{
				Value specular(kObjectType);

				Value val;
				val.SetFloat(comp->GetSpecular().r);
				specular.AddMember("r", val, doc.GetAllocator());
				val.SetFloat(comp->GetSpecular().g);
				specular.AddMember("g", val, doc.GetAllocator());
				val.SetFloat(comp->GetSpecular().b);
				specular.AddMember("b", val, doc.GetAllocator());
				val.SetFloat(comp->GetSpecular().a);
				specular.AddMember("a", val, doc.GetAllocator());

				componentValue.AddMember("specular", specular, doc.GetAllocator());
			}

			val.AddMember("Spot Light", componentValue, doc.GetAllocator());
		}
	}

	void Serialize::GetAllDataFromFile()
	{
		// 모든 값을 string으로 묶어서 '한 번' 가져온다
		std::string serializeDataOnce;
		std::ifstream inputFile(Serialize::filepath);

		if (inputFile.is_open())
		{
			std::stringstream buffer;
			buffer << inputFile.rdbuf();
			serializeDataOnce = buffer.str();
			inputFile.close();
		}

		Document doc;
		doc.Parse(serializeDataOnce.c_str());

		for (rapidjson::SizeType i = 0; i < doc.Size(); ++i)
		{
			const rapidjson::Value& obj = doc[i];

			// 객체 하나를 선언
			DataStruct data;

			// 부모 자식 관계를 확인하는 조건문
			if (obj.HasMember("__object_name__") && obj["__object_name__"].IsString() &&
				obj.HasMember("__parent_name__") && obj["__parent_name__"].IsString() &&
				obj.HasMember("__child_name__") && obj["__child_name__"].IsString())
			{
				data.transform = new transfromData;

				data.transform->name = obj["__object_name__"].GetString();
				data.transform->parent = obj["__parent_name__"].GetString();

				std::string keyword = "__child_name__";

				for (auto c = obj.MemberBegin(); c != obj.MemberEnd(); ++c)
				{
					if (c->name.GetString() == keyword)
					{
						data.transform->child.push_back(c->value.GetString());
					}
				}
			}
			else if (obj.HasMember("__object_name__") && obj["__object_name__"].IsString() &&
				obj.HasMember("__child_name__") && obj["__child_name__"].IsString())
			{
				data.transform = new transfromData;

				data.transform->name = obj["__object_name__"].GetString();

				std::string keyword = "__child_name__";

				for (auto c = obj.MemberBegin(); c != obj.MemberEnd(); ++c)
				{
					if (c->name.GetString() == keyword)
					{
						data.transform->child.push_back(c->value.GetString());
					}
				}
			}
			else if (obj.HasMember("__object_name__") && obj["__object_name__"].IsString() &&
				obj.HasMember("__parent_name__") && obj["__parent_name__"].IsString())
			{
				data.transform = new transfromData;

				data.transform->name = obj["__object_name__"].GetString();
				data.transform->parent = obj["__parent_name__"].GetString();
			}
			else if (obj.HasMember("__object_name__") && obj["__object_name__"].IsString())
			{
				data.transform = new transfromData;

				data.transform->name = obj["__object_name__"].GetString();
			}
			else if (obj.HasMember("__scene_name__"))
			{
				sceneName = obj["__scene_name__"].GetString();
				continue;
			}
			else
			{
				continue;
			}

			// 컴포넌트 유무를 확인하는 조건문
			if (obj.HasMember("Transform"))
			{
				data.transform->name = obj["__object_name__"].GetString();
				data.transform->type = obj["Transform"]["__component_type__"].GetString();

				data.transform->position_x = obj["Transform"]["position"]["x"].GetFloat();
				data.transform->position_y = obj["Transform"]["position"]["y"].GetFloat();
				data.transform->position_z = obj["Transform"]["position"]["z"].GetFloat();

				data.transform->rotation_w = obj["Transform"]["rotation"]["w"].GetFloat();
				data.transform->rotation_x = obj["Transform"]["rotation"]["x"].GetFloat();
				data.transform->rotation_y = obj["Transform"]["rotation"]["y"].GetFloat();
				data.transform->rotation_z = obj["Transform"]["rotation"]["z"].GetFloat();

				data.transform->scale_x = obj["Transform"]["scale"]["x"].GetFloat();
				data.transform->scale_y = obj["Transform"]["scale"]["y"].GetFloat();
				data.transform->scale_z = obj["Transform"]["scale"]["z"].GetFloat();

			}
			if (obj.HasMember("Mesh Renderer"))
			{
				data.meshRenderer = new meshRendererData;

				data.meshRenderer->name = obj["Mesh Renderer"]["__component_type__"].GetString();

				data.meshRenderer->FBXpath = obj["Mesh Renderer"]["Mesh Path"].GetString();
				data.meshRenderer->ShaderPath = obj["Mesh Renderer"]["Shader Path"].GetString();
				data.meshRenderer->DiffusePath = obj["Mesh Renderer"]["Diffuse Path"].GetString();
				data.meshRenderer->NormalPath = obj["Mesh Renderer"]["Normal Path"].GetString();

				// 일반json 호출 시에는 꺼주어야 함...
				data.meshRenderer->renderType = obj["Mesh Renderer"]["Render Type"].GetInt();

				if (data.meshRenderer->renderType == 0 || data.meshRenderer->renderType == 1)
				{
					data.meshRenderer->ambient.r = obj["Mesh Renderer"]["ambient_r"].GetFloat();
					data.meshRenderer->ambient.g = obj["Mesh Renderer"]["ambient_g"].GetFloat();
					data.meshRenderer->ambient.b = obj["Mesh Renderer"]["ambient_b"].GetFloat();
					data.meshRenderer->ambient.a = obj["Mesh Renderer"]["ambient_a"].GetFloat();

					data.meshRenderer->diffuse.r = obj["Mesh Renderer"]["diffuse_r"].GetFloat();
					data.meshRenderer->diffuse.g = obj["Mesh Renderer"]["diffuse_g"].GetFloat();
					data.meshRenderer->diffuse.b = obj["Mesh Renderer"]["diffuse_b"].GetFloat();
					data.meshRenderer->diffuse.a = obj["Mesh Renderer"]["diffuse_a"].GetFloat();

					data.meshRenderer->specular.r = obj["Mesh Renderer"]["specular_r"].GetFloat();
					data.meshRenderer->specular.g = obj["Mesh Renderer"]["specular_g"].GetFloat();
					data.meshRenderer->specular.b = obj["Mesh Renderer"]["specular_b"].GetFloat();
					data.meshRenderer->specular.a = obj["Mesh Renderer"]["specular_a"].GetFloat();

					data.meshRenderer->reflect.r = obj["Mesh Renderer"]["reflect_r"].GetFloat();
					data.meshRenderer->reflect.g = obj["Mesh Renderer"]["reflect_g"].GetFloat();
					data.meshRenderer->reflect.b = obj["Mesh Renderer"]["reflect_b"].GetFloat();
					data.meshRenderer->reflect.a = obj["Mesh Renderer"]["reflect_a"].GetFloat();
				}

				else if (data.meshRenderer->renderType == 2)
				{
					data.meshRenderer->ARMpath = obj["Mesh Renderer"]["pbr_arm"].GetString();

					data.meshRenderer->pbr_albedo.r = obj["Mesh Renderer"]["pbr_albedo_r"].GetFloat();
					data.meshRenderer->pbr_albedo.g = obj["Mesh Renderer"]["pbr_albedo_g"].GetFloat();
					data.meshRenderer->pbr_albedo.b = obj["Mesh Renderer"]["pbr_albedo_b"].GetFloat();
					data.meshRenderer->pbr_albedo.a = obj["Mesh Renderer"]["pbr_albedo_a"].GetFloat();

					data.meshRenderer->pbr_metalic = obj["Mesh Renderer"]["pbr_metalic"].GetFloat();
					data.meshRenderer->pbr_roughness = obj["Mesh Renderer"]["pbr_roughness"].GetFloat();
					data.meshRenderer->pbr_specular = obj["Mesh Renderer"]["pbr_specular"].GetFloat();
					data.meshRenderer->pbr_emission = obj["Mesh Renderer"]["pbr_emission"].GetFloat();
				}

			}
			/*if (obj.HasMember("Skinned Mesh Renderer"))
			{
				data.skinnedMeshRenderer = new SkinnedMeshRendererData;

				data.skinnedMeshRenderer->name = obj["Skinned Mesh Renderer"]["__component_type__"].GetString();

				data.skinnedMeshRenderer->FBXpath = obj["Skinned Mesh Renderer"]["Mesh Path"].GetString();
				data.skinnedMeshRenderer->ShaderPath = obj["Skinned Mesh Renderer"]["Shader Path"].GetString();
				data.skinnedMeshRenderer->DiffusePath = obj["Skinned Mesh Renderer"]["Diffuse Path"].GetString();
				data.skinnedMeshRenderer->NormalPath = obj["Skinned Mesh Renderer"]["Normal Path"].GetString();

				data.skinnedMeshRenderer->ambient.r = obj["Skinned Mesh Renderer"]["ambient_r"].GetFloat();
				data.skinnedMeshRenderer->ambient.g = obj["Skinned Mesh Renderer"]["ambient_g"].GetFloat();
				data.skinnedMeshRenderer->ambient.b = obj["Skinned Mesh Renderer"]["ambient_b"].GetFloat();
				data.skinnedMeshRenderer->ambient.a = obj["Skinned Mesh Renderer"]["ambient_a"].GetFloat();

				data.skinnedMeshRenderer->diffuse.r = obj["Skinned Mesh Renderer"]["diffuse_r"].GetFloat();
				data.skinnedMeshRenderer->diffuse.g = obj["Skinned Mesh Renderer"]["diffuse_g"].GetFloat();
				data.skinnedMeshRenderer->diffuse.b = obj["Skinned Mesh Renderer"]["diffuse_b"].GetFloat();
				data.skinnedMeshRenderer->diffuse.a = obj["Skinned Mesh Renderer"]["diffuse_a"].GetFloat();

				data.skinnedMeshRenderer->specular.r = obj["Skinned Mesh Renderer"]["specular_r"].GetFloat();
				data.skinnedMeshRenderer->specular.g = obj["Skinned Mesh Renderer"]["specular_g"].GetFloat();
				data.skinnedMeshRenderer->specular.b = obj["Skinned Mesh Renderer"]["specular_b"].GetFloat();
				data.skinnedMeshRenderer->specular.a = obj["Skinned Mesh Renderer"]["specular_a"].GetFloat();

				data.skinnedMeshRenderer->reflect.r = obj["Skinned Mesh Renderer"]["reflect_r"].GetFloat();
				data.skinnedMeshRenderer->reflect.g = obj["Skinned Mesh Renderer"]["reflect_g"].GetFloat();
				data.skinnedMeshRenderer->reflect.b = obj["Skinned Mesh Renderer"]["reflect_b"].GetFloat();
				data.skinnedMeshRenderer->reflect.a = obj["Skinned Mesh Renderer"]["reflect_a"].GetFloat();

				data.skinnedMeshRenderer->pbr_albedo.r = obj["Skinned Mesh Renderer"]["pbr_albedo_r"].GetFloat();
				data.skinnedMeshRenderer->pbr_albedo.g = obj["Skinned Mesh Renderer"]["pbr_albedo_g"].GetFloat();
				data.skinnedMeshRenderer->pbr_albedo.b = obj["Skinned Mesh Renderer"]["pbr_albedo_b"].GetFloat();
				data.skinnedMeshRenderer->pbr_albedo.a = obj["Skinned Mesh Renderer"]["pbr_albedo_a"].GetFloat();

				data.skinnedMeshRenderer->pbr_metalic = obj["Skinned Mesh Renderer"]["pbr_metalic"].GetFloat();
				data.skinnedMeshRenderer->pbr_roughness = obj["Skinned Mesh Renderer"]["pbr_roughness"].GetFloat();
				data.skinnedMeshRenderer->pbr_specular = obj["Skinned Mesh Renderer"]["pbr_specular"].GetFloat();
				data.skinnedMeshRenderer->pbr_emission = obj["Skinned Mesh Renderer"]["pbr_emission"].GetFloat();
			}*/
			if (obj.HasMember("Sprite Renderer"))
			{
				data.sprite = new spriteRendererData;

				data.sprite->name = obj["Sprite Renderer"]["__component_type__"].GetString();

				data.sprite->SpritePath = obj["Sprite Renderer"]["Sprite Path"].GetString();

				data.sprite->width = obj["Sprite Renderer"]["Width"].GetFloat();
				data.sprite->height = obj["Sprite Renderer"]["Height"].GetFloat();
				data.sprite->sortOrder = obj["Sprite Renderer"]["sort order"].GetInt();

			}
			/*if (obj.HasMember("Animation Controller"))
			{
				data.aniController = new animationControllerData;

				data.aniController->name = obj["Animation Controller"]["__component_type__"].GetString();

				const Value& animation = obj["Animation Controller"];
				for (Value::ConstMemberIterator itr = animation.MemberBegin(); itr != animation.MemberEnd(); ++itr)
				{
					if (strcmp(itr->name.GetString(), "State") == 0)
					{
						animationData aniData;

						aniData.stateName = itr->value["State Name"].GetString();
						aniData.aniDescName = itr->value["AnimationDesc Name"].GetString();
						aniData.animationPath = itr->value["Animation Path"].GetString();
						aniData.modelPath = itr->value["This Model Path"].GetString();
						aniData.isLoop = itr->value["Is Animation Loop"].GetBool();

						data.aniController->states.push_back(&aniData);
					}

					if (strcmp(itr->name.GetString(), "Condition") == 0)
					{
						std::string conditionName = itr->value["Condition Name"].GetString();
						bool isConditionOn = itr->value["is Condition On"].GetBool();

						data.aniController->conditions.insert({ conditionName, isConditionOn });
					}

				}

			}*/
			if (obj.HasMember("Text Box"))
			{
				data.txtBox = new textBoxData;

				data.txtBox->name = obj["Text Box"]["__component_type__"].GetString();

				data.txtBox->text = obj["Text Box"]["text"].GetString();
				data.txtBox->fontSize = obj["Text Box"]["font size"].GetFloat();

				data.txtBox->color.r = obj["Text Box"]["Color"]["r"].GetFloat();
				data.txtBox->color.b = obj["Text Box"]["Color"]["g"].GetFloat();
				data.txtBox->color.g = obj["Text Box"]["Color"]["b"].GetFloat();
				data.txtBox->color.a = obj["Text Box"]["Color"]["a"].GetFloat();

				data.txtBox->width = obj["Text Box"]["Width"].GetFloat();
				data.txtBox->height = obj["Text Box"]["Height"].GetFloat();
				data.txtBox->sortOrder = obj["Text Box"]["sort order"].GetInt();

			}
			if (obj.HasMember("Box Collider"))
			{
				data.boxColli = new boxColliderData;
				data.boxColli->name = obj["Box Collider"]["__component_type__"].GetString();

				data.boxColli->width = obj["Box Collider"]["width"].GetFloat();
				data.boxColli->height = obj["Box Collider"]["height"].GetFloat();
				data.boxColli->depth = obj["Box Collider"]["depth"].GetFloat();

				data.boxColli->positionOffset[0] = obj["Box Collider"]["Position offset_x"].GetFloat();
				data.boxColli->positionOffset[1] = obj["Box Collider"]["Position offset_y"].GetFloat();
				data.boxColli->positionOffset[2] = obj["Box Collider"]["Position offset_z"].GetFloat();

				data.boxColli->rotationOffset[0] = obj["Box Collider"]["Rotation offset_w"].GetFloat();
				data.boxColli->rotationOffset[1] = obj["Box Collider"]["Rotation offset_x"].GetFloat();
				data.boxColli->rotationOffset[2] = obj["Box Collider"]["Rotation offset_y"].GetFloat();
				data.boxColli->rotationOffset[3] = obj["Box Collider"]["Rotation offset_z"].GetFloat();

				data.boxColli->scaleOffset[0] = obj["Box Collider"]["Scale offset_x"].GetFloat();
				data.boxColli->scaleOffset[1] = obj["Box Collider"]["Scale offset_y"].GetFloat();
				data.boxColli->scaleOffset[2] = obj["Box Collider"]["Scale offset_z"].GetFloat();

				const Value& index = obj["Box Collider"];
				for (Value::ConstMemberIterator itr = index.MemberBegin(); itr != index.MemberEnd(); ++itr)
				{
					if (strcmp(itr->name.GetString(), "servant") == 0)
					{
						std::string name = itr->value.GetString();
						data.boxColli->servants.push_back(name);
					}
				}
			}
			if (obj.HasMember("Capsule Collider"))
			{
				data.capsuleColli = new capsuleColliderData;
				data.capsuleColli->name = obj["Capsule Collider"]["__component_type__"].GetString();

				data.capsuleColli->radius = obj["Capsule Collider"]["radius"].GetFloat();
				data.capsuleColli->halfHeight = obj["Capsule Collider"]["half height"].GetFloat();

				data.capsuleColli->positionOffset[0] = obj["Capsule Collider"]["Position offset_x"].GetFloat();
				data.capsuleColli->positionOffset[1] = obj["Capsule Collider"]["Position offset_y"].GetFloat();
				data.capsuleColli->positionOffset[2] = obj["Capsule Collider"]["Position offset_z"].GetFloat();

				data.capsuleColli->rotationOffset[0] = obj["Capsule Collider"]["Rotation offset_w"].GetFloat();
				data.capsuleColli->rotationOffset[1] = obj["Capsule Collider"]["Rotation offset_x"].GetFloat();
				data.capsuleColli->rotationOffset[2] = obj["Capsule Collider"]["Rotation offset_y"].GetFloat();
				data.capsuleColli->rotationOffset[3] = obj["Capsule Collider"]["Rotation offset_z"].GetFloat();

				data.capsuleColli->scaleOffset[0] = obj["Capsule Collider"]["Scale offset_x"].GetFloat();
				data.capsuleColli->scaleOffset[1] = obj["Capsule Collider"]["Scale offset_y"].GetFloat();
				data.capsuleColli->scaleOffset[2] = obj["Capsule Collider"]["Scale offset_z"].GetFloat();

				const Value& index = obj["Capsule Collider"];
				for (Value::ConstMemberIterator itr = index.MemberBegin(); itr != index.MemberEnd(); ++itr)
				{
					if (strcmp(itr->name.GetString(), "servant") == 0)
					{
						std::string name = itr->value.GetString();
						data.capsuleColli->servants.push_back(name);
					}
				}
			}
			if (obj.HasMember("Sphere Collider"))
			{
				data.sphereColli = new sphereColliderData;
				data.sphereColli->name = obj["Sphere Collider"]["__component_type__"].GetString();

				data.sphereColli->radius = obj["Sphere Collider"]["radius"].GetFloat();

				data.sphereColli->positionOffset[0] = obj["Sphere Collider"]["Position offset_x"].GetFloat();
				data.sphereColli->positionOffset[1] = obj["Sphere Collider"]["Position offset_y"].GetFloat();
				data.sphereColli->positionOffset[2] = obj["Sphere Collider"]["Position offset_z"].GetFloat();

				data.sphereColli->rotationOffset[0] = obj["Sphere Collider"]["Rotation offset_w"].GetFloat();
				data.sphereColli->rotationOffset[1] = obj["Sphere Collider"]["Rotation offset_x"].GetFloat();
				data.sphereColli->rotationOffset[2] = obj["Sphere Collider"]["Rotation offset_y"].GetFloat();
				data.sphereColli->rotationOffset[3] = obj["Sphere Collider"]["Rotation offset_z"].GetFloat();

				data.sphereColli->scaleOffset[0] = obj["Sphere Collider"]["Scale offset_x"].GetFloat();
				data.sphereColli->scaleOffset[1] = obj["Sphere Collider"]["Scale offset_y"].GetFloat();
				data.sphereColli->scaleOffset[2] = obj["Sphere Collider"]["Scale offset_z"].GetFloat();

				const Value& index = obj["Sphere Collider"];
				for (Value::ConstMemberIterator itr = index.MemberBegin(); itr != index.MemberEnd(); ++itr)
				{
					if (strcmp(itr->name.GetString(), "servant") == 0)
					{
						std::string name = itr->value.GetString();
						data.sphereColli->servants.push_back(name);
					}
				}
			}
			if (obj.HasMember("Plane Collider"))
			{
				data.planeColli = new planeColliderData;
				data.planeColli->name = obj["Plane Collider"]["__component_type__"].GetString();

				data.planeColli->positionOffset[0] = obj["Plane Collider"]["Position offset_x"].GetFloat();
				data.planeColli->positionOffset[1] = obj["Plane Collider"]["Position offset_y"].GetFloat();
				data.planeColli->positionOffset[2] = obj["Plane Collider"]["Position offset_z"].GetFloat();

				data.planeColli->rotationOffset[0] = obj["Plane Collider"]["Rotation offset_w"].GetFloat();
				data.planeColli->rotationOffset[1] = obj["Plane Collider"]["Rotation offset_x"].GetFloat();
				data.planeColli->rotationOffset[2] = obj["Plane Collider"]["Rotation offset_y"].GetFloat();
				data.planeColli->rotationOffset[3] = obj["Plane Collider"]["Rotation offset_z"].GetFloat();

				data.planeColli->scaleOffset[0] = obj["Plane Collider"]["Scale offset_x"].GetFloat();
				data.planeColli->scaleOffset[1] = obj["Plane Collider"]["Scale offset_y"].GetFloat();
				data.planeColli->scaleOffset[2] = obj["Plane Collider"]["Scale offset_z"].GetFloat();
			}
			if (obj.HasMember("Static Box Collider"))
			{
				const Value& index = obj["Static Box Collider"];
				for (Value::ConstMemberIterator itr = index.MemberBegin(); itr != index.MemberEnd(); ++itr)
				{
					if (strcmp(itr->name.GetString(), "One Collider") == 0)
					{
						staticBoxColliderData* staticBoxColli = new staticBoxColliderData;

						staticBoxColli->name = itr->value["__component_type__"].GetString();
						staticBoxColli->width = itr->value["width"].GetFloat();
						staticBoxColli->height = itr->value["height"].GetFloat();
						staticBoxColli->depth = itr->value["depth"].GetFloat();
						staticBoxColli->positionOffset[0] = itr->value["Position offset_x"].GetFloat();
						staticBoxColli->positionOffset[1] = itr->value["Position offset_y"].GetFloat();
						staticBoxColli->positionOffset[2] = itr->value["Position offset_z"].GetFloat();
						staticBoxColli->rotationOffset[0] = itr->value["Rotation offset_w"].GetFloat();
						staticBoxColli->rotationOffset[1] = itr->value["Rotation offset_x"].GetFloat();
						staticBoxColli->rotationOffset[2] = itr->value["Rotation offset_y"].GetFloat();
						staticBoxColli->rotationOffset[3] = itr->value["Rotation offset_z"].GetFloat();
						staticBoxColli->scaleOffset[0] = itr->value["Scale offset_x"].GetFloat();
						staticBoxColli->scaleOffset[1] = itr->value["Scale offset_y"].GetFloat();
						staticBoxColli->scaleOffset[2] = itr->value["Scale offset_z"].GetFloat();

						data.staticBoxColli.push_back(staticBoxColli);
					}
				}
			}
			if (obj.HasMember("Button"))
			{
				data.btn = new buttonData;
				data.btn->name = obj["Button"]["__component_type__"].GetString();

				data.btn->height = obj["Button"]["height"].GetFloat();
				data.btn->width = obj["Button"]["width"].GetFloat();
			}
			if (obj.HasMember("Audio Clip"))
			{
				data.audio = new audioClipData;

				data.audio->name = obj["Audio Clip"]["__component_type__"].GetString();

				const Value& audioClip = obj["Audio Clip"];
				for (Value::ConstMemberIterator itr = audioClip.MemberBegin(); itr != audioClip.MemberEnd(); ++itr)
				{
					audioData* oneAudio = new audioData;

					if (strcmp(itr->name.GetString(), "Audio") == 0)
					{
						oneAudio->name = obj["Audio Clip"]["Audio"]["Audio name"].GetString();
						oneAudio->path = obj["Audio Clip"]["Audio"]["Audio path"].GetString();
						oneAudio->is3D = obj["Audio Clip"]["Audio"]["is3D"].GetBool();

						if (obj["Audio Clip"]["Audio"]["sound group"].GetString() == "BackGround")
						{
							oneAudio->type = eSoundGroup::BackGroundMusic;
						}
						else if (obj["Audio Clip"]["Audio"]["sound group"].GetString() == "Effect")
						{
							oneAudio->type = eSoundGroup::EffectSound;
						}
						else
						{
							oneAudio->type = eSoundGroup::error;
						}
					}

					data.audio->audioList.push_back(oneAudio);
				}
			}
			if (obj.HasMember("Camera"))
			{
				data.camera = new cameraData;

				data.camera->name = obj["Camera"]["__component_type__"].GetString();

				data.camera->_farZ = obj["Camera"]["far_z"].GetFloat();
				data.camera->_nearZ = obj["Camera"]["near_z"].GetFloat();
				data.camera->_aspect = obj["Camera"]["aspect"].GetFloat();
				data.camera->_fovY = obj["Camera"]["fov_y"].GetFloat();
				data.camera->_nearWindowHeight = obj["Camera"]["near_winHeight"].GetFloat();
				data.camera->_nearWindowWidth = obj["Camera"]["near_winWidth"].GetFloat();

				data.camera->_viewMatrix = RocketEngine::RMFLOAT4X4(
					obj["Camera"]["View Matrix"]["m_11"].GetFloat(), obj["Camera"]["View Matrix"]["m_12"].GetFloat(), obj["Camera"]["View Matrix"]["m_13"].GetFloat(), obj["Camera"]["View Matrix"]["m_14"].GetFloat(),
					obj["Camera"]["View Matrix"]["m_21"].GetFloat(), obj["Camera"]["View Matrix"]["m_22"].GetFloat(), obj["Camera"]["View Matrix"]["m_23"].GetFloat(), obj["Camera"]["View Matrix"]["m_24"].GetFloat(),
					obj["Camera"]["View Matrix"]["m_31"].GetFloat(), obj["Camera"]["View Matrix"]["m_32"].GetFloat(), obj["Camera"]["View Matrix"]["m_33"].GetFloat(), obj["Camera"]["View Matrix"]["m_34"].GetFloat(),
					obj["Camera"]["View Matrix"]["m_41"].GetFloat(), obj["Camera"]["View Matrix"]["m_42"].GetFloat(), obj["Camera"]["View Matrix"]["m_43"].GetFloat(), obj["Camera"]["View Matrix"]["m_44"].GetFloat());

				data.camera->_projMatrix = RocketEngine::RMFLOAT4X4(
					obj["Camera"]["Proj Matrix"]["m_11"].GetFloat(), obj["Camera"]["Proj Matrix"]["m_12"].GetFloat(), obj["Camera"]["Proj Matrix"]["m_13"].GetFloat(), obj["Camera"]["Proj Matrix"]["m_14"].GetFloat(),
					obj["Camera"]["Proj Matrix"]["m_21"].GetFloat(), obj["Camera"]["Proj Matrix"]["m_22"].GetFloat(), obj["Camera"]["Proj Matrix"]["m_23"].GetFloat(), obj["Camera"]["Proj Matrix"]["m_24"].GetFloat(),
					obj["Camera"]["Proj Matrix"]["m_31"].GetFloat(), obj["Camera"]["Proj Matrix"]["m_32"].GetFloat(), obj["Camera"]["Proj Matrix"]["m_33"].GetFloat(), obj["Camera"]["Proj Matrix"]["m_34"].GetFloat(),
					obj["Camera"]["Proj Matrix"]["m_41"].GetFloat(), obj["Camera"]["Proj Matrix"]["m_42"].GetFloat(), obj["Camera"]["Proj Matrix"]["m_43"].GetFloat(), obj["Camera"]["Proj Matrix"]["m_44"].GetFloat());
			}
			if (obj.HasMember("Directional Light"))
			{
				data.dirLight = new directionalLightData;

				data.dirLight->name = obj["Directional Light"]["__component_type__"].GetString();
				data.dirLight->intensity = obj["Directional Light"]["intensity"].GetFloat();

				data.dirLight->ambient.r = obj["Directional Light"]["ambient"]["r"].GetFloat();
				data.dirLight->ambient.g = obj["Directional Light"]["ambient"]["g"].GetFloat();
				data.dirLight->ambient.b = obj["Directional Light"]["ambient"]["b"].GetFloat();
				data.dirLight->ambient.a = obj["Directional Light"]["ambient"]["a"].GetFloat();

				data.dirLight->diffuse.r = obj["Directional Light"]["diffuse"]["r"].GetFloat();
				data.dirLight->diffuse.g = obj["Directional Light"]["diffuse"]["g"].GetFloat();
				data.dirLight->diffuse.b = obj["Directional Light"]["diffuse"]["b"].GetFloat();
				data.dirLight->diffuse.a = obj["Directional Light"]["diffuse"]["a"].GetFloat();

				data.dirLight->specular.r = obj["Directional Light"]["specular"]["r"].GetFloat();
				data.dirLight->specular.g = obj["Directional Light"]["specular"]["g"].GetFloat();
				data.dirLight->specular.b = obj["Directional Light"]["specular"]["b"].GetFloat();
				data.dirLight->specular.a = obj["Directional Light"]["specular"]["a"].GetFloat();


			}
			if (obj.HasMember("Point Light"))
			{
				data.pointLight = new pointLightData;

				data.pointLight->name = obj["Point Light"]["__component_type__"].GetString();
				data.pointLight->range = obj["Point Light"]["range"].GetFloat();
				data.pointLight->range = obj["Point Light"]["attenuation"]["attenu_x"].GetFloat();
				data.pointLight->range = obj["Point Light"]["attenuation"]["attenu_y"].GetFloat();
				data.pointLight->range = obj["Point Light"]["attenuation"]["attenu_z"].GetFloat();

				data.pointLight->ambient.r = obj["Point Light"]["ambient"]["r"].GetFloat();
				data.pointLight->ambient.g = obj["Point Light"]["ambient"]["g"].GetFloat();
				data.pointLight->ambient.b = obj["Point Light"]["ambient"]["b"].GetFloat();
				data.pointLight->ambient.a = obj["Point Light"]["ambient"]["a"].GetFloat();

				data.pointLight->diffuse.r = obj["Point Light"]["diffuse"]["r"].GetFloat();
				data.pointLight->diffuse.g = obj["Point Light"]["diffuse"]["g"].GetFloat();
				data.pointLight->diffuse.b = obj["Point Light"]["diffuse"]["b"].GetFloat();
				data.pointLight->diffuse.a = obj["Point Light"]["diffuse"]["a"].GetFloat();

				data.pointLight->specular.r = obj["Point Light"]["specular"]["r"].GetFloat();
				data.pointLight->specular.g = obj["Point Light"]["specular"]["g"].GetFloat();
				data.pointLight->specular.b = obj["Point Light"]["specular"]["b"].GetFloat();
				data.pointLight->specular.a = obj["Point Light"]["specular"]["a"].GetFloat();
			}
			if (obj.HasMember("Spot Light"))
			{
				data.spotLight = new spotLightData;

				data.spotLight->name = obj["Spot Light"]["__component_type__"].GetString();
				data.spotLight->range = obj["Spot Light"]["range"].GetFloat();
				data.spotLight->range = obj["Spot Light"]["spot"].GetFloat();

				data.spotLight->range = obj["Spot Light"]["attenuation"]["attenu_x"].GetFloat();
				data.spotLight->range = obj["Spot Light"]["attenuation"]["attenu_y"].GetFloat();
				data.spotLight->range = obj["Spot Light"]["attenuation"]["attenu_z"].GetFloat();

				data.spotLight->ambient.r = obj["Spot Light"]["ambient"]["r"].GetFloat();
				data.spotLight->ambient.g = obj["Spot Light"]["ambient"]["g"].GetFloat();
				data.spotLight->ambient.b = obj["Spot Light"]["ambient"]["b"].GetFloat();
				data.spotLight->ambient.a = obj["Spot Light"]["ambient"]["a"].GetFloat();

				data.spotLight->diffuse.r = obj["Spot Light"]["diffuse"]["r"].GetFloat();
				data.spotLight->diffuse.g = obj["Spot Light"]["diffuse"]["g"].GetFloat();
				data.spotLight->diffuse.b = obj["Spot Light"]["diffuse"]["b"].GetFloat();
				data.spotLight->diffuse.a = obj["Spot Light"]["diffuse"]["a"].GetFloat();

				data.spotLight->specular.r = obj["Spot Light"]["specular"]["r"].GetFloat();
				data.spotLight->specular.g = obj["Spot Light"]["specular"]["g"].GetFloat();
				data.spotLight->specular.b = obj["Spot Light"]["specular"]["b"].GetFloat();
				data.spotLight->specular.a = obj["Spot Light"]["specular"]["a"].GetFloat();
			}

			serializeData.push_back(data);
		}
	}


}

