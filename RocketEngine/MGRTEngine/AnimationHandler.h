#pragma once
#include <map>
#include <string>
#include "../RocketGraphicsInterface/AnimData.h"
#include "SimpleMath.h"

namespace RocketCore::Graphics
{
	class RenderAnimation;
	class SkinnedMeshFormation; //�ڽ��� �ҼӵǾ� �ִ� ���� Formation�� �޾ư���.
	class SkinnedMeshObject; //�ڽ��� �ҼӵǾ� �ִ� ���� Formation�� �޾ư���.

	class AnimationHandler
	{
		friend class SkinnedMeshObject;
		using Matrix = DirectX::SimpleMath::Matrix;
	public:
		AnimationHandler(SkinnedMeshObject* obj);
		~AnimationHandler();
		
		//Handler�� animMap�� Animation ������ �ִ´�.
		void InsertAnimationWithName(const std::string& _name, RenderAnimation* _anim);
		
		//�ε��� �ִϸ��̼� ������ ������� RenderUsageBone�� ���������ش�. 
		//Node�� ������� �ִϸ��̼��� ����� ���̰�,
		//Mapping�� Bone�� ������ �̸� ������, �ƴϸ� ParRelativeTransform���� �ݿ�.


		void Play(AnimData anim);
		RenderAnimation* _nowPlayingAnim = nullptr;

	private:
		////�ڽ��� �ҼӵǾ� �ִ� Formation.
		//SkinnedMeshFormation* _belongForm;

		//�ڽ��� �ҼӵǾ� �ִ� BaseRenderable.
		SkinnedMeshObject* _belongSkinnedObject = nullptr;

		//��� Animation Name���� ����!
		std::map<std::string, RenderAnimation*> animMap;

		//��� Node�� �Ľ��ϴ� ���� �����. 
		std::string _nowAnimDataName;
	};
}


