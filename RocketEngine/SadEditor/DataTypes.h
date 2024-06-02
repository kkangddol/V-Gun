#pragma once
#include <vector>
#include <string>
#include <unordered_map>


#include "../EngineDemo/RocketAPILoader.h"
#include "../RocketMath/RocketMath.h"

#include "../RocketEngine/eSoundGroup.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace SAD
{

	struct transfromData
	{
		std::string name;
		std::vector<std::string> child;
		std::string parent;
		std::string type;
		float position_x = 0.f;
		float position_y = 0.f;
		float position_z = 0.f;

		float rotation_w = 0.f;
		float rotation_x = 0.f;
		float rotation_y = 0.f;
		float rotation_z = 0.f;

		float scale_x = 0.f;
		float scale_y = 0.f;
		float scale_z = 0.f;

		bool isNodeOpen = false;

	};

	struct cameraData
	{
		std::string name;
		
		float _nearZ = 0.f;				// frustum의 가까운 평면까지의 거리
		float _farZ = 0.f;				// frustum의 먼 평면까지의 거리
		float _aspect = 0.f;				// 가로 / 세로 비율
		float _fovY = 0.f;				// fov각도를 60분법으로 갖고있음
		float _nearWindowHeight = 0.f;	// frustum의 가까운 평면의 높이
		float _nearWindowWidth = 0.f;		// frustum의 먼 평면의 높이

		// 행렬값
		RocketEngine::RMFLOAT4X4 _viewMatrix;		// 카메라의 로컬좌표'계'
		RocketEngine::RMFLOAT4X4 _projMatrix;		// 카메라의 투영 행렬
	};

	struct rayCastData
	{
		std::string name;
	
	};

	// object

	struct meshRendererData
	{
		std::string name;
		
		std::string FBXpath;
		std::string ShaderPath;
		std::string DiffusePath;
		std::string NormalPath;
		std::string ARMpath;

		int renderType;

		// material Data
		RocketEngine::RMFLOAT4 ambient = { 0, 0, 0, 0 };
		RocketEngine::RMFLOAT4 diffuse = { 0, 0, 0, 0 };
		RocketEngine::RMFLOAT4 specular = { 0, 0, 0, 0 };
		RocketEngine::RMFLOAT4 reflect = { 0, 0, 0, 0 };

		// pbr materia Data;
		RocketEngine::RMFLOAT4 pbr_albedo = { 0, 0, 0, 0 };
		float pbr_metalic = 0.f;
		float pbr_roughness = 0.f;
		float pbr_specular = 0.f;
		float pbr_emission = 0.f;
	};

	struct SkinnedMeshRendererData
	{
		std::string name;
		
		std::string FBXpath;
		std::string ShaderPath;
		std::string DiffusePath;
		std::string NormalPath;

		// material Data
		RocketEngine::RMFLOAT4 ambient = { 0, 0, 0, 0 };
		RocketEngine::RMFLOAT4 diffuse = { 0, 0, 0, 0 };
		RocketEngine::RMFLOAT4 specular = { 0, 0, 0, 0 };
		RocketEngine::RMFLOAT4 reflect = { 0, 0, 0, 0 };

		// pbr materia Data;
		RocketEngine::RMFLOAT4 pbr_albedo = { 0, 0, 0, 0 };
		float pbr_metalic = 0.f;
		float pbr_roughness = 0.f;
		float pbr_specular = 0.f;
		float pbr_emission = 0.f;
	};

	// UI

	struct spriteRendererData
	{
		std::string name;
		
		std::string SpritePath;
		float width = 0.f;
		float height = 0.f;
		int sortOrder = 0;	
	};

	struct textBoxData
	{
		std::string name;
		
		std::string text;
		float fontSize = 0.f;
		RocketEngine::RMFLOAT4 color = {};

		float width = 0.f;
		float height = 0.f;
		int sortOrder = 0;
	};

	struct buttonData
	{
		std::string name;
	
		// size
		float width = 0.f;
		float height = 0.f;
		int sortOrder = 0.f;		
		bool isFocused = 0.f;

		/// 함수 포인터 시리얼라이즈 방식 고민해볼 것 2023.08.14 오수안 && 강석원
		//std::function<void()> onClickEvent;
	};

	struct boxColliderData
	{
		std::string name;
		
		std::vector<std::string> servants;

		float positionOffset[3] = {};
		float rotationOffset[4] = {};
		float scaleOffset[3] = {};

		float width = 0.f;
		float height = 0.f;
		float depth = 0.f;

		RocketEngine::RMFLOAT3 GetPositionOffset();
		RocketEngine::RMQuaternion GetRotationOffset();
		RocketEngine::RMFLOAT3 GetScaleOffset();
	};

	struct capsuleColliderData
	{
		std::string name;

		std::vector<std::string> servants;

		float positionOffset[3] = {};
		float rotationOffset[4] = {};
		float scaleOffset[3] = {};

		float radius = 0.f;
		float halfHeight = 0.f;

		RocketEngine::RMFLOAT3 GetPositionOffset();
		RocketEngine::RMQuaternion GetRotationOffset();
		RocketEngine::RMFLOAT3 GetScaleOffset();
	};

	struct sphereColliderData
	{
		std::string name;
		
		std::vector<std::string> servants;

		float positionOffset[3] = {};
		float rotationOffset[4] = {};
		float scaleOffset[3] = {};

		float radius = 0.f;

		RocketEngine::RMFLOAT3 GetPositionOffset();
		RocketEngine::RMQuaternion GetRotationOffset();
		RocketEngine::RMFLOAT3 GetScaleOffset();
	};

	struct planeColliderData
	{
		std::string name;

		float positionOffset[3] = {};
		float rotationOffset[4] = {};
		float scaleOffset[3] = {};

		RocketEngine::RMFLOAT3 GetPositionOffset();
		RocketEngine::RMQuaternion GetRotationOffset();
		RocketEngine::RMFLOAT3 GetScaleOffset();
	};

	struct staticBoxColliderData
	{
		std::string name;

		float positionOffset[3] = {};
		float rotationOffset[4] = {};
		float scaleOffset[3] = {};

		float width = 0.f;
		float height = 0.f;
		float depth = 0.f;

		RocketEngine::RMFLOAT3 GetPositionOffset();
		RocketEngine::RMQuaternion GetRotationOffset();
		RocketEngine::RMFLOAT3 GetScaleOffset();
	};

	struct animationData
	{
		std::string name;
	
		std::string stateName;
		std::string aniDescName;
		std::string modelPath;
		std::string animationPath;
		bool isLoop = false;
	};

	struct animationControllerData
	{
		std::string name;
		
		std::vector<animationData*> states;
		std::unordered_map<std::string, bool> conditions;
	};

	struct audioData
	{
		std::string name;
		std::string path;
		bool is3D;
		eSoundGroup type;
		float volume = 0.f;
	};

	struct audioClipData
	{
		std::string name;
	
		std::vector<audioData*> audioList;
	};

	//light

	struct directionalLightData
	{
		std::string name;
		
		float intensity = 0.f;

		// base
		RocketEngine::RMFLOAT4 ambient = { 0, 0, 0, 0 };
		RocketEngine::RMFLOAT4 diffuse = { 0, 0, 0, 0 };
		RocketEngine::RMFLOAT4 specular = { 0, 0, 0, 0 };

	};

	struct pointLightData
	{
		std::string name;
		
		float range = 0.f;

		//해당 거리를 넘어서면 빛을 내지 못함
		float x= 0.f;
		float y= 0.f;
		float z= 0.f;

		// base
		RocketEngine::RMFLOAT4 ambient = { 0, 0, 0, 0 };
		RocketEngine::RMFLOAT4 diffuse = { 0, 0, 0, 0 };
		RocketEngine::RMFLOAT4 specular = { 0, 0, 0, 0 };
	};

	struct spotLightData
	{
		std::string name;
	
		float range = 0.f;
		float spot = 0.f;


		//해당 거리를 넘어서면 빛을 내지 못함
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;

		// base
		RocketEngine::RMFLOAT4 ambient = { 0, 0, 0, 0 };
		RocketEngine::RMFLOAT4 diffuse = { 0, 0, 0, 0 };
		RocketEngine::RMFLOAT4 specular = { 0, 0, 0, 0 };
	};

	struct DataStruct
	{
		transfromData* transform = nullptr;
		cameraData* camera = nullptr;
		rayCastData* raycast = nullptr;
		
		meshRendererData* meshRenderer = nullptr;
		SkinnedMeshRendererData* skinnedMeshRenderer = nullptr;
		
		spriteRendererData* sprite = nullptr;
		buttonData* btn = nullptr;
		textBoxData* txtBox = nullptr;

		animationControllerData* aniController = nullptr;
		audioClipData* audio = nullptr;


		std::vector<staticBoxColliderData*> staticBoxColli;
		boxColliderData* boxColli = nullptr;
		capsuleColliderData* capsuleColli = nullptr;
		sphereColliderData* sphereColli = nullptr;
		planeColliderData* planeColli = nullptr;
		//staticBoxColliderData* staticBoxColli = nullptr;

		directionalLightData* dirLight = nullptr;
		spotLightData* spotLight = nullptr;
		pointLightData* pointLight = nullptr;

		RocketEngine::RMFLOAT3 GetPosition();
		RocketEngine::RMQuaternion GetRotation();
		RocketEngine::RMFLOAT3 GetScale();

	};
}
