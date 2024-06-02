#include "GameObject.h"

#include "Transform.h"
#include "MeshRendererBase.h"
#include "ObjectSystem.h"

namespace RocketEngine
{
	// ������ ���� ������ ��.
	// transform�� ���� �ʱ�ȭ ���� ������ Object �ʱ�ȭ�� �� ���� ����.
	GameObject::GameObject(std::string objName)
		:transform(*(new Transform(this))),
		_renderer(nullptr),
		objName(objName),
		_isActive(true),
		_isStarted(false),
		_components()
	{
		_components[typeid(transform).name()].push_back(&transform);
	}

	void GameObject::Destroy()
	{
		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				delete component;
			}
		}
	}

	void GameObject::Start()
	{
		if (!_isActive)
		{
			return;
		}

		if (_isStarted)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->Start();
				dynamic_cast<Component*>(component)->_isStarted = true;
			}
		}

		_isStarted = true;
	}

	void GameObject::FixedUpdate()
	{
		if (!_isActive)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->FixedUpdate();
			}
		}
	}

	void GameObject::Update()
	{
		if (!_isActive)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->Update();
			}
		}
	}

	void GameObject::LateUpdate()
	{
		if (!_isActive)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->LateUpdate();
			}
		}
	}

	void GameObject::Finalize()
	{
		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->Finalize();
			}
		}
	}

	void GameObject::OnCollisionEnter()
	{
		if (!_isActive)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->OnCollisionEnter();
			}
		}
	}

	void GameObject::OnCollisionStay()
	{
		if (!_isActive)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->OnCollisionStay();
			}
		}
	}

	void GameObject::OnCollisionExit()
	{
		if (!_isActive)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->OnCollisionExit();
			}
		}
	}

	void GameObject::OnDestroy()
	{
		if (!_isActive)
		{
			return;
		}

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				component->OnDestroy();
			}
		}
	}

// 	RocketCore::Graphics::RenderData* GameObject::GetRenderData()
// 	{
// 		if (_renderer)
// 		{
// 			return _renderer->GetData();
// 		}
// 
// 		for (auto& iter : _components)
// 		{
// 			if (dynamic_cast<RocketCore::Renderer*>(iter.second))
// 			{
// 				_renderer = dynamic_cast<RocketCore::Renderer*>(iter.second);
// 				return _renderer->GetData();
// 			}
// 		}
// 
// 		return nullptr;
// 	}

	void GameObject::Enable()
	{
		RocketCore::ObjectSystem::Instance().AddEnable(this);
	}

	void GameObject::Disable()
	{
		RocketCore::ObjectSystem::Instance().AddDisable(this);
	}

	bool GameObject::IsActive() const
	{
		return _isActive;
	}

	void GameObject::FlushEnable()
	{
		_isActive = true;
		for (auto& child : transform.GetChildrenVec())
		{
			child->gameObject->FlushEnable();
		}
	}

	void GameObject::FlushDisable()
	{
		/*if (this == nullptr)
			return;*/

		_isActive = false;
		for (auto& child : transform.GetChildrenVec())
		{
			child->gameObject->FlushDisable();
		}
	}

	void GameObject::SetRestart()
	{
		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				dynamic_cast<Component*>(component)->SetRestart();
			}
		}

		_isStarted = false;
	}

	std::unordered_map<std::string, std::vector<RocketCore::IComponent*>>& GameObject::GetAllComponents()
	{
		return _components;
	}

}
