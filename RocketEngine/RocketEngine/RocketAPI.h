/// Ŭ���̾�Ʈ���� ������ ����� �����ϱ� ���� �ѹ� �����Ͽ� export �ϴ� �ڵ�.
/// Ŭ���̾�Ʈ���� Open�ϰ� ���� �κи� ������ �� �ֵ��� �ߴ�.
///
/// 23.07.07 ������ �����.

#pragma once
#include "DLLExporter.h"

#include <string>
#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

// ���⿡ Ŭ���̾�Ʈ���� �� Ŭ������ �������ְ� (���������?)
/// ���� ���� ���丮
#include "EngineProcess.h"

/// ��ü ���� ���丮.
#include "GameObject.h"
#include "Scene.h"

/// ��� ����.
// ��������� cpp���� ����.
#include "eSoundGroup.h"

/// Ŭ���� export.
// ��ü�� ������Ʈ.
#include "Component.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "Transform.h"
#include "AnimationController.h"
#include "Animator.h"
#include "State.h"
#include "Tween.h"
#include "DynamicCollider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PlaneCollider.h"
#include "StaticBoxCollider.h"
#include "TextBox.h"
#include "SpriteRenderer.h"
#include "Button.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "AudioClip.h"

namespace RocketEngine
{
	/// ���� ���� ���丮.
	ROCKET_API EngineProcess* CreateLauncher();
	ROCKET_API void ReleaseLauncher(IEngineProcess* instance);

	/// ������ ���� �ε�.
	ROCKET_API void LoadSceneFromJSON(std::string sceneDataPath);

	/// ��ü ���� ���丮.
	ROCKET_API GameObject* CreateObject(const std::string& objName);
	ROCKET_API GameObject* CreateStaticObject(const std::string& objName);
	ROCKET_API Scene* CreateScene(const std::string& sceneName, bool withNoCam = false);
	ROCKET_API Tween* CreateTween();
	ROCKET_API GameObject* CreateButton(const std::string& objName);
	ROCKET_API GameObject* CreateImageBox(const std::string& objName);
	ROCKET_API GameObject* CreateTextBox(const std::string& objName);

	/// ��ü ��ȯ. (��ü�� �ּ� ��ȯ)
	ROCKET_API Camera* GetMainCamera();

	/// ���� ī�޶� ����.
	ROCKET_API void SetMainCamera(Camera* camera);

	/// ��� ����.
	ROCKET_API void RocketDestroyWindow();						// ������ ����.
	ROCKET_API bool LoadScene(const std::string& sceneName);
	ROCKET_API float GetDeltaTime();
	ROCKET_API bool GetKeyDown(int keyCode);
	ROCKET_API bool GetKeyUp(int keyCode);
	ROCKET_API bool GetKey(int keyCode);
	ROCKET_API RMFLOAT2 GetMousePosition();
	ROCKET_API RMFLOAT2 GetMouseDelta();
	ROCKET_API RMFLOAT2 MouseDeltaSmooth();
	ROCKET_API void AddStaticComponent(Component* component);
	ROCKET_API float GetMouseSensitivity();
	ROCKET_API void SetMouseSensitivity(float sensitivity);
	ROCKET_API GameObject* FindObjectByName(std::string name);

	/// ���� ��� ����.
	ROCKET_API Collider* ShootRay(RMFLOAT3 origin, RMFLOAT3 direction, float length = 100.0f, int* type = nullptr);

	/// ���� ����.
	ROCKET_API float GetGroupVolume(eSoundGroup group);
	ROCKET_API void SetGroupVolume(eSoundGroup group, float volume);
	ROCKET_API void AddAudio(const std::string& audioPath, eSoundGroup group);
	ROCKET_API void PlayAudioOnce(const std::string& audioPath, float volume);
	ROCKET_API void PlayAudioOnceNested(const std::string& audioPath, float volume);
	ROCKET_API void PlayAudioRepeat(const std::string& audioPath, float volume);
	ROCKET_API void PlayEnd(std::string soundPath);
	ROCKET_API void PlayAllEnd();

	/// ����� ����.
	//ROCKET_API void ToggleDebugMode();
	ROCKET_API void DrawDebugText(RocketEngine::RMFLOAT2 centerPos, std::string content, float size);
	ROCKET_API void DrawDebugBox(RocketEngine::RMFLOAT4X4 worldTM, RocketEngine::RMFLOAT3 widthHeightDepth = { 1.0f,1.0f,1.0f }, bool isWire = true, RocketEngine::RMFLOAT4 color = { 1.0f,0.0f,0.0f,1.0f });
	ROCKET_API void DrawDebugLine(RocketEngine::RMFLOAT3 beginPoint, RocketEngine::RMFLOAT3 endPoint, RocketEngine::RMFLOAT4 color = { 1.0f,0.0f,0.0f,1.0f });
	ROCKET_API void DrawDebug2DBox(RocketEngine::RMFLOAT2 LT, RocketEngine::RMFLOAT2 RB, RocketEngine::RMFLOAT4 color = { 1.0f,0.0f,0.0f,1.0f });


}
