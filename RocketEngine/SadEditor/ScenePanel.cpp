#include "ScenePanel.h"
#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Image.h"
#include "../RocketEngine/RocketAPI.h"
#include "../RocketEngine/RocketCoreAPI.h"
#include "EditorManager.h"
#include "../RocketGraphicsInterface/RenderType.h"

#include "..\\SoundLib\\inc\\fmod.hpp"
#include "..\\SoundLib\\inc\\fmod_common.h"

#pragma comment(lib, "..\\SoundLib\\lib\\x64\\fmod_vc.lib")


#include "../include/rapidjson/document.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/stringbuffer.h"
#include "../include/rapidjson/prettywriter.h" 

#include "../RocketEngine/RocketAPI.h"

#include "../EngineDemo/CameraMove.h"
#include "HierarchyPanel.h"

#include "Serialize.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <typeinfo>

namespace SAD
{
	std::unordered_map<std::string, RocketEngine::GameObject*> ScenePanel::objs;
	RocketEngine::Scene* ScenePanel::currentScene;
	std::vector<DataStruct>& ScenePanel::copiedData = Serialize::serializeData;


	ScenePanel::ScenePanel()
	{
	}

	ScenePanel::~ScenePanel()
	{
	}

	void ScenePanel::SceneInitialize()
	{
		currentScene = RocketEngine::CreateScene(Serialize::sceneName, true);
		assert(currentScene);

		for (auto& obj : Serialize::serializeData)
		{
			CreateNewObjectAndComponent(obj);
		}

		for (auto& objoption : Serialize::serializeData)
		{
			SetObjectOption(objoption);
		}

		auto editorCamObj = RocketEngine::CreateObject("EditorCamera");
		auto editorCam = editorCamObj->AddComponent<RocketEngine::Camera>();
		RocketEngine::SetMainCamera(editorCam);
	}

	void ScenePanel::MouseMove()
	{
		if (!ImGui::IsMouseDown(ImGuiMouseButton_Right))
		{
			return;
		}

		/// Try.0
		RocketEngine::RMFLOAT2 mouseDelta = RocketEngine::GetMousePosition() - RocketEngine::RMFLOAT2(previousMouseX, previousMouseY);
		currentScene->GetMainCamera()->RotateY(mouseDelta.x * deltaTime * 0.5f);
		currentScene->GetMainCamera()->Pitch(mouseDelta.y * deltaTime * 0.5f);
	}

	void ScenePanel::SceneInput()
	{
		deltaTime = RocketEngine::GetDeltaTime();



		if (ImGui::IsKeyDown(ImGuiKey_W))
		{
			currentScene->GetMainCamera()->Walk(moveSpeed * deltaTime);
		}

		if (ImGui::IsKeyDown(ImGuiKey_S))
		{
			currentScene->GetMainCamera()->Walk(-moveSpeed * deltaTime);
		}

		if (ImGui::IsKeyDown(ImGuiKey_A))
		{
			currentScene->GetMainCamera()->Strafe(-moveSpeed * deltaTime);
		}

		if (ImGui::IsKeyDown(ImGuiKey_D))
		{
			currentScene->GetMainCamera()->Strafe(moveSpeed * deltaTime);
		}

		if (ImGui::IsKeyDown(ImGuiKey_Q))
		{
			currentScene->GetMainCamera()->WorldUpDown(-moveSpeed * deltaTime);
		}

		if (ImGui::IsKeyDown(ImGuiKey_E))
		{
			currentScene->GetMainCamera()->WorldUpDown(moveSpeed * deltaTime);
		}

		MouseMove();

		RocketEngine::RMFLOAT2 mousePos = RocketEngine::GetMousePosition();
		previousMouseX = mousePos.x;
		previousMouseY = mousePos.y;
	}


	void ScenePanel::SceneRun()
	{
		// 오브젝트를 순회
		for (auto& oneObject : objs)
		{
			// 현재 선택한 오브젝트를 확인하기 위한 기즈모 박스 ....
			if (HierarchyPanel::currentNode == oneObject.first)
			{
				RocketEngine::RMFLOAT4X4 worldTM =  oneObject.second->transform.GetWorldTM();
				RocketEngine::RMFLOAT3 WHD = {  0.5f, 10.f, 0.5f };
				RocketEngine::RMFLOAT4 color = { 1.0f,0.0f,0.0f,1.f };

				RocketEngine::DrawDebugBox(worldTM, WHD, false, color);
			}

			// 하나의 오브젝트의 모든 컴포넌트를 순회
			for (auto& oneComponent : oneObject.second->GetAllComponents())
			{
				// staticBox 콜라이더는 하나의 오브젝트에 여러개 붙을 수 있기 때문에 
				// index를 체크하기 위한 변수... 급하게 추가된 거라 구조를 변경할 수 없었음 ㅜ. ㅜ
				int staticBoxIndex = 0;

				// 컴포넌트마다의 벡터를 가리킨다
				for (auto& compVec : oneComponent.second)
				{
					if (dynamic_cast<RocketEngine::Transform*>(compVec))
					{
						auto itr = std::find_if(Serialize::serializeData.begin(), Serialize::serializeData.end(), [oneObject](const DataStruct& data) {return data.transform->name == oneObject.first; });
						oneObject.second->transform.SetLocalPosition(itr->GetPosition());
						oneObject.second->transform.SetLocalRotation(itr->GetRotation());
						oneObject.second->transform.SetLocalScale(itr->GetScale());
					}
					else if (dynamic_cast<RocketEngine::TextBox*>(compVec))
					{
						auto itr = std::find_if(Serialize::serializeData.begin(), Serialize::serializeData.end(), [oneObject](const DataStruct& data) {return data.transform->name == oneObject.first; });
						auto component = static_cast<RocketEngine::TextBox*>(compVec);

						component->SetFontSize(itr->txtBox->fontSize);
						component->SetSortOrder(itr->txtBox->sortOrder);
						component->SetHeight(itr->txtBox->height);
						component->SetWidth(itr->txtBox->width);
					}
					else if (dynamic_cast<RocketEngine::MeshRenderer*>(compVec))
					{
						auto itr = std::find_if(Serialize::serializeData.begin(), Serialize::serializeData.end(), [oneObject](const DataStruct& data) {return data.transform->name == oneObject.first; });
						auto component = static_cast<RocketEngine::MeshRenderer*>(compVec);
						
						component->SetLegacyAmbient({ itr->meshRenderer->ambient.r , itr->meshRenderer->ambient.g , itr->meshRenderer->ambient.b , itr->meshRenderer->ambient.a });
						component->SetLegacyDiffuse({ itr->meshRenderer->diffuse.r , itr->meshRenderer->diffuse.g , itr->meshRenderer->diffuse.b , itr->meshRenderer->diffuse.a });
						component->SetLegacySpecular({ itr->meshRenderer->specular.r , itr->meshRenderer->specular.g , itr->meshRenderer->specular.b , itr->meshRenderer->specular.a });
						component->SetLegacyReflect({ itr->meshRenderer->reflect.r , itr->meshRenderer->reflect.g , itr->meshRenderer->reflect.b , itr->meshRenderer->reflect.a });

						component->SetPBRAlbedo({ itr->meshRenderer->pbr_albedo.r , itr->meshRenderer->pbr_albedo.g , itr->meshRenderer->pbr_albedo.b , itr->meshRenderer->pbr_albedo.a });
						component->SetPBRMetallic(itr->meshRenderer->pbr_metalic);
						component->SetPBRSpecular(itr->meshRenderer->pbr_specular);
						component->SetPBREmission(itr->meshRenderer->pbr_emission);

						//component->SetMeshPath(itr->meshRenderer->FBXpath);
						//component->SetShaderPath(itr->meshRenderer->ShaderPath);
						//component->SetDiffusePath(itr->meshRenderer->DiffusePath);
						//component->SetNormalPath(itr->meshRenderer->NormalPath);
					}
					else if (dynamic_cast<RocketEngine::SkinnedMeshRenderer*>(compVec))
					{
						auto itr = std::find_if(Serialize::serializeData.begin(), Serialize::serializeData.end(), [oneObject](const DataStruct& data) {return data.transform->name == oneObject.first; });
						auto component = static_cast<RocketEngine::SkinnedMeshRenderer*>(compVec);
						
						component->SetLegacyAmbient({ itr->skinnedMeshRenderer->ambient.r , itr->skinnedMeshRenderer->ambient.g , itr->skinnedMeshRenderer->ambient.b , itr->skinnedMeshRenderer->ambient.a });
						component->SetLegacyDiffuse({ itr->skinnedMeshRenderer->diffuse.r , itr->skinnedMeshRenderer->diffuse.g , itr->skinnedMeshRenderer->diffuse.b , itr->skinnedMeshRenderer->diffuse.a });
						component->SetLegacySpecular({ itr->skinnedMeshRenderer->specular.r , itr->skinnedMeshRenderer->specular.g , itr->skinnedMeshRenderer->specular.b , itr->skinnedMeshRenderer->specular.a });
						component->SetLegacyReflect({ itr->skinnedMeshRenderer->reflect.r , itr->skinnedMeshRenderer->reflect.g , itr->skinnedMeshRenderer->reflect.b , itr->skinnedMeshRenderer->reflect.a });

						component->SetPBRAlbedo({ itr->skinnedMeshRenderer->pbr_albedo.r , itr->skinnedMeshRenderer->pbr_albedo.g , itr->skinnedMeshRenderer->pbr_albedo.b , itr->skinnedMeshRenderer->pbr_albedo.a });
						component->SetPBRMetallic(itr->skinnedMeshRenderer->pbr_metalic);
						component->SetPBRSpecular(itr->skinnedMeshRenderer->pbr_specular);
						component->SetPBREmission(itr->skinnedMeshRenderer->pbr_emission);

						//component->SetMeshPath(itr->skinnedMeshRenderer->FBXpath);
						//component->SetShaderPath(itr->skinnedMeshRenderer->ShaderPath);
						//component->SetDiffusePath(itr->skinnedMeshRenderer->DiffusePath);
						//component->SetNormalPath(itr->skinnedMeshRenderer->NormalPath);
					}
					else if (dynamic_cast<RocketEngine::SpriteRenderer*>(compVec))
					{
						auto itr = std::find_if(Serialize::serializeData.begin(), Serialize::serializeData.end(), [oneObject](const DataStruct& data) {return data.transform->name == oneObject.first; });
						auto component = static_cast<RocketEngine::SpriteRenderer*>(compVec);
						
						component->SetPath(itr->sprite->SpritePath);
						component->SetWidth(itr->sprite->width);
						component->SetHeight(itr->sprite->height);
						component->SetSortOrder(itr->sprite->sortOrder);
					}
					else if (dynamic_cast<RocketEngine::Button*>(compVec))
					{
						auto itr = std::find_if(Serialize::serializeData.begin(), Serialize::serializeData.end(), [oneObject](const DataStruct& data) {return data.transform->name == oneObject.first; });
						auto component = static_cast<RocketEngine::Button*>(compVec);

						component->SetWidth(itr->btn->width);
						component->SetHeight(itr->btn->height);
						component->SetSortOrder(itr->btn->sortOrder);
					}
					else if (dynamic_cast<RocketEngine::BoxCollider*>(compVec))
					{
						auto itr = std::find_if(Serialize::serializeData.begin(), Serialize::serializeData.end(), [oneObject](const DataStruct& data) {return data.transform->name == oneObject.first; });
						auto component = static_cast<RocketEngine::BoxCollider*>(compVec);
						
						component->SetVolume(itr->boxColli->width, itr->boxColli->height, itr->boxColli->depth);
						component->SetPositionOffset(itr->boxColli->GetPositionOffset());
						component->SetRotationOffset(itr->boxColli->GetRotationOffset());
						component->SetScaleOffset(itr->boxColli->GetScaleOffset());
					}
					else if (dynamic_cast<RocketEngine::CapsuleCollider*>(compVec))
					{
						auto itr = std::find_if(Serialize::serializeData.begin(), Serialize::serializeData.end(), [oneObject](const DataStruct& data) {return data.transform->name == oneObject.first; });
						auto component = static_cast<RocketEngine::CapsuleCollider*>(compVec);

						component->SetRadius(itr->capsuleColli->radius);
						component->SetHalfHeight(itr->capsuleColli->halfHeight);
						component->SetPositionOffset(itr->capsuleColli->GetPositionOffset());
						component->SetRotationOffset(itr->capsuleColli->GetRotationOffset());
						component->SetScaleOffset(itr->capsuleColli->GetScaleOffset());
					}
					else if (dynamic_cast<RocketEngine::SphereCollider*>(compVec))
					{
						auto itr = std::find_if(Serialize::serializeData.begin(), Serialize::serializeData.end(), [oneObject](const DataStruct& data) {return data.transform->name == oneObject.first; });
						auto component = static_cast<RocketEngine::SphereCollider*>(compVec);

						component->SetRadius(itr->sphereColli->radius);
						component->SetPositionOffset(itr->sphereColli->GetPositionOffset());
						component->SetRotationOffset(itr->sphereColli->GetRotationOffset());
						component->SetScaleOffset(itr->sphereColli->GetScaleOffset());
					}
					else if (dynamic_cast<RocketEngine::PlaneCollider*>(compVec))
					{
						auto itr = std::find_if(Serialize::serializeData.begin(), Serialize::serializeData.end(), [oneObject](const DataStruct& data) {return data.transform->name == oneObject.first; });
						auto component = static_cast<RocketEngine::PlaneCollider*>(compVec);

						component->SetPositionOffset(itr->planeColli->GetPositionOffset());
						component->SetRotationOffset(itr->planeColli->GetRotationOffset());
						component->SetScaleOffset(itr->planeColli->GetScaleOffset());
					}
					else if (dynamic_cast<RocketEngine::StaticBoxCollider*>(compVec))
					{
						auto itr = std::find_if(Serialize::serializeData.begin(), Serialize::serializeData.end(), [oneObject](const DataStruct& data) {return data.transform->name == oneObject.first; });
						auto component = static_cast<RocketEngine::StaticBoxCollider*>(compVec);
						auto index = itr->staticBoxColli[staticBoxIndex];

						component->SetVolume(index->width, index->height, index->depth);
						component->SetPositionOffset(index->GetPositionOffset());
						component->SetRotationOffset(index->GetRotationOffset());
						component->SetScaleOffset(index->GetScaleOffset());

						staticBoxIndex++;
					}
					else if (dynamic_cast<RocketEngine::DirectionalLight*>(compVec))
					{
						auto itr = std::find_if(Serialize::serializeData.begin(), Serialize::serializeData.end(), [oneObject](const DataStruct& data) {return data.transform->name == oneObject.first; });
						auto component = static_cast<RocketEngine::DirectionalLight*>(compVec);
						
						component->SetIntensity(itr->dirLight->intensity);
						component->SetAmbient({ itr->dirLight->ambient.r, itr->dirLight->ambient.g, itr->dirLight->ambient.b, itr->dirLight->ambient.a });
						component->SetDiffuse({ itr->dirLight->diffuse.r, itr->dirLight->diffuse.g, itr->dirLight->diffuse.b, itr->dirLight->diffuse.a });
						component->SetSpecular({ itr->dirLight->specular.r, itr->dirLight->specular.g, itr->dirLight->specular.b, itr->dirLight->specular.a });
					}
					else if (dynamic_cast<RocketEngine::PointLight*>(compVec))
					{
						auto itr = std::find_if(Serialize::serializeData.begin(), Serialize::serializeData.end(), [oneObject](const DataStruct& data) {return data.transform->name == oneObject.first; });
						auto component = static_cast<RocketEngine::PointLight*>(compVec);

						component->SetRange(itr->pointLight->range);
						component->SetAttenuation({ itr->pointLight->x, itr->pointLight->y,itr->pointLight->z });
						component->SetAmbient({ itr->pointLight->ambient.r, itr->pointLight->ambient.g, itr->pointLight->ambient.b, itr->pointLight->ambient.a });
						component->SetDiffuse({ itr->pointLight->diffuse.r, itr->pointLight->diffuse.g, itr->pointLight->diffuse.b, itr->pointLight->diffuse.a });
						component->SetSpecular({ itr->pointLight->specular.r, itr->pointLight->specular.g, itr->pointLight->specular.b, itr->pointLight->specular.a });

					}
					else if (dynamic_cast<RocketEngine::SpotLight*>(compVec))
					{
						auto itr = std::find_if(Serialize::serializeData.begin(), Serialize::serializeData.end(), [oneObject](const DataStruct& data) {return data.transform->name == oneObject.first; });
						auto component = static_cast<RocketEngine::SpotLight*>(compVec);

						component->SetRange(itr->spotLight->range);
						component->SetSpot(itr->spotLight->spot);
						component->SetAttenuation({ itr->spotLight->x, itr->spotLight->y,itr->spotLight->z });
						component->SetAmbient({ itr->spotLight->ambient.r, itr->spotLight->ambient.g, itr->spotLight->ambient.b, itr->spotLight->ambient.a });
						component->SetDiffuse({ itr->spotLight->diffuse.r, itr->spotLight->diffuse.g, itr->spotLight->diffuse.b, itr->spotLight->diffuse.a });
						component->SetSpecular({ itr->spotLight->specular.r, itr->spotLight->specular.g, itr->spotLight->specular.b, itr->spotLight->specular.a });
					}
					else
					{

					}
					
				}
			}
		}
	}



	void ScenePanel::CreateNewObjectAndComponent(DataStruct& data)
	{
		// 오브젝트 이름, 게임 오브젝트 생성해서 map에 넣는다
		objs.insert({ data.transform->name, RocketEngine::CreateObject(data.transform->name.c_str()) });
		assert(data.transform);

		if (data.transform != nullptr)
		{
			objs.find(data.transform->name)->second->transform.SetLocalPosition(data.transform->position_x, data.transform->position_y, data.transform->position_z);
			objs.find(data.transform->name)->second->transform.SetLocalScale(data.transform->scale_x, data.transform->scale_y, data.transform->scale_z);
			objs.find(data.transform->name)->second->transform.SetLocalRotation(data.transform->rotation_w, data.transform->rotation_x, data.transform->rotation_y, data.transform->rotation_z);
		}
		if (data.camera != nullptr)
		{
			auto component = objs.find(data.transform->name)->second->AddComponent<RocketEngine::Camera>();
			RocketEngine::SetMainCamera(component);
		}
		if (data.meshRenderer != nullptr)
		{
			auto component = objs.find(data.transform->name)->second->AddComponent<RocketEngine::MeshRenderer>();

			component->SetMeshPath(data.meshRenderer->FBXpath);
			component->SetShaderPath(data.meshRenderer->ShaderPath);
			component->SetDiffusePath(data.meshRenderer->DiffusePath);
			component->SetNormalPath(data.meshRenderer->NormalPath);

			if (data.meshRenderer->renderType == 0)
			{
				component->SetLegacyAmbient(data.meshRenderer->ambient);
				component->SetLegacyDiffuse(data.meshRenderer->diffuse);
				component->SetLegacySpecular(data.meshRenderer->specular);
				component->SetLegacyReflect(data.meshRenderer->reflect);
				component->SetRenderType(RocketCore::Graphics::eRenderType::FORWARD);
			}
			else if (data.meshRenderer->renderType == 1)
			{
				component->SetLegacyAmbient(data.meshRenderer->ambient);
				component->SetLegacyDiffuse(data.meshRenderer->diffuse);
				component->SetLegacySpecular(data.meshRenderer->specular);
				component->SetLegacyReflect(data.meshRenderer->reflect);
				component->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PHONG);
			}
			else if(data.meshRenderer->renderType == 2)
			{
				component->SetARMPath(data.meshRenderer->ARMpath);
				component->SetPBRAlbedo(data.meshRenderer->pbr_albedo);
				component->SetPBRMetallic(data.meshRenderer->pbr_metalic);
				component->SetPBRSpecular(data.meshRenderer->pbr_specular);
				component->SetPBREmission(data.meshRenderer->pbr_emission);
				component->SetPBRRoughness(data.meshRenderer->pbr_roughness);
				component->SetRenderType(RocketCore::Graphics::eRenderType::DEFERRED_PBR);
			}
			
		}
	/*	if (data.skinnedMeshRenderer != nullptr)
		{
			auto component = objs.find(data.transform->name)->second->AddComponent<RocketEngine::SkinnedMeshRenderer>();

			component->SetMeshPath(data.skinnedMeshRenderer->FBXpath);
			component->SetShaderPath(data.skinnedMeshRenderer->ShaderPath);
			component->SetDiffusePath(data.skinnedMeshRenderer->DiffusePath);
			component->SetNormalPath(data.skinnedMeshRenderer->NormalPath);

			component->SetLegacyAmbient(data.skinnedMeshRenderer->ambient);
			component->SetLegacyDiffuse(data.skinnedMeshRenderer->diffuse);
			component->SetLegacySpecular(data.skinnedMeshRenderer->specular);
			component->SetLegacyReflect(data.skinnedMeshRenderer->reflect);

			component->SetPBRAlbedo(data.skinnedMeshRenderer->pbr_albedo);
			component->SetPBRMetallic(data.skinnedMeshRenderer->pbr_metalic);
			component->SetPBRSpecular(data.skinnedMeshRenderer->pbr_specular);
			component->SetPBREmission(data.skinnedMeshRenderer->pbr_emission);
			component->SetPBRRoughness(data.skinnedMeshRenderer->pbr_roughness);
		}*/
		if (data.sprite != nullptr)
		{
			auto component = objs.find(data.transform->name)->second->AddComponent<RocketEngine::SpriteRenderer>();

			component->SetWidth(data.sprite->width);
			component->SetHeight(data.sprite->height);
			component->SetPath(data.sprite->SpritePath);

			component->SetSortOrder(data.sprite->sortOrder);
			//objs.find(data.transform->name)->second->GetComponent<RocketEngine::SpriteRenderer>()->SetIsFocused(data.sprite->isFocused);
		}
		if (data.btn != nullptr)
		{
			auto component = objs.find(data.transform->name)->second->AddComponent<RocketEngine::Button>();

			component->SetHeight(data.btn->height);
			component->SetWidth(data.btn->width);

			component->SetSortOrder(data.btn->sortOrder);
			//objs.find(data.transform->name)->second->GetComponent<RocketEngine::Button>()->SetIsFocused(data.btn->isFocused);
		}
		if (data.txtBox != nullptr)
		{
			auto component = objs.find(data.transform->name)->second->AddComponent<RocketEngine::TextBox>();

			component->SetText(data.txtBox->text);
			component->SetSize(data.txtBox->width, data.txtBox->height);

			//objs.find(data.transform->name)->second->GetComponent<RocketEngine::TextBox>()->SetFontColor(data.txtBox->color);
			//objs.find(data.transform->name)->second->GetComponent<RocketEngine::TextBox>()->SetFontSize(data.txtBox->fontSize);

			objs.find(data.transform->name)->second->GetComponent<RocketEngine::TextBox>()->SetSortOrder(data.txtBox->sortOrder);
		}
		if (data.capsuleColli != nullptr)
		{
			auto component = objs.find(data.transform->name)->second->AddComponent<RocketEngine::CapsuleCollider>();

			component->SetPositionOffset(data.GetPosition());
			component->SetCapsuleInfo(data.capsuleColli->radius, data.capsuleColli->halfHeight);
			component->SetPositionOffset(data.capsuleColli->GetPositionOffset());
			component->SetRotationOffset(data.capsuleColli->GetRotationOffset());
			component->SetScaleOffset(data.capsuleColli->GetScaleOffset());
		}
		if (data.boxColli != nullptr)
		{
			auto component = objs.find(data.transform->name)->second->AddComponent<RocketEngine::BoxCollider>();
			
			component->SetVolume(data.boxColli->width, data.boxColli->height, data.boxColli->depth);
			component->SetPositionOffset(data.boxColli->GetPositionOffset());
			component->SetRotationOffset(data.boxColli->GetRotationOffset());
			component->SetScaleOffset(data.boxColli->GetScaleOffset());

		}
		if (data.sphereColli != nullptr)
		{
			auto component = objs.find(data.transform->name)->second->AddComponent<RocketEngine::SphereCollider>();

			component->SetRadius(data.sphereColli->radius);
			component->SetPositionOffset(data.sphereColli->GetPositionOffset());
			component->SetRotationOffset(data.sphereColli->GetRotationOffset());
			component->SetScaleOffset(data.sphereColli->GetScaleOffset());
		}
		if (!data.staticBoxColli.empty())
		{
			for (auto& element : data.staticBoxColli)
			{
				auto component = objs.find(data.transform->name)->second->AddComponent<RocketEngine::StaticBoxCollider>();

				component->SetVolume(element->width, element->height, element->depth);

				component->SetPositionOffset(element->GetPositionOffset());
				component->SetRotationOffset(element->GetRotationOffset());
				component->SetScaleOffset(element->GetScaleOffset());
			}

		}
		if (data.planeColli != nullptr)
		{
			auto component = objs.find(data.transform->name)->second->AddComponent<RocketEngine::PlaneCollider>();

			component->SetPositionOffset(data.planeColli->GetPositionOffset());
			component->SetRotationOffset(data.planeColli->GetRotationOffset());
			component->SetScaleOffset(data.planeColli->GetScaleOffset());
		}
		/*if (data.aniController != nullptr)
		{
			objs.find(data.transform->name)->second->AddComponent<RocketEngine::AnimationController>();
		}
		if (data.audio != nullptr)
		{
			objs.find(data.transform->name)->second->AddComponent<RocketEngine::AudioClip>();

			for (auto& sound : data.audio->audioList)
			{
				if (sound->is3D == false)
				{
					objs.find(data.transform->name)->second->GetComponent<RocketEngine::AudioClip>()->AddAudio(sound->name, sound->path, sound->type);
				}
				else if (sound->is3D == true)
				{
					float min, max;
					RocketCore::GetSoundMapFromSoundSystem().find(sound->path)->second.sound->get3DMinMaxDistance(&min, &max);
					objs.find(data.transform->name)->second->GetComponent<RocketEngine::AudioClip>()->Add3DAudio(sound->name, sound->path, sound->type, min, max);
				}
			}
		}*/
		if (data.dirLight != nullptr)
		{
			auto component = objs.find(data.transform->name)->second->AddComponent<RocketEngine::DirectionalLight>();

			component->SetAmbient(data.dirLight->ambient);
			component->SetDiffuse(data.dirLight->diffuse);
			component->SetIntensity(data.dirLight->intensity);
			component->SetSpecular(data.dirLight->specular);

		}
		if (data.spotLight != nullptr)
		{
			auto component = objs.find(data.transform->name)->second->AddComponent<RocketEngine::SpotLight>();

			component->SetAmbient(data.spotLight->ambient);
			component->SetDiffuse(data.spotLight->diffuse);
			component->SetSpecular(data.spotLight->specular);

			component->SetAttenuation({ data.spotLight->x, data.spotLight->y, data.spotLight->z });

			component->SetRange(data.spotLight->range);
			component->SetSpot(data.spotLight->spot);
		}
		if (data.pointLight != nullptr)
		{
			auto component = objs.find(data.transform->name)->second->AddComponent<RocketEngine::PointLight>();

			component->SetAmbient(data.pointLight->ambient);
			component->SetDiffuse(data.pointLight->diffuse);
			component->SetSpecular(data.pointLight->specular);

			component->SetRange(data.pointLight->range);

			component->SetAttenuation({ data.pointLight->x, data.pointLight->y, data.pointLight->z });
		}


	}

	void ScenePanel::SetObjectOption(DataStruct& data)
	{
		if (data.transform != nullptr)
		{
			for (auto& child : data.transform->child)
			{
				objs.find(child)->second->GetComponent<RocketEngine::Transform>()->SetParent(objs.find(data.transform->name)->second);
			}
		}
		if (data.boxColli != nullptr)
		{
			for (auto& serv : data.boxColli->servants)
			{
				if (objs.find(serv)->second->GetComponent<RocketEngine::CapsuleCollider>())
				{
					objs.find(data.transform->name)->second->GetComponent<RocketEngine::BoxCollider>()->SetFixedJoint(objs.find(serv)->second->GetComponent<RocketEngine::CapsuleCollider>());
				}
				else if (objs.find(serv)->second->GetComponent<RocketEngine::BoxCollider>())
				{
					objs.find(data.transform->name)->second->GetComponent<RocketEngine::BoxCollider>()->SetFixedJoint(objs.find(serv)->second->GetComponent<RocketEngine::BoxCollider>());
				}
				else if (objs.find(serv)->second->GetComponent<RocketEngine::SphereCollider>())
				{
					objs.find(data.transform->name)->second->GetComponent<RocketEngine::BoxCollider>()->SetFixedJoint(objs.find(serv)->second->GetComponent<RocketEngine::SphereCollider>());
				}
			}

		}
		if (data.sphereColli != nullptr)
		{
			for (auto& serv : data.sphereColli->servants)
			{
				if (objs.find(serv)->second->GetComponent<RocketEngine::CapsuleCollider>())
				{
					objs.find(data.transform->name)->second->GetComponent<RocketEngine::SphereCollider>()->SetFixedJoint(objs.find(serv)->second->GetComponent<RocketEngine::CapsuleCollider>());
				}
				else if (objs.find(serv)->second->GetComponent<RocketEngine::BoxCollider>())
				{
					objs.find(data.transform->name)->second->GetComponent<RocketEngine::SphereCollider>()->SetFixedJoint(objs.find(serv)->second->GetComponent<RocketEngine::BoxCollider>());
				}
				else if (objs.find(serv)->second->GetComponent<RocketEngine::SphereCollider>())
				{
					objs.find(data.transform->name)->second->GetComponent<RocketEngine::SphereCollider>()->SetFixedJoint(objs.find(serv)->second->GetComponent<RocketEngine::SphereCollider>());
				}
			}
		}
		if (data.capsuleColli != nullptr)
		{
			for (auto& serv : data.capsuleColli->servants)
			{
				if (objs.find(serv)->second->GetComponent<RocketEngine::CapsuleCollider>())
				{
					objs.find(data.transform->name)->second->GetComponent<RocketEngine::CapsuleCollider>()->SetFixedJoint(objs.find(serv)->second->GetComponent<RocketEngine::CapsuleCollider>());
				}
				else if (objs.find(serv)->second->GetComponent<RocketEngine::BoxCollider>())
				{
					objs.find(data.transform->name)->second->GetComponent<RocketEngine::CapsuleCollider>()->SetFixedJoint(objs.find(serv)->second->GetComponent<RocketEngine::BoxCollider>());
				}
				else if (objs.find(serv)->second->GetComponent<RocketEngine::SphereCollider>())
				{
					objs.find(data.transform->name)->second->GetComponent<RocketEngine::CapsuleCollider>()->SetFixedJoint(objs.find(serv)->second->GetComponent<RocketEngine::SphereCollider>());
				}
			}
		}
	}
}