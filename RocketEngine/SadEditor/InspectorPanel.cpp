#define _CRT_SECURE_NO_WARNINGS

#include "EditorManager.h"

#include "InspectorPanel.h"
#include "Serialize.h"
#include "HierarchyPanel.h"
#include "../vendor/imgui/imgui.h"

#include <iostream>
#include <string>
#include <windows.h>
#include <cassert>
#include <filesystem>

namespace SAD
{
	std::vector<DataStruct>& InspectorPanel::copiedData = Serialize::serializeData;

	InspectorPanel::InspectorPanel()
	{
		//ImGuiIO& io = ImGui::GetIO(); (void)io;

		//// 폰트 설정
		//io.Fonts->AddFontDefault();  // 기본 폰트 추가
		//io.Fonts->AddFontFromFileTTF("path/to/DroidSans.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesKorean()); // 한글 폰트 추가
	}

	InspectorPanel::~InspectorPanel()
	{

	}

	void InspectorPanel::DrawInspector()
	{
		for (auto obj : copiedData)
		{
			if (obj.transform->name == HierarchyPanel::currentNode)
			{
				DrawComponents(obj);
			}
			else
			{
				obj.transform->isNodeOpen = false;
			}
		}
	}

	void InspectorPanel::DrawComponents(DataStruct data)
	{
		ImGui::Text(data.transform->name.c_str());
		ImGui::Separator();

		if (data.transform->isNodeOpen == false)
		{
			if (data.transform != nullptr)
			{
				GetTransformData(data);
			}
			if (data.meshRenderer != nullptr)
			{
				GetMeshRendererData(data);
			}
			if (data.skinnedMeshRenderer != nullptr)
			{
				GetSkinnedMeshRendererData(data);
			}
			if (data.sprite != nullptr)
			{
				GetSpriteMeshRendererData(data);
			}
			if (data.txtBox != nullptr)
			{
				GetTextBoxData(data);
			}
			if (data.boxColli != nullptr)
			{
				GetBoxColliderData(data);
			}
			if (data.sphereColli != nullptr)
			{
				GetSphereColliderData(data);
			}
			if (data.capsuleColli != nullptr)
			{
				GetCapsuleColliderData(data);
			}
			if (!data.staticBoxColli.empty())
			{
				GetStaticBoxColliderData(data);
			}
			if (data.planeColli != nullptr)
			{
				GetPlaneColliderData(data);
			}
			if (data.btn != nullptr)
			{
				GetButtonData(data);
			}
			if (data.camera != nullptr)
			{
				GetCameraData(data);
			}
			if (data.dirLight != nullptr)
			{
				GetDirLightData(data);
			}
			if (data.spotLight != nullptr)
			{
				GetSpotLightData(data);
			}
			if (data.pointLight != nullptr)
			{
				GetPointLightData(data);
			}
			data.transform->isNodeOpen = true;
		}
		else
		{
			if (data.transform != nullptr)
			{
				DrawTransformData(data);
			}
			if (data.meshRenderer != nullptr)
			{
				DrawMeshRendererData(data);
			}
			if (data.skinnedMeshRenderer != nullptr)
			{
				DrawSkinnedMeshRendererData(data);
			}
			if (data.sprite != nullptr)
			{
				DrawSpriteRendererData(data);
			}
			if (data.txtBox != nullptr)
			{
				DrawTextBoxData(data);
			}
			if (data.boxColli != nullptr)
			{
				DrawBoxColliderData(data);
			}
			if (data.capsuleColli != nullptr)
			{
				DrawCapsuleColliderData(data);
			}
			if (data.sphereColli != nullptr)
			{
				DrawSphereColliderData(data);
			}
			if (!data.staticBoxColli.empty())
			{
				DrawStaticBoxColliderData(data);
			}
			if (data.planeColli != nullptr)
			{
				DrawPlaneColliderData(data);
			}
			if (data.btn != nullptr)
			{
				DrawButtonData(data);
			}
			if (data.camera != nullptr)
			{
				DrawCameraData(data);
			}
			if (data.dirLight != nullptr)
			{
				DrawDirLightData(data);
			}
			if (data.spotLight != nullptr)
			{
				DrawSpotLightData(data);
			}
			if (data.pointLight != nullptr)
			{
				DrawPointLightData(data);
			}
		}
	}

	void InspectorPanel::GetTransformData(DataStruct data)
	{
		ImGui::Text(data.transform->type.c_str());

		pos[0] = data.transform->position_x;
		pos[1] = data.transform->position_y;
		pos[2] = data.transform->position_z;

		q_rot[0] = data.transform->rotation_w;
		q_rot[1] = data.transform->rotation_x;
		q_rot[2] = data.transform->rotation_y;
		q_rot[3] = data.transform->rotation_z;

		euler = EditorManager::QuaternionToEuler(data.transform->rotation_w, data.transform->rotation_x, data.transform->rotation_y, data.transform->rotation_z);

		rot[0] = euler.x;
		rot[1] = euler.y;
		rot[2] = euler.z;

		sca[0] = data.transform->scale_x;
		sca[1] = data.transform->scale_y;
		sca[2] = data.transform->scale_z;

		ImGui::NewLine();
		ImGui::DragFloat3("Position", pos, 0.01);
		ImGui::DragFloat4("Q_Rotation", q_rot, 0.001, -1, 1);
		ImGui::DragFloat3("Rotation", rot, 0.01);
		ImGui::DragFloat3("Scale", sca, 0.01);
		ImGui::Separator();

	}

	void InspectorPanel::DrawTransformData(DataStruct data)
	{
		ImGui::NewLine();
		ImGui::DragFloat3("Position", pos, 0.01);
		ImGui::DragFloat4("Q_Rotation", q_rot, 0.001, -1, 1);
		ImGui::DragFloat3("Rotation", rot, 0.01);
		ImGui::DragFloat3("Scale", sca, 0.01);

		data.transform->position_x = pos[0];
		data.transform->position_y = pos[1];
		data.transform->position_z = pos[2];

		euler.x = rot[0];
		euler.y = rot[1];
		euler.z = rot[2];

		RocketEngine::RMQuaternion change = EditorManager::EulerToQuaternion(euler.x, euler.y, euler.z);

		data.transform->rotation_w = change.w;
		data.transform->rotation_x = change.x;
		data.transform->rotation_y = change.y;
		data.transform->rotation_z = change.z;

		q_rot[0] = change.w;
		q_rot[1] = change.x;
		q_rot[2] = change.y;
		q_rot[3] = change.z;

		data.transform->scale_x = sca[0];
		data.transform->scale_y = sca[1];
		data.transform->scale_z = sca[2];
		ImGui::Separator();
	}

	void InspectorPanel::GetCameraData(DataStruct data)
	{
		// 딱히.. 트랜스폼 외에 따로 띄울 게 없음
	}

	void InspectorPanel::DrawCameraData(DataStruct data)
	{
		ImGui::Text("Camera");
		ImGui::Separator();
	}


	void InspectorPanel::GetMeshRendererData(DataStruct data)
	{
		meshPath = data.meshRenderer->FBXpath;
		shaderPath = data.meshRenderer->ShaderPath;
		diffusePath = data.meshRenderer->DiffusePath;
		normalPath = data.meshRenderer->NormalPath;

		ambient[0] = data.meshRenderer->ambient.r;
		ambient[1] = data.meshRenderer->ambient.g;
		ambient[2] = data.meshRenderer->ambient.b;
		ambient[3] = data.meshRenderer->ambient.a;

		diffuse[0] = data.meshRenderer->diffuse.r;
		diffuse[1] = data.meshRenderer->diffuse.g;
		diffuse[2] = data.meshRenderer->diffuse.b;
		diffuse[3] = data.meshRenderer->diffuse.a;

		specular[0] = data.meshRenderer->specular.r;
		specular[1] = data.meshRenderer->specular.g;
		specular[2] = data.meshRenderer->specular.b;
		specular[3] = data.meshRenderer->specular.a;

		reflect[0] = data.meshRenderer->reflect.r;
		reflect[1] = data.meshRenderer->reflect.g;
		reflect[2] = data.meshRenderer->reflect.b;
		reflect[3] = data.meshRenderer->reflect.a;

		ImGui::Text(data.meshRenderer->name.c_str());

		ImGui::NewLine();

		ImGui::DragFloat4("Ambient", ambient, 0.01, 0, 1);

		ImGui::DragFloat4("Diffuse", diffuse, 0.01, 0, 1);

		ImGui::DragFloat4("Specular", specular, 0.01, 0, 1);

		ImGui::DragFloat4("Reflect", reflect, 0.01, 0, 1);

		ImGui::NewLine();

		SelectFileBtn("Mesh Path", meshPath);
		ImGui::Text(meshPath.c_str());

		SelectFileBtn("Shader Path", shaderPath);
		ImGui::Text(shaderPath.c_str());

		SelectFileBtn("Diffuse Path", diffusePath);
		ImGui::Text(diffusePath.c_str());

		SelectFileBtn("Normal Path", normalPath);
		ImGui::Text(normalPath.c_str());

		ImGui::Separator();
	}

	void InspectorPanel::DrawMeshRendererData(DataStruct data)
	{
		ImGui::Text(data.meshRenderer->name.c_str());

		ImGui::NewLine();

		ImGui::DragFloat4("Ambient", ambient, 0.01, 0, 1);

		ImGui::DragFloat4("Diffuse", diffuse, 0.01, 0, 1);

		ImGui::DragFloat4("Specular", specular, 0.01, 0, 1);

		ImGui::DragFloat4("Reflect", reflect, 0.01, 0, 1);

		ImGui::NewLine();

		SelectFileBtn("Mesh Path", meshPath);
		ImGui::Text(meshPath.c_str());

		SelectFileBtn("Shader Path", shaderPath);
		ImGui::Text(shaderPath.c_str());

		SelectFileBtn("Diffuse Path", diffusePath);
		ImGui::Text(diffusePath.c_str());

		SelectFileBtn("Normal Path", normalPath);
		ImGui::Text(normalPath.c_str());

		ImGui::Separator();

		data.meshRenderer->FBXpath = meshPath;
		data.meshRenderer->ShaderPath = shaderPath;
		data.meshRenderer->DiffusePath = diffusePath;
		data.meshRenderer->NormalPath = diffusePath;

		data.meshRenderer->ambient.r = ambient[0];
		data.meshRenderer->ambient.g = ambient[1];
		data.meshRenderer->ambient.b = ambient[2];
		data.meshRenderer->ambient.a = ambient[3];

		data.meshRenderer->diffuse.r = diffuse[0];
		data.meshRenderer->diffuse.g = diffuse[1];
		data.meshRenderer->diffuse.b = diffuse[2];
		data.meshRenderer->diffuse.a = diffuse[3];

		data.meshRenderer->specular.r = specular[0];
		data.meshRenderer->specular.g = specular[1];
		data.meshRenderer->specular.b = specular[2];
		data.meshRenderer->specular.a = specular[3];

		data.meshRenderer->reflect.r = reflect[0];
		data.meshRenderer->reflect.g = reflect[1];
		data.meshRenderer->reflect.b = reflect[2];
		data.meshRenderer->reflect.a = reflect[3];
	}

	void InspectorPanel::GetSkinnedMeshRendererData(DataStruct data)
	{
		meshPath = data.skinnedMeshRenderer->FBXpath;
		shaderPath = data.skinnedMeshRenderer->ShaderPath;
		diffusePath = data.skinnedMeshRenderer->DiffusePath;
		normalPath = data.skinnedMeshRenderer->NormalPath;

		ambient[0] = data.skinnedMeshRenderer->ambient.r;
		ambient[1] = data.skinnedMeshRenderer->ambient.g;
		ambient[2] = data.skinnedMeshRenderer->ambient.b;
		ambient[3] = data.skinnedMeshRenderer->ambient.a;

		diffuse[0] = data.skinnedMeshRenderer->diffuse.r;
		diffuse[1] = data.skinnedMeshRenderer->diffuse.g;
		diffuse[2] = data.skinnedMeshRenderer->diffuse.b;
		diffuse[3] = data.skinnedMeshRenderer->diffuse.a;

		specular[0] = data.skinnedMeshRenderer->specular.r;
		specular[1] = data.skinnedMeshRenderer->specular.g;
		specular[2] = data.skinnedMeshRenderer->specular.b;
		specular[3] = data.skinnedMeshRenderer->specular.a;

		reflect[0] = data.skinnedMeshRenderer->reflect.r;
		reflect[1] = data.skinnedMeshRenderer->reflect.g;
		reflect[2] = data.skinnedMeshRenderer->reflect.b;
		reflect[3] = data.skinnedMeshRenderer->reflect.a;


		ImGui::Text(data.skinnedMeshRenderer->name.c_str());

		ImGui::NewLine();

		ImGui::DragFloat4("Ambient", ambient, 0.01, 0, 1);

		ImGui::DragFloat4("Diffuse", diffuse, 0.01, 0, 1);

		ImGui::DragFloat4("Specular", specular, 0.01, 0, 1);

		ImGui::DragFloat4("Reflect", reflect, 0.01, 0, 1);

		ImGui::NewLine();

		SelectFileBtn("Mesh Path", meshPath);
		ImGui::Text(meshPath.c_str());

		SelectFileBtn("Shader Path", shaderPath);
		ImGui::Text(shaderPath.c_str());

		SelectFileBtn("Diffuse Path", diffusePath);
		ImGui::Text(diffusePath.c_str());

		SelectFileBtn("Normal Path", normalPath);
		ImGui::Text(normalPath.c_str());

		ImGui::Separator();
	}

	void InspectorPanel::DrawSkinnedMeshRendererData(DataStruct data)
	{
		ImGui::Text(data.skinnedMeshRenderer->name.c_str());

		ImGui::NewLine();

		ImGui::DragFloat4("Ambient", ambient, 0.01, 0, 1);

		ImGui::DragFloat4("Diffuse", diffuse, 0.01, 0, 1);

		ImGui::DragFloat4("Specular", specular, 0.01, 0, 1);

		ImGui::DragFloat4("Reflect", reflect, 0.01, 0, 1);

		ImGui::NewLine();

		SelectFileBtn("Mesh Path", meshPath);
		ImGui::Text(meshPath.c_str());

		SelectFileBtn("Shader Path", shaderPath);
		ImGui::Text(shaderPath.c_str());

		SelectFileBtn("Diffuse Path", diffusePath);
		ImGui::Text(diffusePath.c_str());

		SelectFileBtn("Normal Path", normalPath);
		ImGui::Text(normalPath.c_str());

		ImGui::Separator();

		data.skinnedMeshRenderer->FBXpath = meshPath;
		data.skinnedMeshRenderer->ShaderPath = shaderPath;
		data.skinnedMeshRenderer->DiffusePath = diffusePath;
		data.skinnedMeshRenderer->NormalPath = diffusePath;

		data.skinnedMeshRenderer->ambient.r = ambient[0];
		data.skinnedMeshRenderer->ambient.g = ambient[1];
		data.skinnedMeshRenderer->ambient.b = ambient[2];
		data.skinnedMeshRenderer->ambient.a = ambient[3];

		data.skinnedMeshRenderer->diffuse.r = diffuse[0];
		data.skinnedMeshRenderer->diffuse.g = diffuse[1];
		data.skinnedMeshRenderer->diffuse.b = diffuse[2];
		data.skinnedMeshRenderer->diffuse.a = diffuse[3];

		data.skinnedMeshRenderer->specular.r = specular[0];
		data.skinnedMeshRenderer->specular.g = specular[1];
		data.skinnedMeshRenderer->specular.b = specular[2];
		data.skinnedMeshRenderer->specular.a = specular[3];

		data.skinnedMeshRenderer->reflect.r = reflect[0];
		data.skinnedMeshRenderer->reflect.g = reflect[1];
		data.skinnedMeshRenderer->reflect.b = reflect[2];
		data.skinnedMeshRenderer->reflect.a = reflect[3];

	}

	void InspectorPanel::GetSpriteMeshRendererData(DataStruct data)
	{
		ImGui::Text(data.sprite->name.c_str());

		ImGui::NewLine();

		spritePath = data.sprite->SpritePath;
		spriteWidth = data.sprite->width;
		spriteHeight = data.sprite->height;
		spriteSortOrder = data.sprite->sortOrder;
		//spriteIsFocused = data.sprite->isFocused;

		ImGui::Text("Size");
		ImGui::SameLine();
		ImGui::DragFloat("Width", &spriteWidth, 0.01f);
		ImGui::SameLine();
		ImGui::DragFloat("Height", &spriteHeight, 0.01f);
		//ImGui::Checkbox("isFocused", &spriteIsFocused);
		ImGui::DragInt("Sort order", &spriteSortOrder);

		SelectFileBtn("Sprite Path", spritePath);
		ImGui::Text(spritePath.c_str());

		ImGui::Separator();
	}

	void InspectorPanel::DrawSpriteRendererData(DataStruct data)
	{
		ImGui::Text(data.sprite->name.c_str());
		ImGui::NewLine();

		data.sprite->SpritePath = spritePath;

		ImGui::DragFloat("Width", &spriteWidth, 0.01f);
		ImGui::DragFloat("Height", &spriteHeight, 0.01f);
		//ImGui::Checkbox("isFocused", &spriteIsFocused);
		ImGui::DragInt("Sort order", &spriteSortOrder);

		SelectFileBtn("Sprite Path", spritePath);
		ImGui::Text(spritePath.c_str());

		ImGui::Separator();

		data.sprite->SpritePath = spritePath;
		data.sprite->width = spriteWidth;
		data.sprite->height = spriteHeight;
		data.sprite->sortOrder = spriteSortOrder;
		//data.sprite->isFocused = spriteIsFocused;
	}

	void InspectorPanel::InputTextBox(std::string& originVal)
	{
		char textBuffer[256];
		strcpy(textBuffer, originVal.c_str());
		if (ImGui::InputText("Text", textBuffer, IM_ARRAYSIZE(textBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			originVal = textBuffer;
		}
	}

	void InspectorPanel::ColorTab(RocketEngine::RMFLOAT4& originColor)
	{
		if (ImGui::ColorPicker4("Text Color", (float*)&originColor))
		{
			originColor;
		}
	}

	void InspectorPanel::GetTextBoxData(DataStruct data)
	{
		text = data.txtBox->text;
		fontSize = data.txtBox->fontSize;
		//fontColor = data.txtBox->color;

		ImGui::Text(data.txtBox->name.c_str());

		InputTextBox(text);
		//ColorTab(fontColor);

		ImGui::Separator();
	}

	void InspectorPanel::DrawTextBoxData(DataStruct data)
	{
		ImGui::Text(data.txtBox->name.c_str());

		InputTextBox(text);
		//ColorTab(fontColor);

		ImGui::Separator();

		data.txtBox->text = text;
		data.txtBox->fontSize = fontSize;
		//data.txtBox->color = fontColor;
	}

	void InspectorPanel::GetButtonData(DataStruct data)
	{
		btnHeight = data.btn->height;
		btnWidth = data.btn->width;
		btnSortOrder = data.btn->sortOrder;
		//btnIsFocused = data.btn->isFocused;

		ImGui::Text(data.btn->name.c_str());
		ImGui::NewLine();

		ImGui::DragFloat("Width", &btnWidth, 0.01f);
		ImGui::DragFloat("Height", &btnHeight, 0.01f);
		//ImGui::Checkbox("isFocused", &btnIsFocused);
		ImGui::DragInt("Sort order", &btnSortOrder);

		ImGui::Separator();
	}

	void InspectorPanel::DrawButtonData(DataStruct data)
	{
		ImGui::Text(data.btn->name.c_str());
		ImGui::NewLine();

		ImGui::DragFloat("Width", &btnWidth, 0.01f);
		ImGui::DragFloat("Height", &btnHeight, 0.01f);
		//ImGui::Checkbox("isFocused", &btnIsFocused);
		ImGui::DragInt("Sort order", &btnSortOrder);

		ImGui::Separator();

		data.btn->height = btnHeight;
		data.btn->width = btnWidth;
		data.btn->sortOrder = btnSortOrder;
		//data.btn->isFocused = btnIsFocused;
	}

	void InspectorPanel::GetBoxColliderData(DataStruct data)
	{
		boxVolume[0] = data.boxColli->width;
		boxVolume[1] = data.boxColli->height;
		boxVolume[2] = data.boxColli->depth;

		colliPosOffset[0] = data.boxColli->GetPositionOffset().x;
		colliPosOffset[1] = data.boxColli->GetPositionOffset().y;
		colliPosOffset[2] = data.boxColli->GetPositionOffset().z;

		RocketEngine::RMFLOAT3 change = EditorManager::QuaternionToEuler(data.boxColli->GetRotationOffset().w, data.boxColli->GetRotationOffset().x, data.boxColli->GetRotationOffset().y, data.boxColli->GetRotationOffset().z);

		colliRotOffset[0] = change.x;
		colliRotOffset[1] = change.y;
		colliRotOffset[2] = change.z;

		colliScaOffset[0] = data.boxColli->GetScaleOffset().x;
		colliScaOffset[1] = data.boxColli->GetScaleOffset().y;
		colliScaOffset[2] = data.boxColli->GetScaleOffset().z;

		ImGui::Text(data.boxColli->name.c_str());
		ImGui::NewLine();

		ImGui::DragFloat("Width", &boxVolume[0], 0.01f);
		ImGui::DragFloat("Height", &boxVolume[1], 0.01f);
		ImGui::DragFloat("Depth", &boxVolume[2], 0.01f);

		ImGui::DragFloat3("Position Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Rotation Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Scale Offset", colliPosOffset, 0.01);


		ImGui::Separator();
	}

	void InspectorPanel::DrawBoxColliderData(DataStruct data)
	{
		ImGui::Text(data.boxColli->name.c_str());
		ImGui::NewLine();

		ImGui::DragFloat("Width", &boxVolume[0], 0.01f);
		ImGui::DragFloat("Height", &boxVolume[1], 0.01f);
		ImGui::DragFloat("Depth", &boxVolume[2], 0.01f);

		ImGui::DragFloat3("Position Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Rotation Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Scale Offset", colliPosOffset, 0.01);


		ImGui::Separator();

		data.boxColli->width = boxVolume[0];
		data.boxColli->height = boxVolume[1];
		data.boxColli->depth = boxVolume[2];

		data.boxColli->positionOffset[0] = colliPosOffset[0];
		data.boxColli->positionOffset[1] = colliPosOffset[1];
		data.boxColli->positionOffset[2] = colliPosOffset[2];

		RocketEngine::RMQuaternion change = EditorManager::EulerToQuaternion(colliRotOffset[0], colliRotOffset[1], colliRotOffset[2]);

		data.boxColli->rotationOffset[0] = change.w;
		data.boxColli->rotationOffset[1] = change.x;
		data.boxColli->rotationOffset[2] = change.y;
		data.boxColli->rotationOffset[3] = change.z;

		data.boxColli->scaleOffset[0] = colliScaOffset[0];
		data.boxColli->scaleOffset[1] = colliScaOffset[1];
		data.boxColli->scaleOffset[2] = colliScaOffset[2];
	}

	void InspectorPanel::GetSphereColliderData(DataStruct data)
	{
		capsuleRad = data.sphereColli->radius;

		colliPosOffset[0] = data.sphereColli->GetPositionOffset().x;
		colliPosOffset[1] = data.sphereColli->GetPositionOffset().y;
		colliPosOffset[2] = data.sphereColli->GetPositionOffset().z;

		colliRotOffset[0] = data.sphereColli->GetRotationOffset().x;
		colliRotOffset[1] = data.sphereColli->GetRotationOffset().y;
		colliRotOffset[2] = data.sphereColli->GetRotationOffset().z;

		colliScaOffset[0] = data.sphereColli->GetScaleOffset().x;
		colliScaOffset[1] = data.sphereColli->GetScaleOffset().y;
		colliScaOffset[2] = data.sphereColli->GetScaleOffset().z;

		ImGui::Text(data.sphereColli->name.c_str());
		ImGui::NewLine();

		ImGui::DragFloat("Radius", &sphereRad, 0.01f);

		ImGui::DragFloat3("Position Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Rotation Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Scale Offset", colliPosOffset, 0.01);

		ImGui::Separator();
	}

	void InspectorPanel::DrawSphereColliderData(DataStruct data)
	{
		ImGui::Text(data.sphereColli->name.c_str());
		ImGui::NewLine();

		ImGui::DragFloat("Radius", &sphereRad, 0.01f);

		ImGui::DragFloat3("Position Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Rotation Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Scale Offset", colliPosOffset, 0.01);

		ImGui::Separator();

		data.sphereColli->radius = capsuleRad;

		data.sphereColli->positionOffset[0] = colliPosOffset[0];
		data.sphereColli->positionOffset[1] = colliPosOffset[1];
		data.sphereColli->positionOffset[2] = colliPosOffset[2];

		RocketEngine::RMQuaternion change = EditorManager::EulerToQuaternion(colliRotOffset[0], colliRotOffset[1], colliRotOffset[2]);

		data.sphereColli->rotationOffset[0] = change.w;
		data.sphereColli->rotationOffset[1] = change.x;
		data.sphereColli->rotationOffset[2] = change.y;
		data.sphereColli->rotationOffset[3] = change.z;

		data.sphereColli->scaleOffset[0] = colliScaOffset[0];
		data.sphereColli->scaleOffset[1] = colliScaOffset[1];
		data.sphereColli->scaleOffset[2] = colliScaOffset[2];
	}

	void InspectorPanel::GetStaticBoxColliderData(DataStruct data)
	{
		for (auto& element : data.staticBoxColli)
		{
			staticBoxVol* staticBoxVolume = new staticBoxVol;

			staticBoxVolume->width = element->width;
			staticBoxVolume->height = element->height;
			staticBoxVolume->depth = element->depth;

			staticBoxVolume->colliPosOffset[0] = element->GetPositionOffset().x;
			staticBoxVolume->colliPosOffset[1] = element->GetPositionOffset().y;
			staticBoxVolume->colliPosOffset[2] = element->GetPositionOffset().z;

			RocketEngine::RMFLOAT3 change = EditorManager::QuaternionToEuler(element->GetRotationOffset().w, element->GetRotationOffset().x, element->GetRotationOffset().y, element->GetRotationOffset().z);

			staticBoxVolume->colliRotOffset[0] = change.x;
			staticBoxVolume->colliRotOffset[1] = change.y;
			staticBoxVolume->colliRotOffset[2] = change.z;

			staticBoxVolume->colliScaOffset[0] = element->GetScaleOffset().x;
			staticBoxVolume->colliScaOffset[1] = element->GetScaleOffset().y;
			staticBoxVolume->colliScaOffset[2] = element->GetScaleOffset().z;
			
			staticBoxes.push_back(staticBoxVolume);

			ImGui::Text(element->name.c_str());
			ImGui::NewLine();

			ImGui::DragFloat("Width", &staticBoxVolume->width, 0.01f);
			ImGui::DragFloat("Height", &staticBoxVolume->height, 0.01f);
			ImGui::DragFloat("Depth", &staticBoxVolume->depth, 0.01f);

			ImGui::DragFloat3("Position Offset", staticBoxVolume->colliPosOffset, 0.01);
			ImGui::DragFloat3("Rotation Offset", staticBoxVolume->colliRotOffset, 0.01);
			ImGui::DragFloat3("Scale Offset", staticBoxVolume->colliScaOffset, 0.01);

			ImGui::Separator();
		}


	}

	void InspectorPanel::DrawStaticBoxColliderData(DataStruct data)
	{
		auto itr = staticBoxes.begin();
		int count = 1;
		for (auto& element : data.staticBoxColli)
		{
			std::string num = std::to_string(count);

			ImGui::Text(element->name.c_str());
			ImGui::NewLine();

			ImGui::DragFloat(("Width##" + num).c_str(), &(*itr)->width, 0.01f);
			ImGui::DragFloat(("Height##" + num).c_str(), &(*itr)->height,0.01f);
			ImGui::DragFloat(("Depth##" + num).c_str(), &(*itr)->depth, 0.01f);

			ImGui::DragFloat3(("Position Offset##" + num).c_str(), (*itr)->colliPosOffset, 0.01);
			ImGui::DragFloat3(("Rotation Offset##" + num).c_str(), (*itr)->colliRotOffset, 0.01);
			ImGui::DragFloat3(("Scale Offset##" + num).c_str(), (*itr)->colliScaOffset, 0.01);

			ImGui::Separator();

			element->width = (*itr)->width;
			element->height = (*itr)->height;
			element->depth = (*itr)->depth;

			element->positionOffset[0] = (*itr)->colliPosOffset[0];
			element->positionOffset[1] = (*itr)->colliPosOffset[1];
			element->positionOffset[2] = (*itr)->colliPosOffset[2];

			RocketEngine::RMQuaternion change = EditorManager::EulerToQuaternion((*itr)->colliRotOffset[0] , (*itr)->colliRotOffset[1], (*itr)->colliRotOffset[2]);

			element->rotationOffset[0] = change.w;
			element->rotationOffset[1] = change.x;
			element->rotationOffset[2] = change.y;
			element->rotationOffset[3] = change.z;

			element->scaleOffset[0] = (*itr)->colliScaOffset[0];
			element->scaleOffset[1] = (*itr)->colliScaOffset[1];
			element->scaleOffset[2] = (*itr)->colliScaOffset[2];

			itr++;
			count++;
		}

	}

	void InspectorPanel::GetPlaneColliderData(DataStruct data)
	{
		colliPosOffset[0] = data.planeColli->GetPositionOffset().x;
		colliPosOffset[1] = data.planeColli->GetPositionOffset().y;
		colliPosOffset[2] = data.planeColli->GetPositionOffset().z;

		RocketEngine::RMFLOAT3 change = EditorManager::QuaternionToEuler(data.planeColli->GetRotationOffset().w, data.planeColli->GetRotationOffset().x, data.planeColli->GetRotationOffset().y, data.planeColli->GetRotationOffset().z);

		colliRotOffset[0] = change.x;
		colliRotOffset[1] = change.y;
		colliRotOffset[2] = change.z;

		colliScaOffset[0] = data.planeColli->GetScaleOffset().x;
		colliScaOffset[1] = data.planeColli->GetScaleOffset().y;
		colliScaOffset[2] = data.planeColli->GetScaleOffset().z;

		ImGui::Text(data.planeColli->name.c_str());
		ImGui::NewLine();

		ImGui::DragFloat3("Position Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Rotation Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Scale Offset", colliPosOffset, 0.01);

		ImGui::Separator();
	}

	void InspectorPanel::DrawPlaneColliderData(DataStruct data)
	{
		ImGui::Text(data.planeColli->name.c_str());
		ImGui::NewLine();

		ImGui::DragFloat3("Position Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Rotation Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Scale Offset", colliPosOffset, 0.01);

		ImGui::Separator();

		data.planeColli->positionOffset[0] = colliPosOffset[0];
		data.planeColli->positionOffset[1] = colliPosOffset[1];
		data.planeColli->positionOffset[2] = colliPosOffset[2];

		RocketEngine::RMQuaternion change = EditorManager::EulerToQuaternion(colliRotOffset[0], colliRotOffset[1], colliRotOffset[2]);

		data.planeColli->rotationOffset[0] = change.w;
		data.planeColli->rotationOffset[1] = change.x;
		data.planeColli->rotationOffset[2] = change.y;
		data.planeColli->rotationOffset[3] = change.z;

		data.planeColli->scaleOffset[0] = colliScaOffset[0];
		data.planeColli->scaleOffset[1] = colliScaOffset[1];
		data.planeColli->scaleOffset[2] = colliScaOffset[2];
	}


	void InspectorPanel::GetCapsuleColliderData(DataStruct data)
	{
		capsuleRad = data.capsuleColli->radius;
		capsuleHalfheight = data.capsuleColli->halfHeight;

		colliPosOffset[0] = data.capsuleColli->GetPositionOffset().x;
		colliPosOffset[1] = data.capsuleColli->GetPositionOffset().y;
		colliPosOffset[2] = data.capsuleColli->GetPositionOffset().z;

		RocketEngine::RMFLOAT3 change = EditorManager::QuaternionToEuler(data.capsuleColli->GetRotationOffset().w, data.capsuleColli->GetRotationOffset().x, data.capsuleColli->GetRotationOffset().y, data.capsuleColli->GetRotationOffset().z);

		colliRotOffset[0] = change.x;
		colliRotOffset[1] = change.y;
		colliRotOffset[2] = change.z;

		colliScaOffset[0] = data.capsuleColli->GetScaleOffset().x;
		colliScaOffset[1] = data.capsuleColli->GetScaleOffset().y;
		colliScaOffset[2] = data.capsuleColli->GetScaleOffset().z;

		ImGui::Text(data.capsuleColli->name.c_str());
		ImGui::NewLine();

		ImGui::SameLine();
		ImGui::DragFloat("Radius", &capsuleRad, 0.01);
		ImGui::DragFloat("Half height", &capsuleHalfheight, 0.01);

		ImGui::DragFloat3("Position Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Rotation Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Scale Offset", colliPosOffset, 0.01);

		ImGui::Separator();
	}

	void InspectorPanel::DrawCapsuleColliderData(DataStruct data)
	{

		ImGui::Text(data.capsuleColli->name.c_str());
		ImGui::NewLine();

		ImGui::SameLine();
		ImGui::DragFloat("Radius", &capsuleRad, 0.01);
		ImGui::DragFloat("Half height", &capsuleHalfheight, 0.01);

		ImGui::DragFloat3("Position Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Rotation Offset", colliPosOffset, 0.01);
		ImGui::DragFloat3("Scale Offset", colliPosOffset, 0.01);

		ImGui::Separator();

		data.capsuleColli->radius = capsuleRad;
		data.capsuleColli->halfHeight = capsuleHalfheight;

		data.capsuleColli->positionOffset[0] = colliPosOffset[0];
		data.capsuleColli->positionOffset[1] = colliPosOffset[1];
		data.capsuleColli->positionOffset[2] = colliPosOffset[2];

		RocketEngine::RMQuaternion change = EditorManager::EulerToQuaternion(colliRotOffset[0], colliRotOffset[1], colliRotOffset[2]);

		data.capsuleColli->rotationOffset[0] = change.w;
		data.capsuleColli->rotationOffset[1] = change.x;
		data.capsuleColli->rotationOffset[2] = change.y;
		data.capsuleColli->rotationOffset[3] = change.z;

		data.capsuleColli->scaleOffset[0] = colliScaOffset[0];
		data.capsuleColli->scaleOffset[1] = colliScaOffset[1];
		data.capsuleColli->scaleOffset[2] = colliScaOffset[2];
	}

	void InspectorPanel::GetDirLightData(DataStruct data)
	{
		lightIntensity = data.dirLight->intensity;

		lightAmbient[0] = data.dirLight->ambient.r;
		lightAmbient[1] = data.dirLight->ambient.g;
		lightAmbient[2] = data.dirLight->ambient.b;
		lightAmbient[3] = data.dirLight->ambient.a;

		lightDiffuse[0] = data.dirLight->diffuse.r;
		lightDiffuse[1] = data.dirLight->diffuse.g;
		lightDiffuse[2] = data.dirLight->diffuse.b;
		lightDiffuse[3] = data.dirLight->diffuse.a;

		lightSpecular[0] = data.dirLight->specular.r;
		lightSpecular[1] = data.dirLight->specular.g;
		lightSpecular[2] = data.dirLight->specular.b;
		lightSpecular[3] = data.dirLight->specular.a;

		ImGui::Text(data.dirLight->name.c_str());
		ImGui::NewLine();

		ImGui::DragFloat("Intensity", &lightIntensity, 0.01, 0, 1);
		ImGui::DragFloat4("Ambient", lightAmbient, 0.01, 0, 1);
		ImGui::DragFloat4("Diffuse", lightDiffuse, 0.01, 0, 1);
		ImGui::DragFloat4("Specular", lightSpecular, 0.01, 0, 1);

		ImGui::Separator();
	}

	void InspectorPanel::DrawDirLightData(DataStruct data)
	{
		ImGui::Text(data.dirLight->name.c_str());
		ImGui::NewLine();

		ImGui::DragFloat("Intensity", &lightIntensity, 0.01, 0, 1);
		ImGui::DragFloat4("Ambient", lightAmbient, 0.01, 0, 1);
		ImGui::DragFloat4("Diffuse", lightDiffuse, 0.01, 0, 1);
		ImGui::DragFloat4("Specular", lightSpecular, 0.01, 0, 1);

		ImGui::Separator();

		data.dirLight->intensity = lightIntensity;

		data.dirLight->ambient.r = lightAmbient[0];
		data.dirLight->ambient.g = lightAmbient[1];
		data.dirLight->ambient.b = lightAmbient[2];
		data.dirLight->ambient.a = lightAmbient[3];

		data.dirLight->diffuse.r = lightDiffuse[0];
		data.dirLight->diffuse.g = lightDiffuse[1];
		data.dirLight->diffuse.b = lightDiffuse[2];
		data.dirLight->diffuse.a = lightDiffuse[3];

		data.dirLight->specular.r = lightSpecular[0];
		data.dirLight->specular.g = lightSpecular[1];
		data.dirLight->specular.b = lightSpecular[2];
		data.dirLight->specular.a = lightSpecular[3];
	}

	void InspectorPanel::GetPointLightData(DataStruct data)
	{
		lightRange = data.pointLight->range;

		lightAttenuation[0] = data.pointLight->x;
		lightAttenuation[1] = data.pointLight->y;
		lightAttenuation[2] = data.pointLight->z;

		lightAmbient[0] = data.pointLight->ambient.r;
		lightAmbient[1] = data.pointLight->ambient.g;
		lightAmbient[2] = data.pointLight->ambient.b;
		lightAmbient[3] = data.pointLight->ambient.a;

		lightDiffuse[0] = data.pointLight->diffuse.r;
		lightDiffuse[1] = data.pointLight->diffuse.g;
		lightDiffuse[2] = data.pointLight->diffuse.b;
		lightDiffuse[3] = data.pointLight->diffuse.a;

		lightSpecular[0] = data.pointLight->specular.r;
		lightSpecular[1] = data.pointLight->specular.g;
		lightSpecular[2] = data.pointLight->specular.b;
		lightSpecular[3] = data.pointLight->specular.a;

		ImGui::Text(data.pointLight->name.c_str());
		ImGui::NewLine();

		ImGui::DragFloat("Range", &lightRange, 0.01, 0, 1);
		ImGui::DragFloat3("Attenuation", lightAttenuation, 0.01, 0, 1);

		ImGui::DragFloat4("Ambient", lightAmbient, 0.01, 0, 1);
		ImGui::DragFloat4("Diffuse", lightDiffuse, 0.01, 0, 1);
		ImGui::DragFloat4("Specular", lightSpecular, 0.01, 0, 1);

		ImGui::Separator();
	}

	void InspectorPanel::DrawPointLightData(DataStruct data)
	{
		ImGui::Text(data.pointLight->name.c_str());
		ImGui::NewLine();

		ImGui::DragFloat("Range", &lightRange, 0.01, 0, 1);
		ImGui::DragFloat3("Attenuation", lightAttenuation, 0.01, 0, 1);

		ImGui::DragFloat4("Ambient", lightAmbient, 0.01, 0, 1);
		ImGui::DragFloat4("Diffuse", lightDiffuse, 0.01, 0, 1);
		ImGui::DragFloat4("Specular", lightSpecular, 0.01, 0, 1);

		ImGui::Separator();

		data.pointLight->range = lightRange;

		data.pointLight->x = lightAttenuation[0];
		data.pointLight->y = lightAttenuation[1];
		data.pointLight->z = lightAttenuation[2];

		data.pointLight->ambient.r = lightAmbient[0];
		data.pointLight->ambient.g = lightAmbient[1];
		data.pointLight->ambient.b = lightAmbient[2];
		data.pointLight->ambient.a = lightAmbient[3];

		data.pointLight->diffuse.r = lightDiffuse[0];
		data.pointLight->diffuse.g = lightDiffuse[1];
		data.pointLight->diffuse.b = lightDiffuse[2];
		data.pointLight->diffuse.a = lightDiffuse[3];

		data.pointLight->specular.r = lightSpecular[0];
		data.pointLight->specular.g = lightSpecular[1];
		data.pointLight->specular.b = lightSpecular[2];
		data.pointLight->specular.a = lightSpecular[3];
	}

	void InspectorPanel::GetSpotLightData(DataStruct data)
	{
		lightRange = data.spotLight->range;
		lightSpot = data.spotLight->spot;

		lightAttenuation[0] = data.spotLight->x;
		lightAttenuation[1] = data.spotLight->y;
		lightAttenuation[2] = data.spotLight->z;

		lightAmbient[0] = data.spotLight->ambient.r;
		lightAmbient[1] = data.spotLight->ambient.g;
		lightAmbient[2] = data.spotLight->ambient.b;
		lightAmbient[3] = data.spotLight->ambient.a;

		lightDiffuse[0] = data.spotLight->diffuse.r;
		lightDiffuse[1] = data.spotLight->diffuse.g;
		lightDiffuse[2] = data.spotLight->diffuse.b;
		lightDiffuse[3] = data.spotLight->diffuse.a;

		lightSpecular[0] = data.spotLight->specular.r;
		lightSpecular[1] = data.spotLight->specular.g;
		lightSpecular[2] = data.spotLight->specular.b;
		lightSpecular[3] = data.spotLight->specular.a;

		ImGui::Text(data.spotLight->name.c_str());
		ImGui::NewLine();

		ImGui::DragFloat("Range", &lightRange, 0.01, 0, 1);
		ImGui::DragFloat("Spot", &lightSpot, 0.01, 0, 1);

		ImGui::DragFloat3("Attenuation", lightAttenuation, 0.01, 0, 1);

		ImGui::DragFloat4("Ambient", lightAmbient, 0.01, 0, 1);
		ImGui::DragFloat4("Diffuse", lightDiffuse, 0.01, 0, 1);
		ImGui::DragFloat4("Specular", lightSpecular, 0.01, 0, 1);

		ImGui::Separator();
	}

	void InspectorPanel::DrawSpotLightData(DataStruct data)
	{
		ImGui::Text(data.spotLight->name.c_str());
		ImGui::NewLine();

		ImGui::DragFloat("Range", &lightRange, 0.01, 0, 1);
		ImGui::DragFloat("Spot", &lightSpot, 0.01, 0, 1);

		ImGui::DragFloat3("Attenuation", lightAttenuation, 0.01, 0, 1);

		ImGui::DragFloat4("Ambient", lightAmbient, 0.01, 0, 1);
		ImGui::DragFloat4("Diffuse", lightDiffuse, 0.01, 0, 1);
		ImGui::DragFloat4("Specular", lightSpecular, 0.01, 0, 1);

		ImGui::Separator();

		data.spotLight->range = lightRange;
		data.spotLight->spot = lightSpot;

		data.spotLight->x = lightAttenuation[0];
		data.spotLight->y = lightAttenuation[1];
		data.spotLight->z = lightAttenuation[2];

		data.spotLight->ambient.r = lightAmbient[0];
		data.spotLight->ambient.g = lightAmbient[1];
		data.spotLight->ambient.b = lightAmbient[2];
		data.spotLight->ambient.a = lightAmbient[3];

		data.spotLight->diffuse.r = lightDiffuse[0];
		data.spotLight->diffuse.g = lightDiffuse[1];
		data.spotLight->diffuse.b = lightDiffuse[2];
		data.spotLight->diffuse.a = lightDiffuse[3];

		data.spotLight->specular.r = lightSpecular[0];
		data.spotLight->specular.g = lightSpecular[1];
		data.spotLight->specular.b = lightSpecular[2];
		data.spotLight->specular.a = lightSpecular[3];
	}

	void InspectorPanel::SelectFileBtn(std::string btnName, std::string& changeVal)
	{
		if (ImGui::Button(btnName.c_str()))
		{
			std::string realPath = SelectFile(changeVal);
			changeVal = realPath;
		}
		ImGui::SameLine();
	}

	// 전체 경로를 받아오기 때문에 나중에 수정해야 한다 23.08.17 오수안
	std::string InspectorPanel::SelectFile(std::string originVal)
	{
		std::string filePath;

		OPENFILENAMEW ofn;
		wchar_t szFile[260] = { 0 };

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
		ofn.lpstrFilter = L"All Files (*.*)\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameW(&ofn) == TRUE) {
			std::wstring filePathW = szFile;
			std::string filePath(filePathW.begin(), filePathW.end());
			std::filesystem::path currentPath = std::filesystem::current_path();
			std::filesystem::path relativePath = std::filesystem::relative(filePath, currentPath);
			return relativePath.string();
		}
		else
		{
			return originVal;
		}
	}
}