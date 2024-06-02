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
		//�׷��Ƚ� ������ �ʱ�ȭ�Ѵ�.
		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight, bool isEditor = false) abstract;

		//������ �� ������ ���� ��ġ�� ������ �Ű�������. 
		virtual void SetRenderData(RenderConstantData _rInfo) abstract;

		/// ���ӿ����� �׷��Ƚ� �������� �����޶�� ��û.

		// ������ ��ü. ([TW] eRenderType �߰�..�� ������ �ߴٰ� ModelData�� ����.)
		virtual IRenderable* MakeRenderable(ModelData modelData) abstract; //���������� "MakeMeshRenderable"
		virtual IRenderable* MakeSkinnedRenderable(ModelData modelData) abstract;
		
		// �ִϸ��̼� ������ (�̸��� ����)
		virtual void MakeAnim(AnimDesc desc, AnimData* data) abstract;

 		virtual ISketchable* MakeText(float fontSize, float width, float height, RocketEngine::RMFLOAT4 color) abstract;
 		virtual ISketchable* MakeImage(const char* imagePath, float width, float height) abstract;

		virtual void DeleteRenderable(IRenderable* instance) abstract;
		virtual void DeleteSketchable(ISketchable* instance) abstract;

		virtual void UpdateCamera(const CameraData& cameraData) abstract;

		// ��ü�� ���δ� Begin,End
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

		virtual void CombineRenderTargets() abstract; //Render Target ��ġ��.

		virtual void BeginDebugRender() abstract;
		virtual void EndDebugRender() abstract;
		
		virtual void BeginDebugLineRender() abstract;
		virtual void EndDebugLineRender() abstract;
		
		virtual void BeginUIRender() abstract;
		virtual void EndUIRender() abstract;

		virtual void RenderAxisGrid() abstract;

		// Set CubeMap
		virtual void SetCubeMap(const char* path) abstract;

		//Debug��
		virtual void RenderCube(const RocketEngine::RMFLOAT4X4& worldTM, const PrimitiveData& primitiveData) abstract;
		virtual void RenderCube(const RocketEngine::RMFLOAT4X4& worldTM, float x, float y, float z, const PrimitiveData& primitiveData) abstract;
		
		virtual void RenderSphere(RocketEngine::RMFLOAT4X4 worldTM, PrimitiveData primitiveData) abstract;

		virtual void RenderLine(RocketEngine::RMFLOAT3 firstPos, RocketEngine::RMFLOAT3 secondPos, RocketEngine::RMFLOAT4 color) abstract;
		virtual void RenderRayLine(float distance, RocketEngine::RMFLOAT4 color) abstract;
		
		virtual void RenderDebugText(float x, float y, const char* contents, float size) abstract;
		
		virtual void Render2DBox(RocketEngine::RMFLOAT2 LT, RocketEngine::RMFLOAT2 RB, RocketEngine::RMFLOAT4 color) abstract;

		virtual void Resize(int _width, int _height) abstract; //ccupdagi

		virtual void Finalize() abstract;

		///Particle ������ ���ؼ� �������.
		virtual void TriggerRenderParticle(eParticleType type, RocketEngine::RMFLOAT3 worldPos, float seconds) abstract;
	};

	/// dllexport �ϴ� �������� ��ȯ�ϴ� ���丮 �Լ�.

	// IRocketGraphics�� ��ӹ޴� �������� �����ؼ� ��ĳ�����Ͽ� ��ȯ�Ѵ�.
	extern "C" __declspec(dllexport) IRocketGraphics * CreateGraphicsInstance();

	// �������� �ε����ʿ��� �������� �����Ҷ� ����ϴ� �Լ�.
	// �ε��ϴ��ʿ��� delete���� �ʰ� dll���� ��û�ϴ� ������ �����ߴ�.
	// ������ �������� �´� Release ������ ���ǵǾ� ���� ���̴�.
	extern "C" __declspec(dllexport) void ReleaseGraphicsInstance(IRocketGraphics * instance);
}
