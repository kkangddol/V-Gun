#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include <cassert>
#include "ObjectSystem.h"
#include "InputSystem.h"
#include "UIRenderer.h"

namespace RocketEngine
{
	Scene::Scene(std::string sceneName, bool fromEditor /*= false*/)
		: _sceneName(sceneName),
		_originalList(),
		_runningList(),
		_mainCamera(nullptr)
	{
		if (!fromEditor)
		{
			GameObject* mainCameraObj = CreateObject("MainCamera");
			_mainCamera = mainCameraObj->AddComponent<Camera>();
			_mainCamera->gameObject->transform.SetPosition(0.0f, 5.0f, 20.0f);
			_mainCamera->gameObject->transform.Rotate(0.0f, 180.0f, 0.0f);
		}
	}

	Scene::~Scene()
	{
		_runningList.clear();

		for (auto& object : _originalList)
		{
			delete object;
		}

		_originalList.clear();
	}

	void Scene::Initialize()
	{
		for (auto& obj : _originalList)
		{
			obj->SetRestart();
		}
	}

	void Scene::Start()
	{
		for (auto& object : _originalList)
		{
			object->Start();
		}
	}

	void Scene::Update()
	{
		for (auto& object : _originalList)
		{
			if (object->IsActive())
			{
				object->Update();
			}
		}
	}

	void Scene::LateUpdate()
	{
		for (auto& object : _originalList)
		{
			if (object->IsActive())
			{
				object->LateUpdate();
			}
		}
	}

	void Scene::Finalize()
	{
		for (auto& obj : _originalList)
		{
			obj->Destroy();
			delete obj;
		}
	}

	RocketEngine::GameObject* Scene::CreateObject(std::string objName)
	{
		GameObject* gameObject = new GameObject(objName);
		_originalList.push_back(gameObject);
		return gameObject;
	}

	RocketEngine::GameObject* Scene::CreateStaticObject(std::string objName)
	{
		return RocketCore::ObjectSystem::Instance().CreateStaticObject(objName);
	}

	bool Scene::DeleteObject(std::string gameObjectName)
	{
		std::erase_if(_originalList, [gameObjectName](GameObject* obj) {return obj->objName == gameObjectName; });

		// 임시로 반환값 true 넣어놓음. 삭제 성공 여부에 따라 반환하도록 수정 필요. 23.8.8.AJY.
		return true;
	}

	Camera* Scene::GetMainCamera()
	{
		assert(_mainCamera);
		return _mainCamera;
	}

	void Scene::SetMainCamera(Camera* mainCamera)
	{
		_mainCamera = mainCamera;
	}

	std::string Scene::GetSceneName()
	{
		return _sceneName;
	}

	void Scene::SetSceneName(const std::string& sceneName)
	{
		_sceneName = sceneName;
	}

	RocketEngine::GameObject* Scene::FindObjectByName(std::string name)
	{
		auto iter = std::find_if(_originalList.begin(), _originalList.end(), [name](GameObject* obj) { return obj->objName == name; });
		return *iter;
	}

	std::vector<GameObject*>& Scene::GetOriginalList()
	{
		return _originalList;
	}

	// 원래 오리지널 리스트 있고, 러닝리스트로 복사해서 써야한다.
	// 아직 GameObject 복사하는 것 구현하지 않았으므로 대기
	// 23.07.20 강석원 인재원
	std::vector<GameObject*>& Scene::GetRunningList()
	{
		return _runningList;
	}

	void Scene::CheckFocus()
	{
		if (!RocketCore::InputSystem::Instance().GetKeyDown(VK_LBUTTON))
		{
			return;
		}

		_focusedObj = nullptr;

		for (auto& ui : _uiComponents)
		{
			if (!ui->gameObject->IsActive())
			{
				continue;
			}

			if (ui->CheckFocus())
			{
				if (!_focusedObj || ui->GetSortOrder() > _focusedObj->GetSortOrder())
				{
					_focusedObj = ui;
					continue;
				}
			}

			ui->SetIsFocused(false);
		}

		if (_focusedObj != nullptr)
		{
			_focusedObj->SetIsFocused(true);
			_focusedObj->OnFocusEvent();
		}
	}

	void Scene::AddUI(RocketCore::UIRenderer* ui)
	{
		_uiComponents.push_back(ui);
	}

}
