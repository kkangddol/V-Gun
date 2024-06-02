#pragma once
#include "DataTypes.h"

namespace SAD
{

	class InspectorPanel
	{
	public:
		InspectorPanel();
		~InspectorPanel();

	public:
		void DrawInspector();
		void DrawComponents(DataStruct data);
		static std::vector<DataStruct>& copiedData;


		/// componentList

	private:
		float pos[3] = {};
		float rot[3] = {};
		float sca[3] = {};

		float q_rot[4] = {};
		RocketEngine::RMFLOAT3 euler;
	public:
		void GetTransformData(DataStruct data);
		void GetCameraData(DataStruct data);
		void DrawTransformData(DataStruct data);
		void DrawCameraData(DataStruct data);

	private:
		void SelectFileBtn(std::string btnName, std::string& changeVal);
		std::string SelectFile(std::string originVal);

		std::string meshPath;
		std::string shaderPath;
		std::string diffusePath;
		std::string normalPath;

		//legacy
		float ambient[4] = {};
		float diffuse[4] = {};
		float specular[4] = {};
		float reflect[4] = {};

		//pbr
		float pbr_albedo[4] = {};
		float pbr_metalic = 0.f;
		float pbr_roughness = 0.f;
		float pbr_specular = 0.f;
		float pbr_emission = 0.f;

	public:
		void GetMeshRendererData(DataStruct data);
		void DrawMeshRendererData(DataStruct data);

	public:
		void GetSkinnedMeshRendererData(DataStruct data);
		void DrawSkinnedMeshRendererData(DataStruct data);


	private:
		std::string spritePath;
		float spriteWidth = 0.f;
		float spriteHeight = 0.f;
		int spriteSortOrder = 0;

	public:
		void GetSpriteMeshRendererData(DataStruct data);
		void DrawSpriteRendererData(DataStruct data);

	private:
		void InputTextBox(std::string& originVal);
		void ColorTab(RocketEngine::RMFLOAT4& originColor);

		//RocketEngine::RMFLOAT4 fontColor = {};
		float fontSize = 0.f;
		std::string text;
		std::string buttonFunction;
		
		float btnWidth;
		float btnHeight;
		int btnSortOrder = 0;

	public:
		void GetTextBoxData(DataStruct data);
		void DrawTextBoxData(DataStruct data);
		void GetButtonData(DataStruct data);
		void DrawButtonData(DataStruct data);
		
	private:
		float colliPosOffset[3];
		float colliRotOffset[3];
		float colliScaOffset[3];

		float capsuleRad = 0.f;
		float capsuleHalfheight = 0.f;
		std::vector<std::string> capsuleJointObj;

	public:
		void GetCapsuleColliderData(DataStruct data);
		void DrawCapsuleColliderData(DataStruct data);

	private:
		float boxVolume[3] = {};
		std::vector<std::string> boxJointObj;

	public:
		void GetBoxColliderData(DataStruct data);
		void DrawBoxColliderData(DataStruct data);

	private:
		float sphereRad = 0.f;
		std::vector<std::string> sphereJointObj;

	public:
		void GetSphereColliderData(DataStruct data);
		void DrawSphereColliderData(DataStruct data);

	private:

		struct staticBoxVol
		{
			float width;
			float height;
			float depth;
			float colliPosOffset[3];
			float colliRotOffset[3];
			float colliScaOffset[3];
		};

		std::vector<staticBoxVol*> staticBoxes;

	public:
		void GetStaticBoxColliderData(DataStruct data);
		void DrawStaticBoxColliderData(DataStruct data);

	private:
		float planeUpper[3] = {};

	public:
		void GetPlaneColliderData(DataStruct data);
		void DrawPlaneColliderData(DataStruct data);

	private:
		float lightIntensity = 0.f;
		float lightAmbient[4] = {};
		float lightDiffuse[4] = {};
		float lightSpecular[4] = {};

		float lightRange = 0.f;
		float lightSpot = 0.f;

		float lightAttenuation[3] = {};


	public:
		void GetDirLightData(DataStruct data);
		void GetPointLightData(DataStruct data);
		void GetSpotLightData(DataStruct data);
		void DrawDirLightData(DataStruct data);
		void DrawPointLightData(DataStruct data);
		void DrawSpotLightData(DataStruct data);


	public:
		// 일단 제외
		//void DrawAniControllerData(DataStruct data);
		//void DrawAudioClipData(DataStruct data);
	};

}
