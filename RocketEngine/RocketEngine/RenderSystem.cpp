#include "RenderSystem.h"
#include "MeshRendererBase.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include <cassert>
#include "SceneSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "..\\RocketGraphicsInterface\\IRenderable.h"
#include "AnimationController.h"
#include "Animator.h"
#include "State.h"
#include "..\\RocketGraphicsInterface\\RenderConstantData.h"
#include "TimeSystem.h"
#include "PhysicsSystem.h"
#include "DynamicCollider.h"
#include "BoxCollider.h"
#include "..\\RocketMath\\RocketMath.h"
#include "TextBox.h"
#include "SpriteRenderer.h"
#include "..\\RocketGraphicsInterface\\ISketchable.h"
#include "DebugSystem.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <algorithm>

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

#pragma comment(lib,"..\\x64\\Debug\\RocketUtil.lib")

using GRAPHICS_CREATE_SIGNATURE = RocketCore::Graphics::IRocketGraphics* (*)(void);
constexpr const char* GRAPHICS_CREATE_NAME = "CreateGraphicsInstance";

#define GRAPHICS_RELEASE_SIGNATURE void(*)(RocketCore::Graphics::IRocketGraphics*)
#define GRAPHICS_RELEASE_NAME "ReleaseGraphicsInstance"

namespace RocketCore
{
	RenderSystem::RenderSystem()
		:_hWnd(), _screenWidth(), _screenHeight(),
		hGraphicsModule(LoadLibrary(GRAPHICSDLL_PATH)),
		_rocketGraphics(),
		_renderMap(),
		_uiMap()
	{
		DWORD error_code = GetLastError();
		assert(hGraphicsModule);

		auto a = GetProcAddress(hGraphicsModule, GRAPHICS_CREATE_NAME);
		error_code = GetLastError();

		_rocketGraphics.reset((reinterpret_cast<GRAPHICS_CREATE_SIGNATURE>(GetProcAddress(hGraphicsModule, GRAPHICS_CREATE_NAME)))());

		//extern "C" __declspec(dllexport) IRocketGraphics * CreateGraphicsInstance();
		//extern "C" __declspec(dllexport) void ReleaseGraphicsInstance(IRocketGraphics * instance);
	}

	void RenderSystem::Initialize(HWND hWnd, int screenWidth, int screenHeight, bool isEditor /*= false*/)
	{
		_hWnd = hWnd;
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_isEditor = isEditor;

		_rocketGraphics->Initialize(static_cast<void*>(hWnd), screenWidth, screenHeight, isEditor);

		MakeRenderableAll();
		MakeAnimationAll();
		MakeUIAll();

		///[TW] 라이트가 어디로 가야 하는지 몰라.. 여기에다가 놓습니다!
		RocketEngine::GameObject* dirLightObj = SceneSystem::Instance().GetCurrentScene()->CreateObject("dirLight1");
		RocketEngine::DirectionalLight* dirLight = dirLightObj->AddComponent<RocketEngine::DirectionalLight>();
		dirLight->SetAmbient(RocketEngine::RMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		dirLight->SetDiffuse(RocketEngine::RMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		dirLight->SetIntensity(0.33f);
		dirLight->SetSpecular(RocketEngine::RMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		dirLightObj->transform.Rotate(30.0f, 0.0f, 0.0f);

		RocketEngine::GameObject* dirLightObj2 = SceneSystem::Instance().GetCurrentScene()->CreateObject("dirLight2");
		RocketEngine::DirectionalLight* dirLight2 = dirLightObj2->AddComponent<RocketEngine::DirectionalLight>();
		dirLight2->SetAmbient(RocketEngine::RMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		dirLight2->SetDiffuse(RocketEngine::RMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		dirLight2->SetIntensity(0.33f);
		dirLight2->SetSpecular(RocketEngine::RMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		dirLightObj2->transform.Rotate(30.0f, 120.0f, 0.0f);

		RocketEngine::GameObject* dirLightObj3 = SceneSystem::Instance().GetCurrentScene()->CreateObject("dirLight3");
		RocketEngine::DirectionalLight* dirLight3 = dirLightObj3->AddComponent<RocketEngine::DirectionalLight>();
		dirLight3->SetAmbient(RocketEngine::RMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		dirLight3->SetDiffuse(RocketEngine::RMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		dirLight3->SetIntensity(0.33f);
		dirLight3->SetSpecular(RocketEngine::RMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		dirLightObj3->transform.Rotate(30.0f, 240.0f, 0.0f);

		_lights.push_back(dirLight);
		_lights.push_back(dirLight2);
		_lights.push_back(dirLight3);
	}

	void RenderSystem::Finalize()
	{
		reinterpret_cast<GRAPHICS_RELEASE_SIGNATURE>(GetProcAddress(hGraphicsModule, GRAPHICS_RELEASE_NAME))(_rocketGraphics.release());
		FreeLibrary(hGraphicsModule);
	}

	void RenderSystem::MakeRenderableAll()
	{
		for (auto& sceneIter : SceneSystem::Instance().GetAllScenes())
		{
			if (sceneIter.second == nullptr)
			{
				return;
			}

			for (auto& object : sceneIter.second->GetOriginalList())
			{
				// 각각의 객체가 본인들을 그리는 것이 아닌
				// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
				// object->GetRenderData();

				MeshRendererBase* renderer = object->GetComponent<RocketEngine::MeshRenderer>();

				if (!renderer)
				{
					renderer = object->GetComponent<RocketEngine::SkinnedMeshRenderer>();

					if (!renderer)
					{
						continue;
					}
				}

				if (_renderMap.find(renderer) == _renderMap.end())
				{
					if (dynamic_cast<RocketEngine::MeshRenderer*>(renderer))
					{
						RocketCore::Graphics::IRenderable* renderableObj = _rocketGraphics->MakeRenderable(renderer->GetModelData());
						_renderMap.insert({ renderer, renderableObj });
					}

					if (dynamic_cast<RocketEngine::SkinnedMeshRenderer*>(renderer))
					{
						RocketCore::Graphics::IRenderable* renderableObj = _rocketGraphics->MakeSkinnedRenderable(renderer->GetModelData());
						_renderMap.insert({ renderer, renderableObj });
					}
				}
			}
		}
	}

	void RenderSystem::MakeAnimationAll()
	{
		for (auto& sceneIter : SceneSystem::Instance().GetAllScenes())
		{
			if (sceneIter.second == nullptr)
			{
				return;
			}

			for (auto& object : sceneIter.second->GetOriginalList())
			{
				// 각각의 객체가 본인들을 그리는 것이 아닌
				// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
				// object->GetRenderData();

				RocketEngine::AnimationController* animCtr = object->GetComponent<RocketEngine::AnimationController>();

				if (!animCtr)
				{
					continue;
				}

				for (auto& iter : animCtr->GetAnimator()->GetAllStates())
				{
					_rocketGraphics->MakeAnim(iter.second->GetAnimDesc(), &(iter.second->GetAnimDataRef()));
				}
			}
		}
	}

	void RenderSystem::MakeUIAll()
	{
		for (auto& sceneIter : SceneSystem::Instance().GetAllScenes())
		{
			if (sceneIter.second == nullptr)
			{
				return;
			}

			for (auto& object : sceneIter.second->GetOriginalList())
			{
				// 각각의 객체가 본인들을 그리는 것이 아닌
				// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
				// object->GetRenderData();

				std::vector<UIRenderer*> uiVec = object->GetComponentsDynamic<UIRenderer>();

				for (auto& ui : uiVec)
				{
					if (_uiMap.find(ui) == _uiMap.end())
					{
						RocketEngine::TextBox* tr = nullptr;
						if (tr = dynamic_cast<RocketEngine::TextBox*>(ui))
						{
							RocketCore::Graphics::ISketchable* sketchableObj = _rocketGraphics->MakeText(tr->GetFontSize(), tr->GetWidth(), tr->GetHeight(), tr->GetColor());
							_uiMap.insert({ ui, sketchableObj });
						}

						RocketEngine::SpriteRenderer* sr = nullptr;
						if (sr = dynamic_cast<RocketEngine::SpriteRenderer*>(ui))
						{
							RocketCore::Graphics::ISketchable* sketchableObj = _rocketGraphics->MakeImage(sr->GetPath().c_str(), sr->GetWidth(), sr->GetHeight());
							_uiMap.insert({ ui, sketchableObj });
						}
					}
				}
			}
		}
	}

	void RenderSystem::OnResize(int width, int height)
	{
		_rocketGraphics->Resize(width, height);
	}

	void RenderSystem::DrawProcess()
	{
		//[TW] Render Pipeline Tweaking, 손을 댑니다! 작동하지만, 함수 안에서 마개조 될 것.
		//[TW] Deferred, Forward 혼합 중, 230814

		BeginDraw(); //SetRenderData + 전체 렌더 시작,

		_rocketGraphics->BeginDeferredPhongMeshRender();
		DrawCurrentScene(Graphics::eRenderType::DEFERRED_PHONG); // MeshRenderable / SkinnedMeshRenderable 렌더.
		_rocketGraphics->EndDeferredPhongMeshRender();

		_rocketGraphics->BeginDeferredPBRMeshRender();
		DrawCurrentScene(Graphics::eRenderType::DEFERRED_PBR); // PBR : MeshRenderable  렌더.
		_rocketGraphics->EndDeferredPBRMeshRender();

		_rocketGraphics->BeginDeferredPBRSkinnedMeshRender();
		DrawCurrentScene(Graphics::eRenderType::DEFERRED_PBR_SKINNED); // PBR : SkinnedMeshRenderable 렌더.
		_rocketGraphics->EndDeferredPBRSkinnedMeshRender();

		_rocketGraphics->BeginForwardMeshRender(); // Mesh 렌더 시작.
		DrawCurrentScene(Graphics::eRenderType::FORWARD); // MeshRenderable / SkinnedMeshRenderable 렌더.
		if (DebugSystem::Instance().IsDebugMode() || _isEditor)
		{
			_rocketGraphics->RenderAxisGrid(); // 일단 Axis와 Grid를 렌더함! (일반적인 Mesh로 취급, 동적인 버퍼들이 아니다)
		}
		_rocketGraphics->EndForwardMeshRender(); // Mesh 렌더 끝.

		//Forward <-> Deferred 합치는 용도.
		_rocketGraphics->CombineRenderTargets();
		///얘가 발동되면 Graphic Debugger가 KernelBase.dll 오류를 낸다. 왜?

		///파티클이 들어간다면 이 레이어에 들어가야 한다.
	
		///PostProcessing이 들어간다면 이 레이어에 들어가야 한다.

		_rocketGraphics->BeginDebugRender();
		DrawDebugBox();
		//RenderPhysicsDebug(); //현재로는 이 함수에서 동적인 Primitive (Cube / Sphere) 만 호출 중.
// 		_rocketGraphics->RenderCube({ 1.0f,0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, -1.0f,0.0f,0.0f,1.0f }, { false,1.0f,0.0f,0.0f,1.0f });
// 		_rocketGraphics->RenderSphere({ 1.0f,0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,0.0f, 0.0f,0.0f,1.0f,0.0f, 1.0f,0.0f,0.0f,1.0f }, { false,0.0f,0.0f,1.0f,1.0f });
		_rocketGraphics->EndDebugRender();

		_rocketGraphics->BeginDebugLineRender();
		DrawDebugLine();
		//_rocketGraphics->RenderLine({ 0.0f, 0.0f, 1.0f }, { 10.0f,10.0f,0.0f }, { 1.0f ,0.0f,1.0f ,1.0f });
// 		_rocketGraphics->RenderRayLine(50.0f,{ 1.0f ,0.0f,0.0f ,1.0f });
		_rocketGraphics->EndDebugLineRender();

		_rocketGraphics->BeginUIRender();
		//여기에 "RenderSprite" 같은 함수들이 호출되어야 한다.
		DrawCurrentUI();
		DrawDebugText();
		DrawDebug2DBox();
		//_rocketGraphics->Render2DBox({ 100.0f,100.0f }, { 300.0f,500.0f }, { 1.0f ,0.0f,0.0f ,1.0f });
		_rocketGraphics->EndUIRender();

		EndDraw(); // 전체 렌더 끝.
	}

	void RenderSystem::UpdateConstantData(RocketCore::Graphics::RenderConstantData& data)
	{
		// TODO : 씬 전역적으로 관리하지말고 씬단위로 관리할것 ㄷㄷ;;
		// 지금 라이트가 씬에 따라 가져오지 않고 그냥 전역적으로 갖고온다 세상에..
		// 23.08.23 강석원 인재원.

		for (auto& light : _lights)
		{
			if (!(light->gameObject->IsActive()))
			{
				continue;
			}

			void* temp = nullptr;

			temp = dynamic_cast<RocketEngine::DirectionalLight*>(light);

			if (temp)
			{
				data.dirLightList.push_back(static_cast<RocketEngine::DirectionalLight*>(temp)->GetLightData());
				continue;
			}

			temp = dynamic_cast<RocketEngine::PointLight*>(light);

			if (temp)
			{
				data.pointLightList.push_back(static_cast<RocketEngine::PointLight*>(temp)->GetLightData());
				continue;
			}

			temp = dynamic_cast<RocketEngine::SpotLight*>(light);

			if (temp)
			{
				data.spotLightList.push_back(static_cast<RocketEngine::SpotLight*>(temp)->GetLightData());
				continue;
			}
		}

		data.deltaTime = RocketCore::TimeSystem::Instance().GetDeltaTime();
	}

	void RenderSystem::BeginDraw()
	{
		RocketCore::Graphics::RenderConstantData data;
		UpdateConstantData(data);
		_rocketGraphics->SetRenderData(data);

		_rocketGraphics->BeginTotalRender(); //전체 렌더 시작!
	}

	void RenderSystem::DrawCurrentScene(Graphics::eRenderType renderType)
{
		RocketEngine::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();

		// 현재 씬이 존재하지 않으면 Return.
		if (currentScene == nullptr)
		{
			return;
		}

		/// 현재씬이 존재한다면 진행될 동작들.
		_rocketGraphics->UpdateCamera(currentScene->GetMainCamera()->GetCameraData());

		for (auto& object : currentScene->GetOriginalList())
		{ 
			// 각각의 객체가 본인들을 그리는 것이 아닌
			// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
			// object->GetRenderData();

			if (!object->IsActive())
			{
				continue;
			}

			MeshRendererBase* renderer = object->GetComponentDynamic<MeshRendererBase>();

			if (!renderer)
			{
				continue;
			}

			// 위험하지만 우선 부하때문에 주석처리..?
// 			if (_renderMap.find(renderer) == _renderMap.end())
// 			{
// 				RocketCore::Graphics::IRenderable* renderableObj = _rocketGraphics->MakeRenderable(renderer->GetModelData());
// 				_renderMap.insert({ renderer, renderableObj });
// 			}

			if (renderer->GetModelData().renderingType == renderType)
			{
				_renderMap.at(renderer)->Render(object->transform.GetWorldTM(), renderer->GetLegacyMaterialData());
			}
		}
	}

	void RenderSystem::EndDraw()
	{
		_rocketGraphics->EndTotalRender();
	}

	void RenderSystem::DrawCurrentUI()
	{
		RocketEngine::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();

		// 현재 씬이 존재하지 않으면 Return.
		if (currentScene == nullptr)
		{
			return;
		}

		std::vector<UIRenderer*> UIVec;

		for (auto& object : currentScene->GetOriginalList())
		{
			// 각각의 객체가 본인들을 그리는 것이 아닌
			// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
			// object->GetRenderData();

			if (!object->IsActive())
			{
				continue;
			}

			std::vector<UIRenderer*> renderers = object->GetComponentsDynamic<UIRenderer>();


			for (auto& renderer : renderers)
			{
				// button일때.. button은 sketchable을 만들지 않는다.
				if (dynamic_cast<RocketEngine::Button*>(renderer))
				{
					continue;
				}

				UIVec.push_back(renderer);


			}
		}

		std::sort(UIVec.begin(), UIVec.end(),
			[](UIRenderer* lhs, UIRenderer* rhs) 
			{
				return lhs->GetSortOrder() < rhs->GetSortOrder();
			});

		for (auto& renderer : UIVec)
		{
			RocketCore::Graphics::ISketchable* temp = _uiMap.at(renderer);
			temp->SetData(renderer->GetSketchData().c_str());
			temp->SetFillRatio(renderer->GetFillRatio());
 			_uiMap.at(renderer)->Render(renderer->Get2DWorldTM());
		}
		
	}

	void RenderSystem::DrawDebugBox()
	{
		if (!(DebugSystem::Instance().IsDebugMode() || _isEditor))
		{
			return;
		}

		for (auto& box : DebugSystem::Instance().GetBoxQueue())
		{
			_rocketGraphics->RenderCube(box.worldTM, box.widthHeightDepth.x, box.widthHeightDepth.y, box.widthHeightDepth.z, { box.isWire,box.color.r,box.color.g,box.color.b,box.color.a });
		}

		DebugSystem::Instance().FlushBoxQueue();
	}

	void RenderSystem::DrawDebugText()
	{
		if (!(DebugSystem::Instance().IsDebugMode()))
		{
			return;
		}

		DebugSystem::Instance().DrawDebugText({ 50.0f,50.0f }, "FPS : " + std::to_string(TimeSystem::Instance().GetFrameRate()), 20.0f);

		for (auto& text : DebugSystem::Instance().GetTextQueue())
		{
			_rocketGraphics->RenderDebugText(text.centerPos.x, text.centerPos.y, text.content.c_str(), text.size);
		}

		DebugSystem::Instance().FlushTextQueue();
	}

	void RenderSystem::DrawDebugLine()
	{
		if (!(DebugSystem::Instance().IsDebugMode()))
		{
			return;
		}

		for (auto& line : DebugSystem::Instance().GetLineQueue())
		{
			_rocketGraphics->RenderLine(line.beginPoint, line.endPoint, line.color);
		}

		DebugSystem::Instance().FlushLineQueue();
	}

	void RenderSystem::DrawDebug2DBox()
	{
		if (!(DebugSystem::Instance().IsDebugMode()))
		{
			return;
		}

		for (auto& box2D : DebugSystem::Instance().Get2DBoxQueue())
		{
			_rocketGraphics->Render2DBox(box2D.LT, box2D.RB, box2D.color);
		}

		DebugSystem::Instance().Flush2DBoxQueue();
	}

	void RenderSystem::RenderPhysicsDebug()
	{
		PhysicsSystem& temp = PhysicsSystem::Instance();
		for (auto& rigid : temp.GetRigidDynamicsList())
		{
			RocketEngine::DynamicCollider* col = static_cast<RocketEngine::DynamicCollider*>(rigid->userData);
			RocketEngine::BoxCollider* temp = dynamic_cast<RocketEngine::BoxCollider*>(col);
			RocketEngine::RMFLOAT4X4 worldTM = col->gameObject->transform.GetWorldTM();
			if (temp)
			{
				_rocketGraphics->RenderCube(worldTM, temp->GetWidth(), temp->GetHeight(), temp->GetDepth(), { true,1.0f,0.0f,0.0f,1.0f });
			}
			else
			{
				_rocketGraphics->RenderCube(worldTM, { true,1.0f,0.0f,0.0f,1.0f });
			}
		}

		for (auto& rigid : temp.GetRigidStaticsList())
		{
			RocketEngine::StaticCollider* col = static_cast<RocketEngine::StaticCollider*>(rigid->userData);
			RocketEngine::PlaneCollider* temp = dynamic_cast<RocketEngine::PlaneCollider*>(col);
			RocketEngine::RMFLOAT4X4 worldTM = col->gameObject->transform.GetWorldTM();
			if (temp)
			{
				_rocketGraphics->RenderCube(worldTM, 0.01f, 100.0f, 100.0f, { false,0.0f,1.0f,0.0f,1.0f });
			}
			else
			{
				_rocketGraphics->RenderCube(worldTM, { true,1.0f,0.0f,0.0f,1.0f });
			}
		}
	}

	void RenderSystem::PlayAnimation(MeshRendererBase* renderer, RocketCore::Graphics::AnimData animData)
	{
		if (_renderMap.find(renderer) == _renderMap.end())
		{
			return;
		}

		_renderMap.at(renderer)->PlayAnim(animData);
	}

	int RenderSystem::GetScreenWidth() const
	{
		return _screenWidth;
	}

	int RenderSystem::GetScreenHeight() const
	{
		return _screenHeight;
	}

	void RenderSystem::AddLight(RocketEngine::Light* light)
	{
		_lights.push_back(light);
	}

}
