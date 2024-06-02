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
		ObjectSystem();		// �̱����̱� ������ �ܺο��� ������ �� ������.

	public:
		void Initialize();
		void Finalize();

		/// ������ ����Ŭ ����.
	public:
		void StartCurrentScene();

		// ���� ���� ���� Update�� �����Ѵ�.
		void UpdateCurrentScene();
		void LateUpdateCurrentScene();

		// ���õ� ���� ���� Update�� �����Ѵ�.
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
