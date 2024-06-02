#pragma once
#include "DLLExporter.h"
#include <unordered_map>
#include <typeinfo>
#include <string>
#include <vector>

namespace RocketCore
{
	class IComponent;
	class ObjectSystem;
	class MeshRendererBase;
	class PhysicsSystem;
}

// __declspec(dllexport) std::string;
// template class __declspec(dllexport) std::unordered_map<std::string, RocketCore::IComponent*>;

namespace RocketEngine
{
	class Transform;
}

/// <summary>
/// Component Based ���� ������ ������ Entity�� GameObject Ŭ����.
/// ��� GameObject�� Transform ������ �⺻������ ���� �ִ�.
/// ���� Component�� �ٿ��� ����Ѵ�.
/// 
/// 23.06.26 ������ �����.
/// </summary>
namespace RocketEngine
{
	class ROCKET_API GameObject final
	{
		/// GameObject�� LifeCycle�� �����ϴ� Ŭ�����鿡�� friend �ɾ���.
		friend class Scene;
		friend class RocketCore::PhysicsSystem;
		friend class RocketCore::ObjectSystem;

		/// ������, �Ҹ���
		/// Rule of Three..
	public:
		GameObject(std::string objName);
		~GameObject() = default;
		GameObject(const GameObject& rhs) = delete;
		GameObject(GameObject&& rhs) = delete;
		GameObject& operator=(const GameObject& rhs) = delete;
		GameObject& operator=(GameObject&& rhs) = delete;

		/// GameObject LifeCycle.
	private:
		void Start();
		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Finalize();

		/// GameObject EventCycle.
	private:
		void OnCollisionEnter();
		void OnCollisionStay();
		void OnCollisionExit();
		void OnDestroy();

	public:
		void Destroy();		// �Ҹ��ڸ� ����� Destroy �Լ�.

		/// Transform ������Ʈ�� ��� ���ӿ�����Ʈ�� �ʼ������� ���´�.
		/// ���ο� Transform���� ������ �� ������ ���� ����� ����.
	public:
		std::string objName;
		Transform& transform;

		/// �������� ����� ������Ʈ.
	private:
		RocketCore::MeshRendererBase* _renderer;
	
		/// GameObject�� Ȱ��,��Ȱ��ȭ ����
	public:
		void Enable();
		void Disable();
		bool IsActive() const;

		void FlushEnable();
		void FlushDisable();

	private:
		bool _isActive;

	public:
		void SetRestart();

	private:
		bool _isStarted;

		/// ������Ʈ ����
	public:
		// ������Ʈ�� �߰� �Ǵ� ȹ��
		template <typename T>
		T* AddComponent();

		template <typename T>
		T* GetComponent();

		template <typename T>
		std::vector<T*> GetComponents();

		//template <typename T>
		//bool GetComponents2(std::vector<T>& vec);
		
		template <typename T>
		T* GetComponentDynamic();
		
		template <typename T>
		std::vector<T*> GetComponentsDynamic();

		std::unordered_map<std::string, std::vector<RocketCore::IComponent*>>& GetAllComponents();

	private:
		std::unordered_map<std::string, std::vector<RocketCore::IComponent*>> _components;
	};

	/// ������Ʈ�� �߰��ϴ� �Լ�.
	/// �̹� �߰� �Ǿ��ִ� ������Ʈ��� �� �༮�� ��ȯ�Ѵ�.
	/// �ٵ� ���� ������Ʈ�� ���ͷ� �ް��ִµ� �׷��� �������� ���� �� �ִ°� �ƴѰ�?
	/// 
	/// 23.06.27 ������ �����.
	template <typename T>
	T* GameObject::AddComponent()
	{
		T* component = new T(this);
		_components[typeid(T).name()].push_back(component);
		return component;
	}
	
	/// ã�� ������Ʈ�� �����͸� "�ϳ�" ��ȯ�ϴ� �Լ�.
	/// ã�� ������Ʈ�� ���ٸ� nullptr�� ��ȯ�Ѵ�.
	/// 
	/// 23.06.27 ������ �����.
	template <typename T>
	T* GameObject::GetComponent()
	{
		auto iter = _components.find(typeid(T).name());
		if (iter != _components.end())
		{
			return dynamic_cast<T*>(iter->second.front());
		}

		return nullptr;
	}

	template <typename T>
	std::vector<T*>
		GameObject::GetComponents()
	{
		std::vector<T*> result;
		T* temp;

		auto iter = _components.find(typeid(T).name());
		if (iter != _components.end())
		{
			for (auto& component : iter->second)
			{
				temp = dynamic_cast<T*>(component);
				if (temp)
				{
					result.push_back(temp);
				}
			}
		}

		return result;
	}

	/// ����� �߰��غ������ϴ� 23.8.19.AJY
	//template <typename T>
	//bool GameObject::GetComponents2(std::vector<T>& vec)
	//{
	//	auto iter = _components.find(typeid(T).name());
	//	if (iter != _components.end())
	//	{
	//		//for (iter++ != _components.end())
	//		//{
	//		//	vec.push_back(iter->second);
	//		//}
	//		return true;
	//	}

	//	return false;
	//}

	template <typename T>
	T* GameObject::GetComponentDynamic()
	{
		T* component;
		for (auto& iter : _components)
		{
			component = dynamic_cast<T*>(iter.second.front());
			if (component)
			{
				return component;
			}
		}

		return nullptr;
	}

	template <typename T>
	std::vector<T*>
		GameObject::GetComponentsDynamic()
	{
		std::vector<T*> result;
		T* temp;

		for (auto& iter : _components)
		{
			for (auto& component : iter.second)
			{
				temp = dynamic_cast<T*>(component);
				if (temp)
				{
					result.push_back(temp);
				}
			}
		}

		return result;
	}

}
