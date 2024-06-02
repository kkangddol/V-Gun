#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <string>
#include <cassert>

namespace RocketEngine
{
	class Scene;
}

namespace RocketCore
{
	class SceneSystem : public Singleton<SceneSystem>
	{
		friend Singleton;
	private:
		SceneSystem();		// �̱����̱� ������ �ܺο��� ������ �� ������.

	public:
		void Initialize();
		void Finalize();
		void UpdateLoadScene();

	public:
		RocketEngine::Scene* GetCurrentScene();
		std::unordered_map<std::string, RocketEngine::Scene*>& GetAllScenes();

		/// RocketAPI ���� �����ؼ� ����ϴ� �Լ���.
	public:
		RocketEngine::Scene* CreateScene(const std::string& sceneName, bool withNoCam = false);		
		bool LoadScene(const std::string& sceneName);

	private:
		RocketEngine::Scene* _loadScene;
		RocketEngine::Scene* _currentScene;
		std::unordered_map<std::string, RocketEngine::Scene*> _sceneMap;
	};
}
