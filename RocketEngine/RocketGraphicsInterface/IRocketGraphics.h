#pragma once
#include <string>

#include "RenderConstantData.h"
#include "ModelData.h"
#include "CameraData.h"
#include "AnimData.h"
#include "AnimDesc.h"
#include "RenderType.h"
#include "PrimitiveData.h"
#include "ParticleType.h"

#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG


// Float2, Float3, Float4
// Fmatrix3x3, Fmatrix4x4

namespace RocketCore::Graphics
{
	class IRenderable;
	class ISketchable;

	class IRocketGraphics
	{
	public:
		//그래픽스 엔진을 초기화한다.
		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight, bool isEditor = false) abstract;

		//라이팅 등 렌더링 영향 미치는 정보를 매개변수에. 
		virtual void SetRenderData(RenderConstantData _rInfo) abstract;

		/// 게임엔진이 그래픽스 엔진에게 만들어달라고 요청.

		// 렌더링 객체. ([TW] eRenderType 추가..를 별도로 했다가 ModelData에 편입.)
		virtual IRenderable* MakeRenderable(ModelData modelData) abstract; //실질적으로 "MakeMeshRenderable"
		virtual IRenderable* MakeSkinnedRenderable(ModelData modelData) abstract;
		
		// 애니메이션 데이터 (이름만 받음)
		virtual void MakeAnim(AnimDesc desc, AnimData* data) abstract;

 		virtual ISketchable* MakeText(float fontSize, float width, float height, RocketEngine::RMFLOAT4 color) abstract;
 		virtual ISketchable* MakeImage(const char* imagePath, float width, float height) abstract;

		virtual void DeleteRenderable(IRenderable* instance) abstract;
		virtual void DeleteSketchable(ISketchable* instance) abstract;

		virtual void UpdateCamera(const CameraData& cameraData) abstract;

		// 전체를 감싸는 Begin,End
 		virtual void BeginTotalRender() abstract;
 		virtual void EndTotalRender() abstract;

		//BeginRender,
		virtual void BeginDeferredPhongMeshRender() abstract;
		virtual void EndDeferredPhongMeshRender() abstract;

		virtual void BeginDeferredPBRMeshRender() abstract;
		virtual void EndDeferredPBRMeshRender() abstract;

		virtual void BeginDeferredPBRSkinnedMeshRender() abstract;
		virtual void EndDeferredPBRSkinnedMeshRender() abstract;

		virtual void BeginForwardMeshRender() abstract;
		virtual void EndForwardMeshRender() abstract;

		virtual void CombineRenderTargets() abstract; //Render Target 합치기.

		virtual void BeginDebugRender() abstract;
		virtual void EndDebugRender() abstract;
		
		virtual void BeginDebugLineRender() abstract;
		virtual void EndDebugLineRender() abstract;
		
		virtual void BeginUIRender() abstract;
		virtual void EndUIRender() abstract;

		virtual void RenderAxisGrid() abstract;

		// Set CubeMap
		virtual void SetCubeMap(const char* path) abstract;

		//Debug용
		virtual void RenderCube(const RocketEngine::RMFLOAT4X4& worldTM, const PrimitiveData& primitiveData) abstract;
		virtual void RenderCube(const RocketEngine::RMFLOAT4X4& worldTM, float x, float y, float z, const PrimitiveData& primitiveData) abstract;
		
		virtual void RenderSphere(RocketEngine::RMFLOAT4X4 worldTM, PrimitiveData primitiveData) abstract;

		virtual void RenderLine(RocketEngine::RMFLOAT3 firstPos, RocketEngine::RMFLOAT3 secondPos, RocketEngine::RMFLOAT4 color) abstract;
		virtual void RenderRayLine(float distance, RocketEngine::RMFLOAT4 color) abstract;
		
		virtual void RenderDebugText(float x, float y, const char* contents, float size) abstract;
		
		virtual void Render2DBox(RocketEngine::RMFLOAT2 LT, RocketEngine::RMFLOAT2 RB, RocketEngine::RMFLOAT4 color) abstract;

		virtual void Resize(int _width, int _height) abstract; //ccupdagi

		virtual void Finalize() abstract;

		///Particle 연동을 위해서 열어놓음.
		virtual void TriggerRenderParticle(eParticleType type, RocketEngine::RMFLOAT3 worldPos, float seconds) abstract;
	};

	/// dllexport 하는 렌더러를 반환하는 팩토리 함수.

	// IRocketGraphics를 상속받는 렌더러를 생성해서 업캐스팅하여 반환한다.
	extern "C" __declspec(dllexport) IRocketGraphics * CreateGraphicsInstance();

	// 렌더러를 로드한쪽에서 렌더러를 해제할때 사용하는 함수.
	// 로드하는쪽에서 delete하지 않고 dll한테 요청하는 식으로 구성했다.
	// 각각의 렌더러에 맞는 Release 동작이 정의되어 있을 것이다.
	extern "C" __declspec(dllexport) void ReleaseGraphicsInstance(IRocketGraphics * instance);
}
