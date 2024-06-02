#include "ObjectSystem.h"
#include "SceneSystem.h"
#include "Scene.h"
#include <cassert>
#include "GameObject.h"
#include "Component.h"
#include <algorithm>

namespace RocketCore
{
	ObjectSystem::ObjectSystem()
	{

	}

	void ObjectSystem::Initialize()
	{
		_enableList.clear();
		_disableList.clear();
	}

	void ObjectSystem::Finalize()
	{
		RocketEngine::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();

		for (auto& staticObj : _staticObjList)
		{
			staticObj->Finalize();
		}

		for (auto& staticComponent : _staticComponentList)
		{
			staticComponent->Finalize();
		}

		for (auto& iter : SceneSystem::Instance().GetAllScenes())
		{
			for (auto& gameObject : iter.second->GetOriginalList())
			{
				gameObject->Finalize();
			}
		}
	}

	void ObjectSystem::StartCurrentScene()
	{
		RocketEngine::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();
		//assert(currentScene);

		if (currentScene == nullptr)
		{
			return;
		}

		for (auto& staticObj : _staticObjList)
		{
			staticObj->Start();
		}

		for (auto& staticComponent : _staticComponentList)
		{
			if (!staticComponent->_isStarted)
			{
				staticComponent->Start();
				staticComponent->_isStarted = true;
			}
		}

		currentScene->Start();
	}

	void ObjectSystem::UpdateCurrentScene()
	{
		RocketEngine::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();

		if (currentScene == nullptr)
		{
			return;
		}

		for (auto& staticObj : _staticObjList)
		{
			staticObj->Update();
		}

		for (auto& staticComponent : _staticComponentList)
		{
			staticComponent->Update();
		}

		currentScene->Update();
	}

	void ObjectSystem::LateUpdateCurrentScene()
	{
		RocketEngine::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();

		if (currentScene == nullptr)
		{
			return;
		}

		for (auto& staticObj : _staticObjList)
		{
			staticObj->LateUpdate();
		}

		for (auto& staticComponent : _staticComponentList)
		{
			staticComponent->LateUpdate();
		}

		currentScene->LateUpdate();
	}

	void ObjectSystem::UpdateScene(RocketEngine::Scene* scene)
	{

	}

	void ObjectSystem::FlushEnable()
	{
		for (auto obj : _enableList)
		{
			obj->FlushEnable();
		}

		_enableList.clear();
	}

	void ObjectSystem::FlushDisable()
	{
		for (auto obj : _disableList)
		{
			obj->FlushDisable();
		}

		_disableList.clear();
	}

	void ObjectSystem::AddEnable(RocketEngine::GameObject* obj)
	{
		_enableList.push_back(obj);
	}

	void ObjectSystem::AddDisable(RocketEngine::GameObject* obj)
	{
		_disableList.push_back(obj);
	}

	RocketEngine::GameObject* ObjectSystem::CreateStaticObject(std::string objName)
	{
		RocketEngine::GameObject* gameObject = new RocketEngine::GameObject(objName);
		_staticObjList.push_back(gameObject);
		return gameObject;
	}

	void ObjectSystem::AddStaticComponent(RocketEngine::Component* component)
	{
		if (std::find(_staticComponentList.begin(), _staticComponentList.end(), component) != _staticComponentList.end())
		{
			return;
		}

		_staticComponentList.push_back(component);
	}

}
