#pragma once
#include "Singleton.h"
#include <vector>
#include <string>

namespace RocketEngine
{
	class Scene;
	class GameObject;
	class Component;
}

namespace RocketCore
{
	class ObjectSystem : public Singleton<ObjectSystem>
	{
		friend Singleton;
	private:
		ObjectSystem();		// 싱글턴이기 때문에 외부에서 생성할 수 없도록.

	public:
		void Initialize();
		void Finalize();

		/// 라이프 사이클 관리.
	public:
		void StartCurrentScene();

		// 현재 씬에 대해 Update를 수행한다.
		void UpdateCurrentScene();
		void LateUpdateCurrentScene();

		// 선택된 씬에 대해 Update를 수행한다.
		void UpdateScene(RocketEngine::Scene* scene);

	public:
		void FlushEnable();
		void FlushDisable();

		void AddEnable(RocketEngine::GameObject* obj);
		void AddDisable(RocketEngine::GameObject* obj);

	public:
		RocketEngine::GameObject* CreateStaticObject(std::string objName);

	private:
		std::vector<RocketEngine::GameObject*> _staticObjList;

	public:
		void AddStaticComponent(RocketEngine::Component* component);

	private:
		std::vector<RocketEngine::Component*> _staticComponentList;

	private:
		std::vector<RocketEngine::GameObject*> _enableList;
		std::vector<RocketEngine::GameObject*> _disableList;
	};
}
