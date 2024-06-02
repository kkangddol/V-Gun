#pragma once
#include "../RocketEngine/RocketAPI.h"

#include <unordered_map>
#include <string>

namespace SAD
{
	class Serialize;

	class EditorManager
	{
	public:
		static void SetNewPath();
		static void SaveThisSceneToEngine();
		static std::unordered_map<std::string, RocketEngine::GameObject*>& _objs;

		static bool init;
		static bool isNodeSelect;
		static bool isSceneLoad;
		static bool isDrawAll;

	private:
		static std::string GetSelectedFilePath();

		static Serialize* _serialize;
		static std::string _currentSceneName;

	public:
		// Math
		static RocketEngine::RMQuaternion EulerToQuaternion(float x, float y, float z);
		static RocketEngine::RMFLOAT3 QuaternionToEuler(float w, float x, float y, float z);

	};

}