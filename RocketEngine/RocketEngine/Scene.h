#pragma once
#include"DLLExporter.h"
#include <vector>
#include <string>
#include <functional>

// namespace RocketEngine
// {
// 	class GameObject;
// }
// 
// template class __declspec(dllexport) std::vector<RocketEngine::GameObject*>;
// __declspec(dllexport) std::string;

namespace SAD
{
	class Serialize;
}

namespace RocketEngine
{
	class GameObject;
	class Camera;
	class Serialize;
}

namespace RocketCore
{
	class UIRenderer;
}

/// <summary>
/// ������ �� Ŭ����.
/// 
/// 23.06.30 ������ �����.
/// </summary>
namespace RocketEngine
{
	class ROCKET_API Scene
	{
		/// ������, �Ҹ���.
		/// �ø������� Scene�� �����͸� �޾ƿ;� ��
		friend class SAD::Serialize;
	public:
		Scene(std::string sceneName, bool fromEditor = false);
		~Scene();


		/// Scene LifeCycle.
	public:
		// ��ϵǾ��ִ� ������ runningObject ����.
		void Initialize();

		void Start();

		// Scene�� �ִ� Object ������Ʈ.
		void Update();

		void LateUpdate();

		// Scene �� Unload �Ǹ� ȣ��. RunningList ����.
		void Finalize();

		/// Scene�� Object ����.
	public:
		GameObject* CreateObject(std::string objName);
		GameObject* CreateStaticObject(std::string objName);
		bool DeleteObject(std::string gameObjectName);


		/// mainCamera ����.
	public:
		Camera* GetMainCamera();
		void SetMainCamera(Camera* mainCamera);

	private:
		Camera* _mainCamera;


		/// SceneName ����.
	public:
		std::string GetSceneName();
		void SetSceneName(const std::string& sceneName);

	private:
		std::string _sceneName;


	public:
		GameObject* FindObjectByName(std::string name);

		/// Scene�� ��ϵǾ��ִ� Object ����.
	public:
		std::vector<GameObject*>& GetOriginalList();	// -> serialize
		std::vector<GameObject*>& GetRunningList();


	private:
		// ���� ����Ʈ
		std::vector<GameObject*> _originalList;

		// ���� ����Ʈ
		std::vector<GameObject*> _runningList;


		/// Scene�� UI ����
	public:
		void CheckFocus();

	public:
		void AddUI(RocketCore::UIRenderer* ui);

	private:
		RocketCore::UIRenderer* _focusedObj;
		std::vector<RocketCore::UIRenderer*> _uiComponents;
	};
}
 