#pragma once
#include <map>
#include <string>
#include "../RocketGraphicsInterface/AnimData.h"
#include "SimpleMath.h"

namespace RocketCore::Graphics
{
	class RenderAnimation;
	class SkinnedMeshFormation; //자신이 소속되어 있는 곳의 Formation을 받아간다.
	class SkinnedMeshObject; //자신이 소속되어 있는 곳의 Formation을 받아간다.

	class AnimationHandler
	{
		friend class SkinnedMeshObject;
		using Matrix = DirectX::SimpleMath::Matrix;
	public:
		AnimationHandler(SkinnedMeshObject* obj);
		~AnimationHandler();
		
		//Handler의 animMap에 Animation 정보를 넣는다.
		void InsertAnimationWithName(const std::string& _name, RenderAnimation* _anim);
		
		//로딩된 애니메이션 정보를 기반으로 RenderUsageBone을 연동시켜준다. 
		//Node를 기반으로 애니메이션을 진행될 것이고,
		//Mapping된 Bone이 있으면 이를 따르고, 아니면 ParRelativeTransform만을 반영.


		void Play(AnimData anim);
		RenderAnimation* _nowPlayingAnim = nullptr;

	private:
		////자신이 소속되어 있는 Formation.
		//SkinnedMeshFormation* _belongForm;

		//자신이 소속되어 있는 BaseRenderable.
		SkinnedMeshObject* _belongSkinnedObject = nullptr;

		//얘는 Animation Name으로 관리!
		std::map<std::string, RenderAnimation*> animMap;

		//계속 Node를 파싱하는 것은 힘들다. 
		std::string _nowAnimDataName;
	};
}


