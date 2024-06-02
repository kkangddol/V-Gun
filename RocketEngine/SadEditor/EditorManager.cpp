#include "EditorManager.h"
#include "DataTypes.h"
#include "ScenePanel.h"
#include "Serialize.h"

#include <filesystem>

namespace SAD
{
	bool EditorManager::init = false;
	bool EditorManager::isNodeSelect = false;
	bool EditorManager::isSceneLoad = false;
	bool EditorManager::isDrawAll = false;


	Serialize* EditorManager::_serialize = new Serialize;
	std::string EditorManager::_currentSceneName;
	std::unordered_map<std::string, RocketEngine::GameObject*>& EditorManager::_objs = ScenePanel::objs;

	std::string EditorManager::GetSelectedFilePath()
	{
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
			return filePath;
		}
		else
		{
			return "error";
		}
	}

	void EditorManager::SetNewPath()
	{
		Serialize::filepath =  GetSelectedFilePath();
	}

	void EditorManager::SaveThisSceneToEngine()
	{
		_serialize->SerializeSceneByObjList(ScenePanel::currentScene->GetOriginalList());
	}

	RocketEngine::RMQuaternion EditorManager::EulerToQuaternion(float x, float y, float z)
	{
		float radianX = RMConvertToRadians(x);
		float radianY = RMConvertToRadians(y);
		float radianZ = RMConvertToRadians(z);

		// 회전 각도를 반으로 나누어 준비합니다.
		float half_radianX = radianX * 0.5f;
		float half_radianY = radianY * 0.5f;
		float half_radianZ = radianZ * 0.5f;

		// 각 축의 쿼터니언 요소를 계산합니다.
		float sin_half_radianX = sin(half_radianX);
		float sin_half_radianY = sin(half_radianY);
		float sin_half_radianZ = sin(half_radianZ);
		float cos_half_radianX = cos(half_radianX);
		float cos_half_radianY = cos(half_radianY);
		float cos_half_radianZ = cos(half_radianZ);

		RocketEngine::RMQuaternion rot;

		rot.w = cos_half_radianX * cos_half_radianY * cos_half_radianZ - sin_half_radianX * sin_half_radianY * sin_half_radianZ;
		rot.x = sin_half_radianX * cos_half_radianY * cos_half_radianZ + cos_half_radianX * sin_half_radianY * sin_half_radianZ;
		rot.y = cos_half_radianX * sin_half_radianY * cos_half_radianZ - sin_half_radianX * cos_half_radianY * sin_half_radianZ;
		rot.z = cos_half_radianX * cos_half_radianY * sin_half_radianZ + sin_half_radianX * sin_half_radianY * cos_half_radianZ;

		return rot;
	}

	RocketEngine::RMFLOAT3 EditorManager::QuaternionToEuler(float w, float x, float y, float z)
	{
		// ZYX 회전 순서로 오일러 각 계산
		float yaw = std::atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));
		float pitch = std::asin(2.0f * (w * y - z * x));
		float roll = std::atan2(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));

		// 라디안을 도(degree)로 변환하여 반환
		const float toDegree = 180.0f / 3.14159265358979323846f;
		return { roll * toDegree, pitch * toDegree,  yaw * toDegree };
	}

}