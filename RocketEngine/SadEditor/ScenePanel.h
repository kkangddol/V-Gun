#pragma once
#include "../RocketEngine/RocketAPI.h"

#include <vector>
#include <unordered_map>
#include "DataTypes.h"

class CameraMove;

namespace SAD
{
	class ScenePanel
	{

	public:
		ScenePanel();
		~ScenePanel();
		static std::vector<DataStruct>& copiedData;
		static RocketEngine::Scene* currentScene;

	private:
		float moveSpeed = 10.f;
		float deltaTime;
		int previousMouseX = 0;
		int previousMouseY = 0;
	public:
		void SceneInitialize();
		
		void SceneInput();
		void SceneRun();
		void MouseMove();

		void CreateNewObjectAndComponent(DataStruct& data);
		void SetObjectOption(DataStruct& data);

	private:
		friend class EditorManager;
		static std::unordered_map<std::string,RocketEngine::GameObject*> objs;
	};
}